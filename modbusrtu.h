#ifndef MODBUSRTU_H
#define MODBUSRTU_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

uint16_t ModRTU_CRC(uint8_t* buf, int len);

#ifdef __cplusplus
}
#endif

#endif // MODBUSRTU_H
