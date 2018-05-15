#ifndef QRINGBUFFER_H
#define QRINGBUFFER_H

#include "typedefs.h"
#include <string.h>
#include <stdlib.h>
#include <QMutex>

using namespace std;

class QRingBuffer
{
public:
    QRingBuffer();
    ~QRingBuffer();
    void			BufferInit(uint16_t u16Size, QMutex* mutex); //,
//                               CallbackFunctionNoIO callbackLockFPtr,
//                               CallbackFunctionNoIO callbackUnlockFPtr);


    BOOL 			BufferPush(void* pvData);
    uint8_t         BufferPop(void* pvData);
    BOOL 			BufferPushStream(void* pvStream, uint16_t u16Length);
    uint16_t        BufferPopStream(void* pvStream, uint16_t u16Length);

    BOOL			BufferPushBack(uint16_t u16PushBackNumber);
    uint16_t        BufferGetCount() {return u16ElementCount;}
    uint16_t        BufferGetAvailableCount() {return u16BufferSize - u16ElementCount;}
    void 			BufferFlush();

    void			BufferEnablePop() {bBufferPopEnable = TRUE;}
    void			BufferDisablePop() {bBufferPopEnable = FALSE;}
    void			BufferEnablePush() {bBufferPushEnable = TRUE;}
    void			BufferDisablePush() {bBufferPushEnable = FALSE;}
    BOOL 			BufferIsPushEnable() {return bBufferPushEnable;}
    BOOL 			BufferIsPopEnable() {return bBufferPopEnable;}
private:
    void*           pBuffer;					// Data buffer
    uint16_t		u16BufferSize;				// Size of buffer or the total of elements
    uint16_t    	u16ElementCount;				// The element count of buffer
    uint16_t		u16BufferPopPtr;				// The pointer to start reading
    uint16_t		u16BufferPushPtr;			// The pointer to start writing

    BOOL            bBufferPopEnable;			// Data popping enabling flag
    BOOL            bBufferPushEnable;			// Data pushing enabling flag
    QMutex          *pMutex;
};

#endif // QRINGBUFFER_H
