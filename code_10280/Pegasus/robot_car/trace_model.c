#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "hi_io.h"
#include "hi_time.h"
#include "iot_watchdog.h"
#include "robot_control.h"
#include "iot_errno.h"
#include "hi_pwm.h"
#include "hi_timer.h"
#include "iot_pwm.h"
#include "robot_l9110s.h"
#include "iot_gpio_ex.h"
//*************电机
#define GPIO0 0
#define GPIO1 1
#define GPIO9 9
#define GPIO10 10
//***************循迹
#define GPIO11 11
#define GPIO12 12
#define GPIO2 2

#define GPIO_FUNC 0

//unsigned int g_car_speed_left = 0;
//unsigned int g_car_speed_right = 0;
IotGpioValue io_status_left;//io_status_left变量
IotGpioValue io_status_right;
IotGpioValue io_status_mid;
unsigned int car_state = 0;

void timer1_callback(unsigned int arg)
{
    IotGpioValue io_status;
     //GPIO11——左边传感器，GPIO12——右边传感器，GPIO2——中间传感器，
    IoTGpioGetInputVal(GPIO11, &io_status_left);//读取左边传感器的电平状态
    IoTGpioGetInputVal(GPIO12, &io_status_right);
    IoTGpioGetInputVal(GPIO2, &io_status_mid);
    
    if (io_status_left == 0  && io_status_right==0) {
        car_state=2;// 0 0 0 停
    }
    else if (io_status_left == 0 && io_status_mid == 0 && io_status_right==1) {
        car_state=4;// 0 0 1 左转90
    }
    else if (io_status_left == 1 && io_status_mid == 0 && io_status_right==0) {
        car_state=3;// 1 0 0 右转90 
    }
    else if (io_status_left == 0 && io_status_mid == 1 && io_status_right==1){
        car_state=5;// 0 1 1 左转
    }
    else if (io_status_left == 1 && io_status_mid == 1 && io_status_right==0){
        car_state=6;// 1 1 0 右转
    }
    else if (io_status_left == 1 && io_status_right==1){
        car_state=1;//前进    
    } 
}

void trace_module(void)
{
    unsigned int timer_id1;
    unsigned int delay_time = 20;
    hi_timer_create(&timer_id1);
    hi_timer_start(timer_id1, HI_TIMER_TYPE_PERIOD, 10, timer1_callback, 0);//开定时器
    
    while(1)
    {
        unsigned char status = GetCarStatus();
        if (status != CAR_TRACE_STATUS) break;
        else
        { 
            switch(car_state)
            {
                case 1:
                    car_forward();
                    break;
                case 2:
                    car_stop(); 
                    break;
                case 3:
                    hi_timer_stop(timer_id1);
                    car_right();
                    hi_udelay(1200000);
                    hi_timer_start(timer_id1, HI_TIMER_TYPE_PERIOD, 10, timer1_callback, 0);
                    break;
                case 4:
                    hi_timer_stop(timer_id1);//关定时器
                    car_left(); 
                    hi_udelay(1200000);//延时
                    hi_timer_start(timer_id1, HI_TIMER_TYPE_PERIOD, 10, timer1_callback, 0);//开定时器 
                    break;
                case 5:
                    car_left(); 
                    break;
                case 6:
                    car_right(); 
                    break;
                default:
                    break;
            }
        }
        car_state = 0;
    }
    hi_timer_delete(timer_id1);
}