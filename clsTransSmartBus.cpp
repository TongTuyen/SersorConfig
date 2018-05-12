<<<<<<< HEAD
<<<<<<< HEAD
/*
 * clsTransSmartBus.cpp
 *
 *  Created on: May 13, 2017
 *      Author: MANHBT
 */

#include "clsTransSmartBus.h"
#include "CRC16.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>     /* malloc, free, rand */
//#include <arpa/inet.h>
#include "convert.h"

namespace TransSmartBus {
clsTransSmartBus::clsTransSmartBus() {
	//

}

clsTransSmartBus::~clsTransSmartBus() {
	//
}

uint8_t* clsTransSmartBus::BuidFrame(SMBusFrame_t sFrame, uint8_t* payload)
{
	uint8_t* pu8Result = new uint8_t[sFrame.dlen + 2];

	memcpy((void*)pu8Result, (void*)&sFrame, sizeof(SMBusFrame_t));
    memcpy((void*)&pu8Result[SMBUS_FRM_IDX_DATA0], (void*)payload, sFrame.dlen);
	*(uint16_t*)&pu8Result[sFrame.dlen] = CalculateCRC16(&pu8Result[SMBUS_FRM_IDX_DLEN], sFrame.dlen);

	return pu8Result;

}

//+----------+------+----------+----------+--------+----------+------------+-------+
//| PREAMBLE | DLEN | SRC_ADDR | DEV_TYPE | OPCODE | DST_ADDR | DATA[0..n] | CRC_D |
//|  (2B)    | (1B) | (2B)     | (2B)     | (2B)   | (2B)     | (nB)       | (2B)  |
//+----------+------+----------+----------+--------+----------+------------+-------+
//	  AAAA      0F      0CFE      FFFE       0031      010C     01 64 00 01   B22F
SMBusFrame_t clsTransSmartBus::DecodeFrame(uint8_t* pu8Frame)
{
	SMBusFrame_t sResult = {0};

	if (!IsValidSMBUSDLen(pu8Frame[0]))
		return sResult;

	uint16_t 	u16CRC = CalculateCRC16(pu8Frame, pu8Frame[0] - 2);
	if (u16CRC != htons(*(uint16_t*)&pu8Frame[pu8Frame[0]-2]))
		return sResult;



	sResult.dlen = pu8Frame[0];
	sResult.src_subid = pu8Frame[1];
	sResult.src_devid = pu8Frame[2];
	sResult.devtype = htons(*(uint16_t*)&pu8Frame[3]);
	sResult.opcode = htons(*(uint16_t*)&pu8Frame[5]);

	sResult.dst_subid = pu8Frame[7];
	sResult.dst_devid = pu8Frame[8];

	sResult.data = new uint8_t[sResult.dlen - SMBUS_FRM_HDR_SIZE];
	memcpy(sResult.data, &pu8Frame[9], sResult.dlen - SMBUS_FRM_HDR_SIZE);

	sResult.crc = u16CRC;

	return sResult;
}


} /* namespace TransSmartBus */
=======
/*
 * clsTransSmartBus.cpp
 *
 *  Created on: May 13, 2017
 *      Author: MANHBT
 */

#include "clsTransSmartBus.h"
#include "CRC16.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>     /* malloc, free, rand */
//#include <arpa/inet.h>
#include "convert.h"

namespace TransSmartBus {
clsTransSmartBus::clsTransSmartBus() {
	//

}

clsTransSmartBus::~clsTransSmartBus() {
	//
}

uint8_t* clsTransSmartBus::BuidFrame(SMBusFrame_t sFrame, uint8_t* payload)
{
	uint8_t* pu8Result = new uint8_t[sFrame.dlen + 2];

	memcpy((void*)pu8Result, (void*)&sFrame, sizeof(SMBusFrame_t));
    memcpy((void*)&pu8Result[SMBUS_FRM_IDX_DATA0], (void*)payload, sFrame.dlen);
	*(uint16_t*)&pu8Result[sFrame.dlen] = CalculateCRC16(&pu8Result[SMBUS_FRM_IDX_DLEN], sFrame.dlen);

	return pu8Result;

}

//+----------+------+----------+----------+--------+----------+------------+-------+
//| PREAMBLE | DLEN | SRC_ADDR | DEV_TYPE | OPCODE | DST_ADDR | DATA[0..n] | CRC_D |
//|  (2B)    | (1B) | (2B)     | (2B)     | (2B)   | (2B)     | (nB)       | (2B)  |
//+----------+------+----------+----------+--------+----------+------------+-------+
//	  AAAA      0F      0CFE      FFFE       0031      010C     01 64 00 01   B22F
SMBusFrame_t clsTransSmartBus::DecodeFrame(uint8_t* pu8Frame)
{
	SMBusFrame_t sResult = {0};

	if (!IsValidSMBUSDLen(pu8Frame[0]))
		return sResult;

	uint16_t 	u16CRC = CalculateCRC16(pu8Frame, pu8Frame[0] - 2);
	if (u16CRC != htons(*(uint16_t*)&pu8Frame[pu8Frame[0]-2]))
		return sResult;



	sResult.dlen = pu8Frame[0];
	sResult.src_subid = pu8Frame[1];
	sResult.src_devid = pu8Frame[2];
	sResult.devtype = htons(*(uint16_t*)&pu8Frame[3]);
	sResult.opcode = htons(*(uint16_t*)&pu8Frame[5]);

	sResult.dst_subid = pu8Frame[7];
	sResult.dst_devid = pu8Frame[8];

	sResult.data = new uint8_t[sResult.dlen - SMBUS_FRM_HDR_SIZE];
	memcpy(sResult.data, &pu8Frame[9], sResult.dlen - SMBUS_FRM_HDR_SIZE);

	sResult.crc = u16CRC;

	return sResult;
}


} /* namespace TransSmartBus */
>>>>>>> 15491401a593fcf951f6b05586a0e95e9b6d9d1a
=======
/*
 * clsTransSmartBus.cpp
 *
 *  Created on: May 13, 2017
 *      Author: MANHBT
 */

#include "clsTransSmartBus.h"
#include "CRC16.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>     /* malloc, free, rand */
//#include <arpa/inet.h>
#include "convert.h"

namespace TransSmartBus {
clsTransSmartBus::clsTransSmartBus() {
	//

}

clsTransSmartBus::~clsTransSmartBus() {
	//
}

uint8_t* clsTransSmartBus::BuidFrame(SMBusFrame_t sFrame, uint8_t* payload)
{
	uint8_t* pu8Result = new uint8_t[sFrame.dlen + 2];

	memcpy((void*)pu8Result, (void*)&sFrame, sizeof(SMBusFrame_t));
    memcpy((void*)&pu8Result[SMBUS_FRM_IDX_DATA0], (void*)payload, sFrame.dlen);
	*(uint16_t*)&pu8Result[sFrame.dlen] = CalculateCRC16(&pu8Result[SMBUS_FRM_IDX_DLEN], sFrame.dlen);

	return pu8Result;

}

//+----------+------+----------+----------+--------+----------+------------+-------+
//| PREAMBLE | DLEN | SRC_ADDR | DEV_TYPE | OPCODE | DST_ADDR | DATA[0..n] | CRC_D |
//|  (2B)    | (1B) | (2B)     | (2B)     | (2B)   | (2B)     | (nB)       | (2B)  |
//+----------+------+----------+----------+--------+----------+------------+-------+
//	  AAAA      0F      0CFE      FFFE       0031      010C     01 64 00 01   B22F
SMBusFrame_t clsTransSmartBus::DecodeFrame(uint8_t* pu8Frame)
{
	SMBusFrame_t sResult = {0};

	if (!IsValidSMBUSDLen(pu8Frame[0]))
		return sResult;

	uint16_t 	u16CRC = CalculateCRC16(pu8Frame, pu8Frame[0] - 2);
	if (u16CRC != htons(*(uint16_t*)&pu8Frame[pu8Frame[0]-2]))
		return sResult;



	sResult.dlen = pu8Frame[0];
	sResult.src_subid = pu8Frame[1];
	sResult.src_devid = pu8Frame[2];
	sResult.devtype = htons(*(uint16_t*)&pu8Frame[3]);
	sResult.opcode = htons(*(uint16_t*)&pu8Frame[5]);

	sResult.dst_subid = pu8Frame[7];
	sResult.dst_devid = pu8Frame[8];

	sResult.data = new uint8_t[sResult.dlen - SMBUS_FRM_HDR_SIZE];
	memcpy(sResult.data, &pu8Frame[9], sResult.dlen - SMBUS_FRM_HDR_SIZE);

	sResult.crc = u16CRC;

	return sResult;
}


} /* namespace TransSmartBus */
>>>>>>> 15491401a593fcf951f6b05586a0e95e9b6d9d1a
