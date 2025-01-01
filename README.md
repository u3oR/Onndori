
# **Onndori**

[👉中文](README_zh-cn.md)

**Onndori** is a lightweight and portable software timer library for embedded systems, supporting both one-shot and periodic timers. It is simple to integrate, platform-agnostic, and thread-safe.


This project is inspired by [MultiTimer](https://github.com/0x1abin/MultiTimer) Project, and incorporates some self-perceived optimizations and improvements based on its core design. The Onndori component retains its lightweight and efficient structure while enhancing usability, portability, and certain platform-specific implementations.

---

## **Features**

- **Timer Modes**:
  - One-shot: Fires once after a specified interval.
  - Periodic: Fires repeatedly at a specified interval.
- **Platform Support**:
  - Compatible with Linux, FreeRTOS, or bare-metal environments.
  - Customizable tick function for system-specific timekeeping.
- **Thread Safety**:
  - Uses platform-specific locking mechanisms.
- **Efficient Scheduling**:
  - Maintains timers in ascending order of their next trigger time.
- **Callback Mechanism**:
  - Executes user-defined functions when timers expire.

---

## **Example**

Below is an excerpt from `example.c` demonstrating important parts of the timer usage:

```c
#include "Onndori.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

// Platform-specific tick function
uint64_t GetPlatformTicks(void) {
    struct timeval tTime = {0};
    gettimeofday(&tTime, NULL);
    return tTime.tv_sec * 1000 + tTime.tv_usec / 1000; // Milliseconds
}

// Timer callback function
void TimCallback(OnndoriTimer_st *pTimer, void *pUserData)
{
    static int iTim7 = 0;

    printf("Timer %s fired at %lu s \n", (const char *)(pUserData), GetPlatformTicks());
    
    switch (*(const char *)pUserData)
    {
        case '7':
            iTim7 += 1;
            if (iTim7 >= 10)
            {
                Onndori_Stop(pTimer);
                printf("Tim7 Stopped %d\n", iTim7);
            }
            break;

        case '8':
            printf("Tim8 Stopped\n");
            break;
        
        default:
            break;
    }

}

int main() 
{
    OnndoriTimer_st tTim6;
    OnndoriTimer_st tTim7;
    OnndoriTimer_st tTim8;

    // Initialize the timer library
    Onndori_Init();

    // Set the tick provider
    Onndori_SetTick(GetPlatformTicks);

    // Start timers
    Onndori_Start(&tTim6, ONNODORI_PERIODIC,  200, TimCallback, "6"); /* Periodic */
    Onndori_Start(&tTim7, ONNODORI_PERIODIC,  100, TimCallback, "7"); /* Periodic */
    Onndori_Start(&tTim8, ONNODORI_ONESHOT,  1200, TimCallback, "8"); /* One-shot */

    // Main loop
    while (1) 
    {
        usleep(Onndori_Yield() * 1000);
    }

    return 0;
}

```

For the complete code, refer to `example.c` in the repository.

---

## **License**

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.