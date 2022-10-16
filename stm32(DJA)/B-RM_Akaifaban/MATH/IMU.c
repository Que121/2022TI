/*    
功能：
姿态融合解算
------------------------------------
*/


#include "IMU.h"

#include <math.h>

#include "usart.h"


//#include "mpu6050out.h"
//#include "inv_mpu.h"
//#include "RemoteControl.h"
#include "buzzer.h"


#include "MyConfig.h"

//中间变量
static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;	/** quaternion of sensor frame relative to auxiliary frame */
static float dq0 = 0.0f, dq1 = 0.0f, dq2 = 0.0f, dq3 = 0.0f;	/** quaternion of sensor frame relative to auxiliary frame */
static float gyro_bias[3] = {0.0f, 0.0f, 0.0f}; /** bias estimation */
static float q0q0, q0q1, q0q2, q0q3;
static float q1q1, q1q2, q1q3;
static float q2q2, q2q3;
static float q3q3;
uint8_t bFilterInit = 0;			//快速收敛，有bug
float imu_last;
float pitch,roll,yaw; 		//欧拉角
short aacx,aacy,aacz;			//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
//float Q_ANGLE_Yaw,Q_ANGLE_Y,Q_ANGLE_X;
//函数名：invSqrt(void)
//描述：求平方根的倒数
//该函数是经典的Carmack求平方根算法，效率极高，使用魔数0x5f375a86
static float invSqrt(float number)
{
    volatile long i;
    volatile float x, y;
    volatile const float f = 1.5F;

    x = number * 0.5F;
    y = number;
    i = * (( long * ) &y);
    i = 0x5f375a86 - ( i >> 1 );
    y = * (( float * ) &i);
    y = y * ( f - ( x * y * y ) );
    return y;
}


//! Using accelerometer, sense the gravity vector.
//! Using magnetometer, sense yaw.
static void NonlinearSO3AHRSinit(float ax, float ay, float az, float mx, float my, float mz)
{
    float initialRoll, initialPitch;
    float cosRoll, sinRoll, cosPitch, sinPitch;
    float magX, magY;
    float initialHdg, cosHeading, sinHeading;

    initialRoll = atan2(-ay, -az);
    initialPitch = atan2(ax, -az);

    cosRoll = cosf(initialRoll);
    sinRoll = sinf(initialRoll);
    cosPitch = cosf(initialPitch);
    sinPitch = sinf(initialPitch);

    magX = mx * cosPitch + my * sinRoll * sinPitch + mz * cosRoll * sinPitch;

    magY = my * cosRoll - mz * sinRoll;

    initialHdg = atan2f(-magY, magX);

    cosRoll = cosf(initialRoll * 0.5f);
    sinRoll = sinf(initialRoll * 0.5f);

    cosPitch = cosf(initialPitch * 0.5f);
    sinPitch = sinf(initialPitch * 0.5f);

    cosHeading = cosf(initialHdg * 0.5f);
    sinHeading = sinf(initialHdg * 0.5f);

    q0 = cosRoll * cosPitch * cosHeading + sinRoll * sinPitch * sinHeading;
    q1 = sinRoll * cosPitch * cosHeading - cosRoll * sinPitch * sinHeading;
    q2 = cosRoll * sinPitch * cosHeading + sinRoll * cosPitch * sinHeading;
    q3 = cosRoll * cosPitch * sinHeading - sinRoll * sinPitch * cosHeading;

    // auxillary variables to reduce number of repeated operations, for 1st pass
    q0q0 = q0 * q0;
    q0q1 = q0 * q1;
    q0q2 = q0 * q2;
    q0q3 = q0 * q3;
    q1q1 = q1 * q1;
    q1q2 = q1 * q2;
    q1q3 = q1 * q3;
    q2q2 = q2 * q2;
    q2q3 = q2 * q3;
    q3q3 = q3 * q3;
}


////函数名：NonlinearSO3AHRSupdate()
////描述：姿态解算融合
////使用的是Mahony互补滤波算法，没有使用Kalman滤波算法
////改算法是直接参考pixhawk飞控的算法，可以在Github上看到出处
//https://github.com/hsteinhaus/PX4Firmware/blob/master/src/modules/attitude_estimator_so3/attitude_estimator_so3_main.cpp
static void NonlinearSO3AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, float twoKp, float twoKi, float dt)
{
    float recipNorm;
    float halfex = 0.0f, halfey = 0.0f, halfez = 0.0f;

    // Make filter converge to initial solution faster
    // This function assumes you are in static position.
    // WARNING : in case air reboot, this can cause problem. But this is very unlikely happen.
    if(bFilterInit == 0) {
        NonlinearSO3AHRSinit(-ax,-ay,-az,mx,my,mz);
        bFilterInit = 1;
    }

    //! If magnetometer measurement is available, use it.
    if(!((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f))) {
        float hx, hy, hz, bx, bz;
        float halfwx, halfwy, halfwz;

        // Normalise magnetometer measurement
        // Will sqrt work better? PX4 system is powerful enough?
        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Reference direction of Earth's magnetic field
        hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
        hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
        hz = 2.0f * mx * (q1q3 - q0q2) + 2.0f * my * (q2q3 + q0q1) + 2.0f * mz * (0.5f - q1q1 - q2q2);
        bx = sqrt(hx * hx + hy * hy);
        bz = hz;

        // Estimated direction of magnetic field
        halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
        halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
        halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

        // Error is sum of cross product between estimated direction and measured direction of field vectors
        halfex += (my * halfwz - mz * halfwy);
        halfey += (mz * halfwx - mx * halfwz);
        halfez += (mx * halfwy - my * halfwx);
    }

    //增加一个条件：  加速度的模量与G相差不远时。 0.75*G < normAcc < 1.25*G
    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
    {
        float halfvx, halfvy, halfvz;

        // Normalise accelerometer measurement
        //归一化，得到单位加速度
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);

        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Estimated direction of gravity and magnetic field
        halfvx = q1q3 - q0q2;
        halfvy = q0q1 + q2q3;
        halfvz = q0q0 - 0.5f + q3q3;

        // Error is sum of cross product between estimated direction and measured direction of field vectors
        halfex += ay * halfvz - az * halfvy;
        halfey += az * halfvx - ax * halfvz;
        halfez += ax * halfvy - ay * halfvx;
    }

    // Apply feedback only when valid data has been gathered from the accelerometer or magnetometer
    if(halfex != 0.0f && halfey != 0.0f && halfez != 0.0f) {
        // Compute and apply integral feedback if enabled
        if(twoKi > 0.0f) {
            gyro_bias[0] += twoKi * halfex * dt;	// integral error scaled by Ki
            gyro_bias[1] += twoKi * halfey * dt;
            gyro_bias[2] += twoKi * halfez * dt;

            // apply integral feedback
            gx += gyro_bias[0];
            gy += gyro_bias[1];
            gz += gyro_bias[2];
        }
        else {
            gyro_bias[0] = 0.0f;	// prevent integral windup
            gyro_bias[1] = 0.0f;
            gyro_bias[2] = 0.0f;
        }

        // Apply proportional feedback
        gx += twoKp * halfex;
        gy += twoKp * halfey;
        gz += twoKp * halfez;
    }

    // Time derivative of quaternion. q_dot = 0.5*q\otimes omega.
    //! q_k = q_{k-1} + dt*\dot{q}
    //! \dot{q} = 0.5*q \otimes P(\omega)
    dq0 = 0.5f*(-q1 * gx - q2 * gy - q3 * gz);
    dq1 = 0.5f*(q0 * gx + q2 * gz - q3 * gy);
    dq2 = 0.5f*(q0 * gy - q1 * gz + q3 * gx);
    dq3 = 0.5f*(q0 * gz + q1 * gy - q2 * gx);

    q0 += dt*dq0;
    q1 += dt*dq1;
    q2 += dt*dq2;
    q3 += dt*dq3;

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    // Auxiliary variables to avoid repeated arithmetic
    q0q0 = q0 * q0;
    q0q1 = q0 * q1;
    q0q2 = q0 * q2;
    q0q3 = q0 * q3;
    q1q1 = q1 * q1;
    q1q2 = q1 * q2;
    q1q3 = q1 * q3;
    q2q2 = q2 * q2;
    q2q3 = q2 * q3;
    q3q3 = q3 * q3;
}


#define so3_comp_params_Kp 1.0f
#define so3_comp_params_Ki  0.05f


//函数名：IMUSO3Thread(void)
//描述：姿态软件解算融合函数
//以下为屎山，不建议阅读。该函数执行完毕后会将姿态数据更新到imu结构体中（定义在MPU6500.c中）详细成员请查看MPU6500.h
void IMUSO3Thread(void)
{
    //! Time constant
    float dt = 0.01f;		//s
//    static uint32_t tPrev=0,startTime=0;	//us
    //uint32_t now;
    uint8_t i;

    /* output euler angles */
    float euler[3] = {0.0f, 0.0f, 0.0f};	//rad

    /* Initialization */
    float Rot_matrix[9] = {1.f,  0.0f,  0.0f, 0.0f,  1.f,  0.0f, 0.0f,  0.0f,  1.f };		/**< init: identity matrix */
    float acc[3] = {0.0f, 0.0f, 0.0f};		//m/s^2
    float gyro[3] = {0.0f, 0.0f, 0.0f};		//rad/s
    float mag[3] = {0.0f, 0.0f, 0.0f};
    //need to calc gyro offset before imu start working
//    static float gyro_offsets_sum[3]= { 0.0f, 0.0f, 0.0f }; // gyro_offsets[3] = { 0.0f, 0.0f, 0.0f },
//    static uint16_t offset_count = 0;

//    now=micros();
//    dt=(tPrev>0)?(now-tPrev)/1000000.0f:0;
//    tPrev=now;

    ReadIMUSensorHandle();							//获取IMU姿态数据

//	//校准陀螺仪
//    if(!imu.ready)
//    {
//        if(startTime==0)
//            startTime=now;

//        gyro_offsets_sum[0] += imu.gyroRaw[0];
//        gyro_offsets_sum[1] += imu.gyroRaw[1];
//        gyro_offsets_sum[2] += imu.gyroRaw[2];
//        offset_count++;

//        if(now > startTime + GYRO_CALC_TIME)
//        {
//            imu.gyroOffset[0] = gyro_offsets_sum[0]/offset_count;
//            imu.gyroOffset[1] = gyro_offsets_sum[1]/offset_count;
//            imu.gyroOffset[2] = gyro_offsets_sum[2]/offset_count;

//            offset_count=0;
//            gyro_offsets_sum[0]=0;
//            gyro_offsets_sum[1]=0;
//            gyro_offsets_sum[2]=0;

//            imu.ready = 1;
//            startTime=0;

//        }
//        return;
//    }


    gyro[0] = imu.gyro[0] - imu.gyroOffset[0];
    gyro[1] = imu.gyro[1] - imu.gyroOffset[1];
    gyro[2] = imu.gyro[2] - imu.gyroOffset[2];

    acc[0] = -imu.accb[0];
    acc[1] = -imu.accb[1];
    acc[2] = -imu.accb[2];

    // NOTE : Accelerometer is reversed.
    // Because proper mount of PX4 will give you a reversed accelerometer readings.
    NonlinearSO3AHRSupdate(gyro[0], gyro[1], gyro[2],
                           -acc[0], -acc[1], -acc[2],
                           mag[0], mag[1], mag[2],
                           so3_comp_params_Kp,
                           so3_comp_params_Ki,
                           dt);

    // Convert q->R, This R converts inertial frame to body frame.
    Rot_matrix[0] = q0q0 + q1q1 - q2q2 - q3q3;// 11
    Rot_matrix[1] = 2.f * (q1*q2 + q0*q3);	// 12
    Rot_matrix[2] = 2.f * (q1*q3 - q0*q2);	// 13
    Rot_matrix[3] = 2.f * (q1*q2 - q0*q3);	// 21
    Rot_matrix[4] = q0q0 - q1q1 + q2q2 - q3q3;// 22
    Rot_matrix[5] = 2.f * (q2*q3 + q0*q1);	// 23
    Rot_matrix[6] = 2.f * (q1*q3 + q0*q2);	// 31
    Rot_matrix[7] = 2.f * (q2*q3 - q0*q1);	// 32
    Rot_matrix[8] = q0q0 - q1q1 - q2q2 + q3q3;// 33

    //1-2-3 Representation.
    //Equation (290)
    //Representing Attitude: Euler Angles, Unit Quaternions, and Rotation Vectors, James Diebel.
    // Existing PX4 EKF code was generated by MATLAB which uses coloum major order matrix.
    euler[0] = atan2f(Rot_matrix[5], Rot_matrix[8]);	//! Roll
    euler[1] = -asinf(Rot_matrix[2]);									//! Pitch
    euler[2] = atan2f(Rot_matrix[1], Rot_matrix[0]);

    //DCM . ground to body
    for(i=0; i<9; i++)
    {
        *(&(imu.DCMgb[0][0]) + i)=Rot_matrix[i];
    }

    imu.rollRad=euler[0];
    imu.pitchRad=euler[1];
    imu.yawRad=euler[2];

	//180/M_PI_F=57.2957805
    imu.roll=euler[0] * 57.2957805f;
		//float gryoroll = (imu.gyro[0]-imu.gyroOffset[0])*57.2957795f;
    imu.pitch=euler[1] * 57.2957805f;
		float gryopitch = (imu.gyro[1]-imu.gyroOffset[1])*57.2957795f;
    imu.yaw=euler[2] * 57.2957805f;	
		
		printf("p:%.4f,r:%.4f,y:%.4f\n", imu.pitch, imu.roll, imu.yaw);
}


//static void IMUSO3Threadout(void)
//{
//	    //! Time constant
//    float dt = 0.01f;		//s
//    static uint32_t tPrev=0,startTime=0;	//us
//    uint32_t now;
//    uint8_t i;

//    /* output euler angles */
//    float euler[3] = {0.0f, 0.0f, 0.0f};	//rad

//    /* Initialization */
//    float Rot_matrix[9] = {1.f,  0.0f,  0.0f, 0.0f,  1.f,  0.0f, 0.0f,  0.0f,  1.f };		/**< init: identity matrix */
//    //float acc[3] = {0.0f, 0.0f, 0.0f};		//m/s^2
//		float accout[3] = {0.0f, 0.0f, 0.0f};
//    //float gyro[3] = {0.0f, 0.0f, 0.0f};		//rad/s
//		float gyroout[3] = {0.0f, 0.0f, 0.0f};
//    float mag[3] = {0.0f, 0.0f, 0.0f};
//    //need to calc gyro offset before imu start working
//    static float gyro_offsets_sum[3]= { 0.0f, 0.0f, 0.0f }; // gyro_offsets[3] = { 0.0f, 0.0f, 0.0f },
//    static uint16_t offset_count = 0;

//    now=micros();
//    dt=(tPrev>0)?(now-tPrev)/1000000.0f:0;
//    tPrev=now;

//    ReadIMUSensorHandleout();

//	//校准陀螺仪
//    if(!imu.ready)
//    {
//        if(startTime==0)
//            startTime=now;

//        gyro_offsets_sum[0] += imu.gyroRawout[0];
//        gyro_offsets_sum[1] += imu.gyroRawout[1];
//        gyro_offsets_sum[2] += imu.gyroRawout[2];
//        offset_count++;

//        if(now > startTime + GYRO_CALC_TIME)
//        {
//            imu.gyroOffset[0] = gyro_offsets_sum[0]/offset_count;
//            imu.gyroOffset[1] = gyro_offsets_sum[1]/offset_count;
//            imu.gyroOffset[2] = gyro_offsets_sum[2]/offset_count;

//            offset_count=0;
//            gyro_offsets_sum[0]=0;
//            gyro_offsets_sum[1]=0;
//            gyro_offsets_sum[2]=0;

//            imu.ready = 1;
//            startTime=0;

//        }
//        return;
//    }


//    gyroout[0] = imu.gyroout[0] - imu.gyroOffset[0];
//    gyroout[1] = imu.gyroout[1] - imu.gyroOffset[1];
//    gyroout[2] = imu.gyroout[2] - imu.gyroOffset[2];

//    accout[0] = -imu.accbout[0];
//    accout[1] = -imu.accbout[1];
//    accout[2] = -imu.accbout[2];

//    // NOTE : Accelerometer is reversed.
//    // Because proper mount of PX4 will give you a reversed accelerometer readings.
//    NonlinearSO3AHRSupdate(gyroout[0], gyroout[1], gyroout[2],
//                           -accout[0], -accout[1], -accout[2],
//                           mag[0], mag[1], mag[2],
//                           so3_comp_params_Kp,
//                           so3_comp_params_Ki,
//                           dt);

//    // Convert q->R, This R converts inertial frame to body frame.
//    Rot_matrix[0] = q0q0 + q1q1 - q2q2 - q3q3;// 11
//    Rot_matrix[1] = 2.f * (q1*q2 + q0*q3);	// 12
//    Rot_matrix[2] = 2.f * (q1*q3 - q0*q2);	// 13
//    Rot_matrix[3] = 2.f * (q1*q2 - q0*q3);	// 21
//    Rot_matrix[4] = q0q0 - q1q1 + q2q2 - q3q3;// 22
//    Rot_matrix[5] = 2.f * (q2*q3 + q0*q1);	// 23
//    Rot_matrix[6] = 2.f * (q1*q3 + q0*q2);	// 31
//    Rot_matrix[7] = 2.f * (q2*q3 - q0*q1);	// 32
//    Rot_matrix[8] = q0q0 - q1q1 - q2q2 + q3q3;// 33

//    //1-2-3 Representation.
//    //Equation (290)
//    //Representing Attitude: Euler Angles, Unit Quaternions, and Rotation Vectors, James Diebel.
//    // Existing PX4 EKF code was generated by MATLAB which uses coloum major order matrix.
//    euler[0] = atan2f(Rot_matrix[5], Rot_matrix[8]);	//! Roll
//    euler[1] = -asinf(Rot_matrix[2]);									//! Pitch
//    euler[2] = atan2f(Rot_matrix[1], Rot_matrix[0]);

//    //DCM . ground to body
//    for(i=0; i<9; i++)
//    {
//        *(&(imu.DCMgb[0][0]) + i)=Rot_matrix[i];
//    }

//    imu.rollRadout=euler[0];
//    imu.pitchRadout=euler[1];
//    imu.yawRadout=euler[2];

//	//180/M_PI_F=57.2957805
//    imu.rollout=euler[0] * 57.2957805f;
//		float gryoroll = (imu.gyroout[0]-imu.gyroOffset[0])*57.2957795f;
//    imu.pitchout=euler[1] * 57.2957805f;
//		float gryopitch = (imu.gyroout[1]-imu.gyroOffset[1])*57.2957795f;
//    imu.yawout=euler[2] * 57.2957805f;
//}


//imu温度闭环控制，解决陀螺仪零飘问题。该函数须多次循环调用以更新加热功率
static void IMU_Temp_Ctrl(void)
{
	int16_t err;
	err = IMU_TEMP - imu.temperature;	//测目标值当前值之间的差距，存入error
	
	if(err < 500)			//温度接近目标值，逐渐减低发热功率
	{
		int16_t temp;
		temp = err*5 + 50;
		if(temp>600)	temp=600;
		else if(temp<0)	temp=0;
		
		IMU_TEMP_R = temp;
	}
	else					//温度差距大，使用高功率加热
		IMU_TEMP_R = 999;	
}


//IMU校准
void IMU_Calibration(void)
{
	imu.ready=1;
	while(imu.temperature < 5000)//温度未稳定时
	{
		//buzzer_on(95, 10000);//IMU校准打开蜂鸣器
		IMUSO3Thread();
		IMU_Temp_Ctrl();
	}
	imu.ready = 0;
	while(imu.gyroOffset[1] == 0)
	{
		IMUSO3Thread();
		IMU_Temp_Ctrl();
	}

}

//int b;
void FRT_IMU(void *pvParameters)//IMU姿态解算任务
{
	while(1)
	{
		//taskENTER_CRITICAL();			//进入任务临界区，防止被中断等事情打断导致IMU数据接受异常
		IMUSO3Thread();					//IMU姿态获取，解算
		IMU_Temp_Ctrl();				//IMU温度控制，让IMU恒温，以解决温漂问题
		//b = GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10);		//不知道干啥的
		//vTaskDelay(1);					//任务延时函数		//延时为啥要放到任务临界区，不懂。应该是要放到外面 的
		//taskEXIT_CRITICAL();			//退出临界区
	}
}
