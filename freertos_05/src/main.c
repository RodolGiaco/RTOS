

#include "board.h"
#include "chip.h"
#include "string.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
//#include "semphr.h"
#include "queue.h"
#include "stdlib.h"


# define PRIO_T3 1	//menos prioritaria
# define PRIO_T1 3
# define PRIO_T2 2
# define TAM_PILA 256

# define TAM_COLA 20 /* 20 mensajes */
# define TAM_MSG 4  //"uno\n\0"

QueueHandle_t cola_tiempos;


static void Tarea1 (void * pvParameters)
{
	char t [5];
	//char t="1";
  BaseType_t xStatus;
  uint32_t ul;
	strcpy (t,"1000");
	while (1)
	{
	//  Proceso Tarea1 //
	//	for(ul=0;ul<0xffffff;ul++){}
  //  vTaskDelay(8000 / portTICK_RATE_MS);

		vPrintString("Tarea 1 corriendo\r\n");
		xStatus=xQueueSend (cola_tiempos, (void *)t, portMAX_DELAY);
    if ( xStatus == pdPASS )
		{
		  	Board_LED_Toggle(5);
		}
		vPrintString("Debe encenderse por 1 segundo\r\n");
		vTaskDelay(2000/ portTICK_RATE_MS);
	}


}
static void Tarea2 (void * pvParameters)
{
	char t [5];
  BaseType_t xStatus;
	strcpy (t,"3000");

	while (1)
	{
	  vPrintString("Tarea 2 corriendo\r\n");
	  xStatus = xQueueSend (cola_tiempos, (void *)t, portMAX_DELAY);
	  vPrintString("Debe encenderse por 3 segundos\r\n");
	  vTaskDelay(2000 / portTICK_RATE_MS);
	}

}
static void Tarea3 (void * pvParameters )
{

 char t [5];
 TickType_t xLastWakeTime;
 const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL );
 const TickType_t xDelay1000ms = pdMS_TO_TICKS( 1000UL );
 const TickType_t xDelay3000ms = pdMS_TO_TICKS( 3000UL );
 xLastWakeTime = xTaskGetTickCount();
 TickType_t xDelay;
 int c;
 while (1)
 {

		 vPrintString("Tarea 3 corriendo\r\n");
     if((xQueueReceive (cola_tiempos , (void *)t,(portTickType) 0xFFFFFFFF )) == pdTRUE )
		 {
			 c=atoi(t);
			 xDelay=pdMS_TO_TICKS(c);
			 printf("%d\r\n",c);
			 if(c==1000)
			 {

			      vPrintString( "Led prendido 1 seg\r\n");
      			Board_LED_Set(3,TRUE);
				    vTaskDelayUntil( &xLastWakeTime, xDelay1000ms);
			      vPrintString( "Led apagado 0,5 seg\r\n");
				    Board_LED_Set(3,FALSE);
				    vTaskDelayUntil( &xLastWakeTime, xDelay500ms );
			  }
		       if(c==3000)
			     {
				      vPrintString( "Led prendido 3 seg\r\n");
      				Board_LED_Set(3,TRUE);
							vTaskDelayUntil( &xLastWakeTime, xDelay3000ms );
			  			vPrintString( "Led apagado 0,5 seg\r\n");
							Board_LED_Set(3,FALSE);
							vTaskDelayUntil( &xLastWakeTime, xDelay500ms );
				    }
		    }else Board_LED_Toggle(2);
	}
}


/*==================[external functions definition]==========================*/

int main(void)
{

	cola_tiempos = xQueueCreate (TAM_COLA, TAM_MSG);

	/* Se crean las tareas */
	xTaskCreate (Tarea3, (const char *)"Tarea3", TAM_PILA, NULL,PRIO_T3, NULL);
	xTaskCreate (Tarea1, (const char *)"Tarea1", TAM_PILA, NULL,PRIO_T1, NULL);
	xTaskCreate (Tarea2, (const char *)"Tarea2", TAM_PILA , NULL,PRIO_T2, NULL);

	vTaskStartScheduler(); /* y por último se arranca el planificador . */
}

/** @} doxygen end group definition */

/*==================[end of file]============================================*/
