#include "SPI.h"

void SPI1_init(){ 	//��ʼ��SPI1
	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//ʹ�� GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ�� SPI1 ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	
	 /* SPI1 configuration *///��ʼ��SPI1�ṹ��
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI1����Ϊ����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //����SPI1Ϊ��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //����ʱ���ڲ�����ʱ��ʱ��Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //��һ��ʱ���ؿ�ʼ��������
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS�ź��������ʹ��SSIλ������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; //SPI������Ԥ��ƵֵΪ4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRCֵ����Ķ���ʽ
	SPI_Init(SPI1,&SPI_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, SPI1_SCK_P5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, SPI1_MISO_P6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, SPI1_MOSI_P7, GPIO_AF_SPI1);
	
	SPI_Cmd(SPI1, ENABLE);
	
	return;
}


u8 SPI1_ReadWriteByte(u8 TxData)                                        //SPI��д���ݺ���
{		
	u8 retry=0;				 	
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)      //���ͻ����־λΪ��
	{
		retry++;
		if(retry>200)return 0;
	}			  
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, TxData);                                    //ͨ������SPI1����һ������
	retry=0;
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)   //���ջ����־λ��Ϊ��
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);                                 //ͨ��SPI1���ؽ�������				    
}

