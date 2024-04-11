/***************************** Include Files *********************************/
#include "FIFO_FFT_driver.h"
#include <stdio.h>
#include "sleep.h"
#include <stdlib.h>



#undef DEBUG

/*
 * Flags interrupt handlers use to notify the application context the events.
 */
volatile int Done;
volatile int Error;
char flagDonnesOiseau = 0;
extern u32 Received;
//short ReceiveTempBuffer[MAX_DATA_BUFFER_SIZE];
//int BufferMain[MAX_DATA_BUFFER_SIZE];


XLlFifo FifoInstance, FifoInstance2;

/************************** Variable Definitions *****************************/


/*
 * Instance of the Interrupt Controller
 */

//stuff for interrupt management
XIntc InterruptController; /* Instance of the Interrupt Controller */
char flagDonnes = 0;





int XLFifoInit(XLlFifo *InstancePtr, u16 DeviceId){

	XLlFifo_Config *Config;
	int Status;
	Status = XST_SUCCESS;

	/* Initialize the Device Configuration Interface driver */
	Config = XLlFfio_LookupConfig(DeviceId);
	if (!Config) {
		xil_printf("No config found for %d\r\n", DeviceId);
		return XST_FAILURE;
	}

	/*
	 * This is where the virtual address would be used, this example
	 * uses physical address.
	 */
	Status = XLlFifo_CfgInitialize(InstancePtr, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("Initialization failed\n\r");
		return Status;
	}

	/* Check for the Reset value */
	Status = XLlFifo_Status(InstancePtr);
	XLlFifo_IntClear(InstancePtr,0xffffffff);
	Status = XLlFifo_Status(InstancePtr);
	if(Status != 0x0) {
		xil_printf("\n ERROR : Reset value of ISR0 : 0x%x\t"
			    "Expected : 0x0\n\r",
			    XLlFifo_Status(InstancePtr));
		return XST_FAILURE;
	}


	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function demonstrates the usage of AXI FIFO
* It does the following:
*       - Set up the output terminal if UART16550 is in the hardware build
*       - Initialize the Axi FIFO Device.
*	- Set up the interrupt handler for fifo
*	- Transmit the data
*	- Compare the data
*	- Return the result
*
* @param	InstancePtr is a pointer to the instance of the
*		XLlFifo instance.
* @param	DeviceId is Device ID of the Axi Fifo Deive instance,
*		typically XPAR_<AXI_FIFO_instance>_DEVICE_ID value from
*		xparameters.h.
*
* @return
*		-XST_SUCCESS to indicate success
*		-XST_FAILURE to indicate failure
*
******************************************************************************/
/*int XLlFifoSendData(XLlFifo *InstancePtr, u16 DeviceId, u32* DataBuffer)
{
	int Status;

	Done = 0;

	Status = TxSend(InstancePtr, DataBuffer);
	while(!Done); //wait for the fifo to be fully transmitted; this is indicated by an interrupt flag that sets Done


	if(Error) {
		xil_printf("Errors in the FIFO\n\r");
		return XST_FAILURE;
	}

	return Status;
}*/

/*****************************************************************************/
/**
*
* TxSend routine, It will send the requested amount of data at the
* specified addr.
*
* @param	InstancePtr is a pointer to the instance of the
*		XLlFifo component.
*
* @param	SourceAddr is the address of the memory
*
* @return
*		-XST_SUCCESS to indicate success
*		-XST_FAILURE to indicate failure
*
* @note		None
*
******************************************************************************/
#if 0
int TxSend(XLlFifo *InstancePtr, u32  *SourceAddr)
{
	int i;
	int j;
	xil_printf("Transmitting Data ... \r\n");

	for(i=0; i < NO_OF_PACKETS; i++){
		/* Writing into the FIFO Transmit Port Buffer */
		for (j=0; j < MAX_FFT_LEN; j++){
			if( XLlFifo_iTxVacancy(InstancePtr) ){
				XLlFifo_TxPutWord(InstancePtr,
					*(SourceAddr+(i*MAX_FFT_LEN)+j));
			}
		}

	}

	/* Start Transmission by writing transmission length into the TLR */
	XLlFifo_iTxSetLen(InstancePtr, (MAX_DATA_BUFFER_SIZE * WORD_SIZE));


	/* Transmission Complete */
	return XST_SUCCESS;
}
#endif
/*****************************************************************************/
/**
*
* This is the interrupt handler for the fifo it checks for the type of interrupt
* and proceeds according to it.
*
* @param	InstancePtr is a reference to the Fifo device instance.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void FifoHandler(XLlFifo *InstancePtr)
{
	u32 Pending;

	Pending = XLlFifo_IntPending(InstancePtr);
	while (Pending) {
		// Modifie, ajout de fifo full
		if (Pending & XLLF_INT_RC_MASK /*|| XLLF_INT_RFPF_MASK*/) {
			FifoRecvHandler(InstancePtr);
			XLlFifo_IntClear(InstancePtr, XLLF_INT_RC_MASK/*|| XLLF_INT_RFPF_MASK*/);
		}
		else if (Pending & XLLF_INT_TC_MASK) {
			FifoSendHandler(InstancePtr);
		}
		else if (Pending & XLLF_INT_ERROR_MASK){
			FifoErrorHandler(InstancePtr, Pending);
			XLlFifo_IntClear(InstancePtr, XLLF_INT_ERROR_MASK);
		} else {
			XLlFifo_IntClear(InstancePtr, Pending);
		}
		Pending = XLlFifo_IntPending(InstancePtr);
	}

}

/*****************************************************************************/
/**
*
* This is the Receive handler callback function.
*
* @param	InstancePtr is a reference to the Fifo device instance.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
int cnt_bruyant,cnt_coq,cnt_huard,cnt_pasbon = 0;
char flag_mef_bruyant,flag_mef_coq,flag_mef_huard = 0;
char etape = 0;

#define SEUIL_BRUYANT 10
#define SEUIL_HUARD 10
#define SEUIL_COQ 10
#define SEUIL_PAS_BON 20

void mef_oiseau(){
	xil_printf("etape %d\r\n",etape);
	switch(etape){
	case 0: {
		flagDonnesOiseau = 0;
		cnt_bruyant= 0;
		cnt_coq = 0;
		cnt_huard = 0;
		cnt_pasbon = 0;


		if(flag_mef_bruyant || flag_mef_huard || flag_mef_huard){
			flag_mef_bruyant = 0;
			flag_mef_huard = 0;
			flag_mef_huard = 0;
			etape = 1;
		}
		break;
	}


	case 1: {
			if(flag_mef_bruyant) cnt_bruyant++;
			if(flag_mef_huard) cnt_huard++;
			if(flag_mef_coq) cnt_coq++;
			if(!flag_mef_bruyant && !flag_mef_huard && !flag_mef_coq) cnt_pasbon++;

			flag_mef_bruyant = 0;
			flag_mef_huard = 0;
			flag_mef_huard = 0;

			if(cnt_bruyant >= SEUIL_BRUYANT){
				etape = 2;
			}

			if(cnt_huard >= SEUIL_HUARD){
				etape = 4;
			}

			if(cnt_coq >= SEUIL_COQ){
				etape = 3;
			}

			if(cnt_pasbon >= SEUIL_PAS_BON){
				etape = 0;
			}
			break;
		}



	case 2: {
				flagDonnesOiseau = 0x01;
				etape = 5;
				break;
			}

	case 3: {
				flagDonnesOiseau = 0x04;
				etape = 5;
				break;
			}

	case 4: {
				flagDonnesOiseau = 0x02;
				etape = 5;
				break;
			}


	case 5: {
				if(!flag_mef_bruyant && !flag_mef_huard && !flag_mef_coq) cnt_pasbon++;
				flag_mef_bruyant = 0;
				flag_mef_huard = 0;
				flag_mef_coq = 0;
				flagDonnesOiseau = 0;


				if(cnt_pasbon >= SEUIL_PAS_BON*2){
					etape = 0;
				}
				break;
			}

	default: break;


	}




}

void FifoRecvHandler(XLlFifo *InstancePtr)
{
	unsigned int i;
	u32 RxWord;
	u32 ReceiveLength;
	//xil_printf("Receiving Data... \n\r");
	int occ = XLlFifo_iRxOccupancy(InstancePtr);
	while(occ >= MAX_FFT_LEN) {
		ReceiveLength = (XLlFifo_iRxGetLen(InstancePtr)) & 0x7FFFFFFF;
		ReceiveLength /= 4;
		//xil_printf("%d \n\r", ReceiveLength);

		short max,imax = 1;
		for (i=0; i < ReceiveLength; i++) {
			short re = XLlFifo_RxGetWord(InstancePtr) & 0x0000FFFF;
			//ReceiveTempBuffer[i] = (ReceiveTempBuffer[i] << 16) >> 16;
			if(abs(re) > max){
				imax = i;
				max = abs(re);

			}


		}

		if(imax > 10){
			//xil_printf("\t\t\t%d \n\r", imax);

			if((imax >= 300) && (imax < 350)){
				flag_mef_bruyant = 1;
				//flagDonnesOiseau |= 0x01;
				//xil_printf("Bruyant \n\r", imax);
			}

			if((imax >= 90) && (imax < 110)){
				flag_mef_huard = 1;
				//flagDonnesOiseau |= 0x02;
				//xil_printf("Huard \n\r", imax);
			}

			if((imax >= 110) && (imax < 130)){
				flag_mef_coq = 1;
				//flagDonnesOiseau |= 0x04;
				//xil_printf("Coq \n\r", imax);
			}

		}else{
			//xil_printf("%d %d\n\r", imax,max);
		}


		mef_oiseau();

		//memcpy(BufferMain,ReceiveTempBuffer,ReceiveLength*4);
		flagDonnes  =1;



		occ = XLlFifo_iRxOccupancy(InstancePtr);
	}
	Received = 1;
}

/*****************************************************************************/
/*
*
* This is the transfer Complete Interrupt handler function.
*
* This clears the trasmit complete interrupt and set the done flag.
*
* @param	InstancePtr is a pointer to Instance of AXI FIFO device.
*
* @return	None
*
* @note		None
*
******************************************************************************/
void FifoSendHandler(XLlFifo *InstancePtr)
{
	XLlFifo_IntClear(InstancePtr, XLLF_INT_TC_MASK);

	Done = 1;
}

/*****************************************************************************/
/**
*
* This is the Error handler callback function and this function increments the
* the error counter so that the main thread knows the number of errors.
*
* @param	InstancePtr is a pointer to Instance of AXI FIFO device.
*
* @param	Pending is a bitmask of the pending interrupts.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void FifoErrorHandler(XLlFifo *InstancePtr, u32 Pending)
{
	if (Pending & XLLF_INT_RPURE_MASK) {
		XLlFifo_RxReset(InstancePtr);
	} else if (Pending & XLLF_INT_RPORE_MASK) {
		XLlFifo_RxReset(InstancePtr);
	} else if(Pending & XLLF_INT_RPUE_MASK) {
		XLlFifo_RxReset(InstancePtr);
	} else if (Pending & XLLF_INT_TPOE_MASK) {
		XLlFifo_TxReset(InstancePtr);
	} else if (Pending & XLLF_INT_TSE_MASK) {
	}
	Error++;
}


/*****************************************************************************/
/**
*
* This function disables the interrupts for the AXI FIFO device.
*
* @param	IntcInstancePtr is the pointer to the INTC component instance
* @param	FifoIntrId is interrupt ID associated for the FIFO component
*
* @return	None
*
* @note		None
*
******************************************************************************/
void DisableIntrSystem(XIntc *IntcInstancePtr, u16 FifoIntrId)
{
	/* Disconnect the interrupts */
	XIntc_Disconnect(IntcInstancePtr, FifoIntrId);
}

