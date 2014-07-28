/* 
 * File:   scheduler.h
 * Author: Kim Chesed Paller
 *
 * Created on May 21, 2014, 6:59 PM
 */

#ifndef SCHEDULER_H
#define	SCHEDULER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "typedefs.h"
#include "error.h"
#include <xc.h>

typedef struct{
    //pointer to the task
    void (*pTask)(void);

    // Delay (ticks) until the function will (next) be run
    volatile tWord delay;

    // Interval (ticks) between subsequent runs.
    volatile tWord period;

    // Incremented (by scheduler) when task is due to execute
    volatile tByte run_me;

} sTask;

// The maximum number of tasks required at any one time
#define SCH_MAX_TASKS 1

// Comment out to disable scheduler reporting
#define SCH_REPORT_ERRORS

extern void SCHInit(void);
extern tByte SCHAddTask(void (*pFunction)(void),const tWord delay,const tWord period);
extern void SCHDispatchTask(void);
extern void SCHStart(void);
extern tByte SCHDeleteTask(tByte);
extern void SCHReportStatus(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SCHEDULER_H */

