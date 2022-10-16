#include "superCap.h"

SuperCap_t SuperCap;


void SuperCap_GetInformation(uint8_t * RAWdata)							//��ȡ�������������Ϣ
{
	SuperCap.Vot_input = (RAWdata[1]<<8 | RAWdata[0])/100.0;	//�����ѹ
	SuperCap.Vot_cap 	 = (RAWdata[3]<<8 | RAWdata[2])/100.0;	//���ݵ�ѹ
	SuperCap.Cur_input = (RAWdata[5]<<8 | RAWdata[4])/100.0;	//�������
	SuperCap.Set_power = (RAWdata[7]<<8 | RAWdata[6])/100.0;	//֮ǰ���õĹ���
	
	SuperCap.power = SuperCap.Vot_input * SuperCap.Cur_input;	//��ѹ x ���� = ����
}


