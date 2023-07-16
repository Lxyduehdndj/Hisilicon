

#ifndef APP_DEMO_I2C_OLED_H
#define APP_DEMO_I2C_OLED_H

unsigned int OledInit(void);
void OledSetPosition(unsigned char x, unsigned char y);
void OledFillScreen(unsigned char fiiData);
void OledPositionCleanScreen(unsigned char fillData, unsigned char line,
    unsigned char pos, unsigned char len);
void OledShowChar(unsigned char x, unsigned char y, unsigned char chr, unsigned char charSize);
void OledShowStr(unsigned char x, unsigned char y, unsigned char *chr, unsigned char charSize);
unsigned char *FlaotToString(double d, unsigned char *str);
void AllLedOff(void);

void OLED_SetPos(unsigned char x, unsigned char y); //设置起始点坐标
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);

#endif