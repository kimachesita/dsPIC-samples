/* 
 * File:   error.h
 * Author: Kim Chesed Paller
 *
 * Created on May 28, 2014, 11:11 AM
 */


#ifndef ERROR_H
#define	ERROR_H

#ifdef	__cplusplus
extern "C" {
#endif

#define RETURN_NORMAL                                       0x00
#define ERROR_SCH_TOO_MANY_TASKS                            0x01
#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK                  0x02
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER     0x03
#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START          0x04
#define ERROR_SCH_LOST_SLAVE                                0x05
#define ERROR_SCH_CAN_BUS_ERROR                             0x06
#define ERROR_SCH_CANNOT_DELETE_TASK                        0x07
#define RETURN_ERROR                                        0xFF




#ifdef	__cplusplus
}
#endif

#endif	/* ERROR_H */

