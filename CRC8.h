#ifndef _CRC8_H_
#define _CRC8_H_

#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////// Include Files /////////////////////////////////////////////
#include "typedefs.h"

///////////////////////////////////// Constant Definitions ///////////////////////////////////////////

/////////////////////////////////////// Type Definitions /////////////////////////////////////////////

///////////////////////////// Macros (Inline Functions) Definitions //////////////////////////////////
#define ByteCRC8(crc, ch) 	(g_byCRC8Table[crc ^ ch])



///////////////////////////////////// Variable Definitions ///////////////////////////////////////////
extern const uint8_t g_byCRC8Table[];

///////////////////////////////////// Function Prototypes ////////////////////////////////////////////
/**
 * Calculate the CRC-8 of data buffer
 * @param pBuff data buffer
 * @param nSize data size
 * @return calculated CRC8 value
 */
uint8_t CalCRC8(uint8_t* pBuff, uint16_t nSize);
#ifdef __cplusplus
}
#endif

#endif // CRC8_H

