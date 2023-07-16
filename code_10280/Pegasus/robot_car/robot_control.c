#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "hi_io.h"
#include "hi_time.h"
#include "hi_adc.h"
#include "iot_errno.h"
#include "robot_l9110s.h"
#include "trace_model.h"
#include "robot_control.h"
#include "iot_gpio_ex.h"

#define     GPIO5 5
#define     FUNC_GPIO 0
#define     IOT_IO_PULL_UP 1
#define     VLT_MIN                     (100)

unsigned short  g_gpio5_adc_buf[ADC_TEST_LENGTH] = {0 };
unsigned int    g_gpio5_tick = 0;
unsigned int    g_car_control_demo_task_id = 0;
unsigned char   g_car_status = CAR_STOP_STATUS;

void switch_init(void)
{
    IoTGpioInit(GPIO5);
    hi_io_set_func(GPIO5, FUNC_GPIO);
    IoTGpioSetDir(GPIO5, IOT_GPIO_DIR_IN);
    hi_io_set_pull(GPIO5, IOT_IO_PULL_UP);
}

void traceModeInit(void)
{
    IoTGpioInit(11);
    hi_io_set_func(11, 0);
    IoTGpioSetDir(11, IOT_GPIO_DIR_IN);

    IoTGpioInit(12);
    hi_io_set_func(12, 0);
    IoTGpioSetDir(12, IOT_GPIO_DIR_IN);

    IoTGpioInit(2);
    hi_io_set_func(2, 0);
    IoTGpioSetDir(2, IOT_GPIO_DIR_IN);
}

unsigned char GetCarStatus(void)
{
    return g_car_status;
}

// 按键中断响应函数*************************
void gpio5_isr_func_mode(void)
{
    unsigned int forward_time = 500000;
    printf("gpio5_isr_func_mode start\r\n");
    unsigned int tick_interval = 0;
    unsigned int current_gpio5_tick = 0;

    current_gpio5_tick = hi_get_tick();
    tick_interval = current_gpio5_tick - g_gpio5_tick;
    
    if (tick_interval < KEY_INTERRUPT_PROTECT_TIME) {
        return NULL;
    }
    g_gpio5_tick = current_gpio5_tick;
    
//***************************************通过按键改变模式
    if (g_car_status == CAR_STOP_STATUS) {//停止状态按键按下，前进一段距离，之后继续循迹
        car_forward();//前进
         hi_udelay(forward_time);///延时
        g_car_status = CAR_TRACE_STATUS;         
        printf("trace\r\n");
    } else if (g_car_status == CAR_TRACE_STATUS) {//循迹状态按键按下，直接切换到停止状态
        g_car_status = CAR_STOP_STATUS;                  
        printf("stop\r\n");
    }
}

unsigned char get_gpio5_voltage(void *param)
{
    int i;
    unsigned short data;
    unsigned int ret;
    unsigned short vlt;
    float voltage;
    float vlt_max = 0;
    float vlt_min = VLT_MIN;
    float a = 1.8;
    float c = 4096.0;
    int b = 4;
    float vlt_1 = 0.6;
    float vlt_2 = 1.0;

    hi_unref_param(param);
    memset_s(g_gpio5_adc_buf, sizeof(g_gpio5_adc_buf), 0x0, sizeof(g_gpio5_adc_buf));
    for (i = 0; i < ADC_TEST_LENGTH; i++) {
        ret = hi_adc_read(HI_ADC_CHANNEL_2, &data, HI_ADC_EQU_MODEL_4, HI_ADC_CUR_BAIS_DEFAULT, 0xF0);
		// ADC_Channal_2  自动识别模式  CNcomment:4次平均算法模式 CNend
        if (ret != IOT_SUCCESS) {
            printf("ADC Read Fail\n");
            return  NULL;
        }
        g_gpio5_adc_buf[i] = data;
    }

    for (i = 0; i < ADC_TEST_LENGTH; i++) {
        vlt = g_gpio5_adc_buf[i];
        voltage = (float)vlt * a * b / c;
		/* vlt * 1.8* 4 / 4096.0为将码字转换为电压 */
        vlt_max = (voltage > vlt_max) ? voltage : vlt_max;
        vlt_min = (voltage < vlt_min) ? voltage : vlt_min;
    }
    printf("vlt_max is %f\r\n", vlt_max);
    // if (vlt_max > vlt_1 && vlt_max < vlt_2) {
    if (vlt_max > vlt_1 && vlt_max < vlt_2) {    
        gpio5_isr_func_mode();
     }
}

// 按键中断
void interrupt_monitor(void)
{
    unsigned int  ret = 0;
    /* gpio5 switch2 mode */
    g_gpio5_tick = hi_get_tick();
    ret = IoTGpioRegisterIsrFunc(GPIO5, IOT_INT_TYPE_EDGE, IOT_GPIO_EDGE_FALL_LEVEL_LOW, get_gpio5_voltage, NULL);
    if (ret == IOT_SUCCESS) {
        printf(" register gpio5\r\n");
    }
}

void *RobotCarTestTask(void* param)
{
    unsigned int time = 20;
    printf("switch\r\n");
    switch_init();
    interrupt_monitor();
    traceModeInit();
    car_stop();
    IoTWatchDogDisable();
    while (1) {        
        switch (g_car_status) {
            case CAR_STOP_STATUS:
                car_stop();
                break;
            case CAR_TRACE_STATUS:
                trace_module();
                break;
            default:
                break;
                
        }
        
        osDelay(time);
    }
}

void RobotCarDemo(void)
{
    osThreadAttr_t attr;

    attr.name = "RobotCarTestTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = CAR_CONTROL_DEMO_TASK_STAK_SIZE;
    attr.priority = CAR_CONTROL_DEMO_TASK_PRIORITY;

    if (osThreadNew(RobotCarTestTask, NULL, &attr) == NULL) {
        printf("[Ssd1306TestDemo] Falied to create RobotCarTestTask!\n");
    }
}
APP_FEATURE_INIT(RobotCarDemo);
