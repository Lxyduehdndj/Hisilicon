
#include <hi_stdlib.h>
#include <hisignalling_protocol.h>
#include <hi_uart.h>
#include <app_demo_uart.h>
#include <iot_uart.h>
#include <hi_gpio.h>
#include <hi_io.h>
#include "iot_gpio_ex.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "ssd1306_oled.h"
#include "app_demo_i2c_oled.h"
#include "iot_i2c.h"
#include "iot_watchdog.h"

UartDefConfig uartDefConfig = {0};
extern unsigned char BMP[];
extern unsigned char BMP0[];
extern unsigned char BMP1[];
extern unsigned char BMP2[];
extern unsigned char BMP3[];
extern unsigned char BMP4[];
extern unsigned char BMP5[];
extern unsigned char BMP6[];
extern unsigned char BMP7[];
extern unsigned char BMP8[];
#define OLED_I2C_BAUDRATE (400*1000)

static void Uart1GpioCOnfig(void)
{
#ifdef ROBOT_BOARD
    IoSetFunc(HI_IO_NAME_GPIO_5, IOT_IO_FUNC_GPIO_5_UART1_RXD);
    IoSetFunc(HI_IO_NAME_GPIO_6, IOT_IO_FUNC_GPIO_6_UART1_TXD);
    /* IOT_BOARD */
#elif defined (EXPANSION_BOARD)
    IoSetFunc(HI_IO_NAME_GPIO_0, IOT_IO_FUNC_GPIO_0_UART1_TXD);
    IoSetFunc(HI_IO_NAME_GPIO_1, IOT_IO_FUNC_GPIO_1_UART1_RXD);
#endif
}

int SetUartRecvFlag(UartRecvDef def)
{
    if (def == UART_RECV_TRUE) {
        uartDefConfig.g_uartReceiveFlag = HI_TRUE;
    } else {
        uartDefConfig.g_uartReceiveFlag = HI_FALSE;
    }
    
    return uartDefConfig.g_uartReceiveFlag;
}

int GetUartConfig(UartDefType type)
{
    int receive = 0;

    switch (type) {
        case UART_RECEIVE_FLAG:
            receive = uartDefConfig.g_uartReceiveFlag;
            break;
        case UART_RECVIVE_LEN:
            receive = uartDefConfig.g_uartLen;
            break;
        default:
            break;
    }
    return receive;
}

void ResetUartReceiveMsg(void)
{
    (void)memset_s(uartDefConfig.g_receiveUartBuff, sizeof(uartDefConfig.g_receiveUartBuff),
        0x0, sizeof(uartDefConfig.g_receiveUartBuff));
}

unsigned char *GetUartReceiveMsg(void)
{
    return uartDefConfig.g_receiveUartBuff;
}

static hi_void *UartDemoTask(char *param)
{
    hi_u8 uartBuff[UART_BUFF_SIZE] = {0};
    //hi_u8 uartOled[UART_BUFF_SIZE+10] = {0};
    //hi_u8 uart_R= 1;

    printf("Initialize uart demo successfully, please enter some datas via DEMO_UART_NUM port...\n");
    Uart1GpioCOnfig();
    IoTGpioInit(13);
    hi_io_set_func(13, 6);
    IoTGpioInit(14);
    hi_io_set_func(14, 6);
    OledInit();
    OledFillScreen(0);
    IoTI2cInit(0, OLED_I2C_BAUDRATE);
    while (1) {
        uartDefConfig.g_uartLen = IoTUartRead(DEMO_UART_NUM, uartBuff, UART_BUFF_SIZE);
       
        if ((uartDefConfig.g_uartLen > 0) && (uartBuff[0] == 0xaa) && (uartBuff[1] == 0x55)) 
        {
            if (GetUartConfig(UART_RECEIVE_FLAG) == HI_FALSE) {
                (void)memcpy_s(uartDefConfig.g_receiveUartBuff, uartDefConfig.g_uartLen,
                    uartBuff, uartDefConfig.g_uartLen);
                (void)SetUartRecvFlag(UART_RECV_TRUE);
            }
            switch(uartBuff[5])//到时候这里可以显示OLED
            {//uartBuff[5]是串口传输的数据，为什么从2开始？ 因为定义的时候uartBuff数组为0，防止未识别时，仍发送数据
                
                case 2:
                    OLED_DrawBMP(0, 0, 128, 8, BMP0);
                    printf("LHH %d\r\n",uartBuff[5]);
                    break;//0_hetun
                case 3:
                    OLED_DrawBMP(0, 0, 128, 8, BMP1);
                    printf("LHH %d\r\n",uartBuff[5]);
                    break;//1
                case 4:
                    OLED_DrawBMP(0, 0, 128, 8, BMP2);
                    printf("LHH %d\r\n",uartBuff[5]);
                    break;//2
                case 5:
                    OLED_DrawBMP(0, 0, 128, 8, BMP3);
                    printf("LHH %d\r\n",uartBuff[5]);
                    break;//3
                case 6:
                    OLED_DrawBMP(0, 0, 128, 8, BMP4);
                    printf("LHH %d\r\n",uartBuff[5]);
                    break;//4
                case 7:
                    OLED_DrawBMP(0, 0, 128, 8, BMP5);
                    printf("LHH %d\r\n",uartBuff[5]);
                    break;//5
                case 8:
                    OLED_DrawBMP(0, 0, 128, 8, BMP6);
                    printf("LHH %d\r\n",uartBuff[5]);
                    break;//6
                case 9:
                    OLED_DrawBMP(0, 0, 128, 8, BMP7);
                    printf("LHH %d\r\n",uartBuff[5]);
                    break;//7
                case 10:
                    OLED_DrawBMP(0, 0, 128,8, BMP8);
                    printf("LHH %d\r\n",uartBuff[5]);
                    break;//8 
                default:
                    break;
            }
        }
        osDelay(20); /* 20:sleep 20ms */
    }
    return HI_NULL;
}

/*
 * This demo simply shows how to read datas from UART2 port and then echo back.
 */
hi_void UartTransmit(hi_void)
{
    hi_u32 ret = 0;

    IotUartAttribute uartAttr = {
        .baudRate = 115200, /* baudRate: 115200 */
        .dataBits = 8, /* dataBits: 8bits */
        .stopBits = 1, /* stop bit */
        .parity = 0,
    };
    /* Initialize uart driver */
    ret = IoTUartInit(DEMO_UART_NUM, &uartAttr);
    if (ret != HI_ERR_SUCCESS) {
        printf("Failed to init uart! Err code = %d\n", ret);
        return;
    }
    /* Create a task to handle uart communication */
    osThreadAttr_t attr = {0};
    attr.stack_size = UART_DEMO_TASK_STAK_SIZE;
    attr.priority = UART_DEMO_TASK_PRIORITY;
    attr.name = (hi_char*)"uart demo";
    if (osThreadNew((osThreadFunc_t)UartDemoTask, NULL, &attr) == NULL) {
        printf("Falied to create uart demo task!\n");
    }
}
SYS_RUN(UartTransmit);