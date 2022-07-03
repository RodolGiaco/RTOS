/*  Copyright 2020 - FreeRTOS Kernel V10.0.1

 * este codigo NO esta en el libro de Sistemas Empotrados en tiempo real
 * pero el objetivo es intriducir a FreeRTOS usando dos tareas que
 * imprime por el puerto serie, e invocando al planificador

    -  vTaskDelayUntil()
    -  xTaskGetTickCount()  --- solo una vez .. despues se encarga el operativo

*/

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "board.h" //definicion de leds
#include "stdio.h"

/* Demo includes. */
#include "supporting_functions.h"

TaskHandle_t xTask2Handle; //variable global guardara id de tarea2
const char *pcTextForTask1 = "Tarea1 Ejecutando bucle\r\n";

void vTask1( void *pvParameters )
{

   UBaseType_t uxPriority;
   uxPriority = uxTaskPriorityGet( NULL ); //NULL retorna la propia prioridad

   char *pcTaskName;
   pcTaskName = ( char * ) pvParameters; //casteo de void a char




   const TickType_t xDelay50ms = pdMS_TO_TICKS( 50UL );

   for( ;; ) {
      vPrintString( pcTaskName );
      vTaskPrioritySet( xTask2Handle, ( uxPriority + 1 ) ); // Tarea2 mas prioritaria
   }
}
/*-----------------------------------------------------------*/

void vTask2( void *pvParameters )
{
  UBaseType_t uxPriority;
  uxPriority = uxTaskPriorityGet( NULL ); //NULL retorna la propia prioridad
  TickType_t xLastWakeTime;
  const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL );
  xLastWakeTime = xTaskGetTickCount();


  for( ;; ) {
     vPrintString( "Tarea2 ON\r\n" );
     vPrintString( "Disminuyo mi prioridad Tarea2\r\n" );
     Board_LED_Set(3, TRUE); //prende "LED 3" ( verde )
     vTaskPrioritySet( NULL, ( uxPriority - 2 ) ); //NULL propio ID
     vTaskDelayUntil( &xLastWakeTime, xDelay500ms );
  }
}

/*-----------------------------------------------------------*/

int main( void )
{
   xTaskCreate( vTask1, "Task 1", configMINIMAL_STACK_SIZE, (void*)pcTextForTask1, tskIDLE_PRIORITY+2, NULL );
   xTaskCreate( vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,&xTask2Handle );

	vTaskStartScheduler(); /* y por �ltimo se arranca el planificador . */
    //Nunca llegara a ese lazo  .... espero

   for( ;; );
   return 0;
}
/*-----------------------------------------------------------*/
