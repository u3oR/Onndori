#ifndef _ONNDORI_H
#define _ONNDORI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__linux__)
    #include <pthread.h>
    typedef pthread_mutex_t OnndoriLock_t;
    #define ONNDORI_LOCK_INIT PTHREAD_MUTEX_INITIALIZER
#elif defined(USE_FREERTOS)

#else
    typedef volatile uint32_t OnndoriLock_t;
#endif


typedef enum _OnndoriStat 
{
    ONNODORI_IDLE,
    ONNODORI_RUNNING,
    ONNODORI_EXPIRED,
} OnndoriStat_et;


typedef enum _OnndoriType 
{
    ONNODORI_ONESHOT,
    ONNODORI_PERIODIC,
} OnndoriType_et;


typedef struct _OnndoriTimer
{
    struct _OnndoriTimer *pNext;

    OnndoriType_et eType;
    OnndoriStat_et eStat;

    uint64_t uIntervalTime;
    uint64_t uNextTrigger;

    struct {
        void (*Callback)(struct _OnndoriTimer *pTimer, void *pUserData);
        void *pUserData;
    } cb;
    
} OnndoriTimer_st;

typedef uint64_t (*PlatformTick_ft)(void);
typedef void (*OnndoriCallback_ft)(struct _OnndoriTimer *pTimer, void *pUserData);

int Onndori_Init(void);
int Onndori_SetTick(PlatformTick_ft PlatformTick);
int Onndori_Start(OnndoriTimer_st *pTimer, OnndoriType_et eType, uint64_t uTime, OnndoriCallback_ft Cb, void *pUserData);
int Onndori_Stop(OnndoriTimer_st *pHandle);
int Onndori_Yield(void);

OnndoriStat_et Onndori_GetStat(OnndoriTimer_st *pHandle);

#ifdef __cplusplus
}
#endif

#endif

// end of file
