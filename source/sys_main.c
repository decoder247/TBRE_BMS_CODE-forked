/** @file sys_main.c 
*   @brief Application main file
*   @date 07-July-2017
*   @version 04.07.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "adc.h"
#include "gio.h"
#include "sci.h"
#include "rti.h"
#include "sys_vim.h"
#include "swi_util.h"

#include "pl455.h"

int UART_RX_RDY = 0;
int RTI_TIMEOUT = 0;

/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

//	systemInit();
	gioInit();
	sciInit();
	sciSetBaudrate(sciREG, BAUDRATE);
	rtiInit();
	vimInit();

	_enable_IRQ();

//	gioSetBit(gioPORTA, 0, 1); // assert wake (active low)
//	delayus(1000);

	WakePL455();

	CommClear();

	CommReset();

	// initialize local variables
	int nSent, nRead, nTopFound = 0;
	int nDev_ID, nGrp_ID;
	BYTE  bFrame[132];
	uint32  wTemp = 0;

//	uint8 send[5] = {0xe1, 0x02, 0x02, 0xd0, 0x97};//{225, 2, 2, 208, 151};
//	char * pSend;

//	send =
//	pSend = &send;

//	while(1){
//		WakePL455();
//
//		sciSend(sciREG, 5, send);
////		sciSendByte(sciREG, 0xe1);
////		sciSendByte(sciREG, 0x02);
////		sciSendByte(sciREG, 0x02);
////		sciSendByte(sciREG, 0xd0);
////		sciSendByte(sciREG, 0x97);
//		delayms(100);
//	}




//	// Configure AFE (section 2.2.1)
//
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 60, 0x00, 1, FRMWRT_SGL_NR); // set 0 mux delay
//	nSent = WriteReg(nDev_ID, 61, 0x00, 1, FRMWRT_SGL_NR); // set 0 initial delay
//
//	// Configure voltage and internal sample period (section 2.2.2)
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 62, 0xCC, 1, FRMWRT_SGL_NR); // set 99.92us ADC sampling period
//
//	// Configure the oversampling rate (section 2.2.3)
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 7, 0x00, 1, FRMWRT_SGL_NR); // set no oversampling period
//
//	// Clear and check faults (section 2.2.4)
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 81, 0x38, 1, FRMWRT_SGL_NR); // clear fault flags in the system status register
//	nSent = WriteReg(nDev_ID, 82, 0xFFC0, 2, FRMWRT_SGL_NR); // clear all fault summary flags
//	nRead = ReadReg(nDev_ID, 81, &wTemp, 1, 0); // 0ms timeout
//	nRead = ReadReg(nDev_ID, 82, &wTemp, 2, 0); // 0ms timeout
//
//	// Select number of cells and channels to sample (section 2.2.5.1)
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 13, 0x10, 1, FRMWRT_SGL_NR); // set number of cells to 16
//	nSent = WriteReg(nDev_ID, 3, 0xFFFF03C0, 4, FRMWRT_SGL_NR); // select all cell, AUX channels 0 and 1, and internal digital die and internal analog die temperatures
//
//	// Select identical number of cells and channels on all modules simultaneously (section 2.2.5.2)
//	nSent = WriteReg(0, 13, 0x10, 1, FRMWRT_ALL_NR); // set number of cells to 16
//	nSent = WriteReg(0, 3, 0xFFFF03C0, 4, FRMWRT_ALL_NR); // select all cell, AUX channels 0 and 1, and internal digital die and internal analog die temperatures
//	nSent = WriteReg(0, 13, 0x08, 1, FRMWRT_ALL_NR); // set number of cells to 8
//	nSent = WriteReg(0, 3, 0x00FF03C0, 4, FRMWRT_ALL_NR); // select all cell channels 1-8, AUX channels 0 and 1, and internal digital die and internal analog die temperatures
//
//	// Set cell over-voltage and cell under-voltage thresholds on a single board (section 2.2.6.1)
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 144, 0xD1EC, 2, FRMWRT_SGL_NR); // set OV threshold = 4.1000V
//	nSent = WriteReg(nDev_ID, 142, 0x6148, 2, FRMWRT_SGL_NR); // set UV threshold = 1.9000V
//
//	// Set cell over-voltage and cell under-voltage thresholds on all boards simultaneously (section 2.2.6.2)
//	nSent = WriteReg(0, 144, 0xD1EC, 2, FRMWRT_ALL_NR); // set OV threshold = 4.1000V
//	nSent = WriteReg(0, 142, 0x6148, 2, FRMWRT_ALL_NR); // set UV threshold = 1.9000V
//
//	// Send broadcast request to all boards to sample and send results (section 3.2)
//	nSent = WriteReg(0, 2, 0x02, 1, FRMWRT_ALL_NR); // send sync sample command
//	nSent = WaitRespFrame(bFrame, 81, 0); // 24 bytes data (x3) + packet header (x3) + CRC (x3), 0ms timeout
//
//	// Send broadcast request to all boards to sample and store results (section 3.3.1)
//	nSent = WriteReg(0, 2, 0x00, 1, FRMWRT_ALL_NR); // send sync sample command
//
//	// Read sampled data from boards (section 3.3.2)
//	// 24 bytes - still configured for 8 AFE channels plus 2 AUX channels plus internal digital and analog die
//	// temperatures (see code for section 2.2.5.2)
//	nDev_ID = 2;
//	nSent = WriteReg(nDev_ID, 2, 0x20, 1, FRMWRT_SGL_R); // send read sampled values command
//	nSent = WaitRespFrame(bFrame, 27, 0); // 24 bytes data + packet header + CRC, 0ms timeout
//	nDev_ID = 1;
//	nSent = WriteReg(nDev_ID, 2, 0x20, 1, FRMWRT_SGL_R); // send read sampled values command
//	nSent = WaitRespFrame(bFrame, 27, 0); // 24 bytes data + packet header + CRC, 0ms timeout
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 2, 0x20, 1, FRMWRT_SGL_R); // send read sampled values command
//	nSent = WaitRespFrame(bFrame, 27, 0); // 24 bytes data + packet header + CRC, 0ms timeout
//
//	// Send sample request to single board to sample and send results (section 4.2)
//	nDev_ID = 1;
//	nSent = WriteReg(nDev_ID, 2, 0x01, 1, FRMWRT_SGL_NR); // send sync sample command
//	nSent = WaitRespFrame(bFrame, 27, 0); // 24 bytes data + packet header + CRC, 0ms timeout
//
//	// Send sample request with embedded channel and oversample information (section 4.3.1)
//	nDev_ID = 3;
//	nSent = WriteReg(nDev_ID, 2, 0x0000FF03C000, 6, FRMWRT_SGL_NR); // send sync sample command with channel
//	// selection and oversample selection embedded
//
//	// Read previously sampled data from single board (section 4.3.2)
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 2, 0x20, 1, FRMWRT_SGL_R); // send read sampled values command
//	nSent = WaitRespFrame(bFrame, 27, 0); // 24 bytes data + packet header + CRC, 0ms timeout
//
//	// Configure GPIO pin direction and set new pin values (section 5.2.1)
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 123, 0x00, 1, FRMWRT_SGL_NR);	// turn off all GPIO pull downs
//	nSent = WriteReg(nDev_ID, 122, 0x00, 1, FRMWRT_SGL_NR);	// turn off all GPIO pull ups
//	nSent = WriteReg(nDev_ID, 120, 0x07, 1, FRMWRT_SGL_NR);	// set GPIO[2:0] to output direction
//	nSent = WriteReg(nDev_ID, 122, 0x07, 1, FRMWRT_SGL_NR);	// turn off GPIO[5:4] pull ups and turn on GPIO[2:0] pull ups
//	nSent = WriteReg(nDev_ID, 121, 0x01, 1, FRMWRT_SGL_NR);	// set GPIO outputs (pattern b001)
//	nSent = WriteReg(nDev_ID, 121, 0x02, 1, FRMWRT_SGL_NR);	// set GPIO outputs (pattern b010)
//	nSent = WriteReg(nDev_ID, 121, 0x04, 1, FRMWRT_SGL_NR);	// set GPIO outputs (pattern b100)
//	nSent = WriteReg(nDev_ID, 121, 0x07, 1, FRMWRT_SGL_NR);	// set GPIO outputs (pattern b111)
//	nSent = WriteReg(nDev_ID, 121, 0x00, 1, FRMWRT_SGL_NR);	// set GPIO outputs (pattern b000)
//
//	// Configure GPIO inputs with or without pull ups or pull downs (section 5.2.2)
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 123, 0x04, 1, FRMWRT_SGL_NR);	// enable pull down for GPIO2, turn off all other GPIO pull downs
//	nSent = WriteReg(nDev_ID, 122, 0x03, 1, FRMWRT_SGL_NR);	// enable pull ups for GPIO[1:0], turn off all other GPIO pull ups
//	nSent = WriteReg(0, 122, 0x03, 1, FRMWRT_ALL_NR);	// broadcast to all boards to enable pull ups for GPIO[1:0] and turn off all other GPIO pull ups
//
//	// Setting a GPIO output value (section 5.2.3.1)
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 120, 0x17, 1, FRMWRT_SGL_NR);	// set GPIO direction for GPIO4 and GPIO[2:0] as outputs, GPIO3 and GPIO5 as inputs
//	nSent = WriteReg(nDev_ID, 121, 0x12, 1, FRMWRT_SGL_NR);	// set GPIO4 and GPIO1, clear GPIO2 and GPIO0
//
//	// Reading a GPIO input value (section 5.2.3.2)
//	nDev_ID = 0;
//	nSent = WriteReg(nDev_ID, 120, 0x30, 1, FRMWRT_SGL_NR);	// set GPIO direction for GPIO[5:4] as outputs, GPIO[3:0] as inputs
//	nRead = ReadReg(nDev_ID, 124, &wTemp, 1, 0);	// read GPIO inputs, 0ms timeout
//
//	// Steps for saving register configuration to EEPROM (sections 6.2.1 to 6.2.4)
//	// ** the code is commented out below to avoid accidental use - uncomment to use
//	nDev_ID = 0;
//	//nSent = WriteReg(nDev_ID, 130, 0x8C2DB194, 4, FRMWRT_SGL_NR);	// write Magic Number 1
//	//nSent = WriteReg(nDev_ID, 252, 0xA375E60F, 4, FRMWRT_SGL_NR);	// write Magic Number 2
//	//nSent = WriteReg(nDev_ID, 12, 0x10, 1, FRMWRT_SGL_NR);	// send Write RAM to EEPROM command
//	nRead = ReadReg(nDev_ID, 12, &wTemp, 1, 0);	// read WRITE_EEPROM status bit, 0ms timeout
//	//test WRITE_EEPROM bit to check for completion
//
//	// Steps for saving register configuration to EEPROM in all devices in stack (sections 6.2.5)
//	// ** the code is commented out below to avoid accidental use - uncomment to use
//	//nSent = WriteReg(0, 130, 0x8C2DB194, 4, FRMWRT_ALL_NR);	// write Magic Number 1
//	//nSent = WriteReg(0, 252, 0xA375E60F, 4, FRMWRT_ALL_NR);	// write Magic Number 2
//	//nSent = WriteReg(0, 12, 0x10, 1, FRMWRT_ALL_NR);	// send Write RAM to EEPROM command
//	nDev_ID = 0;
//	nRead = ReadReg(nDev_ID, 12, &wTemp, 1, 0);	// read WRITE_EEPROM status bit, 0ms timeout
//	//test WRITE_EEPROM bit to check for completion
//	nDev_ID = 1;
//	nRead = ReadReg(nDev_ID, 12, &wTemp, 1, 0);	// read WRITE_EEPROM status bit, 0ms timeout
//	//test WRITE_EEPROM bit to check for completion
//	nDev_ID = 2;
//	nRead = ReadReg(nDev_ID, 12, &wTemp, 1, 0);	// read WRITE_EEPROM status bit, 0ms timeout
//	//test WRITE_EEPROM bit to check for completion
//
//	// Assign devices to specific group IDs (section 7)
//	nDev_ID = 1;
//	nSent = WriteReg(nDev_ID, 11, 0x01, 1, FRMWRT_SGL_NR); // assign Dev ID 1 to Group ID 1
//	nDev_ID = 2;
//	nSent = WriteReg(nDev_ID, 11, 0x01, 1, FRMWRT_SGL_NR); // assign Dev ID 1 to Group ID 1
//
//	delayms(1);
//	nRead = ReadReg(1, 11, &wTemp, 1, 0);	// read group ID register, 0ms timeout
//	nRead = ReadReg(2, 11, &wTemp, 1, 0);	// read group ID register, 0ms timeout
//
//	// Send group sample request with embedded channel and oversample information (section 7.1)
//	// send sync sample command (first data byte = command (b[7:5] = 0) | highest device to respond (0x01)) with
//	// channel selection and oversample selection embedded
//	nGrp_ID = 1;
//	nSent = WriteReg(nGrp_ID, 2, 0x02FFFF550000, 6, FRMWRT_GRP_R);
//	nSent = WaitRespFrame(bFrame, 86, 0); // 40 bytes data (x2) + packet header (x2) + CRC (x2), 0ms timeout
//
//	// Send group sample request - 1 byte method (section 7.2)
//	// send sync sample command with channel (data byte = command (b[7:5] = 0) | highest device to respond (0x01))
//	// sampling parameters are taken from values already stored in Command Channel Select and Oversampling registers
//	nGrp_ID = 1;
//	nSent = WriteReg(nGrp_ID, 2, 0x02, 1, FRMWRT_GRP_R);
//	nSent = WaitRespFrame(bFrame, 86, 0); // 40 bytes data (x2) + packet header (x2) + CRC (x2), 0ms timeout
//
//	// Send group register read - 2 byte method (section 7.3)
//	// read Command Channel Select register (first data byte = highest device to respond (0x01), second data byte
//	// = number of data bytes - 1 (4 bytes = 0x03))
//	// sampling parameters are taken from values already stored in Command Channel Select and Oversampling registers
//	nGrp_ID = 1;
//	nSent = WriteReg(nGrp_ID, 3, 0x0203, 2, FRMWRT_GRP_R);
//	nSent = WaitRespFrame(bFrame, 14, 0); // 4 bytes data (x2) + packet header (x2) + CRC (x2), 0ms timeout
//
//	// Send group register read - 1 byte method (section 7.4)
//	// read Command Channel Select register (data byte = number of data bytes - 1 (4 bytes = b011) | highest device
//	// to respond (0x01))
//	// sampling parameters are taken from values already stored in Command Channel Select and Oversampling registers
//	nGrp_ID = 1;
//	nSent = WriteReg(nGrp_ID, 3, 0x62, 1, FRMWRT_GRP_R);
//	nSent = WaitRespFrame(bFrame, 14, 0); // 4 bytes data (x2) + packet header (x2) + CRC (x2), 0ms timeout
//
//	// Send general broadcast register read - 2 byte method (section 8.1)
//	// read Command Channel Select register (first data byte = highest device to respond (0x01), second data byte
//	// = number of data bytes - 1 (0x03))
//	nSent = WriteReg(0, 3, 0x0203, 2, FRMWRT_ALL_R);
//	nSent = WaitRespFrame(bFrame, 14, 0); // 4 bytes data (x2) + packet header (x2) + CRC (x2), 0ms timeout
//
//	// Send general broadcast register read - 1 byte method (section 8.2)
//	// read Command Channel Select register (data byte = number of data bytes - 1 (4 bytes = b011) | highest device
//	// to respond (0x01))
//	nSent = WriteReg(0, 3, 0x62, 1, FRMWRT_ALL_R);
//	nSent = WaitRespFrame(bFrame, 14, 0); // 4 bytes data (x2) + packet header (x2) + CRC (x2), 0ms timeout


		while(1){
//			sciSend(sciREG, 5, send);
			nSent = WriteReg(0, 2, 0x00, 1, FRMWRT_ALL_NR); // send sync sample command
			nSent = WaitRespFrame(bFrame, 27, 0); // 24 bytes data (x3) + packet header (x3) + CRC (x3), 0ms timeout
			delayms(10);
		}
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* USER CODE END */
