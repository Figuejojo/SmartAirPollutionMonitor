/** @file Debug _print.h
 *  @brief The file contains the print function declarations for debugging thruogh 
 *          the USB in Raspberry pi pico, without bloquing the program.  
 *
 *  @author Y3913624
 */

#ifndef _DEBUG_PRINT_H_
#define _DEBUG_PRINT_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"

/*******************************************************************************
* Type definitions
*******************************************************************************/
#define DEBUGMSG_SIZE (30)  // Max Message length

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/** @name 	debug_queue_setup
*   @brief Setup debug communication and RTOS Queue
*
*   @param 	Void
*   @return Void
*/
void debug_queue_setup(void);

/** @name 	Print_debug
*   @brief  Print_debug function to be used. Max length can be changed in Debug_print.h
*
*   @param 	msg array of characters
*   @return Void
*/
void Print_debug(int8_t msg[]);

/** @name 	
*   @brief Concurrent printing task for debugging. It uses xqueues.
*
*   @param 	Void * parameters
*   @return Void
*/
void TaskDebugPrint(void * pvParameters);

#endif //_DEBUG_PRINT_H_
