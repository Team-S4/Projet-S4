#include <xiic.h>
#include <xbasic_types.h>
#include "xparameters.h"
#include "initCodec.h"
#include "sleep.h"

#define IIC_BASE_ADDRESS	XPAR_IIC_0_BASEADDR




void initCodec(){

	//Activer le bloc i2c
	XIic_WriteReg(IIC_BASE_ADDRESS, XIIC_CR_REG_OFFSET, XIIC_CR_ENABLE_DEVICE_MASK);
	usleep(7);
	for(int i =0;i<12;i++){
		char donnesInversees[2];
		donnesInversees[0] = *((char*)(donnees+i)+1);
		donnesInversees[1] = *(char*)(donnees+i);

		XIic_Send(IIC_BASE_ADDRESS, ADRESSE_CODEC_SSM2603, donnesInversees, 2,XIIC_STOP);
		usleep(7000);
	}


}




u16 donnees[12] =  {

 0b0001111000000000 ,     //      0  R15 Software reset
 0b0000110000110000 ,     //      1  R6 Power management
//       ||               //         R6 D8=0;
//       ||               //         R6 D7=0; pwroff         : power up;
//       ||               //         R6 D6=0; clkout power   : power up;
//       ||               //         R6 D5=1; crystal power  : power down;
//       ||               //         R6 D4=1; output power   : power down;
//       ||               //         R6 D3=0; DAC power      : power up;
//       ||               //         R6 D2=0; ADC power      : power up;
//       ||               //         R6 D1=0; MIC in power   : power up;
//       ||               //         R6 D0=0; LINE in power  : power up;
//		 ||				  //
 0b0000000000010111 ,     //      2  R0 Left-channel ADC input volume
//       ||               //         R0: disable loading left to right;  disable mute; 0 dB
 0b0000001000010111 ,     //      3  R1 Right-channel ADC input volume
//       ||               //         R1: disable loading right to left;  disable mute; 0 dB
//		 ||				  //
 0b0000010001111001 ,     //      4  R2 Left-channel DAC volume  (defaut / datasheet)
//       ||               //         R2 disable left ch head phone vol data to right; volume 0dB (defaut / datasheet)
 0b0000011001111001 ,     //      5  R3 Right-channel DAC volume  (defaut / datasheet)
//       ||               //         R3 disable right ch head phone vol data to left; volume 0dB (defaut / datasheet)
//		 ||				  //
 0b0000100000010100, //0b0000100000010000 ,     //      6  R4 Analog audio path                INSEL = LINE
//       ||               //		 R4 D8=0;                          : 0        (def)
//       ||               //		 R4 D7=0; |                        : 0
//       ||               //		 R4 D6=0; | mic sidetone gain ctrl : 00 -6dB   (def)
//       ||               //		 R4 D5=0; sidetone_en              : 0 disable (def)
//       ||               //		 R4 D4=1; DACSEL  (mix DAC output) : 1 select DAC
//       ||               //		 R4 D3=0; BypAss                   : 0 = Bypass disabled
//       ||               //		 R4 D2=0; INSEL                    : 0 = LINE input select to ADC (def)  ***** 1: MIC
//       ||               //		 R4 D1=0; MUTEMIC                  : 0 (disabled)
//       ||               //		 R4 D0=0; MICBOOST                 : 0 = 0 dB (def);
 0b0000101000000000 ,     //      7  R5 Digital audio path
 0b0000111000000010 ,     //      8  R7 Digital audio I/F
//       ||               //           		ref data sheet page 25/31
//       ||               //                24 bits I2S mode, sampling ADC 12 KHz   DAC 12 KHz
 0b0001000000010000 ,     //      9  R8 Sampling rate
 0b0001001000000001 ,     //     10  R9 Active
 0b0000110000100000 };    //     11  R6 Power management
//       ||                         //  		 R6 D8=0;
                                    //  		 R6 D7=0; pwroff         : power up;
                                    //  		 R6 D6=0; clkout power   : power up;
                                    //  		 R6 D5=1; crystal power  : power down;
                                    //  		 R6 D4=0; output power   : power up;
                                    //  		 R6 D3=0; DAC power      : power up;
                                    //  		 R6 D2=0; ADC power      : power up;
                                    //  		 R6 D1=0; MIC in power   : power up;









//Exemple dans vivado
/*
 * -- adresse reg    contenu             seq registre
-- 15     9        8        0
"0001111"     & "000000000", --    0    R15 Software reset
"0000110"     & "000110000", --    1    R6 Power management
                                     -- R6 D8=0;
                                     -- R6 D7=0; pwroff         : power up;
                                     -- R6 D6=0; clkout power   : power up;
                                     -- R6 D5=1; crystal power  : power down;
                                     -- R6 D4=1; output power   : power down;
                                     -- R6 D3=0; DAC power      : power up;
                                     -- R6 D2=0; ADC power      : power up;
                                     -- R6 D1=0; MIC in power   : power up;
                                     -- R6 D0=0; LINE in power  : power up;

"0000000"     & "000010111", --    2    R0 Left-channel ADC input volume
                                      --   R0: disable loading left to right;  disable mute; 0 dB
"0000001"     & "000010111", --    3    R1 Right-channel ADC input volume
                                      --    R1: disable loading right to left;  disable mute; 0 dB

"0000010"     & "001111001", --    4    R2 Left-channel DAC volume  (defaut / datasheet)
                                               --   R2 disable left ch head phone vol data to right; volume 0dB (defaut / datasheet)
"0000011"     & "001111001", --    5    R3 Right-channel DAC volume  (defaut / datasheet)
                                              --    R3 disable right ch head phone vol data to left; volume 0dB (defaut / datasheet)
--"0000010"     & "001101001", --    4    R2 Left-channel DAC volume  (defaut / datasheet)
--                                         --   R2 disable left ch head phone vol data to right; volume -16 dB (conseillé avec écouteurs....)
--"0000011"     & "001101001", --    5    R3 Right-channel DAC volume  (defaut / datasheet)
--                                        --    R3 disable right ch head phone vol data to left; volume -16 dB  (conseillé avec écouteurs....)

--          "0000100"     & "000000100", --    6    R4 Analog audio path    -- audio.c  INSEL = mic
"0000100"     & "000010000", --    6    R4 Analog audio path    --          INSEL = LINE
                              -- R4 D8=0;                          : 0        (def)
                              -- R4 D7=0; |                        : 0
                              -- R4 D6=0; | mic sidetone gain ctrl : 00 -6dB   (def)
                              -- R4 D5=0; sidetone_en              : 0 disable (def)
                              -- R4 D4=1; DACSEL  (mix DAC output) : 1 select DAC
                              -- R4 D3=0; BypAss                   : 0 = Bypass disabled
                              -- R4 D2=0; INSEL                    : 0 = LINE input select to ADC (def)  ***** 1: MIC
                              -- R4 D1=0; MUTEMIC                  : 0 (disabled)
                              -- R4 D0=0; MICBOOST                 : 0 = 0 dB (def);
"0000101"     & "000000000", --    7    R5 Digital audio path
"0000111"     & "000000010", --    8    R7 Digital audio I/F
                           --                   ref data sheet page 25/31
                           --                   24 bits I2S mode, sampling ADC 48 KHz   DAC 48 KHz
"0001000"     & "000000000", --    9    R8 Sampling rate
"0001001"     & "000000001", --   10    R9 Active
"0000110"     & "000100000", --   11    R6 Power management
                               -- R6 D8=0;
                               -- R6 D7=0; pwroff         : power up;
                               -- R6 D6=0; clkout power   : power up;
                               -- R6 D5=1; crystal power  : power down;
                               -- R6 D4=0; output power   : power up;
                               -- R6 D3=0; DAC power      : power up;
                               -- R6 D2=0; ADC power      : power up;
                               -- R6 D1=0; MIC in power   : power up;
others =>
"1111111"     & "000000000"  --   code adresse de registre non reelle signifie extremite table atteinte
);
 */
