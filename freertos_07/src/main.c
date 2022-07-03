 /*==================[inclusions]=============================================*/

#include "board.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "chip.h"
#include "string.h"
#include "stdlib.h"



/*==================[macros and definitions]=================================*/

#define TAM_PILA 150
# define TAM_COLA 20 /* 20 mensajes */
# define TAM_MSG 10

QueueHandle_t cola; // Global, se usa de distintas tareas
SemaphoreHandle_t sem_hora;

static void initHardware(void)
{
    SystemCoreClockUpdate();
    Board_Init();
}


static void InitTimer(void)
{
	Chip_RIT_Init(LPC_RITIMER);
	Chip_RIT_SetTimerInterval(LPC_RITIMER,3000);
}

static void vTarea2(void *pvParameters)
{
	char mensaje[10];
while(1)
{
	if (( xSemaphoreTake( sem_hora, (portTickType) 1000 )) == pdTRUE) //Se desperto de la ISR
	{
		printf("Tarea 2 leyendo cola: ");
		if (xQueueReceive (cola, (void *) mensaje,(portTickType) 0xFFFFFFFF )==pdTRUE)
		{
		printf("%s. \r\n", mensaje);
		Board_LED_Toggle(3);
		}
		else printf("Error en la lectura.\r\n");

	}
}
}


static void vTarea1(void *pvParameters)
{

  BaseType_t xStatus;
	int c=0;
	char mensaje[10];
	for ( ;; ){
    while(c<100000)
	  {sprintf(mensaje, "%d", c);
     printf("Tarea 1 escribiendo en la cola\r\n");
	   xStatus = xQueueSend (cola, (void *) mensaje, portMAX_DELAY);

	if (xStatus==pdFAIL) printf("Error en la escritura (Tarea1)\r\n");
	c++;
			}
		c=0;
      	}
}


void RIT_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR( sem_hora, &xHigherPriorityTaskWoken );
	  if( xHigherPriorityTaskWoken == pdTRUE ){
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
		 /* Si el semáforo ha despertado
		  una tarea , se fuerza un cambio
		  de contexto */
	}
	/* Borra el flag de interrupción */
	Chip_RIT_ClearInt(LPC_RITIMER);
}


int main(void)
{
	initHardware(); /* Inicializa el Hardware del microcontrolador */
	InitTimer();

	/* Se inicializan los semáforos */
	sem_hora = xSemaphoreCreateBinary ();  //se inicializa por defecto en 0

	//Se inicializa cola
	cola = xQueueCreate (TAM_COLA, TAM_MSG);

	/* Se crean las tareas */
	xTaskCreate(vTarea1, (const char *)"Tarea1", TAM_PILA, NULL, 1, NULL );
	xTaskCreate(vTarea2, (const char *)"Tarea2", TAM_PILA, NULL, 2, NULL );

	NVIC_EnableIRQ(RITIMER_IRQn);
	NVIC_SetPriority(RITIMER_IRQn, (1<<__NVIC_PRIO_BITS) -1);


	vTaskStartScheduler(); /* y por último se arranca el planificador . */
    //Nunca llegara a ese lazo  .... espero
  for( ;; );
  return 0;
}

/*==================[end of file]============================================*/
