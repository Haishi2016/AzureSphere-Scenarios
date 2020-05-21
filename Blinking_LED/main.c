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

   const struct timespec sleepTime = {.tv_sec = 0, .tv_nsec = 500000000};
    while (true) {
        GPIO_SetValue(dataPin, GPIO_Value_Low);
        nanosleep(&sleepTime, NULL);
        GPIO_SetValue(dataPin, GPIO_Value_High);
        nanosleep(&sleepTime, NULL);
    }
}