
#ifndef HISIGNALLING_H
#define HISIGNALLING_H

#define HISIGNALLING_MSG_HEADER_LEN         (1)
#define HISGNALLING_MSG_FRAME_HEADER_LEN    (2)
#define HISIGNALLING_MSG_HEADER_TAIL_LEN    (3)
#define HISGNALLING_FREE_TASK_TIME          (10)
#define HISIGNALLING_MSG_MOTOR_ENGINE_LEN   (11)
#define HISIGNALLING_MSG_ONE_FRAME_LEN      (16)
#define HISIGNALLING_MSG_BUFF_LEN           (512)

/*
 * @brief 外设扩展板和Robot板的选择
 * 使用时选择打开宏，使用外设扩展板打开#define BOARD_SELECT_IS_EXPANSION_BOARD这个宏
 * 使用Robot板打开#define BOARD_SELECT_IS_ROBOT_BOARD这个宏
 */
#define BOARD_SELECT_IS_EXPANSION_BOARD
//#define BOARD_SELECT_IS_ROBOT_BOARD
#ifdef BOARD_SELECT_IS_EXPANSION_BOARD
#define EXPANSION_BOARD
#else
#define ROBOT_BOARD
#endif

typedef enum {
    EAGE_0 = 0,
    EAGE_1,
    EAGE_2,
    EAGE_3
} GPioEage;
//LHH
typedef enum {
    hetun_0 = 2,
    hudieyu_1=3,
    jinyu_2=4,
    jiyu_3=5,
    luohanyu_4=6,
    longyu_5=7,
    nianyu_6=8,
    qingdaofu_7=9,
    shiban_8=10,
   
} refuseClassification;

typedef struct {
    unsigned char frameHeader[HISGNALLING_MSG_FRAME_HEADER_LEN];
    unsigned char hisignallingMsgBuf[HISIGNALLING_MSG_BUFF_LEN];
    unsigned int hisigallingMsgLen;
    unsigned char endOfFrame;
    unsigned int hisignallingCrc32Check;
} HisignallingProtocalType;

typedef enum {
    HISIGNALLING_RET_VAL_CORRECT = 0,
    HISIGNALLING_RET_VAL_ERROR,
    HISGNALLING_RET_VAL_MAX
} HisignallingErrorType;

/*
 * hisignalling协议函数声明
 */
unsigned int UartOpenInit(void);
void UartSendRead(int fd, refuseClassification refuseType);
static HisignallingErrorType HisignallingMsgReceive(int fd, unsigned char *buf, unsigned int len);
static unsigned int HisignallingMsgSend(int fd, char *buf, unsigned int dataLen);
unsigned int HisignallingMsgTask(void);

#endif