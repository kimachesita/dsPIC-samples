/*
 * File:   scheduler.c
 * Author: Kim Chesed Paller
 *
 * Created on May 21, 2014, 4:15 PM
 */

#include "scheduler.h"

/*--------------------GLOBAL SCHEDULER VARIABLES--------------------*/

//The array of task
sTask SCH_tasks_G[SCH_MAX_TASKS];

#ifdef SCH_REPORT_ERRORS
//Global Error Variable
volatile tByte Error_code_G;
volatile tByte Last_error_code_G;
volatile tWord Error_tick_count_G;
#endif

/*------------------------------------------------------------------*-
SCHInit()
Scheduler initialization function. Prepares scheduler
data structures and sets up timer interrupts at required rate.
Must call this function before using the scheduler.
-*------------------------------------------------------------------*/

void SCHInit(void){
    tByte i;
    for(i = 0;i < SCH_MAX_TASKS; i++ ){
        SCHDeleteTask(i);
    }
    Error_code_G = 0;

    //setup timer 1 (16 Bit in timer mode)
    T1CONbits.TON = 0; // Disable Timer

    T1CONbits.TCS = 0; // Select internal instruction cycle clock
    T1CONbits.TGATE = 0; // Disable Gated Timer mode
    T1CONbits.TCKPS = 0b11; // Select 1:256 Prescaler

    TMR1 = 0x00; // Clear timer register
    PR1 = 156; // Load the period value for 1ms Period

    IPC0bits.T1IP = 0x01; // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer1 interrupt

    //timer1 can be started using SCHStart();
}

/*-----------------------------------------------------------------*-
_T1Interrupt
This is the scheduler ISR(Update Function). It is called at a rate
determined by the timer settings in the 'init' function.
-*-----------------------------------------------------------------*/

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    tByte i;

    for(i = 0;i < SCH_MAX_TASKS; i++){
        //check if there is a task at this location
        if(SCH_tasks_G[i].pTask){
            if(SCH_tasks_G[i].delay == 0){
                //if a task is due to run
                //increment the run_me flag
                SCH_tasks_G[i].run_me += 1;
                if(SCH_tasks_G[i].period){
                    //schedule periodic task to run again
                    SCH_tasks_G[i].delay = SCH_tasks_G[i].period;
                }
            }else{
                //not yet ready to run just decrement the delay
                SCH_tasks_G[i].delay -= 1;
            }
        }
    }
    IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
}

/*------------------------------------------------------------------*-
 SCHAddTask(void (*)(void),const tWord,const tWord)
Causes a task (function) to be executed at regular intervals
or after a user-defined delay
-*------------------------------------------------------------------*/

tByte SCHAddTask(void (*pFunction)(void),const tWord delay,const tWord period){
    tByte i = 0;

    //first find a gap at an array if there is one
    while((SCH_tasks_G[i].pTask != 0) && (i < SCH_MAX_TASKS)){
        i++;
    }
    //Have we reach the end of the list?
    if(i == SCH_MAX_TASKS){
        //task list if full
        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
    }else{
        //if there is available space
        SCH_tasks_G[i].pTask = pFunction;
        SCH_tasks_G[i].delay = delay;
        SCH_tasks_G[i].period = period;
        SCH_tasks_G[i].run_me = 0;
    }
    //return index
    return i;
}

/*--------------------------------------------------------------------*-
SCHDispatchTask()
This is the 'dispatcher' function. When a task (function)
is due to run, SCHDispatchTask will run it.
This function must be called (repeatedly) from the main loop.
-*---------------------------------------------------------------------*/

void SCHDispatchTask(void){
    tByte i;

    //Dispatches the next tasks if one is ready
    for(i = 0;i < SCH_MAX_TASKS;i++){
        if(SCH_tasks_G[i].run_me > 0){
            (*SCH_tasks_G[i].pTask)();  //run the task
            SCH_tasks_G[i].run_me -= 1; //reduce/reset the run_me flag
        }
        // Periodic Task will run again
        // If this is one shot , remove from array
        if(SCH_tasks_G[i].period == 0){
            SCHDeleteTask(i);
        }
    }

    //report scheduler status
    SCHReportStatus();

    //Scheduler can take idle mode now to reduce power consumption
    //SCHGoToSleep();
}

/*---------------------------------------------------------------*-
* SCHStart() , enables the scheduler main timer,
* should be done only when necessary timer registers
* are initialized and scheduler task are added
-*---------------------------------------------------------------*/
void SCHStart(){
    T1CONbits.TON = 1; // Start Timer1
}

tByte SCHDeleteTask(tByte Id){
    tByte return_code;
    if(SCH_tasks_G[Id].pTask == 0){
        //no task at these location
        //set the global error variable
        //and return error code
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
        return_code = RETURN_ERROR;
    }else{
        return_code = RETURN_NORMAL;
    }
    SCH_tasks_G[Id].pTask = 0x000000;
    SCH_tasks_G[Id].delay = 0;
    SCH_tasks_G[Id].period = 0;
    SCH_tasks_G[Id].run_me = 0;

    return return_code; // return status
}

/*----------------------------------------------------------------*-
 * SCHReportStatus()
 * Error reporting routine
 * called at the 'Update' Function
 -*----------------------------------------------------------------*/

void SCHReportStatus(void){
    #ifdef SCH_REPORT_ERRORS
    // ONLY APPLIES IF WE ARE REPORTING ERRORS
    // Check for a new error code
    if (Error_code_G != Last_error_code_G)
    {
        // Negative logic on LEDs assumed
        //Error_port = 255 - Error_code_G;
        Last_error_code_G = Error_code_G;
        if (Error_code_G != 0){
            Error_tick_count_G = 60000;
        }else{
            Error_tick_count_G = 0;
        }

    }else{

        if (Error_tick_count_G != 0){
            if (--Error_tick_count_G == 0){
                Error_code_G = 0; // Reset error code
            }
        }

    }
    #endif
}