#include "stdio.h"

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* LPCOpen includes*/
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

/* LPCOpen includes*/
#include "board.h"

/* Demo includes. */
#include "supporting_functions.h"

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT            ( 0xffffff )

/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/*-----------------------------------------------------------*/

int main( void )
{
   /* Create one of the two tasks. */
   xTaskCreate( vTask1,                   /* Pointer to the function that implements the task. */
                (const char *)"Task 1",   /* Text name for the task.  This is to facilitate debugging only. */
                configMINIMAL_STACK_SIZE, /* Stack depth - most small microcontrollers will use much less stack than this. */
                NULL,                     /* We are not using the task parameter. */
                tskIDLE_PRIORITY+2,       /* This task will run at priority 1. */
                NULL );                   /* We are not using the task handle. */

   
   /* Start the scheduler to start the tasks executing. */
   vTaskStartScheduler();

   /* The following line should never be reached because vTaskStartScheduler()
   will only return if there was not enough FreeRTOS heap memory available to
   create the Idle and (if configured) Timer tasks.  Heap management, and
   techniques for trapping heap exhaustion, are described in the book text. */
   for( ;; );
   return 0;
}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
   const char *pcTaskName = "Tarea 1 ON\r\n";
   volatile uint32_t ul;
   TickType_t xLastWakeTime; //Esta variable es para poder mostrar los tick

  
   for( ;; ) {
      vPrintString( pcTaskName );
      printf("Cuenta Ticks %d \r\n",xLastWakeTime ); 
      xLastWakeTime = xTaskGetTickCount();  //Me da la cuenta de los tick
      vPrintString("Tarea 1 creo Tarea 2\r\n" ); 
      vTaskDelay(500/ portTICK_RATE_MS);  
      xTaskCreate( vTask2, (const char *)"Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );   
      
      //DUDA porque cuando le doy mayor prioridad a Tarea 2 me aparece antes el "Tarea1 creo a tarea 2" y si las pongo iguales me aparece despues que se prende el LED
      
      }
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
   const char *pcTaskName = "Tarea 2 ON -> LED Amarillo ON\r\n";
   volatile uint32_t ul;


   for( ;; ) {
      
       vPrintString( pcTaskName );
       Board_LED_Toggle(3); /* Yellow */
       vTaskDelete(NULL);
      }
   
}

