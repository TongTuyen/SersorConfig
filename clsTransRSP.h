/*
 * clsTransRSP.h
 *
 *  Created on: May 13, 2017
 *      Author: MANHBT
 */

#ifndef TRANSRSP_CLSTRANSRSP_H_
#define TRANSRSP_CLSTRANSRSP_H_

#include <stdint.h>

namespace TransRSP {


#define 	ENABLE_CHECK_DUPLICATED_SEQ_NUMBER		1

///////////////////////////////////// Constant Definitions ///////////////////////////////////////////
#define IS_DATA_FRM(fk)					((fk & RSP_FRM_TYPE_MASK) ? FALSE : TRUE)
#define IS_CMD_FRM(fk)					((fk & RSP_FRM_TYPE_MASK) ? TRUE : FALSE)

#define IS_ACK_FRAME(f)                 (((f & ~RSP_FRM_CTRL_MASK) == RSP_CMD_ACK ) ? TRUE : FALSE)
#define IS_NACK_FRAME(f)                (((f & ~RSP_FRM_CTRL_MASK) == RSP_CMD_NACK ) ? TRUE : FALSE)

#define RETURN_PACKAGE					TRUE	// use for ACK/NACK/NOTDONE frame
#define SEND_PACKAGE					FALSE

#define RSP_ADDR_BROADCAST				0xFFFF
#define RSP_ADDR_PC						0x0001
#define RSP_ADDR_USBCANCONV				0x0002

// Frame structure and macros
#if 1
#define RSP_FRM_IDX_SOF					0		// Start of Frame - 2 bytes - must be 0x5555 in RSP protocol
#define RSP_FRM_IDX_DLEN				2		// Data_len - 2 bytes
#define RSP_FRM_IDX_FLAGS				4		// Flags - 1 bytes
#define	RSP_FRM_IDX_SEQ					5		// Frame Sequence number
#define RSP_FRM_IDX_DSTADDR				6		// Dest. Address - 2 bytes nguoi gui
#define RSP_FRM_IDX_SRCADDR				8		// Src. Address - 2 bytes nguoi nhan
#define RSP_FRM_IDX_CRCH				10		// CRC-8 of RSP Header
#define RSP_FRM_IDX_DATA0				11		// Data payload - DLEN*sizeof(uint8_t) bytes
#else

#define RSP_FRM_IDX_SOF					0		// Start of Frame - 2 bytes - must be 0x5555 in RSP protocol
#define RSP_FRM_IDX_DLEN				2		// Data_len - 2 bytes
#define RSP_FRM_IDX_FLAGS				4		// Flags - 1 bytes
#define RSP_FRM_IDX_DSTADDR				5		// Dest. Address - 2 bytes
#define RSP_FRM_IDX_SRCADDR				7		// Src. Address - 2 bytes
#define RSP_FRM_IDX_CRCH				9		// CRC-8 of RSP Header
#define RSP_FRM_IDX_DATA0				10		// Data payload - DLEN*sizeof(uint8_t) bytes
#endif


#define RSP_SOF_PREAMBLE				0x55
#define RSP_SOF_VALUE					0x5555
#define RSP_FRM_DATA_MAX_SIZE       	0x400	// 1024 bytes max

#define RSP_FRM_HDR_SIZE				11

#define RSP_FRM_CRCD_SIZE				2
#define RSP_FRM_EOF_SIZE				1
#define RSP_FRM_SOF_SIZE				2
#define	RSP_FRM_MAX_SIZE				(RSP_FRM_HDR_SIZE + RSP_FRM_DATA_MAX_SIZE + RSP_FRM_SOF_SIZE)

#define IS_DLEN_VALID(dlen)				((dlen > 0) && (dlen < RSP_FRM_DATA_MAX_SIZE))

//#define FORM_CTRL(ACKReq, FrmTpy)		(((ACKReq) ? 0x80 : 0x00) | (FrmTpy & 0x7F))
#define IS_ACK_REQ(flags)				((flags) & RSP_FRM_ACKREQ_MASK ? TRUE : FALSE)
//#define GET_FRM_TYPE(ctrl)				((ctrl) & 0x7F)


#define RSP_FRM_TYPE_MASK				0x80	// Bit 7-6: type cmd or data frame
#define RSP_FRM_TYPE_DATA				0x00	// Bit 7: 0 - Data frame
#define RSP_FRM_TYPE_CMD				0x80	// Bit 7: 1 - Command Frame

#define RSP_FRM_ACKREQ_MASK				0x40	// bit 6: Ack required
#define RSP_FRM_ACKREQ_REQUIRED      	0x40
#define RSP_FRM_ACKREQ_NOT_REQUIRED  	0x00

#define RSP_FRM_CTRL_MASK               (RSP_FRM_TYPE_MASK | RSP_FRM_ACKREQ_MASK)

// Command Control for CMD_FRAME
#define RSP_CMD_NONE					0x00	// None command
#define	RSP_CMD_ACK                     0x01	// ACK command, which returned when a frame is processed successfully
#define RSP_CMD_NACK					0x02	// NACK command, which returned when a frame is processed unsuccessfully



/**
 * RSP Frame structure
 * 0_____2______4______5_____6_____8_____10______11__________________________
 * +-----+------+------+-----+-----+-----+-------+-------------+-------+-----+
 * | SOF | DLEN | FLAG | SEQ | DST | SRC | CRC_H | DATA[0...n] | CRC_D | EOF |
 * | (2) |  (2) |  (1) | (1) | (2) | (2) |  (1)  |   (DLEN)    |  (2)  | (1) |
 * +-----+------+------+-----+-----+-----+-------+-------------+-------+-----+
 */

//+-----+-----+------+------+-------+----------+----------+----------+----------+-------+------+-----+------+-------+
//| SOF | SOF | DLEN | DLEN | FLAGS | DST_ADDR | DST_ADDR | SRC_ADDR | SRC_ADDR | CRC_H | DAT0 | ... | DATn | CRC_D |
//+-----+-----+------+------+-------+----------+----------+----------+----------+-------+------+-----+------+-------+
typedef struct __attribute__((packed)) _S_RSP_FRAME
{
	uint16_t	dlen;
	uint8_t		flags;
	uint8_t		seq;
	uint16_t	dstaddr;
	uint16_t 	srcaddr;
	uint8_t		crch;
	uint8_t*	data;
	uint16_t	crcd;
} RSPFrame_t;

class clsTransRSP {
public:
	clsTransRSP();
	virtual ~clsTransRSP();
	static uint8_t* BuildFrame(RSPFrame_t frame, uint8_t* pu8DataPayload);
	static RSPFrame_t DecodeFrame(uint8_t *pu8Frame);
};




} /* namespace TransRSP */

#endif /* TRANSRSP_CLSTRANSRSP_H_ */
