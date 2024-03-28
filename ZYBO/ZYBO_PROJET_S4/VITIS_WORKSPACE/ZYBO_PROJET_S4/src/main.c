#include "xil_printf.h"
#include "xparameters.h"
#include "sleep.h"
#include <xiic.h>
#include "initCodec.h"
#include "gestionAudio.h"
#include "uart.h"
#include "fft.h"
#include "FIFO_FFT_driver.h"


#define IIC_BASE_ADDRESS	XPAR_IIC_0_BASEADDR


u8 buffer_test[2] = {0x88,0x88};
int maxAmpFFT = 0;
int maxBin = 0;
int mask = 0xFFFF;
u32 bufferFFT[32];
int mainFreq;


void init(){
	xil_printf("Init OK\n");
	initAudio();
	sleep(1);
	initCodec();
	sleep(1);
	uartInit();
	initFIFO_FFT();
	sleep(1);
	configProjetS4();



}



int main(){
	sleep(1);
	init();
	while(1){
		uartTask();
		//usleep(500000);
		xil_printf("B");
		//int bytes = XUartLite_Send(&UartLite, buffer_test , 2);
		//xil_printf("%d",bytes);

		if(flagDonnes){
			u32 *premierInt = (u32 *)(BufferMain);
			XUartLite_Send(&UartLite, BufferMain , MAX_FFT_LEN*4);
			flagDonnes = 0;

		}
		maxBin = 0;
		maxAmpFFT = 0;



		for(unsigned int i=0;i<MAX_DATA_BUFFER_SIZE;i++){
			BufferMain[i] = BufferMain[i] & mask;
				if (BufferMain[i] > maxAmpFFT){
					maxAmpFFT = BufferMain[i];
					maxBin = i;

				}

				maxAmpFFT = maxAmpFFT;
				maxBin = maxBin;
			}
		mainFreq = maxBin*64000/MAX_FFT_LEN;
		xil_printf("%d : ", maxBin);
			/*
		 avec 8x la fft

		for (unsigned int j = 1; j < 9; j++) {

		for(unsigned int i=0;i<MAX_DATA_BUFFER_SIZE/8;i++){
			BufferMain[i*j] = BufferMain[i*j] & mask;
				if (BufferMain[i*j] > maxAmpFFT){
					maxAmpFFT = BufferMain[i*j];
					maxBin[j] = i;

				}
				maxAmpFFT = maxAmpFFT;
				maxBin = maxBin;
			}
		}

		for (unsigned int k = 0; k < 9; k++{
			mainFreq += maxBin[k]

		}
			mainFreq = mainFreq*64000/(MAX_FFT_LEN * 9)



		*/

	}


}
