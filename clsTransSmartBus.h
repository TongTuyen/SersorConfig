/*
 * clsTransSmartBus.h
 *
 *  Created on: May 13, 2017
 *      Author: MANHBT
 */

#ifndef TRANSSMARTBUS_CLSTRANSSMARTBUS_H_
#define TRANSSMARTBUS_CLSTRANSSMARTBUS_H_
#include <stdint.h>

namespace TransSmartBus {

#define SMBUS_FRM_IDX_SOF					0		// Start of Frame - 2 bytes - must be 0x5555 in SMBUS protocol
#define SMBUS_FRM_IDX_DLEN					2		// Data_len - 2 bytes
#define SMBUS_FRM_IDX_SRC_SUB_ID			3		// Flags - 1 bytes
#define SMBUS_FRM_IDX_SRC_DEV_ID			4		// Dest. Address - 2 bytes
#define SMBUS_FRM_IDX_DEV_TYPE				5		// Src. Address - 2 bytes
#define SMBUS_FRM_IDX_OP_CODE				7		// CRC-8 of SMBUS Header
#define SMBUS_FRM_IDX_DST_SUB_ID			9		// Data payload - DLEN*sizeof(uint8_t) bytes
#define SMBUS_FRM_IDX_DST_DEV_ID			10
#define SMBUS_FRM_IDX_DATA0				11

#define SMBUS_FRM_HDR_SIZE				11

#define SMBUS_SOF_PREAMBLE				0xAA
#define SMBUS_SOF_VALUE					0xAAAA



#define SMBUS_FRM_DATA_MIN_SIZE       	13			// 13 bytes max
#define SMBUS_FRM_DATA_MAX_SIZE       	78			// 78 bytes max
#define IsValidSMBUSDLen(len)		((len > SMBUS_FRM_DATA_MIN_SIZE) && (len < SMBUS_FRM_DATA_MAX_SIZE))


//#define	SMBUS_FRM_MAX_SIZE				(RSP_FRM_HDR_SIZE + RSP_FRM_DATA_MAX_SIZE + 2)



//	+------+------+------------+------------+----------+---------+------------+------------+---------+-----+---------+-------+
//	| SOF  | DLEN | SRC_SUB_ID | SRC_DEV_ID | DEV_TYPE | OP_CODE | DST_SUB_ID | SDT_DEV_ID | DATA[0] | ... | DATA[n] | CRC_D |
//	| (2B) | (1B) | (1B)       | (1B)       | (2B)     | (2B)    | (1B)       | (1B)       |         |     |         | (2B)  |
//	+------+------+------------+------------+----------+---------+------------+------------+---------+-----+---------+-------+
//	  AAAA    0F       0C            FE        FF FE      00 31     01          0C             01   64 00 01            B22F
typedef struct __attribute__((packed)) _S_SMBUS_FRAME
{
	uint8_t		dlen;
	uint8_t		src_subid;
	uint8_t		src_devid;
	uint16_t	devtype;
	uint16_t	opcode;
	uint8_t		dst_subid;
	uint8_t		dst_devid;
	uint8_t*	data;
	uint16_t	crc;

} SMBusFrame_t;

class clsTransSmartBus {
public:
	clsTransSmartBus();
	virtual ~clsTransSmartBus();

	static uint8_t* BuidFrame(SMBusFrame_t sFrame, uint8_t* payload);
	static SMBusFrame_t DecodeFrame(uint8_t* pu8Frame);
};

} /* namespace TransSmartBus */


#endif /* TRANSSMARTBUS_CLSTRANSSMARTBUS_H_ */
