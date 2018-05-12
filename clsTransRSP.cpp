<<<<<<< HEAD
<<<<<<< HEAD
/*
 * clsTransRSP.cpp
 *
 *  Created on: May 13, 2017
 *      Author: MANHBT
 */

#include "clsTransRSP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>     /* malloc, free, rand */
//#include <arpa/inet.h>
#include <CRC8.h>
#include <CRC16.h>
#include <QDebug>
#include <convert.h>

namespace TransRSP {

clsTransRSP::clsTransRSP() {
	//

}

clsTransRSP::~clsTransRSP() {
	//
}

RSPFrame_t clsTransRSP::DecodeFrame(uint8_t *pu8Frame)
{


	RSPFrame_t sResult;



	sResult.dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE]);
	sResult.flags	= pu8Frame[RSP_FRM_IDX_FLAGS - RSP_FRM_SOF_SIZE];
	sResult.seq	    = pu8Frame[RSP_FRM_IDX_SEQ - RSP_FRM_SOF_SIZE];
	sResult.dstaddr = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DSTADDR - RSP_FRM_SOF_SIZE]);
	sResult.srcaddr = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_SRCADDR - RSP_FRM_SOF_SIZE]);
	sResult.crch	= pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE];
	sResult.crcd  	= htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + sResult.dlen]);

//	LREP("[clsTransRSP::DecodeFram] Raw data: ");

//		for(int i = 0; i < sResult.dlen + RSP_FRM_HDR_SIZE + 2; i++)

//		LREP_RAW("%.2x ", pu8Frame[i]);


	if (IS_DLEN_VALID(sResult.dlen))
	{
		sResult.data = new uint8_t[sResult.dlen];

		memcpy(sResult.data, &pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE], sResult.dlen);
	}

    qDebug("\r\n[clsTransRSP::DecodeFrame] Decoded frame: len: %.4x, flags: %.2x, seq: %.2x, dst: %.4x, src: %.4x, crch: %.2x, crcd: %.4x",
            sResult.dlen,
            sResult.flags,
            sResult.seq,
            sResult.dstaddr,
            sResult.srcaddr,
            sResult.crch,
            sResult.crcd);
	return sResult;

}

uint8_t* clsTransRSP::BuildFrame(RSPFrame_t frame, uint8_t* pu8DataPayload)
{
	if(!IS_DLEN_VALID(frame.dlen) || (pu8DataPayload == nullptr))
		return nullptr;

	uint16_t	u16FrameLen = frame.dlen + RSP_FRM_HDR_SIZE + RSP_FRM_CRCD_SIZE + RSP_FRM_EOF_SIZE;
	uint8_t* pRawFrame = new uint8_t[u16FrameLen];

	// Insert Start-Of-Frame
	pRawFrame[0] = RSP_SOF_PREAMBLE;
	pRawFrame[1] = RSP_SOF_PREAMBLE;

	// Insert End-Of-Frame
	pRawFrame[u16FrameLen] = 0xFF;

	memcpy(&pRawFrame[2], (void*)&frame, 8);
	// Calculate CRCH
	pRawFrame[RSP_FRM_IDX_CRCH] = CalCRC8(&pRawFrame[2], 8);

	// Copy data
	memcpy(&pRawFrame[RSP_FRM_IDX_DATA0], pu8DataPayload, frame.dlen);

	// Calculate CRC_D
	*(uint16_t*)&pRawFrame[u16FrameLen - 3] = htons(CalculateCRC16(&pRawFrame[RSP_FRM_IDX_DATA0], frame.dlen));

	return pRawFrame;


}

} /* namespace TransRSP */
=======
/*
 * clsTransRSP.cpp
 *
 *  Created on: May 13, 2017
 *      Author: MANHBT
 */

#include "clsTransRSP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>     /* malloc, free, rand */
//#include <arpa/inet.h>
#include <CRC8.h>
#include <CRC16.h>
#include <QDebug>
#include <convert.h>

namespace TransRSP {

clsTransRSP::clsTransRSP() {
	//

}

clsTransRSP::~clsTransRSP() {
	//
}

RSPFrame_t clsTransRSP::DecodeFrame(uint8_t *pu8Frame)
{


	RSPFrame_t sResult;



	sResult.dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE]);
	sResult.flags	= pu8Frame[RSP_FRM_IDX_FLAGS - RSP_FRM_SOF_SIZE];
	sResult.seq	    = pu8Frame[RSP_FRM_IDX_SEQ - RSP_FRM_SOF_SIZE];
	sResult.dstaddr = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DSTADDR - RSP_FRM_SOF_SIZE]);
	sResult.srcaddr = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_SRCADDR - RSP_FRM_SOF_SIZE]);
	sResult.crch	= pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE];
	sResult.crcd  	= htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + sResult.dlen]);

//	LREP("[clsTransRSP::DecodeFram] Raw data: ");

//		for(int i = 0; i < sResult.dlen + RSP_FRM_HDR_SIZE + 2; i++)

//		LREP_RAW("%.2x ", pu8Frame[i]);


	if (IS_DLEN_VALID(sResult.dlen))
	{
		sResult.data = new uint8_t[sResult.dlen];

		memcpy(sResult.data, &pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE], sResult.dlen);
	}

    qDebug("\r\n[clsTransRSP::DecodeFrame] Decoded frame: len: %.4x, flags: %.2x, seq: %.2x, dst: %.4x, src: %.4x, crch: %.2x, crcd: %.4x",
            sResult.dlen,
            sResult.flags,
            sResult.seq,
            sResult.dstaddr,
            sResult.srcaddr,
            sResult.crch,
            sResult.crcd);
	return sResult;

}

uint8_t* clsTransRSP::BuildFrame(RSPFrame_t frame, uint8_t* pu8DataPayload)
{
	if(!IS_DLEN_VALID(frame.dlen) || (pu8DataPayload == nullptr))
		return nullptr;

	uint16_t	u16FrameLen = frame.dlen + RSP_FRM_HDR_SIZE + RSP_FRM_CRCD_SIZE + RSP_FRM_EOF_SIZE;
	uint8_t* pRawFrame = new uint8_t[u16FrameLen];

	// Insert Start-Of-Frame
	pRawFrame[0] = RSP_SOF_PREAMBLE;
	pRawFrame[1] = RSP_SOF_PREAMBLE;

	// Insert End-Of-Frame
	pRawFrame[u16FrameLen] = 0xFF;

	memcpy(&pRawFrame[2], (void*)&frame, 8);
	// Calculate CRCH
	pRawFrame[RSP_FRM_IDX_CRCH] = CalCRC8(&pRawFrame[2], 8);

	// Copy data
	memcpy(&pRawFrame[RSP_FRM_IDX_DATA0], pu8DataPayload, frame.dlen);

	// Calculate CRC_D
	*(uint16_t*)&pRawFrame[u16FrameLen - 3] = htons(CalculateCRC16(&pRawFrame[RSP_FRM_IDX_DATA0], frame.dlen));

	return pRawFrame;


}

} /* namespace TransRSP */
>>>>>>> 15491401a593fcf951f6b05586a0e95e9b6d9d1a
=======
/*
 * clsTransRSP.cpp
 *
 *  Created on: May 13, 2017
 *      Author: MANHBT
 */

#include "clsTransRSP.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>     /* malloc, free, rand */
//#include <arpa/inet.h>
#include <CRC8.h>
#include <CRC16.h>
#include <QDebug>
#include <convert.h>

namespace TransRSP {

clsTransRSP::clsTransRSP() {
	//

}

clsTransRSP::~clsTransRSP() {
	//
}

RSPFrame_t clsTransRSP::DecodeFrame(uint8_t *pu8Frame)
{


	RSPFrame_t sResult;



	sResult.dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE]);
	sResult.flags	= pu8Frame[RSP_FRM_IDX_FLAGS - RSP_FRM_SOF_SIZE];
	sResult.seq	    = pu8Frame[RSP_FRM_IDX_SEQ - RSP_FRM_SOF_SIZE];
	sResult.dstaddr = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DSTADDR - RSP_FRM_SOF_SIZE]);
	sResult.srcaddr = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_SRCADDR - RSP_FRM_SOF_SIZE]);
	sResult.crch	= pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE];
	sResult.crcd  	= htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + sResult.dlen]);

//	LREP("[clsTransRSP::DecodeFram] Raw data: ");

//		for(int i = 0; i < sResult.dlen + RSP_FRM_HDR_SIZE + 2; i++)

//		LREP_RAW("%.2x ", pu8Frame[i]);


	if (IS_DLEN_VALID(sResult.dlen))
	{
		sResult.data = new uint8_t[sResult.dlen];

		memcpy(sResult.data, &pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE], sResult.dlen);
	}

    qDebug("\r\n[clsTransRSP::DecodeFrame] Decoded frame: len: %.4x, flags: %.2x, seq: %.2x, dst: %.4x, src: %.4x, crch: %.2x, crcd: %.4x",
            sResult.dlen,
            sResult.flags,
            sResult.seq,
            sResult.dstaddr,
            sResult.srcaddr,
            sResult.crch,
            sResult.crcd);
	return sResult;

}

uint8_t* clsTransRSP::BuildFrame(RSPFrame_t frame, uint8_t* pu8DataPayload)
{
	if(!IS_DLEN_VALID(frame.dlen) || (pu8DataPayload == nullptr))
		return nullptr;

	uint16_t	u16FrameLen = frame.dlen + RSP_FRM_HDR_SIZE + RSP_FRM_CRCD_SIZE + RSP_FRM_EOF_SIZE;
	uint8_t* pRawFrame = new uint8_t[u16FrameLen];

	// Insert Start-Of-Frame
	pRawFrame[0] = RSP_SOF_PREAMBLE;
	pRawFrame[1] = RSP_SOF_PREAMBLE;

	// Insert End-Of-Frame
	pRawFrame[u16FrameLen] = 0xFF;

	memcpy(&pRawFrame[2], (void*)&frame, 8);
	// Calculate CRCH
	pRawFrame[RSP_FRM_IDX_CRCH] = CalCRC8(&pRawFrame[2], 8);

	// Copy data
	memcpy(&pRawFrame[RSP_FRM_IDX_DATA0], pu8DataPayload, frame.dlen);

	// Calculate CRC_D
	*(uint16_t*)&pRawFrame[u16FrameLen - 3] = htons(CalculateCRC16(&pRawFrame[RSP_FRM_IDX_DATA0], frame.dlen));

	return pRawFrame;


}

} /* namespace TransRSP */
>>>>>>> 15491401a593fcf951f6b05586a0e95e9b6d9d1a
