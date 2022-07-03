/*  Copyright 2020 - FreeRTOS Kernel V10.0.1

 * este codigo NO esta en el libro de Sistemas Empotrados en tiempo real
 * pero el objetivo es intriducir a FreeRTOS usando solo una (o dos) tarea que
 * imprime por el puerto serie, e invocando al planificador
 *
 *  - xTaskCreate()
 *  - vTaskStartScheduler()
 *  - vTaskDelay(ticks)
 *
 *  */
 /*==================[inclusions]=============================================*/

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

/*==================[macros and definitions]=================================*/

#define TAM_PILA 150

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
const char *pcTextoTarea1 = "\t Tarea1 ON\r ";
const char *pcTextoTarea2 = "\t Tarea2 ON\r ";

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void vTarea(void *pvParameters)
{

   UBaseType_t uxPriority;
   uxPriority = uxTaskPriorityGet( NULL ); //NULL retorna la propia prioridad

   char *pcTaskName;
   pcTaskName = ( char * ) pvParameters;

   TickType_t xLastWakeTime;
   xLastWakeTime = xTaskGetTickCount();
   for ( ;; ){

       printf( pcTaskName );
       printf("Prioridad = %d\n",uxPriority );
       printf("Cuenta Ticks %d \n\r",xLastWakeTime);
       vTaskDelay( 1000/ portTICK_RATE_MS);
       
   }
}


/*==================[external functions definition]==========================*/

int main(void)
{
    //Se inicializa HW
	/* Se crean las tareas */
	xTaskCreate(vTarea, (const char *)"Tarea1", TAM_PILA, (void*)pcTextoTarea1, tskIDLE_PRIORITY, NULL );
	xTaskCreate(vTarea, (const char *)"Tarea2", TAM_PILA, (void*)pcTextoTarea2, tskIDLE_PRIORITY+2, NULL );

	vTaskStartScheduler(); /* y por último se arranca el planificador . */
    //Nunca llegara a ese lazo  .... espero
     for( ;; );
     return 0;
}

/*==================[end of file]============================================*/
