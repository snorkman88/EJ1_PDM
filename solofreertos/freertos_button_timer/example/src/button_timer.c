/*/*
 * Modified form FreeRTOS Blinky example
 */

#include "FreeRTOS.h"
#include "task.h"
#include "chip.h"
/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
const int LED1[2] = {3, 7}; //El led1 correspond al GPIO3 bit7
const int LED2[2] = {3, 6}; //El led2 correspond al GPIO3 bit 6
const int LED3[2] = {3, 5}; //El led3 corresponde al GPIO1 bit 12
const int LEDA[2] = {2, 8}; //El led A corresponde al GPIO2 bit 8


const int TEC1[2] = {3, 12}; // Pulsador 1 corresponde a GPIO 3 bit 12
const int TEC2[2] = {3, 10}; // Pulsador 2 corresponde a GPIO 3 bit 10
const int TEC3[2] = {3, 11}; // Pulsador 3 corresponde a GPIO 3 bit 11
const int TEC4[2] = {3, 9}; // Pulsador 4 corresponde a GPIO 3 bit 9


/*****************************************************************************
 * Private functions
 ****************************************************************************/
/* System configuration variables used by chip driver */

//Estas dos lineas se sacaron del board.h
const uint32_t ExtRateIn = 0;
const uint32_t OscRateIn = 12000000;


TaskHandle_t handlerLed1 = NULL;
TaskHandle_t handlerLed2 = NULL;
TaskHandle_t handlerLed3 = NULL;

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Chip_GPIO_Init(LPC_GPIO_PORT);

	Chip_SCU_PinMuxSet(6,  9, SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN);
	Chip_SCU_PinMuxSet(6, 10, SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN);
	Chip_SCU_PinMuxSet(6, 11, SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN);//LED1
	Chip_SCU_PinMuxSet(6, 12, SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN);//LEDA

	Chip_SCU_PinMuxSet(7, 4,  SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN); //tecA joystick
	Chip_SCU_PinMuxSet(7, 3,  SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN); //tecB joystick
	Chip_SCU_PinMuxSet(7, 2,  SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN); //tecC joystick
	Chip_SCU_PinMuxSet(7, 1,  SCU_MODE_FUNC0 | SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN); //tecD joystick

	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LEDA[0], LEDA[1]);//Configuro el GPIO como salida para los leds
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LED1[0], LED1[1]);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LED2[0], LED2[1]);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LED3[0], LED3[1]);

	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TEC1[0], TEC1[1]); //Configuro el GPIO como entrada para pulsadores
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TEC3[0], TEC3[1]);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TEC2[0], TEC2[1]);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TEC4[0], TEC4[1]);
}


/*Tarea LED1*/
static void vLed1Task1(void *pvParameters) {
	long ms_counter = 0;
	bool event;
	while (1) {
		event = false;
		ms_counter = 0;
		while (!Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC1[0], TEC1[1])){
			vTaskDelay(1 * configTICK_RATE_HZ / 1000);
			ms_counter++;
			event = true;
		}
		if (event){
			Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED1[0], LED1[1], true);
			vTaskDelay(ms_counter * configTICK_RATE_HZ / 1000);
			Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED1[0], LED1[1], false);
		}
	}
}



/*Tarea LED2*/
static void vLed2Task2(void *pvParameters) {
	long ms_counter = 0;
	bool event;
	while (1)
	{
		event = false;
		ms_counter = 0;
		while (!Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC2[0], TEC2[1])){
			vTaskDelay(1 * configTICK_RATE_HZ / 1000);
			ms_counter++;
			event = true;
		}
		if (event){
			Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED2[0], LED2[1], true);
			vTaskDelay(ms_counter * configTICK_RATE_HZ / 1000);
			Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED2[0], LED2[1], false);
		}
	}
}


/*Tarea LED3*/
static void vLed3Task3(void *pvParameters) {
	long ms_counter = 0;
	bool event;
	while (1)
	{
		event = false;
		ms_counter = 0;
		while (!Chip_GPIO_GetPinState(LPC_GPIO_PORT, TEC3[0], TEC3[1])){
			vTaskDelay(1 * configTICK_RATE_HZ / 1000);
			ms_counter++;
			event = true;
		}
		if (event){
			Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], true);
			vTaskDelay(ms_counter * configTICK_RATE_HZ / 1000);
			Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED3[0], LED3[1], false);
		}
	}

}
/*****************************************************************************
 * Public functions
 ****************************************************************************/

int main(void)
{
	prvSetupHardware();

	// Creo Tarea para LED1//
	xTaskCreate(vLed1Task1, "Led1", 200,
			NULL, tskIDLE_PRIORITY, &handlerLed1);

	// Creo Tarea LED2 //
        xTaskCreate(vLed2Task2, "Led2", 200,
			NULL, tskIDLE_PRIORITY, &handlerLed2);

	// Creo Tarea LED3 //
	xTaskCreate(vLed3Task3, "Led3", 200,
			NULL, tskIDLE_PRIORITY, &handlerLed3);

	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}


