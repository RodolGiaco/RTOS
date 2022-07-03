
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define PRIO_TAREA1 3
#define PRIO_TAREA2 2
#define PRIO_TAREA3 1
#define TAM_PILA 150
#define mainDELAY_LOOP_COUNT        ( 0xffffff )

SemaphoreHandle_t sem_exclu;


static void vTarea1(void *pvParameters)
{
	const TickType_t xDelay800ms = pdMS_TO_TICKS( 800UL );
  for( ;; ) {
  xSemaphoreTake( sem_exclu, portMAX_DELAY);
	vPrintString( "Tarea 1 escribiendo por puerto\r\n" );
  xSemaphoreGive(sem_exclu);
	vTaskDelay( xDelay800ms );
        }
}

static void vTarea2(void *pvParameters)
{
    const TickType_t xDelay200ms = pdMS_TO_TICKS( 200UL );
    const TickType_t xDelay1000ms = pdMS_TO_TICKS( 1000UL );
    TickType_t xLastWakeTime;

    for( ;; ) {
	  vPrintString( "Tarea 2 corriendo\r\n" );
    xLastWakeTime=xTaskGetTickCount();

	  while((xTaskGetTickCount()-xLastWakeTime)<xDelay1000ms)
	  {}
	  Board_LED_Toggle(0);
  	vTaskDelay( xDelay200ms );
   }
}
static void vTarea3(void *pvParameters)
{
  const TickType_t xDelay800ms = pdMS_TO_TICKS( 800UL );

   for( ;; ) {
      xSemaphoreTake( sem_exclu, portMAX_DELAY);
    	vPrintString( "Tarea 3 escribiendo por puerto\r\n" );
      xSemaphoreGive(sem_exclu);
	    vTaskDelay( xDelay800ms );
  }
}

/*==================[external functions definition]==========================*/

int main(void)
{

//	sem_exclu = xSemaphoreCreateBinary ();
	sem_exclu = xSemaphoreCreateMutex ();

	xTaskCreate(vTarea1, (const char *)"Tarea1", TAM_PILA, NULL, PRIO_TAREA1, NULL );
	xTaskCreate(vTarea2, (const char *)"Tarea2", TAM_PILA, NULL, PRIO_TAREA2, NULL );
	xTaskCreate(vTarea3, (const char *)"Tarea3", TAM_PILA, NULL, PRIO_TAREA3, NULL );


	vTaskStartScheduler(); /* y por último se arranca el planificador . */
    //Nunca llegara a ese lazo  .... espero
     for( ;; );
     return 0;

}

/*==================[end of file]============================================*/
