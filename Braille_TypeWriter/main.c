#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <applibs/log.h>
#include <applibs/gpio.h>
#include <hw/sample_hardware.h>

typedef enum {
    ExitCode_Success = 0,

    ExitCode_Main_Led = 1
} ExitCode;

const int LSBFIRST = 1;
const int MSBFIRST = 2;
unsigned char buffer[] = {'H','E','L','L','O',' '};
unsigned char charLoc = 0;

unsigned char braille[]={0x00,  //space
    0x20,   //A - 100 000   
    0x30,   //B - 110 000
    0x24,   //C - 100 100
    0x26,   //D - 100 110
    0x22,   //E - 100 010
    0x34,   //F - 110 100
    0x36,   //G - 110 110
    0x32,   //H - 110 010
    0x14,   //I - 010 100
    0x16,   //J - 010 110
    0x28,   //K - 101 000
    0x38,   //L - 111 000
    0x2C,   //M - 101 100
    0x2E,   //N - 101 110
    0x2A,   //O - 101 010
    0x3C,   //P - 111 100
    0x3E,   //Q - 111 110
    0x3A,   //R - 111 010
    0x1C,   //S - 011 100
    0x1E,   //T - 011 110
    0x29,   //U - 101 001
    0x39,   //V - 111 001
    0x17,   //W - 010 111
    0x2D,   //X - 101 101
    0x2F,   //Y - 101 111
    0x2B,   //Z - 101 011
    0x0F   //#
}; //[Space]A-Z#1
unsigned char masks[] = {0x38, 0x7, 0x0};

void _shiftOut(int dPin, int cPin, int order, int val) {
    int i;
    const struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 10000};
    for ( i = 0; i < 8; i++) {
        GPIO_SetValue(cPin, GPIO_Value_Low);
        if (order == LSBFIRST) {
            GPIO_SetValue(dPin, ((0x01&(val>>i)) == 0x01) ? GPIO_Value_High : GPIO_Value_Low);
            nanosleep(&sleepTime, NULL);
        } else {
            GPIO_SetValue(dPin,((0x80&(val<<i)) == 0x80) ? GPIO_Value_High : GPIO_Value_Low);
            nanosleep(&sleepTime, NULL);
        }
        GPIO_SetValue(cPin,GPIO_Value_High);
        nanosleep(&sleepTime, NULL);
    }
}
int charToBrailleIndex(unsigned char ch) {
    if (ch >= 65 && ch <= 90) {
        return ch - 64;
    } else if (ch >= 97 && ch <= 122) {
        return ch - 96;
    }else if (ch >= 48 && ch <= 57) {
        return ch - 47;
    } else {
        return 0;
    }
}
unsigned char makeColumn(int col) {
    unsigned char ret = ((braille[charToBrailleIndex(buffer[col/3])] & masks[col % 3 ]) << (col%3 == 1 ? 4 : 1)) | ((braille[charToBrailleIndex(buffer[col/3+3])] & masks[col % 3 ]) >> (col %3 ==0 ? 3: 0));
    return ret;
}
void printBuffer(int dataPin, int latchPin, int clockPin) {
     const struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 50};
    int i, j, k;
    unsigned char x;
  
    x = 0x80;
    for (i = 0; i < 8; i++) {
        GPIO_SetValue(latchPin, GPIO_Value_Low);
        _shiftOut(dataPin, clockPin, LSBFIRST, makeColumn(7-i));
        _shiftOut(dataPin, clockPin, MSBFIRST, ~x);
        GPIO_SetValue(latchPin, GPIO_Value_High);
        x>>=1;
    }
}

int main(void)
{
    Log_Debug("Starting the application...\n");

   int dataPin = GPIO_OpenAsOutput(MT3620_RDB_HEADER1_PIN4_GPIO, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (dataPin == -1) {
        Log_Debug(
            "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return ExitCode_Main_Led;
    }

    int latchPin = GPIO_OpenAsOutput(MT3620_RDB_HEADER1_PIN6_GPIO, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (latchPin == -1) {
        Log_Debug(
            "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return ExitCode_Main_Led;
    }

    int clockPin = GPIO_OpenAsOutput(MT3620_RDB_HEADER1_PIN8_GPIO, GPIO_OutputMode_PushPull, GPIO_Value_High);
    if (clockPin == -1) {
        Log_Debug(
            "Error opening GPIO: %s (%d). Check that app_manifest.json includes the GPIO used.\n",
            strerror(errno), errno);
        return ExitCode_Main_Led;
    }

    while (true) {
       printBuffer(dataPin, latchPin, clockPin);
    }
}