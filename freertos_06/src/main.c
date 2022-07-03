

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*==================[macros and definitions]=================================*/

#define PRIO_TAREA1 1
#define PRIO_TAREA2 2
#define TAM_PILA 150
#define mainDELAY_LOOP_COUNT        ( 0xffffff )

SemaphoreHandle_t sem_exclu;


static void vTarea1(void *pvParameters)
{
   TickType_t xLastWakeTime;
   const TickType_t xDelay1000ms = pdMS_TO_TICKS( 1000UL );
   for( ;; ) {
   xSemaphoreTake( sem_exclu, portMAX_DELAY);
   xLastWakeTime=xTaskGetTickCount();
	 vPrintString("Tarea 1 en espera activa\r\n");

	 while((xTaskGetTickCount()-xLastWakeTime)<xDelay1000ms)
   {}
   xSemaphoreGive(sem_exclu);
   }
}

static void vTarea2(void *pvParameters)
{
   const char *pcTaskName = "Tarea 2 menos prioritaria \r\n";
   const TickType_t xDelay200ms = pdMS_TO_TICKS( 200UL );


   for( ;; ) {

	 vTaskDelay( xDelay200ms );
   xSemaphoreTake( sem_exclu, portMAX_DELAY);
	 vPrintString("Cambiando estado de led\r\n");
	 Board_LED_Toggle(2);
   xSemaphoreGive(sem_exclu);

   }
}

/*==================[external functions definition]==========================*/

int main(void)
{

	sem_exclu = xSemaphoreCreateBinary ();
//	sem_exclu = xSemaphoreCreateMutex ();

  xSemaphoreGive (sem_exclu); //caso contrario ninguna tarea trabaja
	xTaskCreate(vTarea1, (const char *)"Tarea1", TAM_PILA, NULL, PRIO_TAREA1, NULL );
	xTaskCreate(vTarea2, (const char *)"Tarea2", TAM_PILA, NULL, PRIO_TAREA2, NULL );

	vTaskStartScheduler();
  for( ;; );
  return 0;

}

/*==================[end of file]============================================*/
