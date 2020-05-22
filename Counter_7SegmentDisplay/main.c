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

unsigned char num[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

void _shiftOut(int dPin, int cPin, int order, int val) {
    int i;
    const struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 10000000};
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

int main(void)
{
    Log_Debug("Starting application...\n");

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

    const struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 500000000};
    while (true) {
        for (int i = 0; i < sizeof(num); i++) {
            GPIO_SetValue(latchPin, GPIO_Value_Low);
            _shiftOut(dataPin, clockPin, MSBFIRST, num[i]);
            GPIO_SetValue(latchPin, GPIO_Value_High);
            nanosleep(&sleepTime, NULL);
        }
        for (int i = 0; i < sizeof(num); i++) {
            GPIO_SetValue(latchPin, GPIO_Value_Low);
            _shiftOut(dataPin, clockPin, MSBFIRST, num[i] & 0x7f);
            GPIO_SetValue(latchPin, GPIO_Value_High);
            nanosleep(&sleepTime, NULL);
        }
    }
}