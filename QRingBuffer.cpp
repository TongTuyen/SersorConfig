<<<<<<< HEAD
<<<<<<< HEAD
#include "QRingBuffer.h"
#include <QObject>
#include <QDebug>
QRingBuffer::QRingBuffer()
{
    pBuffer = nullptr;

}

QRingBuffer::~QRingBuffer()
{
    free(pBuffer);
}

void QRingBuffer::BufferInit(uint16_t u16Size, QMutex* mutex)
//                             CallbackFunctionNoIO callbackLockFPtr,
//                             CallbackFunctionNoIO callbackUnlockFPtr)
{
    if(!pBuffer)
        pBuffer = malloc(u16Size);

//    qDebug() << "Malloc pBuffer at " << QString.fromStdString((uint32_t)pBuffer);
//    callbackLock = callbackLockFPtr;
//    callbackUnlock = callbackUnlockFPtr;
    if (mutex != nullptr)
        pMutex = mutex;

    u16BufferSize = u16Size;
    u16ElementCount 	= 0;
    u16BufferPopPtr	= 0;
    u16BufferPushPtr	= 0;

    bBufferPopEnable	= TRUE;
    bBufferPushEnable	= TRUE;
}

BOOL QRingBuffer::BufferPush(void* pvData)
  {
      uint8_t*	pu8Buffer	= NULL;

      if ((u16ElementCount >= u16BufferSize) || (bBufferPushEnable == FALSE))
      {
          return FALSE;
      }

      // Lock accessing to the buffer
//      if (callbackLock)
//      {
//          callbackLock();
//      }
      pMutex->lock();

      // Calculate the start address for pushing in
      pu8Buffer = (uint8_t*)pBuffer + u16BufferPushPtr;

      // Push data element
      memcpy(pu8Buffer, pvData, 1);

      // Point the push pointer to the new position
      u16BufferPushPtr++;
      if (u16BufferPushPtr >= u16BufferSize)
      {
          u16BufferPushPtr = 0;
      }

      // Increase element count of the buffer
      u16ElementCount++;

      // Unlock accessing to the buffer
//      if (callbackUnlock)
//      {
//          callbackUnlock();
//      }
      pMutex->unlock();

      return TRUE;
  }

uint8_t QRingBuffer::BufferPop(void* pvData)
{
    uint8_t*	pu8Data	= NULL;

    if ((u16ElementCount == 0) || (bBufferPopEnable == FALSE))
    {
        return 0;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Calculate the start address for popping out
    pu8Data = (uint8_t*)pBuffer + u16BufferPopPtr;

    // Pop data element
    memcpy(pvData, pu8Data, 1);

    // Point the pop pointer to the new position
    u16BufferPopPtr++;
    if (u16BufferPopPtr >= u16BufferSize)
    {
        u16BufferPopPtr = 0;
    }

    // Decrease element count of the buffer
    u16ElementCount--;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return 1;
}

BOOL QRingBuffer::BufferPushStream(void* pvStream, uint16_t u16Length)
{
    uint8_t*	pu8Buffer		= NULL;
    uint8_t*	pu8RestStream	= NULL;

    if (((u16ElementCount + u16Length) > u16BufferSize) || (bBufferPushEnable == FALSE))
    {
        return FALSE;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Calculate the start address for pushing in
    pu8Buffer = (uint8_t*)pBuffer + u16BufferPushPtr;// * psRingBuffer->uiElementSize;

    // If the pushing address is out of address range of the buffer then we need to push twice
    if ((u16BufferPushPtr + u16Length) > u16BufferSize)
    {
        // Push data stream from start address to the final address of the buffer
        memcpy(pu8Buffer, pvStream, u16BufferSize - u16BufferPushPtr);

        // Return the start address to the first address of the buffer for pushing the rest of stream
        pu8Buffer = (uint8_t*)pBuffer;

        // Point to address of the rest of stream
        pu8RestStream = (uint8_t*)pvStream + u16BufferSize - u16BufferPushPtr;

        // Push the rest of stream
        memcpy(pu8Buffer, pu8RestStream, u16Length + u16BufferPushPtr - u16BufferSize);
    }
    else
    {
        // Push data stream
        memcpy(pu8Buffer, pvStream, u16Length);
    }

    // Point the push pointer to the new position
    u16BufferPushPtr += u16Length;
    if (u16BufferPushPtr >= u16BufferSize)
    {
        u16BufferPushPtr -= u16BufferSize;
    }

    // Increase element count of the buffer
    u16ElementCount += u16Length;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return TRUE;	// Push successfully
}

uint16_t QRingBuffer::BufferPopStream(void* pvStream, uint16_t u16Length)
{
    uint16_t	u16PopCount		= 0;
    uint8_t*	pu8Buffer		= NULL;
    uint8_t*    pu8RestStream	= NULL;

    if ((u16ElementCount == 0) || (bBufferPopEnable == FALSE))
    {
        return 0;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Limit length of data stream will be popped
    if (u16Length < u16ElementCount)
    {
        u16PopCount = u16Length;
    }
    else
    {
        u16PopCount = u16ElementCount;
    }

    // Calculate the start address for popping out
    pu8Buffer = (uint8_t*)pBuffer + u16BufferPopPtr ;

    // If the popping address is out of address range of the buffer then we need to pop twice
    if ((u16BufferPopPtr + u16PopCount) > u16BufferSize)
    {
        // Pop data stream from start address to the final address of the buffer
        memcpy(pvStream, pu8Buffer,u16BufferSize - u16BufferPopPtr);

        // Return the start address to the first address of the buffer for popping the rest of stream
        pu8Buffer = (uint8_t*)pBuffer;

        // Point to address of the rest of stream
        pu8RestStream = (uint8_t*)pvStream + u16BufferSize - u16BufferPopPtr;

        // Pop the rest of stream
        memcpy(pu8RestStream, pu8Buffer, u16PopCount + u16BufferPopPtr - u16BufferSize);
    }
    else
    {
        // Pop data stream
        memcpy(pvStream, pu8Buffer, u16PopCount);
    }

    // Point the pop pointer to the new position
    u16BufferPopPtr += u16PopCount;
    if (u16BufferPopPtr >= u16BufferSize)
    {
        u16BufferPopPtr -= u16BufferSize;
    }

    // Decrease element count of the buffer
    u16ElementCount -= u16PopCount;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return u16PopCount;	// Return the number of elements popped out actually
}

void QRingBuffer::BufferFlush()
{
    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    u16ElementCount 	= 0;
    u16BufferPopPtr	= 0;
    u16BufferPushPtr	= 0;
    bBufferPopEnable	= TRUE;
    bBufferPushEnable = TRUE;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();
}

BOOL QRingBuffer::BufferPushBack(uint16_t u16PushBackNumber)
{
    // Cannot push back if the push back number is greater than the available element count
    if (u16PushBackNumber > (u16BufferSize - u16ElementCount))
    {
        return FALSE;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Disable pushing to prohibiting changing of push pointer
    bBufferPushEnable = FALSE;

    // Push back element count
    u16ElementCount += u16PushBackNumber;

    // Push back pop pointer
    if (u16BufferPopPtr >= u16PushBackNumber)
    {
        u16BufferPopPtr -= u16PushBackNumber;
    }
    else
    {
        u16BufferPopPtr = u16BufferPopPtr + u16BufferSize - u16PushBackNumber;
    }

    // Enable pushing
    bBufferPushEnable = TRUE;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return TRUE;
}
=======
#include "QRingBuffer.h"
#include <QObject>
#include <QDebug>
QRingBuffer::QRingBuffer()
{
    pBuffer = nullptr;

}

QRingBuffer::~QRingBuffer()
{
    free(pBuffer);
}

void QRingBuffer::BufferInit(uint16_t u16Size, QMutex* mutex)
//                             CallbackFunctionNoIO callbackLockFPtr,
//                             CallbackFunctionNoIO callbackUnlockFPtr)
{
    if(!pBuffer)
        pBuffer = malloc(u16Size);

//    qDebug() << "Malloc pBuffer at " << QString.fromStdString((uint32_t)pBuffer);
//    callbackLock = callbackLockFPtr;
//    callbackUnlock = callbackUnlockFPtr;
    if (mutex != nullptr)
        pMutex = mutex;

    u16BufferSize = u16Size;
    u16ElementCount 	= 0;
    u16BufferPopPtr	= 0;
    u16BufferPushPtr	= 0;

    bBufferPopEnable	= TRUE;
    bBufferPushEnable	= TRUE;
}

BOOL QRingBuffer::BufferPush(void* pvData)
  {
      uint8_t*	pu8Buffer	= NULL;

      if ((u16ElementCount >= u16BufferSize) || (bBufferPushEnable == FALSE))
      {
          return FALSE;
      }

      // Lock accessing to the buffer
//      if (callbackLock)
//      {
//          callbackLock();
//      }
      pMutex->lock();

      // Calculate the start address for pushing in
      pu8Buffer = (uint8_t*)pBuffer + u16BufferPushPtr;

      // Push data element
      memcpy(pu8Buffer, pvData, 1);

      // Point the push pointer to the new position
      u16BufferPushPtr++;
      if (u16BufferPushPtr >= u16BufferSize)
      {
          u16BufferPushPtr = 0;
      }

      // Increase element count of the buffer
      u16ElementCount++;

      // Unlock accessing to the buffer
//      if (callbackUnlock)
//      {
//          callbackUnlock();
//      }
      pMutex->unlock();

      return TRUE;
  }

uint8_t QRingBuffer::BufferPop(void* pvData)
{
    uint8_t*	pu8Data	= NULL;

    if ((u16ElementCount == 0) || (bBufferPopEnable == FALSE))
    {
        return 0;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Calculate the start address for popping out
    pu8Data = (uint8_t*)pBuffer + u16BufferPopPtr;

    // Pop data element
    memcpy(pvData, pu8Data, 1);

    // Point the pop pointer to the new position
    u16BufferPopPtr++;
    if (u16BufferPopPtr >= u16BufferSize)
    {
        u16BufferPopPtr = 0;
    }

    // Decrease element count of the buffer
    u16ElementCount--;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return 1;
}

BOOL QRingBuffer::BufferPushStream(void* pvStream, uint16_t u16Length)
{
    uint8_t*	pu8Buffer		= NULL;
    uint8_t*	pu8RestStream	= NULL;

    if (((u16ElementCount + u16Length) > u16BufferSize) || (bBufferPushEnable == FALSE))
    {
        return FALSE;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Calculate the start address for pushing in
    pu8Buffer = (uint8_t*)pBuffer + u16BufferPushPtr;// * psRingBuffer->uiElementSize;

    // If the pushing address is out of address range of the buffer then we need to push twice
    if ((u16BufferPushPtr + u16Length) > u16BufferSize)
    {
        // Push data stream from start address to the final address of the buffer
        memcpy(pu8Buffer, pvStream, u16BufferSize - u16BufferPushPtr);

        // Return the start address to the first address of the buffer for pushing the rest of stream
        pu8Buffer = (uint8_t*)pBuffer;

        // Point to address of the rest of stream
        pu8RestStream = (uint8_t*)pvStream + u16BufferSize - u16BufferPushPtr;

        // Push the rest of stream
        memcpy(pu8Buffer, pu8RestStream, u16Length + u16BufferPushPtr - u16BufferSize);
    }
    else
    {
        // Push data stream
        memcpy(pu8Buffer, pvStream, u16Length);
    }

    // Point the push pointer to the new position
    u16BufferPushPtr += u16Length;
    if (u16BufferPushPtr >= u16BufferSize)
    {
        u16BufferPushPtr -= u16BufferSize;
    }

    // Increase element count of the buffer
    u16ElementCount += u16Length;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return TRUE;	// Push successfully
}

uint16_t QRingBuffer::BufferPopStream(void* pvStream, uint16_t u16Length)
{
    uint16_t	u16PopCount		= 0;
    uint8_t*	pu8Buffer		= NULL;
    uint8_t*    pu8RestStream	= NULL;

    if ((u16ElementCount == 0) || (bBufferPopEnable == FALSE))
    {
        return 0;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Limit length of data stream will be popped
    if (u16Length < u16ElementCount)
    {
        u16PopCount = u16Length;
    }
    else
    {
        u16PopCount = u16ElementCount;
    }

    // Calculate the start address for popping out
    pu8Buffer = (uint8_t*)pBuffer + u16BufferPopPtr ;

    // If the popping address is out of address range of the buffer then we need to pop twice
    if ((u16BufferPopPtr + u16PopCount) > u16BufferSize)
    {
        // Pop data stream from start address to the final address of the buffer
        memcpy(pvStream, pu8Buffer,u16BufferSize - u16BufferPopPtr);

        // Return the start address to the first address of the buffer for popping the rest of stream
        pu8Buffer = (uint8_t*)pBuffer;

        // Point to address of the rest of stream
        pu8RestStream = (uint8_t*)pvStream + u16BufferSize - u16BufferPopPtr;

        // Pop the rest of stream
        memcpy(pu8RestStream, pu8Buffer, u16PopCount + u16BufferPopPtr - u16BufferSize);
    }
    else
    {
        // Pop data stream
        memcpy(pvStream, pu8Buffer, u16PopCount);
    }

    // Point the pop pointer to the new position
    u16BufferPopPtr += u16PopCount;
    if (u16BufferPopPtr >= u16BufferSize)
    {
        u16BufferPopPtr -= u16BufferSize;
    }

    // Decrease element count of the buffer
    u16ElementCount -= u16PopCount;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return u16PopCount;	// Return the number of elements popped out actually
}

void QRingBuffer::BufferFlush()
{
    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    u16ElementCount 	= 0;
    u16BufferPopPtr	= 0;
    u16BufferPushPtr	= 0;
    bBufferPopEnable	= TRUE;
    bBufferPushEnable = TRUE;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();
}

BOOL QRingBuffer::BufferPushBack(uint16_t u16PushBackNumber)
{
    // Cannot push back if the push back number is greater than the available element count
    if (u16PushBackNumber > (u16BufferSize - u16ElementCount))
    {
        return FALSE;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Disable pushing to prohibiting changing of push pointer
    bBufferPushEnable = FALSE;

    // Push back element count
    u16ElementCount += u16PushBackNumber;

    // Push back pop pointer
    if (u16BufferPopPtr >= u16PushBackNumber)
    {
        u16BufferPopPtr -= u16PushBackNumber;
    }
    else
    {
        u16BufferPopPtr = u16BufferPopPtr + u16BufferSize - u16PushBackNumber;
    }

    // Enable pushing
    bBufferPushEnable = TRUE;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return TRUE;
}
>>>>>>> 15491401a593fcf951f6b05586a0e95e9b6d9d1a
=======
#include "QRingBuffer.h"
#include <QObject>
#include <QDebug>
QRingBuffer::QRingBuffer()
{
    pBuffer = nullptr;

}

QRingBuffer::~QRingBuffer()
{
    free(pBuffer);
}

void QRingBuffer::BufferInit(uint16_t u16Size, QMutex* mutex)
//                             CallbackFunctionNoIO callbackLockFPtr,
//                             CallbackFunctionNoIO callbackUnlockFPtr)
{
    if(!pBuffer)
        pBuffer = malloc(u16Size);

//    qDebug() << "Malloc pBuffer at " << QString.fromStdString((uint32_t)pBuffer);
//    callbackLock = callbackLockFPtr;
//    callbackUnlock = callbackUnlockFPtr;
    if (mutex != nullptr)
        pMutex = mutex;

    u16BufferSize = u16Size;
    u16ElementCount 	= 0;
    u16BufferPopPtr	= 0;
    u16BufferPushPtr	= 0;

    bBufferPopEnable	= TRUE;
    bBufferPushEnable	= TRUE;
}

BOOL QRingBuffer::BufferPush(void* pvData)
  {
      uint8_t*	pu8Buffer	= NULL;

      if ((u16ElementCount >= u16BufferSize) || (bBufferPushEnable == FALSE))
      {
          return FALSE;
      }

      // Lock accessing to the buffer
//      if (callbackLock)
//      {
//          callbackLock();
//      }
      pMutex->lock();

      // Calculate the start address for pushing in
      pu8Buffer = (uint8_t*)pBuffer + u16BufferPushPtr;

      // Push data element
      memcpy(pu8Buffer, pvData, 1);

      // Point the push pointer to the new position
      u16BufferPushPtr++;
      if (u16BufferPushPtr >= u16BufferSize)
      {
          u16BufferPushPtr = 0;
      }

      // Increase element count of the buffer
      u16ElementCount++;

      // Unlock accessing to the buffer
//      if (callbackUnlock)
//      {
//          callbackUnlock();
//      }
      pMutex->unlock();

      return TRUE;
  }

uint8_t QRingBuffer::BufferPop(void* pvData)
{
    uint8_t*	pu8Data	= NULL;

    if ((u16ElementCount == 0) || (bBufferPopEnable == FALSE))
    {
        return 0;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Calculate the start address for popping out
    pu8Data = (uint8_t*)pBuffer + u16BufferPopPtr;

    // Pop data element
    memcpy(pvData, pu8Data, 1);

    // Point the pop pointer to the new position
    u16BufferPopPtr++;
    if (u16BufferPopPtr >= u16BufferSize)
    {
        u16BufferPopPtr = 0;
    }

    // Decrease element count of the buffer
    u16ElementCount--;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return 1;
}

BOOL QRingBuffer::BufferPushStream(void* pvStream, uint16_t u16Length)
{
    uint8_t*	pu8Buffer		= NULL;
    uint8_t*	pu8RestStream	= NULL;

    if (((u16ElementCount + u16Length) > u16BufferSize) || (bBufferPushEnable == FALSE))
    {
        return FALSE;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Calculate the start address for pushing in
    pu8Buffer = (uint8_t*)pBuffer + u16BufferPushPtr;// * psRingBuffer->uiElementSize;

    // If the pushing address is out of address range of the buffer then we need to push twice
    if ((u16BufferPushPtr + u16Length) > u16BufferSize)
    {
        // Push data stream from start address to the final address of the buffer
        memcpy(pu8Buffer, pvStream, u16BufferSize - u16BufferPushPtr);

        // Return the start address to the first address of the buffer for pushing the rest of stream
        pu8Buffer = (uint8_t*)pBuffer;

        // Point to address of the rest of stream
        pu8RestStream = (uint8_t*)pvStream + u16BufferSize - u16BufferPushPtr;

        // Push the rest of stream
        memcpy(pu8Buffer, pu8RestStream, u16Length + u16BufferPushPtr - u16BufferSize);
    }
    else
    {
        // Push data stream
        memcpy(pu8Buffer, pvStream, u16Length);
    }

    // Point the push pointer to the new position
    u16BufferPushPtr += u16Length;
    if (u16BufferPushPtr >= u16BufferSize)
    {
        u16BufferPushPtr -= u16BufferSize;
    }

    // Increase element count of the buffer
    u16ElementCount += u16Length;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return TRUE;	// Push successfully
}

uint16_t QRingBuffer::BufferPopStream(void* pvStream, uint16_t u16Length)
{
    uint16_t	u16PopCount		= 0;
    uint8_t*	pu8Buffer		= NULL;
    uint8_t*    pu8RestStream	= NULL;

    if ((u16ElementCount == 0) || (bBufferPopEnable == FALSE))
    {
        return 0;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Limit length of data stream will be popped
    if (u16Length < u16ElementCount)
    {
        u16PopCount = u16Length;
    }
    else
    {
        u16PopCount = u16ElementCount;
    }

    // Calculate the start address for popping out
    pu8Buffer = (uint8_t*)pBuffer + u16BufferPopPtr ;

    // If the popping address is out of address range of the buffer then we need to pop twice
    if ((u16BufferPopPtr + u16PopCount) > u16BufferSize)
    {
        // Pop data stream from start address to the final address of the buffer
        memcpy(pvStream, pu8Buffer,u16BufferSize - u16BufferPopPtr);

        // Return the start address to the first address of the buffer for popping the rest of stream
        pu8Buffer = (uint8_t*)pBuffer;

        // Point to address of the rest of stream
        pu8RestStream = (uint8_t*)pvStream + u16BufferSize - u16BufferPopPtr;

        // Pop the rest of stream
        memcpy(pu8RestStream, pu8Buffer, u16PopCount + u16BufferPopPtr - u16BufferSize);
    }
    else
    {
        // Pop data stream
        memcpy(pvStream, pu8Buffer, u16PopCount);
    }

    // Point the pop pointer to the new position
    u16BufferPopPtr += u16PopCount;
    if (u16BufferPopPtr >= u16BufferSize)
    {
        u16BufferPopPtr -= u16BufferSize;
    }

    // Decrease element count of the buffer
    u16ElementCount -= u16PopCount;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return u16PopCount;	// Return the number of elements popped out actually
}

void QRingBuffer::BufferFlush()
{
    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    u16ElementCount 	= 0;
    u16BufferPopPtr	= 0;
    u16BufferPushPtr	= 0;
    bBufferPopEnable	= TRUE;
    bBufferPushEnable = TRUE;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();
}

BOOL QRingBuffer::BufferPushBack(uint16_t u16PushBackNumber)
{
    // Cannot push back if the push back number is greater than the available element count
    if (u16PushBackNumber > (u16BufferSize - u16ElementCount))
    {
        return FALSE;
    }

    // Lock accessing to the buffer
//    if (callbackLock)
//    {
//        callbackLock();
//    }
    pMutex->lock();

    // Disable pushing to prohibiting changing of push pointer
    bBufferPushEnable = FALSE;

    // Push back element count
    u16ElementCount += u16PushBackNumber;

    // Push back pop pointer
    if (u16BufferPopPtr >= u16PushBackNumber)
    {
        u16BufferPopPtr -= u16PushBackNumber;
    }
    else
    {
        u16BufferPopPtr = u16BufferPopPtr + u16BufferSize - u16PushBackNumber;
    }

    // Enable pushing
    bBufferPushEnable = TRUE;

    // Unlock accessing to the buffer
//    if (callbackUnlock)
//    {
//        callbackUnlock();
//    }
    pMutex->unlock();

    return TRUE;
}
>>>>>>> 15491401a593fcf951f6b05586a0e95e9b6d9d1a
