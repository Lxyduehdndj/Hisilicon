
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "hi_io.h"
#include "hi_time.h"
#include "iot_pwm.h"
#include "hi_pwm.h"

#define GPIO0 0
#define GPIO1 1
#define GPIO9 9
#define GPIO10 10
#define GPIOFUNC 0
#define PWM_FREQ_FREQUENCY  (60000)

void gpio_control (unsigned int  gpio, IotGpioValue value)
{
    hi_io_set_func(gpio, GPIOFUNC);
    IoTGpioSetDir(gpio, IOT_GPIO_DIR_OUT);
    IoTGpioSetOutputVal(gpio, value);
}

void car_backward(void)
{
    gpio_control(GPIO0, 0);
    gpio_control(GPIO1, 1);
    gpio_control(GPIO9, 0);
    gpio_control(GPIO10, 1);
}

void car_forward(void)
{
    gpio_control(GPIO0, 1);
    gpio_control(GPIO1, 0);
    gpio_control(GPIO9, 1);
    gpio_control(GPIO10, 0);
}

void car_left(void)
{
    gpio_control(GPIO0, 0);
    gpio_control(GPIO1, 0);
    gpio_control(GPIO9, 1);
    gpio_control(GPIO10, 0);
}

void car_right(void)
{
    gpio_control(GPIO0, 1);
    gpio_control(GPIO1, 0);
    gpio_control(GPIO9, 0);
    gpio_control(GPIO10, 0);
}

void car_stop(void)
{
    gpio_control(GPIO0, 1);
    gpio_control(GPIO1, 1);
    gpio_control(GPIO9, 1);
    gpio_control(GPIO10, 1);
}
