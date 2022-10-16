#include "myMPU6500.h"
#include "spi.h"

uint8_t MPU_id = 0;
imu_t imu = {0}; 	//imu数据


#define SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)

uint8_t SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG)
{
	uint8_t bitstatus = 0;
  /* Check the status of the specified SPI flag */
  if ((SPIx->SR & SPI_I2S_FLAG) != (uint16_t)RESET)
  {
    /* SPI_I2S_FLAG is set */
    bitstatus = 1;
  }
  else
  {
    /* SPI_I2S_FLAG is reset */
    bitstatus = 0;
  }
  /* Return the SPI_I2S_FLAG status */
  return  bitstatus;
}

uint8_t SPI5_ReadWriteByte(uint8_t TxData)
{
	uint8_t Rxdata;
  HAL_SPI_TransmitReceive(&hspi5,&TxData,&Rxdata,1, 0x01);			//太慢了，重写吧！
 	return Rxdata;
//	uint8_t txflag=1;
//	uint8_t count=0;
//	while(1)
//	{
//		if(txflag==1&&SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_TXE))
//		{	
//			for(count=0;count<200;count++){}
//			SPI5 -> DR = TxData;//发送数据
//			txflag=0;
//		}
//		if(txflag==0&&SPI_I2S_GetFlagStatus(SPI5, SPI_I2S_FLAG_RXNE)) //等待SPI接收标志位空
//		{
//			for(count=0;count<200;count++){}
//			return SPI5 -> DR; //接收数据	
//		}
//	}
}



//Read a register from MPU6500
uint8_t MPU6500_Read_Reg(uint8_t const reg)
{
  static uint8_t MPU_Rx, MPU_Tx;
  
  MPU6500_NSS_LOW;
  
  MPU_Tx = reg|0x80;
  SPI5_ReadWriteByte(MPU_Tx);
  MPU_Rx=SPI5_ReadWriteByte(reg|0x80);
  
  MPU6500_NSS_HIGH;
  return  MPU_Rx;
}

//Write a register to MPU6500
uint8_t MPU6500_Write_Reg(uint8_t const reg, uint8_t const data)
{
	static uint8_t MPU_Tx;

	MPU6500_NSS_LOW;
  
  MPU_Tx = reg&0x7f;
  SPI5_ReadWriteByte(MPU_Tx);
  MPU_Tx = data;
  SPI5_ReadWriteByte(MPU_Tx);
  
  MPU6500_NSS_HIGH;
  return 0;
}

////////////////////////////////////////////////////////////////////////
/////////////////////////////////读数据/////////////////////////////////
//读acc
void MPU6050AccRead(int16_t *accData)
{
	uint8_t buf[6];
	uint8_t i;

	for(i=0;i<6;i++)
	{
		buf[i] = SPI5_ReadWriteByte(0);
	}
	accData[0] = (int16_t)((buf[0] << 8) | buf[1]);
	accData[1] = (int16_t)((buf[2] << 8) | buf[3]);
	accData[2] = (int16_t)((buf[4] << 8) | buf[5]);
}

//读gyro
void MPU6050GyroRead(int16_t *gyroData)
{
	uint8_t buf[6];
	uint8_t i;

	for(i=0;i<6;i++)
	{
		buf[i] = SPI5_ReadWriteByte(0);
	}
	gyroData[0] = (int16_t)((buf[0] << 8) | buf[1]);
	gyroData[1] = (int16_t)((buf[2] << 8) | buf[3]);
	gyroData[2] = (int16_t)((buf[4] << 8) | buf[5]);
}


/****************************************************************************
 *原函数：void ReadIMUSensorHandle(void)
 *功  能：读取mpu6500六轴ad值，并单位化，再进行低通滤波处理
 ***************************************************************************/
#define SENSOR_MAX_G 8.0f		//constant g		// tobe fixed to 8g. but IMU need to correct at the same time
#define SENSOR_MAX_W 1000.0f	//deg/s
#define ACC_SCALE  (SENSOR_MAX_G/32768.0f)
#define GYRO_SCALE  (SENSOR_MAX_W/32768.0f)
#define CONSTANTS_ONE_G	9.80665f	//重力加速度* m/s^2


void ReadIMUSensorHandle(void)
{
	uint8_t i;

	//read raw
	MPU6500_NSS_LOW;	//选通器件
	SPI5_ReadWriteByte(MPU6500_ACCEL_XOUT_H|0x80);		//写地址、读命令
	MPU6050AccRead(imu.accADC);												//接收数据
	imu.temperature = SPI5_ReadWriteByte(0)<<8;				//两位温度数据
	imu.temperature |= SPI5_ReadWriteByte(0);
	MPU6050GyroRead(imu.gyroADC);
	MPU6500_NSS_HIGH;	//禁止器件
	
	//tutn to physical
	for(i=0; i<3; i++)
	{
		imu.accRaw[i]= (float)imu.accADC[i] *ACC_SCALE * CONSTANTS_ONE_G ;
		imu.gyroRaw[i]=(float)imu.gyroADC[i] * GYRO_SCALE * M_PI_F /180.f;		//deg/s
	}

	imu.accb[0]=LPF2pApply_1(imu.accRaw[0]-imu.accOffset[0]);
	imu.accb[1]=LPF2pApply_2(imu.accRaw[1]-imu.accOffset[1]);
	imu.accb[2]=LPF2pApply_3(imu.accRaw[2]-imu.accOffset[2]);

	imu.gyro[0]=LPF2pApply_4(imu.gyroRaw[0]);
	imu.gyro[1]=LPF2pApply_5(imu.gyroRaw[1]);
	imu.gyro[2]=LPF2pApply_6(imu.gyroRaw[2]);
	
	//printf("raw: %4.5f  last: %4.5f\n",imu.accRaw[0],imu.accb[0]);
}
	
//void ReadIMUSensorHandleout(void)
//{
// MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
// MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
// imu.accRawout[0] = aacx*ACC_SCALE * CONSTANTS_ONE_G ;
// imu.accRawout[1] = aacy*ACC_SCALE * CONSTANTS_ONE_G ;
// imu.accRawout[2] = aacz*ACC_SCALE * CONSTANTS_ONE_G ;
// imu.gyroRawout[0] = gyrox* GYRO_SCALE * M_PI_F /180.f;
// imu.gyroRawout[1] = gyroy* GYRO_SCALE * M_PI_F /180.f;
// imu.gyroRawout[2] = gyroz* GYRO_SCALE * M_PI_F /180.f;
// 
// imu.accbout[0]=LPF2pApply_1(imu.accRawout[0]-imu.accOffset[0]);
// imu.accbout[1]=LPF2pApply_2(imu.accRawout[1]-imu.accOffset[1]);
// imu.accbout[2]=LPF2pApply_3(imu.accRawout[2]-imu.accOffset[2]);
// 
// imu.gyroout[0]=LPF2pApply_4(imu.gyroRawout[0]);
// imu.gyroout[1]=LPF2pApply_5(imu.gyroRawout[1]);
// imu.gyroout[2]=LPF2pApply_6(imu.gyroRawout[2]);
//}

uint8_t MPU6500_Init(void)
{
	uint8_t i = 0;
  uint8_t MPU6500_Init_Data[10][2] = 
  {
    {MPU6500_PWR_MGMT_1,    0x80},      // Reset Device
    {MPU6500_PWR_MGMT_1,    0x03},      // Clock Source - Gyro-Z
    {MPU6500_PWR_MGMT_2,    0x00},      // Enable Acc & Gyro
    {MPU6500_CONFIG,        0x02},      // LPF 98Hz
    {MPU6500_GYRO_CONFIG,   0x10},      // +-1000dps
    {MPU6500_ACCEL_CONFIG,  0x10},      // +-8G
    {MPU6500_ACCEL_CONFIG_2,0x02},      // enable LowPassFilter  Set Acc LPF
    {MPU6500_USER_CTRL,     0x20},      // Enable AUX
  };
  
  HAL_Delay(50);
  MPU_id = MPU6500_Read_Reg(MPU6500_WHO_AM_I);  //read id of device,check if MPU6500 or not
  
  for(i = 0; i < 10; i++) 
  {
    MPU6500_Write_Reg(MPU6500_Init_Data[i][0], MPU6500_Init_Data[i][1]);
    HAL_Delay(1);
  }
	printf("\nMPU_ID: %d\n",MPU_id);
  return 0;
}

//函数名：IMU_Init(void)
//描  述：初始化滤波器
#define IMU_FILTER_CUTOFF_FREQ	30.0f		//截止频率
#define IMU_SAMPLE_RATE 		500.0f			//1000.0f/(float)DMP_CALC_PRD
void IMU_Init(void)
{
	imu.ready=0;
	
	//filter rate
	LPF2pSetCutoffFreq_1(IMU_SAMPLE_RATE,IMU_FILTER_CUTOFF_FREQ);		//30Hz
	LPF2pSetCutoffFreq_2(IMU_SAMPLE_RATE,IMU_FILTER_CUTOFF_FREQ);
	LPF2pSetCutoffFreq_3(IMU_SAMPLE_RATE,IMU_FILTER_CUTOFF_FREQ);
	LPF2pSetCutoffFreq_4(IMU_SAMPLE_RATE,IMU_FILTER_CUTOFF_FREQ);
	LPF2pSetCutoffFreq_5(IMU_SAMPLE_RATE,IMU_FILTER_CUTOFF_FREQ);
	LPF2pSetCutoffFreq_6(IMU_SAMPLE_RATE,IMU_FILTER_CUTOFF_FREQ);
}
