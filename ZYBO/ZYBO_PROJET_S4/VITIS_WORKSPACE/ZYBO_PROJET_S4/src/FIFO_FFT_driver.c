/***************************** Include Files *********************************/
#include "FIFO_FFT_driver.h"
#include "uart.h"
#include <stdio.h>



#undef DEBUG

/*
 * Flags interrupt handlers use to notify the application context the events.
 */
volatile int Done;
volatile int Error;

extern u32 Received;
u32 ReceiveTempBuffer[MAX_DATA_BUFFER_SIZE];
u32 BufferMain[MAX_DATA_BUFFER_SIZE];
u32 Buffer_in_FIFO[MAX_DATA_BUFFER_SIZE];


u8 FFT_ReorderIndex1[MAX_DATA_BUFFER_SIZE]=
 {0x00, 0x40, 0x20, 0x60, 0x10, 0x50, 0x30, 0x70, 0x08, 0x48, 0x28, 0x68, 0x18, 0x58, 0x38, 0x78,
  0x04, 0x44, 0x24, 0x64, 0x14, 0x54, 0x34, 0x74, 0x0C, 0x4C, 0x2C, 0x6C, 0x1C, 0x5C, 0x3C, 0x7C,
  0x02, 0x42, 0x22, 0x62, 0x12, 0x52, 0x32, 0x72, 0x0A, 0x4A, 0x2A, 0x6A, 0x1A, 0x5A, 0x3A, 0x7A,
  0x06, 0x46, 0x26, 0x66, 0x16, 0x56, 0x36, 0x76, 0x0E, 0x4E, 0x2E, 0x6E, 0x1E, 0x5E, 0x3E, 0x7E,
  0x01, 0x41, 0x21, 0x61, 0x11, 0x51, 0x31, 0x71, 0x09, 0x49, 0x29, 0x69, 0x19, 0x59, 0x39, 0x79,
  0x05, 0x45, 0x25, 0x65, 0x15, 0x55, 0x35, 0x75, 0x0D, 0x4D, 0x2D, 0x6D, 0x1D, 0x5D, 0x3D, 0x7D,
  0x03, 0x43, 0x23, 0x63, 0x13, 0x53, 0x33, 0x73, 0x0B, 0x4B, 0x2B, 0x6B, 0x1B, 0x5B, 0x3B, 0x7B,
  0x07, 0x47, 0x27, 0x67, 0x17, 0x57, 0x37, 0x77, 0x0F, 0x4F, 0x2F, 0x6F, 0x1F, 0x5F, 0x3F, 0x7F};

XLlFifo FifoInstance, FifoInstance2;

/************************** Variable Definitions *****************************/


/*
 * Instance of the Interrupt Controller
 */

//stuff for interrupt management
XIntc InterruptController; /* Instance of the Interrupt Controller */
char flagDonnes = 0;

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

	Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);

	XIntc_Enable(&InterruptController, 0x00);

	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XIntc_InterruptHandler,
				&InterruptController);

	Xil_ExceptionEnable();

	return XST_SUCCESS;
}

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
int XLlFifoSendData(XLlFifo *InstancePtr, u16 DeviceId, u32* DataBuffer)
{
	int Status;

	Done = 0;
	/* Transmit the Data Stream */
	Status = TxSend(InstancePtr, DataBuffer);
	while(!Done); //wait for the fifo to be fully transmitted; this is indicated by an interrupt flag that sets Done

	/* Check for errors */
	if(Error) {
		xil_printf("Errors in the FIFO\n\r");
		return XST_FAILURE;
	}

	return Status;
}

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

	xil_printf("Transmitting Data ... \r\n");
	/* Transmission Complete */
	return XST_SUCCESS;
}

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
void FifoRecvHandler(XLlFifo *InstancePtr)
{
	unsigned int i;
	u32 RxWord;
	u32 ReceiveLength;

	//xil_printf("Receiving Data... \n\r");
	int occ = XLlFifo_iRxOccupancy(InstancePtr);
	while(occ) {
		ReceiveLength = (XLlFifo_iRxGetLen(InstancePtr)) & 0x7FFFFFFF;
		ReceiveLength /= 4;
		//xil_printf("%d ", ReceiveLength);
		for (i=0; i < ReceiveLength; i++) {
				RxWord = XLlFifo_RxGetWord(InstancePtr);
				/*if(RxWord & 0x0000FFFF){
					RxWord = 0;
				}*/

				ReceiveTempBuffer[i] = RxWord;


		}

		for(unsigned int i=0;i<MAX_DATA_BUFFER_SIZE;i++){
			Buffer_in_FIFO[i] = ReceiveTempBuffer[FFT_ReorderIndex1[i]];

			}
		memcpy(BufferMain,ReceiveTempBuffer,ReceiveLength*4);

		flagDonnes  =1;



		occ = XLlFifo_iRxOccupancy(InstancePtr);
	}
	Received = 1;
	print("i");
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

