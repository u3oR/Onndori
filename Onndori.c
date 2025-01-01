#include "Onndori.h"

static OnndoriTimer_st *s_pList = NULL;
static PlatformTick_ft  s_PlatformTick = NULL;
static OnndoriLock_t    s_tLock;

#if defined(__linux__)
    #define Onndori_Lock()      pthread_mutex_lock(&s_tLock)
    #define Onndori_UnLock()    pthread_mutex_unlock(&s_tLock)
    #define Onndori_LockInit()  pthread_mutex_init(&s_tLock, NULL)
#elif defined(USE_FREERTOS)
    // TODO
#else
    #define Onndori_Lock()      do{ s_tLock = 1; } while (0)
    #define Onndori_UnLock()    do{ s_tLock = 0; } while (0)
    #define Onndori_LockInit()  do{ s_tLock = 1; } while (0)
#endif


static uint64_t GetTicks(void)
{
    return s_PlatformTick ? s_PlatformTick() : 0;
}


static void RemoveTimer(OnndoriTimer_st *pTimer)
{
    OnndoriTimer_st **pIter = &s_pList;

    while (*pIter) {
        if (*pIter == pTimer) {
            *pIter = pTimer->pNext;
            pTimer->eStat = ONNODORI_IDLE;
            break;
        }
        pIter = &((*pIter)->pNext);
    }
}


static void InsertTimer(OnndoriTimer_st *pTimer)
{
    OnndoriTimer_st **pIter = &s_pList;

    /* insert in ascending order of next trigger time */
    while (*pIter && ((*pIter)->uNextTrigger < pTimer->uNextTrigger)) {
        pIter = &((*pIter)->pNext);
    }
    pTimer->pNext = *pIter;
    *pIter = pTimer;
}


int Onndori_Init(void)
{
    Onndori_LockInit();
    return 0;
}


int Onndori_SetTick(PlatformTick_ft PlatformTick)
{
    if (PlatformTick) {
        s_PlatformTick = PlatformTick;
        return 0;
    }
    return -1;
}


int Onndori_Start(OnndoriTimer_st *pTimer, OnndoriType_et eType, uint64_t uTime, OnndoriCallback_ft Callback, void *pUserData)
{
    if (pTimer == NULL || uTime == 0 || Callback == NULL || s_PlatformTick == NULL)
    {
        return -1;
    }
    
    Onndori_Lock();
    RemoveTimer(pTimer);
    pTimer->eType = eType;
    pTimer->uIntervalTime = uTime;
    pTimer->uNextTrigger = GetTicks() + uTime;
    pTimer->eStat = ONNODORI_RUNNING;
    pTimer->cb.Callback = Callback;
    pTimer->cb.pUserData = pUserData;
    InsertTimer(pTimer);
    Onndori_UnLock();
    return 0;
}


int Onndori_Stop(OnndoriTimer_st *pTimer)
{
    Onndori_Lock();
    if (pTimer) {
        RemoveTimer(pTimer);
        Onndori_UnLock();
        return 0;
    }
    Onndori_UnLock();
    return -1;
}


int Onndori_Yield(void)
{
    if (s_PlatformTick == NULL)
    {
        return -1;
    }
    
    OnndoriTimer_st *pTimer = NULL;

    uint64_t uCurrentTicks = GetTicks();

    Onndori_Lock();
    /* find timer that needs to be triggered */
    while (s_pList && (uCurrentTicks >= s_pList->uNextTrigger))
    {
        pTimer = s_pList;
        s_pList = pTimer->pNext; /* remove this timer from list first */

        /* in periodic mode */
        if (pTimer->eType == ONNODORI_PERIODIC) {
            /* update next trigger time */
            pTimer->uNextTrigger = uCurrentTicks + pTimer->uIntervalTime;
            /* add this timer back to the list */
            InsertTimer(pTimer); 
        }else {
            /* only trigger once in one-shot mode */
            pTimer->eStat = ONNODORI_EXPIRED;
        }
        
        if (pTimer->cb.Callback != NULL)
        {
            OnndoriCallback_ft Callback = pTimer->cb.Callback;
            void *pUserData = pTimer->cb.pUserData;

            Onndori_UnLock();
            Callback(pTimer, pUserData);
            Onndori_Lock();
        }
        
    }

    /* get the minimum time interval required for the next trigger */
    const int iNextTimeout = s_pList ? (int)(s_pList->uNextTrigger - uCurrentTicks) : 0;
    
    Onndori_UnLock();

    return iNextTimeout;
}


OnndoriStat_et Onndori_GetStat(OnndoriTimer_st *pTimer)
{
    return pTimer ? pTimer->eStat : ONNODORI_IDLE;
}

// end of file
