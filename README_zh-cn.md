### **Onndori**

**Onndori** 是一个轻量级且可移植的软件定时器库，适用于嵌入式系统。它支持单次触发（one-shot）和周期性触发（periodic）定时器，设计简洁，易于集成，具备跨平台能力，并支持线程安全。


本项目参考了 [MultiTimer](https://github.com/0x1abin/MultiTimer) 项目 的设计理念，并在此基础上进行了自认为的优化和改进。Onndori 组件保留了其轻量化和高效的核心特性，同时在可用性、可移植性以及某些平台特性实现上做出了增强。

---

## **功能特点**

- **定时器模式**:
  - **单次触发 (One-shot)**: 定时器在指定时间触发一次。
  - **周期性触发 (Periodic)**: 定时器在指定的时间间隔内反复触发。
- **跨平台支持**:
  - 兼容 Linux、FreeRTOS 或裸机（bare-metal）环境。
  - 支持自定义时间获取函数以适配不同平台的时钟机制。
- **线程安全**:
  - 使用平台相关的锁机制，适用于多线程环境。
- **高效调度**:
  - 按下次触发时间的升序维护定时器队列，保证高效的定时器管理。
- **回调机制**:
  - 定时器到期时执行用户定义的回调函数，支持传递用户自定义数据。

---

## **使用示例**

以下代码展示了 `example.c` 中的关键使用部分：

```c
#include "Onndori.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

// 平台相关的时间获取函数
uint64_t GetPlatformTicks(void) {
    struct timeval tTime = {0};
    gettimeofday(&tTime, NULL);
    return tTime.tv_sec * 1000 + tTime.tv_usec / 1000; // 毫秒级时间
}

// 定时器回调函数
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

    // 初始化
    Onndori_Init();

    // 设置时间获取函数
    Onndori_SetTick(GetPlatformTicks);

    // 启动定时器
    Onndori_Start(&tTim6, ONNODORI_PERIODIC,  200, TimCallback, "6"); /* 周期触发 */
    Onndori_Start(&tTim7, ONNODORI_PERIODIC,  100, TimCallback, "7"); /* 周期触发 */
    Onndori_Start(&tTim8, ONNODORI_ONESHOT,  1200, TimCallback, "8"); /* 单次触发 */

    // 主循环
    while (1) 
    {
        usleep(Onndori_Yield() * 1000);
    }

    return 0;
}
```

完整代码请参考仓库中的 `example.c` 文件。

---

## **许可证**

本项目基于 MIT 许可证发布，详情请参阅 [LICENSE](LICENSE) 文件。