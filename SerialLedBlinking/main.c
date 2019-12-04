#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <strsafe.h>
#include <stdbool.h>
#include <sys/stat.h>


/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "serial.h"

/* Demo includes. */
#include "supporting_functions.h"
#define MAX_SERIAL_PORT		10

char *serial_list[MAX_SERIAL_PORT] = { NULL };
struct AdapterTypedef_t adapter = { 0 };

/*
* monitor serial data come from stm32 vcp
*/
void vSerialTask(void *pvParameters) {
	pvParameters = pvParameters;
	while (true) {
		if (adapter.Handle) {
			if (adapter.is_port_open(&adapter)) {
				adapter.ser_mon(&adapter);
			}
		}
		vTaskDelay(1);
	}
}

void vLedTask(void *arg) {
	uint8_t buf[12];
	bool led=false;
	arg = arg;						//avoid warning
	buf[0] = SERIAL_PACKET__LED;	//opcode low byte
	buf[1] = 0x00;					//opcode high byte
	buf[2] = 0x01;					//led number
	while (true) {
		buf[3] = led ? 0x01 : 0x0;		//led on/off
		led = !led;
		adapter.write(&adapter, buf, 4);
		vTaskDelay(200);
		if (_kbhit() != 0) {
			printf("exit\n");
			vTaskEndScheduler();
		}
	}
}

/*
* create a serial connection.
*/
void serial_create(void) {
	char **p_ptr;

	//check if stm32 comport in the window
	if (serial_devices_mon()) {
		int id = 0;
		//get the stm32 comport list
		serial_devices_get(&serial_list);
		p_ptr = serial_list;
		if (*p_ptr != NULL) {
			printf("\nlist of stm32 comports:\n");
			while (*p_ptr) {
				printf("%d: %s\n", id++, *p_ptr++);
			}
			printf("Please select comport: ");
			scanf("%d", &id);
			//create a serial port instance
			serial_devices_create(&adapter, NULL, serial_list[id]);
			//now open it.
			if (serial_devices_open(&adapter)) {
				printf("Serial port open successfully\n");
				printf("press any key to exit:");
			}
		}
	}
}

int main(void)
{	
	//creae an instance of serial
	serial_create();

	//task to toggle led
	xTaskCreate(vLedTask, "input task", 1024, NULL, 1, NULL);
	//task to read the serial data
	xTaskCreate(vSerialTask, "connection", 1024, NULL, 1, NULL);

	vTaskStartScheduler();
	for (;; );
}
