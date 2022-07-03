 /*==================[inclusions]=============================================*/

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"

/*==================[macros and definitions]=================================*/

#define PRIO_TAREA1 2
#define PRIO_TAREA2 1
#define TAM_PILA 150
#define mainDELAY_LOOP_COUNT        ( 0xffffff )

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/


static void vTarea1(void *pvParameters)
{
  const char *pcTaskName = "Tarea 1 is running\r\n";
  uint32_t ul;
  TickType_t xLastWakeTime; //Esta variable es para poder mostrar los tick
  //volatile uint32_t ul;

   for( ;; ) {

      printf( pcTaskName );

      xLastWakeTime = xTaskGetTickCount();  //Me da la cuenta de los tick
      printf("Time %d\r\n",xLastWakeTime);  /*Muestro los ticks*/
      vTaskDelay(1000 / portTICK_RATE_MS);  // Voy a tener 100 tick y por cada tick tengo 10ms 100*10ms = 1s


      Board_LED_Toggle(1); // Green (RGB)

      }
}

static void vTarea2(void *pvParameters)
{
  const char *pcTaskName = "Tarea 2 is running\r\n";
    uint32_t ul;
   /* As per most tasks, this task is implemented in an infinite loop. */
   for( ;; ) {
      /* Print out the name of this task. */
      printf( pcTaskName );
      /* Delay for a period. */
      for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ ) {
      }
   }
}

/*==================[external functions definition]==========================*/

int main(void)
{
    //Se inicializa HW
	/* Se crean las tareas */
	xTaskCreate(vTarea1, (const char *)"Tarea1", TAM_PILA, NULL, PRIO_TAREA1, NULL );
	xTaskCreate(vTarea2, (const char *)"Tarea2", TAM_PILA, NULL, PRIO_TAREA2, NULL );

	vTaskStartScheduler(); /* y por último se arranca el planificador . */
    //Nunca llegara a ese lazo  .... espero
     for( ;; );
     return 0;

}

/*==================[end of file]============================================*/
