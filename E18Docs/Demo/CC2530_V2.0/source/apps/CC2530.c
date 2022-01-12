/*********************************************ͷ�ļ�����******************************************/
#include <hal_led.h>
#include <hal_assert.h>
#include <hal_board.h>
#include <hal_int.h>
#include "hal_mcu.h"
#include "hal_button.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "c51_gpio.h"
#include "c51_rtc.h"

/************************************************��������****************************************/
#define TX                  1       // ����ģʽ
#define RX                  0       // ����ģʽ

#define TX_ADDR             0x2520  // ���͵�ַ                 
#define RX_ADDR             0xBEEF  // ���յ�ַ

#define RF_CHANNEL          25      // 2.4 GHz RF channel 11 - 26  ����Ϊ5MHZ
#define PAN_ID              0x2007  // PAN ID

#define SEND_LENGTH         5       // ��������ÿ���ĳ���
#define RF_PKT_MAX_SIZE     30      // ���ݰ����ֵ

/************************************************ȫ�ֱ���****************************************/
static basicRfCfg_t basicRfConfig;
static uint8    pRxData[RF_PKT_MAX_SIZE];               // ���ջ���
static uint8    pTxData[SEND_LENGTH] = { "123\r\n" };   // ��Ҫ���͵�����
static uint16   SendCnt = 0;                            // �������͵����ݰ���
static uint16   RecvCnt = 0;                            // �������յ����ݰ���

/************************************************��������****************************************/
static void RF_SendPacket(void);        // ���ͺ���
static void RF_RecvPacket(void);        // ���պ���
static void RF_Initial(uint8 mode);     // RF��ʼ��
static void RST_System(void);           // ����ϵͳ

/*************************************************************************************************
* ���� ��   RF_Initial() => ��ʼ��RFоƬ
* ���� ��   mode, =0,����ģʽ�� else,����ģʽ
*************************************************************************************************/
static void RF_Initial(uint8 mode)
{
    // ���õ�ַ
	if (RX == mode)     { basicRfConfig.myAddr = RX_ADDR; } 
	else                { basicRfConfig.myAddr = TX_ADDR; }    

    basicRfConfig.panId = PAN_ID;           // ���ýڵ�PAN ID
    basicRfConfig.channel = RF_CHANNEL;     // ���ýڵ��ŵ�
    basicRfConfig.ackRequest = TRUE;        // Ӧ������  
    
    if (basicRfInit(&basicRfConfig) == FAILED)      { HAL_ASSERT(FALSE); }
          
//    halRfSetTxPower(1);                     // �����������Ϊ4dbm

    if (RX == mode)     { basicRfReceiveOn();  } 
    else                { basicRfReceiveOff(); }                 
}

/*************************************************************************************************
* ���� ��RST_System() => ��������ϵͳ������ϵͳ�����Ѻ�
*************************************************************************************************/
static void RST_System(void)
{
    halMcuInit();       // ���³�ʼ��MCU
    halMcuWaitMs(5);    // �ȴ�5ms��ϵͳ�ȶ�
    RF_Initial(TX);     // ���³�ʼ��RF
}

/*************************************************************************************************
* ���� : BSP_RF_SendPacket() => ���߷������ݺ���                            *
* ˵�� ��Sendbufferָ������͵����ݣ�length�������ݳ���                     *
*************************************************************************************************/
static void RF_SendPacket(void)
{ 
    // ����һ�����ݣ����ж��Ƿ��ͳɹ����յ�Ӧ��
    if (!basicRfSendPacket(RX_ADDR, pTxData, SEND_LENGTH))
    {
        SendCnt++;
        halLedClear(1);         // LED��˸������ָʾ���ͳɹ������յ�Ӧ��
    }
    halMcuWaitMs(500);
    halLedSet(1);
    C51_RTC_EnterSleep();       // ϵͳ����ģʽPM2���͹��ģ�2s��RTC����
    RST_System();               // ���³�ʼ��ϵͳ  
}

/*************************************************************************************************
* ���� ��RF_RecvPacket() => �������ݽ��մ���
*************************************************************************************************/
static void RF_RecvPacket(void)
{
    uint8 length = 0;
    
    while (!basicRfPacketIsReady());    // ���ģ���Ƿ��Ѿ����Խ�����һ������

    // ���յ������ݸ��Ƶ�pRxData��
    if ((length=basicRfReceive(pRxData, RF_PKT_MAX_SIZE, NULL)) > 0) 
    {
        // �жϽ��������Ƿ���ȷ
        if ((SEND_LENGTH==length) && (pRxData[0]=='1')\
        && (pRxData[1]=='2') && (pRxData[2]=='3'))
        {
            RecvCnt++;
            
            // ��˸LED��ָʾ�յ���ȷ����
            halLedClear(1);
            halMcuWaitMs(500);
            halLedSet(1);
        }                                  
    }
}

/*************************************************************************************************
* ���� : main() => ���������������
* ˵�� �����ͷ���ÿ2s��RTC����һ�Σ���������һ�����ݣ�ÿ�����ݳ���Ϊ5���ֽڣ�"123/r/n",��ɫLED
         ��˸�������ͳɹ��������յ���ȷ��Ӧ�����ݡ�
         ���շ�: ������յ����ݳ���Ϊ5���ֽڣ���ǰ����Ϊ��123�������ݽ�����ȷ��ͬʱ��ɫLED��˸һ��
         ָʾ�յ���ȷ���ݡ�
         ע�⣺appMode = TX�����ͳ��򣩣� =RX�����ճ���
*************************************************************************************************/
void main(void)
{
    uint8 appMode = TX;
    
    halBoardInit();                                         // ��ʼ��Χ�豸

    if (appMode == TX)              { C51_RTC_Initial(); }  // ��ʼ��RTC
    
    if (halRfInit() == FAILED)      { HAL_ASSERT(FALSE); }  // ��Ӳ��������rf���г�ʼ��
        
    RF_Initial(appMode);                                    // ��ʼ��RF

    if (!C51_GPIO_ioDetective())    { halLedClear(2); }     // IO��⣬�ж�IO�Ƿ��ж̽ӣ���·
    
    C51_GPIO_OffDetective();                                // �����޹�IOΪ���룬���͹���
    
    while (1)
    {
        if      (appMode == RX)     { RF_RecvPacket(); }    // ����ģ��
        else if (appMode == TX)     { RF_SendPacket(); }    // ����ģ��
        // Role is undefined. This code should not be reached
        //HAL_ASSERT(FALSE);   
    }
}












