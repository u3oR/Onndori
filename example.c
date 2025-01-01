#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include "Onndori.h"

uint64_t GetPlatformTicks(void)
{
#if defined(__linux__)
    struct timeval tTime = {0};
    gettimeofday(&tTime, NULL);
    return tTime.tv_sec;
    // return tTime.tv_sec * 1000 + tTime.tv_usec / 1000;
#else
    return 0;
#endif
}

void TimCallback(OnndoriTimer_st *pTimer, void *pUserData)
{
    static int iTim7 = 0;

    printf("Timer %s fired at %lu s \n", (const char *)(pUserData), GetPlatformTicks());
    
    switch (*(const char *)pUserData)
    {
        case '7':
        {
            iTim7 += 1;
            if (iTim7 >= 10)
            {
                Onndori_Stop(pTimer);
                printf("Tim7 Stopped %d\n", iTim7);
                return;
            }
            break;
        }

        case '8':
        {
            printf("Tim8 Shoot \n");
        }
        
        default:
            break;
    }

}

int main()
{
    OnndoriTimer_st tTim0;
    OnndoriTimer_st tTim1;
    OnndoriTimer_st tTim2;
    OnndoriTimer_st tTim3;
    OnndoriTimer_st tTim4;
    OnndoriTimer_st tTim5;
    OnndoriTimer_st tTim6;
    OnndoriTimer_st tTim7;
    OnndoriTimer_st tTim8;
    OnndoriTimer_st tTim9;

    Onndori_Init();
    Onndori_SetTick(GetPlatformTicks);

    Onndori_Start(&tTim0, ONNODORI_PERIODIC,    9, TimCallback, "0");
    Onndori_Start(&tTim1, ONNODORI_PERIODIC,    3, TimCallback, "1");
    Onndori_Start(&tTim2, ONNODORI_PERIODIC,    5, TimCallback, "2");
    Onndori_Start(&tTim3, ONNODORI_ONESHOT,     7, TimCallback, "3");
    Onndori_Start(&tTim4, ONNODORI_PERIODIC,    5, TimCallback, "4");
    Onndori_Start(&tTim5, ONNODORI_PERIODIC,    6, TimCallback, "5");
    Onndori_Start(&tTim6, ONNODORI_PERIODIC,    2, TimCallback, "6");
    Onndori_Start(&tTim7, ONNODORI_PERIODIC,    1, TimCallback, "7");
    Onndori_Start(&tTim8, ONNODORI_ONESHOT,    12, TimCallback, "8");
    Onndori_Start(&tTim9, ONNODORI_PERIODIC,   10, TimCallback, "9");

    while (1) 
    {
        sleep(Onndori_Yield());
        // usleep(Onndori_Yield() * 1000);
    }
    
    return 0;
}
