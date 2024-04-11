#include "xil_printf.h"
#include "xparameters.h"
#include "sleep.h"
#include <xiic.h>
#include "initCodec.h"
#include "gestionAudio.h"
#include "uart.h"
#include "fft.h"
#include "FIFO_FFT_driver.h"



//Dependencies
#include <stdlib.h>
#include <stdio.h>
#include "xil_io.h"
#include "cyclone_tcp/core/net.h"
#include "cyclone_tcp/net_config.h"
#include "cyclone_tcp/drivers/eth/enc624j600_driver.h"
#include "cyclone_tcp/core/udp.h"
#include "xspi.h"
#include "cyclone_tcp/spi_driver.h"
#include "cyclone_tcp/ext_int_driver.h"
#include <stdbool.h>
#include "comm.h"
#include "common/os_port_none.h"
#include "cyclone_tcp/std_services/echo.h"


#include "FIFO_FFT_driver.h"
#include <stdio.h>
#include "sleep.h"
#include <stdlib.h>
#include <stdbool.h>


#define IIC_BASE_ADDRESS	XPAR_IIC_0_BASEADDR
char dataToSend[1] = {0};
void AXIS_InterruptHandler(void *CallbackRef)
{
	//xil_printf("In interrupt\n\r");
	FifoHandler(&FifoInstance);
}

int SetupInterruptSystem()
{
	int Status;

	Status = XIntc_Initialize(&InterruptController, XPAR_INTC_0_DEVICE_ID);

	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the
	 * specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(&InterruptController, 0x00,
			   (XInterruptHandler)AXIS_InterruptHandler,
			   (void *)0);


	Status = XIntc_Connect(&InterruptController, XPAR_MICROBLAZE_0_AXI_INTC_SYSTEM_INT_NIC100_INTR,
				   (XInterruptHandler)ENC_Int_Handler,
				   (void *)0);

	Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);

	XIntc_Enable(&InterruptController, 0x00);

	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XIntc_InterruptHandler,
				&InterruptController);

	Xil_ExceptionEnable();

	return XST_SUCCESS;
}



void init(){
	xil_printf("Init OK\n\r");
	initAudio();
	//sleep(1);
	initCodec();
	//sleep(1);
	SetupInterruptSystem();
	initComm();
	//sleep(1);
	initFIFO_FFT();
	//sleep(1);
	configProjetS4();




}



bool udpSocketUp;


int main(){
	sleep(1);
	init();

	while(1){
		//doTick();
		//uartTask();
		udpSendPacketTask((char*)dataToSend);
		dataToSend[0] = 0;


		if(flagDonnesOiseau){
			//xil_printf("d\n\r");
			//u32 *premierInt = (u32 *)(BufferMain);
			//XUartLite_Send(&UartLite, BufferMain , MAX_FFT_LEN*4);

			if(flagDonnesOiseau & 0x01){
				xil_printf("Bruyant \n\r");
				dataToSend[0] = 1;


			}

			if(flagDonnesOiseau & 0x02){
				xil_printf("Huard \n\r");
				dataToSend[0] = 3;
			}

			if(flagDonnesOiseau & 0x04){
				xil_printf("Coq \n\r");
				dataToSend[0] = 2;
			}


			flagDonnesOiseau = 0;

		}

		netTask();
		if(netInterface[0].linkState == 1){
				   if(!udpSocketUp){
					   	   udpEchoStart();
						   udpSocketUp = true;
				   }
				   else{
					   int packetToRead = udpReadPacketTask();
					   //xil_printf("bob");
				}
			   }
			   else
			   {
				   if(udpSocketUp){

					   udpSocketUp = false;
				   }

			   }



	}


}
