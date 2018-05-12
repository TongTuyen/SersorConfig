/*
 * agriculture.h
 *
 *  Created on: May 12, 2017
 *      Author: MANHBT
 */

#ifndef AGRICULTURE_H_
#define AGRICULTURE_H_
#include <stdint.h>
#include <string>

namespace AHGSystems {


typedef 	int (*CallbackFunction)(std::string szDeviceID, char* pcStatus, void* pvParent);
typedef 	void (*ThreadFuncPtr)(void* pInput, bool *pIsTerminate);
typedef  	void* (*PthreadPtr)(void*);

typedef enum _E_DEV_TYPE
{
	DEV_TYPE_BEGIN = 0,
	DEV_TYPE_SENSOR_NODE,
	DEV_TYPE_SENSOR_AIR_TEMPERATURE,
	DEV_TYPE_SENSOR_AIR_HUMIDITY,
	DEV_TYPE_SENSOR_SOIL_TEMPERATURE,
	DEV_TYPE_SENSOR_SOIL_MOISTURE,
	DEV_TYPE_ACTUATOR_VALVE,
	DEV_TYPE_ACTUATOR_PUMP,
	DEV_TYPE_INPUT_DIGITAL,
	DEV_TYPE_INPUT_ANALOG,
	DEV_TYPE_COUNTER,
	DEV_TYPE_TANK,
	DEV_TYPE_END
} DevType_t;

typedef enum _E_EVENT_TYPE
{
	DEV_EVENT_TYPE_BEGIN = 0,
	DEV_EVENT_TYPE_ON_STATUS_CHANGED,
	DEV_EVENT_TYPE_ON_SENSOR_STATUS_CHANGED,
	DEV_EVENT_TYPE_NONE,
	DEV_EVENT_TYPE_END
} Event_t;

#define MIN(a,b)	(a>b ? b: a)

#define IsDigitalOutDevice(dev_type) ((dev_type == DEV_TYPE_ACTUATOR_VALVE) || \
										(dev_type == DEV_TYPE_ACTUATOR_PUMP))

#define IsSensorDevice(dev_type)	((dev_type == DEV_TYPE_SENSOR_COMMON) || \
									(dev_type == DEV_TYPE_SENSOR_AIR_TEMPERATURE) || \
									(dev_type == DEV_TYPE_SENSOR_AIR_HUMIDITY) || \
									(dev_type == DEV_TYPE_SENSOR_SOIL_TEMPERATURE) || \
									(dev_type == DEV_TYPE_SENSOR_SOIL_HUMIDITY) || \
									(dev_type == DEV_TYPE_SENSOR_LIGHT_INTENSITY))

typedef enum _E_CONN_TYPE {
	CONN_TYPE_START = 0,
	CONN_TYPE_RS485,
	CONN_TYPE_UART,
	CONN_TYPE_SPI,
	CONN_TYPE_I2C,
	CONN_TYPE_END
} ConnType_t;

typedef struct _S_FRAME
{
	uint8_t 	len;
	uint8_t* 	data;
}ProtoFrame_t;

typedef enum _E_SENS_RESPONSE_RESULT
{
	SENS_RESPONSE_OK = 0x00,
	SENS_RESPONSE_HW_ERROR,
	SENS_RESPONSE_TIMEDOUT,
	SENS_RESPONSE_INVALID_PARAMS,
	SENS_RESPONSE_UNKNOWN
} SensResponseResult_t;

//typedef struct __attribute__((packed)) _S_SENS_VALUE
//{
//	int16_t 	air_temp;
//	int16_t		air_humid;
//	int16_t		soil_temp;
//	int16_t		soil_humid;
//} SensValue_t;

//typedef enum _CMD_EXEC_ERR_CODE {
//	CMD_EXEC_ERR_OK,
//	CMD_EXEC_ERR_CMD_INVALID,
//	CMD_EXEC_ERR_PARAM_INVALID
//} CmdExecErrorCode_t;
//
//typedef enum	S_I2C_CLOCK
//{
//	I2C_CLOCK_100KHZ = 0,
//	I2C_CLOCK_400KHz,
//	I2C_CLOCK_1MHZ
//} I2CClock_t;

#define  		CMD_SENSOR_AIR_TEMPERATURE_GET_VALUE           	0x10
#define     	CMD_SENSOR_AIR_HUMIDITY_GET_VALUE              	0x11
#define      	CMD_SENSOR_SOIL_TEMPERATURE_GET_VALUE          	0x12
#define      	CMD_SENSOR_SOIL_HUMIDITY_GET_VALUE        		0x13
#define 	   	CMD_SENSOR_GET_ALL_VALUE						0x14
#define 	   	CMD_SENSOR_LIGHT_INTENSITY_GET_VALUE			0x15

#define 		CMD_COUNTER_GET_VALUE							0x16
#define			CMD_COUNTER_RESET_VALUE							0x17

#define        	CMD_ACTUATOR_ON									0x18
#define        	CMD_ACTUATOR_OFF								0x19
#define        	CMD_ACTUATOR_GET_ON_OFF							0x31

//0x18//0x19//0x1A
// ---------------------- Tank commands ----------------------------
#define 		CMD_SET_HUMI_SOIL_THRES1						0x20
#define 		CMD_SET_HUMI_SOIL_THRES2						0x21
#define 		CMD_RUN_1										0x22
#define 		CMD_RUN_2										0x23
#define 		CMD_RUN_3										0x24
#define 		CMD_STOP_1										0x25
#define 		CMD_STOP_2										0x26
#define 		CMD_STOP_3										0x27
#define 		CMD_GET_STATUS									0x28

#define 		CMD_W_PUMP_1_TURN_ON							0x29
#define 		CMD_W_PUMP_1_TURN_OFF							0x2A
#define 		CMD_W_PUMP_2_TURN_ON							0x2B
#define 		CMD_W_PUMP_2_TURN_OFF							0x2C

#define 		CMD_W_VALVE_2_TURN_ON							0x2D
#define 		CMD_W_VALVE_2_TURN_OFF							0x2E
#define 		CMD_W_VALVE_3_TURN_ON							0x2F
#define 		CMD_W_VALVE_3_TURN_OFF							0x30

#define        	ACTUATOR_STATUS_ON        						1
#define        	ACTUATOR_STATUS_OFF      						0


#ifndef ON
#define ON	 	1
#endif

#ifndef OFF
#define OFF		0
#endif


#ifndef HIGH
#define HIGH	1
#endif

#ifndef LOW
#define LOW	 	0
#endif
}



#endif /* AGRICULTURE_H_ */
