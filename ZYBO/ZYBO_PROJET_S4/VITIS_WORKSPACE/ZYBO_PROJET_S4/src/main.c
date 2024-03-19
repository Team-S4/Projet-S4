#include "xil_printf.h"
#include "xparameters.h"
#include "sleep.h"
#include <xiic.h>

#define IIC_BASE_ADDRESS	XPAR_IIC_0_BASEADDR


u8 buffer_test[2] = {0x88,0x88};

int main(){
	sleep(2);
	xil_printf("b");
	while(1){
		xil_printf("A");
		sleep(1);
		xil_printf("B");
		//u32 CntlReg;


		//CntlReg = XIic_ReadReg(IIC_BASE_ADDRESS,
		//		XIIC_CR_REG_OFFSET);
		//XIic_WriteReg(IIC_BASE_ADDRESS, XIIC_CR_REG_OFFSET, CntlReg | XIIC_CR_TX_FIFO_RESET_MASK);
		XIic_WriteReg(IIC_BASE_ADDRESS, XIIC_CR_REG_OFFSET, XIIC_CR_ENABLE_DEVICE_MASK);

		XIic_Send(IIC_BASE_ADDRESS, 0x58,buffer_test, 2,XIIC_STOP);
		buffer_test[1]++;


	}


}
