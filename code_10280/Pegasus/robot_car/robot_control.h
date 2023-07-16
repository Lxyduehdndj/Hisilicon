
#ifndef __ROBOT_CONTROL_H__
#define __ROBOT_CONTROL_H__


#define     CAR_CONTROL_DEMO_TASK_STAK_SIZE   (1024*10)
#define     CAR_CONTROL_DEMO_TASK_PRIORITY    (25)
#define     DISTANCE_BETWEEN_CAR_AND_OBSTACLE (20)
#define     KEY_INTERRUPT_PROTECT_TIME        (30)
#define     CAR_TURN_LEFT                     (0)
#define     CAR_TURN_RIGHT                    (1)
#define     ADC_TEST_LENGTH                   (20)

typedef enum {
    CAR_STOP_STATUS = 0,
    CAR_TRACE_STATUS
} CarStatus;

void switch_init(void);
void interrupt_monitor(void);
unsigned char GetCarStatus(void);
#endif
