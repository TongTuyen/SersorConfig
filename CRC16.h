/*
 * CRC16.h
 *
 *  Created on: May 13, 2017
 *      Author: MANHBT
 */

#ifndef COMMON_CRC16_H_
#define COMMON_CRC16_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

extern uint16_t CRC_TAB[];
void Pack_crc(uint8_t *ptr, unsigned int len);
bool Check_crc(uint8_t *ptr, unsigned int len);
uint16_t CalculateCRC16(uint8_t* pData, uint16_t u16Len);

#ifdef __cplusplus
}
#endif


#endif /* COMMON_CRC16_H_ */
