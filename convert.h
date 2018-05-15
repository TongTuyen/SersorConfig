#ifndef CONVERT_H
#define CONVERT_H

#define htons   hton16
#define ntohs   ntoh16

#define MIN(a,b)	(a>b ? b: a)

static inline uint16_t hton16(uint16_t val){
    return ((val & (uint16_t)0xFF00) >> 8) | ((val & (uint16_t)0x00FF) << 8);
}

static inline uint16_t ntoh16(uint16_t val){
    return ((val & (uint16_t)0xFF00) >> 8) | ((val & (uint16_t)0x00FF) << 8);
}

#define SENSOR_AIR_TEMPERATURE 		0x01
#define SENSOR_AIR_HUMIDITY			0x02
#define SENSOR_SOIL_TEMPERATURE		0x03
#define SENSOR_SOIL_MOISTURE		0x04
#define SENSOR_LIGHT_AMBIENT		0x05
#define SENSOR_WATER_EC				0x06
#define SENSOR_WATER_PH				0x07
#define SENSOR_WATER_TEMPERATURE	0x08
#define SENSOR_WATER_LEVEL			0x09
#define SENSOR_SOIL_EC				0x0A
#define SENSOR_SOIL_PH				0x0B
#define SENSOR_WATER_STATUS   		0x0C

#endif // CONVERT_H
