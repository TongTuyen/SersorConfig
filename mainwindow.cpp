#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "convert.h"
#include <CRC8.h>
#include <CRC16.h>
#include "modbusrtu.h"
#include <stdio.h>
#include <QTime>
#include <QTimer>
#include <QDate>
#include <QMessageBox>

#if 1
#define LREP_RAW(str, args...)          MainWindow::LogToConsoleTime(str, ##args)
#define LREP(str, args...)              MainWindow::LogToConsoleTime(str "\r\n", ##args)
#define LREP_ERROR(str, args...)        MainWindow::LogToConsoleTime("[ERR] %s:%d" str "\r\n", __FILE__, __LINE__, ##args)
#else
#define LREP_RAW(str, args...)          qDebug(str, ##args)
#define LREP(str, args...)              qDebug(str, ##args)
#define LREP_ERROR(str, args...)        qDebug("[ERR] %s:%d" str, __FILE__, __LINE__, ##args)
#endif
void MainWindow::LogToConsole(const char* string, ...)
{
    char szBuffer[2048];
    va_list arglist;
    va_start(arglist, string);
    memset(szBuffer, 0, 2048);
    vsprintf(szBuffer, string, arglist);
    va_end(arglist);
    if(this->ui->txtLog->toPlainText().length() > 2048)
    {
        this->ui->txtLog->setPlainText("Old data has been deleted!... \r\n");
    }
    this->ui->txtLog->setPlainText(this->ui->txtLog->toPlainText().append(szBuffer));

    QTextCursor cursor(this->ui->txtLog->textCursor());
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    this->ui->txtLog->setTextCursor(cursor);
}


void MainWindow::LogToConsoleTime(const char* string, ...)
{
    QTime now = QTime::currentTime();

    char szBuffer[2048];
    va_list arglist;
    va_start(arglist, string);
    memset(szBuffer, 0, 2048);
    vsprintf(szBuffer, string, arglist);
    va_end(arglist);

    char dateTime[128] = {0};
    sprintf(dateTime, "[%.2d:%.2d:%.2d] ",
            now.hour(),
            now.minute(),
            now.second());

    if(this->ui->txtLog->toPlainText().length() > 2048)
    {
        this->ui->txtLog->setPlainText("Old data has been deleted!... \r\n");
    }

    this->ui->txtLog->setPlainText(this->ui->txtLog->toPlainText() + dateTime + szBuffer);

    QTextCursor cursor(this->ui->txtLog->textCursor());
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    this->ui->txtLog->setTextCursor(cursor);

}

void MainWindow::ShowTime()
{
    QTime time = QTime::currentTime();
    time_text = time.toString("hh:mm:ss");
    if(time.second()%2==0)
    {
        time_text[2]=' ';
        time_text[5]=' ';
    }
    ui->lineEdit_WriteRTC->setText(time_text);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->cboComPort->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
            QString s = info.portName();
            ui->cboComPort->addItem(s);
    }

//    Setup filter mask
    this->ui->txtSensorAddress->setMaxLength(5);
    this->ui->txtSensorAddress->setInputMask("HH HH;_");
    this->ui->sldAirTemp->setValue(0);
    this->ui->sldAirHumid->setValue(0);
    this->ui->sldSoilTemp->setValue(0);
    this->ui->sldSoilMoisture->setValue(0);
    this->ui->sldLightIntensity->setValue(0);
    this->ui->sldWaterEC->setValue(0);
    this->ui->sldWaterPH->setValue(0);
    this->ui->sldWaterTemp->setValue(0);
    this->ui->sldWaterLevel->setValue(0);
    this->ui->sldSoidPH->setValue(0);
    this->ui->sldSoilEC->setValue(0);

    // Tab RSP Frame Generator
    this->ui->txtFlags->setMaxLength(2);
    this->ui->txtFlags->setInputMask("HH;_");

    this->ui->txtSeqNo->setMaxLength(2);
    this->ui->txtFlags->setInputMask("HH;_");

    this->ui->txtDstAddr->setMaxLength(5);
    this->ui->txtDstAddr->setInputMask("HH HH;_");

    this->ui->txtSrcAddr->setMaxLength(5);
    this->ui->txtSrcAddr->setInputMask("HH HH;_");

    // Time date
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(ShowTime()));
    timer->start();
    QDate date =QDate::currentDate();
    ui->lineEdit_WriteRTC_date->setText(date.toString());
    QString datetxt = date.toString(Qt::SystemLocaleShortDate);
    qDebug()<<datetxt;
    int wd = date.dayOfWeek();
    qDebug()<<wd;
    qDebug()<<datetxt.right(2);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sldAirTemp_valueChanged(int value)
{
    char tmp[128];
    sprintf(tmp, "%.2f *C", (float)value/100);
    this->ui->lblAirTemp->setText(tmp);

}

void MainWindow::on_sldAirHumid_valueChanged(int value)
{
    char tmp[128];
   sprintf(tmp, "%.2f %RH", (float)value/100);
    this->ui->lblAirHumid->setText(tmp);
}


void MainWindow::on_sldSoilTemp_valueChanged(int value)
{
    char tmp[128];
    sprintf(tmp, "%.2f *C", (float)value/100);
    this->ui->lblSoilTemp->setText(tmp);
}

void MainWindow::on_sldSoilMoisture_valueChanged(int value)
{
    char tmp[128];
    sprintf(tmp, "%.2f %RH", (float)value/100);
    this->ui->lblSoilMoisture->setText(tmp);
}

void MainWindow::on_sldLightIntensity_valueChanged(int value)
{
    char tmp[128];
    sprintf(tmp, "%d lux", value);
    this->ui->lblLightIntensity->setText(tmp);
}

void MainWindow::on_sldWaterEC_valueChanged(int value)
{
    char tmp[128];
    sprintf(tmp, "%.2f TDS", (float)(value)/100);
//    qDebug("%.2f", (float)value/100);
    this->ui->lblWaterEC->setText(tmp);
}

void MainWindow::on_sldWaterPH_valueChanged(int value)
{
    char tmp[128];
    sprintf(tmp, "%.2f", (float)value/100);
//    qDebug("%.2f", (float)value/100);
    this->ui->lblWaterPH->setText(tmp);
}

void MainWindow::on_sldWaterLevel_valueChanged(int value)
{
    char tmp[128];
    sprintf(tmp, "%d", value);
    this->ui->lblWaterLevel->setText(tmp);
}

void MainWindow::on_sldSoilEC_valueChanged(int value)
{
    char tmp[128];
    sprintf(tmp, "%.2f TDS", (float)value/100);
    this->ui->lblSoilEC->setText(tmp);
}

void MainWindow::on_sldSoidPH_valueChanged(int value)
{
    char tmp[128];
    sprintf(tmp, "%.2f", (float)value/100);
    this->ui->lblSoilPH->setText(tmp);
}

void MainWindow::on_sldWaterTemp_valueChanged(int value)
{
    char tmp[128];
    sprintf(tmp, "%.2f *C", (float)value/100);
    this->ui->lblWaterTemp->setText(tmp);
}


void MainWindow::on_btnRefreshPorts_clicked()
{
    this->ui->cboComPort->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
            QString s = info.portName();
            ui->cboComPort->addItem(s);
    }
}

void MainWindow::on_txtSensorAddress_textChanged()
{
    bool b;
    this->m_u16SensorID = this->ui->txtSensorAddress->text().remove(QChar(' ')).toUInt(&b, 16);
//    qDebug() << this->ui->txtSensorAddress->text() << "value: " << this->m_u16SensorID;
}

void MainWindow::on_btnStartSimulation_clicked()
{
    on_txtSensorAddress_textChanged();

    if(this->m_bRunning == false)
    {
        this->ui->btnRefreshPorts->setEnabled(false);
        this->ui->cboComPort->setEnabled(false);
        this->ui->txtSensorAddress->setEnabled(false);
        this->ui->btnStartSimulation->setText("STOP");
//        this->ui->gboxSensorData->setEnabled(true);
        this->m_bRunning = true;

        RunSimulation(true);

    }
    else
    {
        this->m_bRunning = false;
        this->ui->btnRefreshPorts->setEnabled(true);
        this->ui->cboComPort->setEnabled(true);
        this->ui->txtSensorAddress->setEnabled(true);
        this->ui->btnStartSimulation->setText("START");
//        this->ui->gboxSensorData->setEnabled(false);
        this->m_bRunning = false;
        RunSimulation(false);
    }
}


void MainWindow::RunSimulation(bool bIsterminated)
{
    if(bIsterminated == true)
    {
        this->pSerialPort = new QSerialPort(nullptr);
        pSerialPort->setBaudRate(QSerialPort::Baud9600);
        pSerialPort->setParity(QSerialPort::NoParity);
        pSerialPort->setDataBits(QSerialPort::Data8);
        pSerialPort->setFlowControl(QSerialPort::NoFlowControl);
        pSerialPort->setPortName(this->ui->cboComPort->currentText());

        connect(pSerialPort, SIGNAL(readyRead()), this, SLOT(on_SerialportDataReceived()));
//        qDebug() << "Openning COM port " << pSerialPort->portName() << " baud: " << pSerialPort->baudRate();
//        this->LogToConsole("Openning comport %s, baudrate: %d\r\n", pSerialPort->portName().toStdString().c_str(), pSerialPort->baudRate());
        LREP("Openning comport %s, baudrate: %d", pSerialPort->portName().toStdString().c_str(), pSerialPort->baudRate());

        if(!pSerialPort->open(QIODevice::ReadWrite))
        {
            qDebug() << "COM port open Error!";
//            this->LogToConsole( "COM port open Error!\r\n");
            LREP("%s port open error!", pSerialPort->portName().toStdString().c_str());
            return;
        }

        // Initialize Rx Ringbuffer
        pRxBuffMutex = new QMutex();
        pRxBuffer = new QRingBuffer();
        pRxBuffer->BufferInit(1024, pRxBuffMutex);


        pTimer = new QTimer(this);
        pTimer->setInterval(10);
        connect(pTimer, SIGNAL(timeout()), this, SLOT(onTimerTicked()));
        qDebug() << "Starting RxProcess Thread";
        this->LogToConsole("Starting Rx Thread ...\r\n");
        LREP("Starting Rx Thread ...");
        pTimer->start();


    }
    else
    {
        qDebug() << "Close COM port " << pSerialPort->portName();
        this->LogToConsole( "Close COM port %s\r\n" , pSerialPort->portName().toStdString().c_str());
        LREP( "Close COM port %s" , pSerialPort->portName().toStdString().c_str());
        pSerialPort->close();
        pTimer->stop();
        delete pSerialPort;
        delete pRxBuffer;
        delete pRxBuffMutex;
        delete pTimer;
    }

}

void MainWindow::on_SerialportDataReceived()
{

    QByteArray arr =  this->pSerialPort->readAll();

    qDebug() << "[DBG-L1]Rcv data: " << arr.toHex();

    pRxBuffer->BufferPushStream((void*)arr.data(), arr.size());
}

void MainWindow::onTimerTicked()
{

    if(pRxBuffer->BufferGetCount()==0)
    {
        return;
    }
    else
    {
        ProcessRawData();
    }


}

typedef enum _E_PROTOC_TYPE
{
    PROTO_TYPE_UNDEFINED = 0,
    PROTO_TYPE_SMBUS,
    PROTO_TYPE_RSP
} ProtocType_t;

void 	MainWindow::ProcessRawData()
{
//	RS485State_t		state =	RS485_STATE_IDLE;
    uint8_t 			ucTmp;
    static BOOL			bIsSearchingHeader	= TRUE;
    static uint8_t		pucRecvFrame[258] = {0};
    static uint8_t		u8RecvDataCount		= 0;
    static ProtocType_t	u8ProtocType		= PROTO_TYPE_UNDEFINED;	// Protocol type: 0-Undefined, 1-SMBUS, 2-RSP
    while(pRxBuffer->BufferGetCount() > 0)
    {

//		LREP("RxRingBUff count; %d",pRxBuffer->BufferGetCount() );
        // Layer 2 Starts processing received data from Layer 1 by searching Header
        if(bIsSearchingHeader)
        {
            // Start receiving only if received at least a header
            if(pRxBuffer->BufferGetCount() < MIN(SMBUS_FRM_HDR_SIZE, RSP_FRM_HDR_SIZE))
            {
                break;
            }

            // search for RSP_SOF
            pRxBuffer->BufferPop(&ucTmp);
            // Seek for a valid preamble

            // first byte seem to a SMBUS_SOF_PREAMBLE
            if(ucTmp == SMBUS_SOF_PREAMBLE)
            {
                pRxBuffer->BufferPop(&ucTmp);
                if(ucTmp == SMBUS_SOF_PREAMBLE)
                {
                    u8ProtocType = PROTO_TYPE_SMBUS;

                }
                else
                {
                    u8ProtocType = PROTO_TYPE_UNDEFINED;
                    continue;
                }

              }
            else if (ucTmp == RSP_SOF_PREAMBLE)
            {
                pRxBuffer->BufferPop(&ucTmp);
                if(ucTmp == RSP_SOF_PREAMBLE)
                {
                    u8ProtocType = PROTO_TYPE_RSP;
                    // Continue receive full RSP header, then set isSearchingHeader to true

                }
                else
                {
                    u8ProtocType = PROTO_TYPE_UNDEFINED;
                    continue;
                }

            }

            // Found 2 continous byte seems to be a RSP_SOF
            // Try to get a full header for checking a valid one

            if(u8ProtocType == PROTO_TYPE_SMBUS)
            {
                pRxBuffer->BufferPopStream( &pucRecvFrame[SMBUS_FRM_IDX_DLEN], SMBUS_FRM_HDR_SIZE-SMBUS_FRM_IDX_DLEN);

                if (IsValidSMBUSDLen(pucRecvFrame[SMBUS_FRM_IDX_DLEN]))
                {
                    bIsSearchingHeader = FALSE;
                    continue;
                }
            }

            /**
             * RSP Frame Structure
             *
             */
            //+-----+-----+------+------+-------+----------+----------+----------+----------+-------+------+-----+------+-------+
            //| SOF | SOF | DLEN | DLEN | FLAGS | DST_ADDR | DST_ADDR | SRC_ADDR | SRC_ADDR | CRC_H | DAT0 | ... | DATn | CRC_D |
            //+-----+-----+------+------+-------+----------+----------+----------+----------+-------+------+-----+------+-------+

            else if (u8ProtocType == PROTO_TYPE_RSP)
            {
                pRxBuffer->BufferPopStream( &pucRecvFrame[RSP_FRM_IDX_DLEN], RSP_FRM_HDR_SIZE - RSP_FRM_SOF_SIZE);
//#if (RS485_DEBUG_ENABLE == ON)
//                LREP_RAW("RSP header: ");
//                for(int i = RSP_FRM_IDX_DLEN; i < RSP_FRM_IDX_DLEN + RSP_FRM_HDR_SIZE - RSP_FRM_SOF_SIZE; i ++)
//                    LREP_RAW("%.2x ", pucRecvFrame[i]);
//#endif
                uint16_t u16Dlen = htons(*(uint16_t*)&pucRecvFrame[RSP_FRM_IDX_DLEN]);
//#if (RS485_DEBUG_ENABLE == ON)
//                LREP("\r\n[RSP] Data len %d", u16Dlen);
//#endif
                if (IS_DLEN_VALID(u16Dlen))
                {
                    bIsSearchingHeader = FALSE;
                    continue;
                }
            }
        }		// End of searching header

        else	// Start getting data field
        {
            if (u8ProtocType == PROTO_TYPE_SMBUS)
            {
                uint8_t u8PopLength = pRxBuffer->BufferGetCount();

                // Limit, if the receive buffer count is greater than data length of this frame then we just
                // get enough data byte for it
                if (u8PopLength > (int8_t)pucRecvFrame[SMBUS_FRM_IDX_DLEN] - SMBUS_FRM_HDR_SIZE + 2 - u8RecvDataCount)
                {
                    u8PopLength = (int8_t)pucRecvFrame[SMBUS_FRM_IDX_DLEN] - SMBUS_FRM_HDR_SIZE + 2 - u8RecvDataCount;
                }
    //			LREP("Pop out %d data bytes, u8RecvDataCount: %d, len_required: %d", u8PopLength, u8RecvDataCount, pucRecvFrame[SMBUS_FRM_IDX_DLEN] - SMBUS_FRM_HDR_SIZE + 2);

                if(pRxBuffer->BufferPopStream(&pucRecvFrame[SMBUS_FRM_IDX_DATA0+u8RecvDataCount], u8PopLength) != u8PopLength)
                {
//                    LREP("Unable to Pop stream");
                }

    //			// Count up the number of received data for the current frame
                u8RecvDataCount += u8PopLength;

    //			// If we didn't get enough data bytes, continue to check, do not switch to "Search header" state & return TRUE
                if (u8RecvDataCount < (uint8_t)pucRecvFrame[SMBUS_FRM_IDX_DLEN] - SMBUS_FRM_HDR_SIZE + 2)
                {
                    continue;
                }
//    #if (RS485_DEBUG_ENABLE == ON)
//                LREP_RAW("Get Full Data: ");
//                for (int i = 0; i < u8RecvDataCount; i ++)
//                    LREP_RAW("%.2x ", pucRecvFrame[SMBUS_FRM_IDX_DATA0 + i]);
//    #endif
                // Got a complete frame then switch the machine to "Search Header" state
                bIsSearchingHeader	= TRUE;
                u8RecvDataCount		= 0;
                u8ProtocType		= PROTO_TYPE_UNDEFINED;

                // Decode SMBUS Frame
                TransSmartBus::SMBusFrame_t frame = TransSmartBus::clsTransSmartBus::DecodeFrame(&pucRecvFrame[2]);

                 // TODO: [manhbt] process received SMBUS Frame
//                clsSMBusProcessor::Instance()->Process((void*)&frame);
                  qDebug("Got 01 SBUS Frame with dlen: %d!", frame.dlen);
                  for(int i = 0; i < frame.dlen; i ++)
                  {
                      qDebug("%.2x ", pucRecvFrame[2 + i]);
                  }
                memset(pucRecvFrame, 0, sizeof(pucRecvFrame));
                break;
            }

            else if (u8ProtocType == PROTO_TYPE_RSP)
            {
                uint8_t u8PopLength = pRxBuffer->BufferGetCount();

                uint16_t u16Dlen = htons(*(uint16_t*)&pucRecvFrame[RSP_FRM_IDX_DLEN]);
                // Limit, if the receive buffer count is greater than data length of this frame then we just
                // get enough data byte for it

                if (u8PopLength > u16Dlen + RSP_FRM_CRCD_SIZE - u8RecvDataCount)
                {
                    u8PopLength = u16Dlen + RSP_FRM_CRCD_SIZE - u8RecvDataCount;
                }

                // Pop out the rest of frame (including CRC16)

    //			LREP("Pop out %d data bytes, u8RecvDataCount: %d, len_required: %d", u8PopLength, u8RecvDataCount, pucRecvFrame[SMBUS_FRM_IDX_DLEN] - SMBUS_FRM_HDR_SIZE + 2);

                if(pRxBuffer->BufferPopStream(&pucRecvFrame[RSP_FRM_IDX_DATA0+u8RecvDataCount], u8PopLength) != u8PopLength)
                {
//                    LREP("Unable to Pop stream");
                }


    //			// Count up the number of received data for the current frame
                u8RecvDataCount += u8PopLength;

    //			// If we didn't get enough data bytes, continue to check, do not switch to "Search header" state & return TRUE
                if (u8RecvDataCount < u16Dlen + RSP_FRM_CRCD_SIZE)
                {
                    continue;
                }
//#if (RS485_DEBUG_ENABLE == ON)
//                LREP("Get Full Data: ");
//                for (int i = 0; i < u8RecvDataCount; i ++)
//                    LREP_RAW("%.2x ", pucRecvFrame[RSP_FRM_IDX_DATA0 + i]);
//#endif
                // Got a complete frame then switch the machine to "Search Header" state
                bIsSearchingHeader	= TRUE;
                u8RecvDataCount		= 0;
                u8ProtocType		= PROTO_TYPE_UNDEFINED;

                // Decode RSP Frame
                TransRSP::RSPFrame_t frame = TransRSP::clsTransRSP::DecodeFrame(&pucRecvFrame[RSP_FRM_IDX_DLEN]);
                uint16_t u16FrameLen = RSP_FRM_HDR_SIZE + u16Dlen + RSP_FRM_CRCD_SIZE +RSP_FRM_IDX_SOF ;
                uint8_t* pvFrame = new uint8_t[u16FrameLen];
                memcpy(pvFrame, &pucRecvFrame[RSP_FRM_IDX_DLEN], u16FrameLen);

               // LREP("[%s:%d] Raw Frame", __FILE__, __LINE__);
//                qDebug("%d ", u16FrameLen);
//                for(int i = 0; i < u16FrameLen; i ++)
//                {
//                    qDebug("%.2x ", pvFrame[i]);
//                }

//				clsRSPProcessor::Instance()->Process((void*)&pucRecvFrame[RSP_FRM_IDX_DLEN]);
                // TODO: [manhbt] process received RSP Frame
//                AHGSystems::clsRSPProcessor::Instance()->Process((void*)pvFrame);

                //config sensor box by TUNKIO
                //ProcessRSPFrame((void*)pvFrame);
                ProcessRSPFrameReaddata((void*)pvFrame);
                ProcessRSPFrameChangeAddress((void*)pvFrame);
                ProcessRSPFrameGetSlaveAddress((void*)pvFrame);
                ProcessRSPFrameReadRTCTime((void*)pvFrame);
                ProcessRSPFrameWriteRTCTime((void*)pvFrame);
                ProcessRSPFrameReadRTCDate((void*)pvFrame);
                ProcessRSPFrameWriteRTCDate((void*)pvFrame);
                ProcessRSPFrameGetSensorOnPort((void*)pvFrame);
                qDebug() << "Got 01 RSP Frame!";
                memset(pucRecvFrame, 0, sizeof(pucRecvFrame));
                break;
            }

        }	// End of getting data
//
    }	// End of while loop
//	return FALSE;
}

void MainWindow::ProcessRSPFrame(void *pvFrame)
{
//    TransRSP::RSPFrame_t *pFrame = (TransRSP::RSPFrame_t*)pvFrame;

    uint8_t* pu8Frame = (uint8_t*)pvFrame;
//   uint16_t value= htons(*(uint16_t*)&pu8Frame[12]);
//    qDebug("\nvalue =%d ", value,"\n");
//    on_sldAirTemp_valueChanged(value);
//    uint16_t value2= htons(*(uint16_t*)&pu8Frame[15]);
//     qDebug("\nvalue2 =%d ", value2,"\n");

//    for(int i=11;i<47;i++)
//    {
//        qDebug("%.2x ", pu8Frame[i]);
//    }

    if(pu8Frame == nullptr)
    {
//		LREP("Invalid Frame");
        return;
    }
//
//    qDebug() <<"Processing RSP Frame!\r\n";

    uint8_t u8CRCH = CalCRC8(&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE ], 8 );
    if(u8CRCH != pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE])
    {
//       qDebug() << "Invalid CRC_H";
        LREP("Invalid CRC_H");
//       printf("fuck \r\n");
//		LREP("[clsRSPProcessor::Process] Invalid CRC_H, cal: %.2x, get: %.2x",
//				u8CRCH,
//				pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE]
////				pFrame->crch
//		);
        return;
    }


    uint16_t u16Dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN- RSP_FRM_SOF_SIZE]);
//	LREP("[clsRSPProcessor::Process] dlen: %d",	u16Dlen);
    uint16_t u16CRCD = CalculateCRC16(&pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE], u16Dlen + 1);
    if (u16CRCD != htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + u16Dlen]))
    {
//		LREP("[clsRSPProcessor::Process] Invalid CRC_D, cal: %.4x, get: %.4x",
//				u16CRCD,
//				htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + u16Dlen])
//				);
//        qDebug() << "Invalid CRC_D";
//        LogToConsole("Invalid CRC_D\r\n");
        LREP("Invalid CRC_D");
        delete pu8Frame;
        return;
    }

//    qDebug() << "RSP Frame OK! dlen: " << u16Dlen << ", dst: " << htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DSTADDR- RSP_FRM_SOF_SIZE]);

    /**
     * RSP Frame structure
     * 0_____2______4______5_____6_____8_____10______11__________________________
     * +-----+------+------+-----+-----+-----+-------+-------------+-------+-----+
     * | SOF | DLEN | FLAG | SEQ | DST | SRC | CRC_H | DATA[0...n] | CRC_D | EOF |
     * | (2) |  (2) |  (1) | (1) | (2) | (2) |  (1)  |   (DLEN)    |  (2)  | (1) |
     * +-----+------+------+-----+-----+-----+-------+-------------+-------+-----+
     */
//    uint8_t data[] = { 0x55, 0x55, 	// sof  - idx = 0
//            0x00, 0x02, 	// dlen	- idx = 2
//            0x00, 			// flag - idx = 4
//            0x00, 			// seq -  idx = 5
//            0x00, 0x01,		// dst - idx = 6
//            0x00, 0xff,		// src - idx = 8
//            0x00,			// crc_h - idx = 10
//            0x01, 0x02,		// Data[0] - idx = 11
//            0x00, 0x00,		// crc
//            0xff			// eof
//            };
    if((u16Dlen == 2)
            && ( htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DSTADDR- RSP_FRM_SOF_SIZE]) == m_u16SensorID)
            && (pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE] == 0x01)
            && (pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + 1] == 0x02)
       )
    {
//        qDebug() << "Got a Sensor read Request from host";
//        LogToConsole("Got a Sensor read Request from host\r\n");
        LREP("Got a Sensor read Request from host");
        // TODO: [manhbt] Fake sensor data and reply to host
        uint8_t pu8SensorFrame[256] = {
            0x55, 0x55,         // SOF
            0x00, 0x0E,         // len
            0x00,               // flags
            0x00,               // seq
            0xFF, 0xFF,         // dst addr (Broadcast)
            (m_u16SensorID >> 8)&0xFF, m_u16SensorID & 0xFF,         // src addr, replace with m_u16RSPID
            0x77,               // CRC_H
            // Data
            0x01, 0x02,         // sub-command
            0x02, 0x00, 0x00,   // field 1
            0x01, 0x00, 0x00,   // field 2
            0x04, 0x00, 0x00,   // filed 3
            0x03, 0x00, 0x00,   // filed 4
            0xC2, 0x9F,         // CRC_D
            0xFF                // EOF
        };
        uint8_t u8FrameLen = 2;
        uint8_t *pTmp = &pu8SensorFrame[RSP_FRM_IDX_DATA0 + 2];



        if(this->ui->chkAirTemp->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_AIR_TEMPERATURE; pTmp++; u8FrameLen++;
            *pTmp = (this->ui->sldAirTemp->value() >> 8)&0xFF; pTmp++; u8FrameLen ++;
            *pTmp = (this->ui->sldAirTemp->value())&0xFF; pTmp++; u8FrameLen++;

        }

        if(this->ui->chkAirHumid->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_AIR_HUMIDITY; pTmp++; u8FrameLen++;
            *pTmp = (this->ui->sldAirHumid->value() >> 8)&0xFF; pTmp++; u8FrameLen ++;// chuyen thanh bytes cao bytes cao
            *pTmp = (this->ui->sldAirHumid->value())&0xFF; pTmp++; u8FrameLen++;
        }

        if(this->ui->chkSoilTemp->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_SOIL_TEMPERATURE; pTmp++; u8FrameLen++;
            *pTmp = (this->ui->sldSoilTemp->value() >> 8)&0xFF; pTmp++; u8FrameLen ++;
            *pTmp = (this->ui->sldSoilTemp->value())&0xFF; pTmp++; u8FrameLen++;
        }

        if(this->ui->chkSoilMoisture->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_SOIL_MOISTURE; pTmp++; u8FrameLen++;
            *pTmp = (this->ui->sldSoilMoisture->value() >> 8)&0xFF; pTmp++; u8FrameLen ++;
            *pTmp = (this->ui->sldSoilMoisture->value())&0xFF; pTmp++; u8FrameLen++;
        }



//        SENSOR_LIGHT_AMBIENT
        if(this->ui->chkLightIntensity->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_LIGHT_AMBIENT; pTmp++; u8FrameLen++;
            *pTmp = (this->ui->sldLightIntensity->value() >> 8)&0xFF; pTmp++; u8FrameLen ++;
            *pTmp = (this->ui->sldLightIntensity->value())&0xFF; pTmp++; u8FrameLen++;
        }
//        SENSOR_WATER_EC
        if(this->ui->chkWaterEC->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_WATER_EC; pTmp++; u8FrameLen++;
            *pTmp = (this->ui->sldWaterEC->value() >> 8)&0xFF; pTmp++; u8FrameLen ++;
            *pTmp = (this->ui->sldWaterEC->value())&0xFF; pTmp++; u8FrameLen++;
        }
//        SENSOR_WATER_PH
        if(this->ui->chkWaterPH->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_WATER_PH; pTmp++; u8FrameLen++;
            *pTmp = (this->ui->sldWaterPH->value() >> 8)&0xFF; pTmp++; u8FrameLen ++;
            *pTmp = (this->ui->sldWaterPH->value())&0xFF; pTmp++; u8FrameLen++;
        }

//        SENSOR_WATER_TEMPERATURE
        if(this->ui->chkWaterTemp->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_WATER_TEMPERATURE; pTmp++; u8FrameLen++;
            *pTmp = (this->ui->sldWaterTemp->value() >> 8)&0xFF; pTmp++; u8FrameLen ++;
            *pTmp = (this->ui->sldWaterTemp->value())&0xFF; pTmp++; u8FrameLen++;
        }
//        SENSOR_WATER_LEVEL
        if(this->ui->chkWaterLevel->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_WATER_LEVEL; pTmp++; u8FrameLen++;
            *pTmp = (this->ui->sldWaterLevel->value() >> 8)&0xFF; pTmp++; u8FrameLen ++;
            *pTmp = (this->ui->sldWaterLevel->value())&0xFF; pTmp++; u8FrameLen++;
        }
//        SENSOR_SOIL_EC
        if(this->ui->chkSoilEC->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_SOIL_EC; pTmp++; u8FrameLen++;
            *pTmp = (this->ui->sldSoilEC->value() >> 8)&0xFF; pTmp++; u8FrameLen ++;
            *pTmp = (this->ui->sldSoilEC->value())&0xFF; pTmp++; u8FrameLen++;
        }
//        SENSOR_SOIL_PH
        if(this->ui->chkSoilPH->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_SOIL_PH; pTmp++; u8FrameLen++;
            *pTmp = (this->ui->sldSoidPH->value() >> 8)&0xFF; pTmp++; u8FrameLen ++;
            *pTmp = (this->ui->sldSoidPH->value())&0xFF; pTmp++; u8FrameLen++;
        }

//        SENSOR_WATER_STATUS
        if(this->ui->chkWaterStatus->checkState() == Qt::Checked)
        {
            *pTmp = SENSOR_WATER_STATUS; pTmp++; u8FrameLen++;
            *pTmp = 0; pTmp++; u8FrameLen ++;
            *pTmp = this->ui->rbWaterStatusEmpty->isChecked() ? 0 : 1; pTmp++; u8FrameLen++;
        }
        // Insert data length
        pu8SensorFrame[RSP_FRM_IDX_DLEN] = (u8FrameLen >> 8)&0xFF;
        pu8SensorFrame[RSP_FRM_IDX_DLEN+1] = u8FrameLen&0xFF;

        // Calculate CRC_H
        pu8SensorFrame[RSP_FRM_IDX_CRCH] = CalCRC8(&pu8SensorFrame[RSP_FRM_IDX_DLEN], RSP_FRM_HDR_SIZE - 3);

        // Calculate CRC_D
        u16CRCD = CalculateCRC16(&pu8SensorFrame[RSP_FRM_IDX_CRCH], u8FrameLen + 1);
        pu8SensorFrame[RSP_FRM_IDX_DATA0 + u8FrameLen] = (u16CRCD >> 8)&0xFF;
        pu8SensorFrame[RSP_FRM_IDX_DATA0 + u8FrameLen + 1] = u16CRCD & 0xFF;

        pu8SensorFrame[RSP_FRM_IDX_DATA0 + u8FrameLen + 2] = 0xFF;   // End-of-frame

        uint16_t u16Bts = RSP_FRM_HDR_SIZE + u8FrameLen + 3;
//        qDebug("Dlen: %d, Frame_len: %d\r\n", u8FrameLen, u16Bts);
//        LogToConsole("Dlen: %d, Frame_len: %d\r\n", u8FrameLen, u16Bts);
        LREP("Dlen: %d, Frame_len: %d", u8FrameLen, u16Bts);
//        for (int idx = 0; idx < u16Bts; idx++)
//        {
//            qDebug("%.2x", pu8SensorFrame[idx]);
//        }
        if( pSerialPort->write((const char*)pu8SensorFrame, u16Bts) != u16Bts)
        {
//            qDebug("Send data failed!");
//            LogToConsole("Send data failed!\r\n");
            LREP("Send data failed!");
        }

//        free(pu8SensorFrame);

    }
    // Parse frame to the Corresponding Device
//	for (auto itr : m_lstRSPDevice)
//	{
//		itr->UpdateValues(pvFrame);
//	}
//    free(pu8Frame);

}
// READ DATA Done
void MainWindow::ProcessRSPFrameReaddata(void *pvFrame)
{
    uint8_t* pu8Frame = (uint8_t*)pvFrame;

    if(pu8Frame == nullptr)
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Read values flase"));
        return;

    }
    uint8_t u8CRCH = CalCRC8(&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE ], 8 );
    if(u8CRCH != pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE])
    {
         QMessageBox::warning(this,tr("Dialog"),tr("Read values flase"));
        return;
    }


    uint16_t u16Dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN- RSP_FRM_SOF_SIZE]);
    uint16_t u16CRCD = CalculateCRC16(&pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE], u16Dlen + 1);
    if (u16CRCD != htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + u16Dlen]))
    {
         QMessageBox::warning(this,tr("Dialog"),tr("Read values flase"));
        LREP("Invalid CRC_D");
        delete pu8Frame;
        return;
    }
    qDebug("\nDATALEN %.4x", u16Dlen);
    int valdata=u16Dlen;
    qDebug("\nDATALEN %d", valdata);
    if(pu8Frame[RSP_FRM_IDX_DATA0-2]==0x01&&pu8Frame[RSP_FRM_IDX_DATA0-1]==0x02)
    {
    for(int i=RSP_FRM_IDX_DATA0;i<RSP_FRM_IDX_DATA0+valdata-2;i=i+3)
    {
       // qDebug("\nIndex %d",i);
       // qDebug("\nDATA %.2x", pu8Frame[i]);
        if(pu8Frame[i]==0x01)
        {
            uint16_t value1= htons(*(uint16_t*)&pu8Frame[i+1]);
            this->ui->sldAirTemp->setValue(value1);
            on_sldAirTemp_valueChanged(value1);

           // qDebug("\n0x01");
        }
        if(pu8Frame[i]==0x02)
        {
            uint16_t value1= htons(*(uint16_t*)&pu8Frame[i+1]);
            this->ui->sldAirHumid->setValue(value1);
            on_sldAirHumid_valueChanged(value1);
           // qDebug("\n0x02");
        }
        if(pu8Frame[i]==0x03)
        {
           uint16_t value= htons(*(uint16_t*)&pu8Frame[i+1]);
           this->ui->sldSoilTemp->setValue(value);
           on_sldSoilTemp_valueChanged(value);
        }
        if(pu8Frame[i]==0x04)
        {
           uint16_t value4= htons(*(uint16_t*)&pu8Frame[i+1]);
           this->ui->sldSoilMoisture->setValue(value4);
           on_sldSoilMoisture_valueChanged(value4);

        }
        if(pu8Frame[i]==0x05)
        {
           uint16_t value5= htons(*(uint16_t*)&pu8Frame[i+1]);
           this->ui->sldLightIntensity->setValue(value5);
           on_sldLightIntensity_valueChanged(value5);
        }
        if(pu8Frame[i]==0x06)
        {
           uint16_t value6= htons(*(uint16_t*)&pu8Frame[i+1]);
           this->ui->sldWaterEC->setValue(value6);
           on_sldWaterEC_valueChanged(value6);
        }
        if(pu8Frame[i]==0x07)
        {
           uint16_t value7= htons(*(uint16_t*)&pu8Frame[i+1]);
           this->ui->sldWaterPH->setValue(value7);
           on_sldWaterPH_valueChanged(value7);
        }
        if(pu8Frame[i]==0x08)
        {
           uint16_t value8= htons(*(uint16_t*)&pu8Frame[i+1]);
           this->ui->sldWaterTemp->setValue(value8);
           on_sldWaterTemp_valueChanged(value8);
        }
        if(pu8Frame[i]==0x09)
        {
           uint16_t value9= htons(*(uint16_t*)&pu8Frame[i+1]);
           this->ui->sldWaterLevel->setValue(value9);
           on_sldWaterLevel_valueChanged(value9);
        }
        if(pu8Frame[i]==0x0A)
        {
           uint16_t value10= htons(*(uint16_t*)&pu8Frame[i+1]);
           this->ui->sldSoilEC->setValue(value10);
           on_sldSoilEC_valueChanged(value10);
        }
        if(pu8Frame[i]==0x0B)
        {
           uint16_t value11= htons(*(uint16_t*)&pu8Frame[i+1]);
           this->ui->sldSoidPH->setValue(value11);
           on_sldSoidPH_valueChanged(value11);
        }
        if(pu8Frame[i]==0x0C)
        {
           uint16_t value12= htons(*(uint16_t*)&pu8Frame[i+1]);
              if(value12 ==0)
              {
                  this->ui->rbWaterStatusEmpty->setChecked(true);
              }
              else if(value12==1)
              {
                  this->ui->rbWaterStatusFull->setChecked(true);
              }
        }

    }
    }
  // QMessageBox::information(this,tr("Dialog"),tr("Read values done"));
}
// READ TIME done
void MainWindow::ProcessRSPFrameReadRTCTime(void *pvFrame)
{
    qDebug("Read RTC TIME ");
    uint8_t* pu8Frame = (uint8_t*)pvFrame;

    if(pu8Frame == nullptr)
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Read RTC Time False"));
        return;
    }
    uint8_t u8CRCH = CalCRC8(&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE ], 8 );
    if(u8CRCH != pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE])
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Read RTC Time False"));
        return;
    }


    uint16_t u16Dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN- RSP_FRM_SOF_SIZE]);
    uint16_t u16CRCD = CalculateCRC16(&pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE], u16Dlen + 1);
    if (u16CRCD != htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + u16Dlen]))
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Read RTC Time False"));
        LREP("Invalid CRC_D");
        delete pu8Frame;
        return;
    }
    // su ly frame
    if(pu8Frame[9]==0x01&&pu8Frame[10]==0x05)
    {
        QString strH = QString::number(pu8Frame[11]);
        QString strP = QString::number(pu8Frame[12]);
        QString strG = QString::number(pu8Frame[13]);
        strH.append(":");
        strH.append(strP);
        strH.append(":");
        strH.append(strG);
        ui->lineEdit_ReadRTCtime->setText(strH);

        QMessageBox::information(this,tr("Dialog"),tr("Read RTC Time Done"));
    }

}
//done
void MainWindow::ProcessRSPFrameWriteRTCTime(void *pvFrame)
{
    qDebug("Write RTC TIME ");
    uint8_t* pu8Frame = (uint8_t*)pvFrame;

    if(pu8Frame == nullptr)
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Write RTC Time False"));
        return;
    }
    uint8_t u8CRCH = CalCRC8(&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE ], 8 );
    if(u8CRCH != pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE])
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Write RTC Time False"));
        return;
    }


    uint16_t u16Dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN- RSP_FRM_SOF_SIZE]);
    uint16_t u16CRCD = CalculateCRC16(&pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE], u16Dlen + 1);
    if (u16CRCD != htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + u16Dlen]))
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Write RTC Time False"));
        LREP("Invalid CRC_D");
        delete pu8Frame;
        return;
    }
    // su ly frame
    if(pu8Frame[9]==0x01&&pu8Frame[10]==0x06)
    {
        QMessageBox::information(this,tr("Dialog"),tr("Write RTC Time Done"));
    }

}
//done
void MainWindow::ProcessRSPFrameReadRTCDate(void *pvFrame)
{
    qDebug("Read RTC Date ");
    uint8_t* pu8Frame = (uint8_t*)pvFrame;

    if(pu8Frame == nullptr)
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Read RTC Date False"));
        return;
    }
    uint8_t u8CRCH = CalCRC8(&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE ], 8 );
    if(u8CRCH != pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE])
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Read RTC Date False"));
        return;
    }


    uint16_t u16Dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN- RSP_FRM_SOF_SIZE]);
    uint16_t u16CRCD = CalculateCRC16(&pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE], u16Dlen + 1);
    if (u16CRCD != htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + u16Dlen]))
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Read RTC Date False"));
        LREP("Invalid CRC_D");
        delete pu8Frame;
        return;
    }
    if(pu8Frame[9]==0x01&&pu8Frame[10]==0x07)
    {
        QString strW;
        if(pu8Frame[11]==1)
        {
            strW="Mon";
        }
        if(pu8Frame[11]==2)
        {
            strW="Tu";
        }
        if(pu8Frame[11]==3)
        {
            strW="We";
        }
        if(pu8Frame[11]==4)
        {
            strW="Th";
        }
        if(pu8Frame[11]==5)
        {
            strW="Fr";
        }
        if(pu8Frame[11]==6)
        {
            strW="Sa";
        }
        if(pu8Frame[11]==7)
        {
            strW="Su";
        }

        //QString strW = QString::number(pu8Frame[11]);
        QString strD = QString::number(pu8Frame[12]);
        QString strM = QString::number(pu8Frame[13]);
        QString strY = QString::number(pu8Frame[14]);
        strW.append("/");
        strW.append(strD);
        strW.append("/");
        strW.append(strM);
        strW.append("/20");
        strW.append(strY);
        ui->lineEdit_readRTCdate->setText(strW);
        QMessageBox::information(this,tr("Dialog"),tr("Read RTC Date True"));
    }

}
//done
void MainWindow::ProcessRSPFrameWriteRTCDate(void *pvFrame)
{
    qDebug("Write RTC Date ");
    uint8_t* pu8Frame = (uint8_t*)pvFrame;

    if(pu8Frame == nullptr)
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Write RTC Date False"));
        return;
    }
    uint8_t u8CRCH = CalCRC8(&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE ], 8 );
    if(u8CRCH != pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE])
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Write RTC Date False"));
        return;
    }


    uint16_t u16Dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN- RSP_FRM_SOF_SIZE]);
    uint16_t u16CRCD = CalculateCRC16(&pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE], u16Dlen + 1);
    if (u16CRCD != htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + u16Dlen]))
    {
        QMessageBox::warning(this,tr("Dialog"),tr("Write RTC Date False"));
        LREP("Invalid CRC_D");
        delete pu8Frame;
        return;
    }
    if(pu8Frame[9]==0x01&&pu8Frame[10]==0x08)
    {
        //qDebug()<<pu8Frame[9]<<pu8Frame[10];
        QMessageBox::information(this,tr("Dialog"),tr("Write address done"));
    }
}
//done
void MainWindow::ProcessRSPFrameChangeAddress(void *pvFrame)
{
    //qDebug("Change Address ");
    uint8_t* pu8Frame = (uint8_t*)pvFrame;

    if(pu8Frame == nullptr)
    {
        return;
    }
    uint8_t u8CRCH = CalCRC8(&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE ], 8 );
    if(u8CRCH != pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE])
    {
        return;
    }


    uint16_t u16Dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN- RSP_FRM_SOF_SIZE]);
    uint16_t u16CRCD = CalculateCRC16(&pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE], u16Dlen + 1);
    if (u16CRCD != htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + u16Dlen]))
    {
        LREP("Invalid CRC_D");
        delete pu8Frame;
        return;
    }
    if(pu8Frame[9]==0x01&&pu8Frame[10]==0x25)
    {
        //ui->lineEdit_change_slaveAdd->setText("DONE");
        QMessageBox::information(this,tr("Dialog"),tr("Change address done"));
    }


}
//done
void MainWindow::ProcessRSPFrameGetSlaveAddress(void *pvFrame)
{
    uint8_t* pu8Frame = (uint8_t*)pvFrame;

    if(pu8Frame == nullptr)
    {
        return;
    }
    uint8_t u8CRCH = CalCRC8(&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE ], 8 );
    if(u8CRCH != pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE])
    {
        return;
    }


    uint16_t u16Dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN- RSP_FRM_SOF_SIZE]);
    uint16_t u16CRCD = CalculateCRC16(&pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE], u16Dlen + 1);
    if (u16CRCD != htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + u16Dlen]))
    {
        LREP("Invalid CRC_D");
        delete pu8Frame;
        return;
    }
    if(pu8Frame[9]==0x01&&pu8Frame[10]==0x01)
    {

        QString str1= QString::number(pu8Frame[11]);
        QString str2= QString::number(pu8Frame[12]);
            str1.append(str2);
        ui->lineEdit_getslaveadd->setText(str1);
        //qDebug()<<str1;
        QMessageBox::information(this,tr("Dialog"),tr("Get address done"));
    }
}
//done
void MainWindow::ProcessRSPFrameGetSensorOnPort(void *pvFrame)
{
    uint8_t* pu8Frame = (uint8_t*)pvFrame;

    if(pu8Frame == nullptr)
    {
        return;
    }
    uint8_t u8CRCH = CalCRC8(&pu8Frame[RSP_FRM_IDX_DLEN - RSP_FRM_SOF_SIZE ], 8 );
    if(u8CRCH != pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE])
    {
        return;
    }


    uint16_t u16Dlen = htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DLEN- RSP_FRM_SOF_SIZE]);
    uint16_t u16CRCD = CalculateCRC16(&pu8Frame[RSP_FRM_IDX_CRCH - RSP_FRM_SOF_SIZE], u16Dlen + 1);
    if (u16CRCD != htons(*(uint16_t*)&pu8Frame[RSP_FRM_IDX_DATA0 - RSP_FRM_SOF_SIZE + u16Dlen]))
    {
        LREP("Invalid CRC_D");
        delete pu8Frame;
        return;
    }
    if(pu8Frame[9]==0x01&&pu8Frame[10]==0x09)
    {
        //Port 1
        QString str1= QString::number(pu8Frame[11]);
        QString StrPort1 = "Port: ";
                StrPort1.append(str1);
         //channel 1
        QString StrChannel1 = " Channel 1: ";
        QString Sensorname1;
        if(pu8Frame[12]==0xff)
        {
            Sensorname1 ="No Sensor";
        }
        if(pu8Frame[12]==1)
        {
            Sensorname1 = "AM2315";
        }
        if(pu8Frame[12]==2)
        {
            Sensorname1 = "BH1750";
        }
        StrChannel1.append(Sensorname1);
        StrPort1.append(StrChannel1);
        //channel 2
        QString StrChannel2 = " Channel 2: ";
        QString Sensorname2;
        if(pu8Frame[13]==0xff)
        {
            Sensorname2 ="No Sensor";
        }
        if(pu8Frame[13]==1)
        {
            Sensorname2 ="AM2315";
        }
        if(pu8Frame[13]==2)
        {
            Sensorname2 ="BH1750";
        }
        StrChannel2.append(Sensorname2);
        StrPort1.append(StrChannel2);
        ui->lineEdit_GetOnPort->setText(StrPort1);
        //qDebug()<<StrPort1;
        //Port 2 14,15,16
        QString str2= QString::number(pu8Frame[14]);
        QString StrPort2 = "Port: ";
                StrPort2.append(str2);
                //channel 1
               QString StrChannel1Port2 = " Channel 1: ";
               QString Sensorname1Port2;
               if(pu8Frame[15]==0xff)
               {
                   Sensorname1Port2 ="No Sensor";
               }
               if(pu8Frame[15]==1)
               {
                   Sensorname1Port2 = "AM2315";
               }
               if(pu8Frame[15]==2)
               {
                   Sensorname1Port2 = "BH1750";
               }
               StrChannel1Port2.append(Sensorname1Port2);
               StrPort2.append(StrChannel1Port2);
               //channel 2
              QString StrChannel2Port2 = " Channel 2: ";
              QString Sensorname2Port2;
              if(pu8Frame[15]==0xff)
              {
                  Sensorname2Port2 ="No Sensor";
              }
              if(pu8Frame[15]==1)
              {
                  Sensorname2Port2 = "AM2315";
              }
              if(pu8Frame[15]==2)
              {
                  Sensorname2Port2 = "BH1750";
              }
              StrChannel2Port2.append(Sensorname2Port2);
              StrPort2.append(StrChannel2Port2);
         ui->lineEdit_GetOnPort2->setText(StrPort2);
        //Port 3 17,18,19
         QString str3= QString::number(pu8Frame[17]);
         QString StrPort3 = "Port: ";
                 StrPort3.append(str3);
                 //channel 1
                QString StrChannel1Port3 = " Channel 1: ";
                QString Sensorname1Port3;
                if(pu8Frame[18]==0xff)
                {
                    Sensorname1Port3 ="No Sensor";
                }
                if(pu8Frame[18]==1)
                {
                    Sensorname1Port3 = "AM2315";
                }
                if(pu8Frame[18]==2)
                {
                    Sensorname1Port3 = "BH1750";
                }
                StrChannel1Port3.append(Sensorname1Port3);
                StrPort3.append(StrChannel1Port3);
                //channel 2
               QString StrChannel2Port3 = " Channel 2: ";
               QString Sensorname2Port3;
               if(pu8Frame[19]==0xff)
               {
                   Sensorname2Port3 ="No Sensor";
               }
               if(pu8Frame[19]==1)
               {
                   Sensorname2Port3 = "AM2315";
               }
               if(pu8Frame[19]==2)
               {
                   Sensorname2Port3 = "BH1750";
               }
               StrChannel2Port3.append(Sensorname2Port3);
               StrPort3.append(StrChannel2Port3);
        ui->lineEdit_GetOnPort3->setText(StrPort3);
        //Port 4 20,21,22
        QString str4= QString::number(pu8Frame[20]);
        QString StrPort4 = "Port: ";
                StrPort4.append(str4);
                //channel 1
               QString StrChannel1Port4 = " Channel 1: ";
               QString Sensorname1Port4;
               if(pu8Frame[21]==0xff)
               {
                   Sensorname1Port4 ="No Sensor";
               }
               if(pu8Frame[21]==1)
               {
                   Sensorname1Port4 = "AM2315";
               }
               if(pu8Frame[21]==2)
               {
                   Sensorname1Port4 = "BH1750";
               }
               StrChannel1Port4.append(Sensorname1Port4);
               StrPort4.append(StrChannel1Port4);

               //channel 2
              QString StrChannel2Port4 = " Channel 2: ";
              QString Sensorname2Port4;
              if(pu8Frame[22]==0xff)
              {
                  Sensorname2Port4 ="No Sensor";
              }
              if(pu8Frame[22]==1)
              {
                  Sensorname2Port4 = "AM2315";
              }
              if(pu8Frame[22]==2)
              {
                  Sensorname2Port4 = "BH1750";
              }
              StrChannel2Port4.append(Sensorname2Port4);
              StrPort4.append(StrChannel2Port4);
         ui->lineEdit_GetOnPort4->setText(StrPort4);
        QMessageBox::information(this,tr("Dialog"),tr("Get Sensor Port Done"));
    }
}



void MainWindow::on_btnGenerate_clicked()
{
#if 1
    QByteArray pData    = QByteArray::fromHex(ui->txtData->toPlainText().remove(QChar(' ')).toLocal8Bit().constData());

    uint8_t u8Dlen  = pData.length();
    uint16_t u16FrameLen = RSP_FRM_HDR_SIZE + u8Dlen + RSP_FRM_EOF_SIZE + RSP_FRM_CRCD_SIZE;
    uint8_t *pu8RSPFrame = new uint8_t[u16FrameLen];

    QByteArray pFlags = QByteArray::fromHex(this->ui->txtFlags->text().remove(QChar(' ')).toLocal8Bit().constData());
    QByteArray pSeqNo = QByteArray::fromHex(this->ui->txtSeqNo->text().remove(QChar(' ')).toLocal8Bit().constData());
    QByteArray pSrcAddr = QByteArray::fromHex(this->ui->txtSrcAddr->text().remove(QChar(' ')).toLocal8Bit().constData());
    QByteArray pDstAddr = QByteArray::fromHex(this->ui->txtDstAddr->text().remove(QChar(' ')).toLocal8Bit().constData());

    if(pFlags.length() != 1)
        return;

    if(pSeqNo.length() != 1)
        return;

    if(pSrcAddr.length() != 2)
        return;

    if(pDstAddr.length() != 2)
        return;




    /**
     * RSP Frame structure
     * 0_____2______4______5_____6_____8_____10______11__________________________
     * +-----+------+------+-----+-----+-----+-------+-------------+-------+-----+
     * | SOF | DLEN | FLAG | SEQ | DST | SRC | CRC_H | DATA[0...n] | CRC_D | EOF |
     * | (2) |  (2) |  (1) | (1) | (2) | (2) |  (1)  |   (DLEN)    |  (2)  | (1) |
     * +-----+------+------+-----+-----+-----+-------+-------------+-------+-----+
     */

    pu8RSPFrame[RSP_FRM_IDX_SOF] = RSP_SOF_PREAMBLE;
    pu8RSPFrame[RSP_FRM_IDX_SOF + 1] = RSP_SOF_PREAMBLE;

    pu8RSPFrame[RSP_FRM_IDX_DLEN] = 0;
    pu8RSPFrame[RSP_FRM_IDX_DLEN + 1] = u8Dlen;

    pu8RSPFrame[RSP_FRM_IDX_FLAGS] = pFlags[0];
    pu8RSPFrame[RSP_FRM_IDX_SEQ] = pSeqNo[0];

    pu8RSPFrame[RSP_FRM_IDX_DSTADDR] = pDstAddr[0];
    pu8RSPFrame[RSP_FRM_IDX_DSTADDR+1] = pDstAddr[1];

    pu8RSPFrame[RSP_FRM_IDX_SRCADDR] = pSrcAddr[0];
    pu8RSPFrame[RSP_FRM_IDX_SRCADDR+1] = pSrcAddr[1];

    // Calculate CRC header
    pu8RSPFrame[RSP_FRM_IDX_CRCH] = CalCRC8(&pu8RSPFrame[RSP_FRM_IDX_DLEN], RSP_FRM_HDR_SIZE - RSP_FRM_SOF_SIZE - 1);

    // Copy data
    memcpy(&pu8RSPFrame[RSP_FRM_IDX_DATA0], pData, u8Dlen);

    // Calculate CRC Data
    uint16_t    u16CRCD = CalculateCRC16(&pu8RSPFrame[RSP_FRM_IDX_CRCH], u8Dlen + 1);
    pu8RSPFrame[RSP_FRM_IDX_DATA0 + u8Dlen] = (u16CRCD >> 8) & 0xFF;
    pu8RSPFrame[RSP_FRM_IDX_DATA0 + u8Dlen + 1] = u16CRCD & 0xFF;

    // EOF
    pu8RSPFrame[RSP_FRM_IDX_DATA0 + u8Dlen + 2] = 0xFF;

    this->ui->txtRSPFrame->setText("");

    char tmp[10] = {0};
    for(uint8_t idx = 0; idx < u16FrameLen; idx ++)
    {
        if(this->ui->chkPrefixEnable->checkState() == Qt::Checked)
        {
            sprintf(tmp, "0x%.2X, ", pu8RSPFrame[idx]);
        }
        else
        {
            sprintf(tmp, "%.2X ", pu8RSPFrame[idx]);
        }

        this->ui->txtRSPFrame->setText(this->ui->txtRSPFrame->toPlainText() + QString::fromStdString(tmp) );

    }

    delete pu8RSPFrame;
#endif
}

void MainWindow::on_btnSBusGenFrame_clicked()
{
    QByteArray pData    = QByteArray::fromHex(ui->txtSBusData->toPlainText().remove(QChar(' ')).toLocal8Bit().constData());

    uint8_t u8Dlen  = pData.length();

    uint16_t u16FrameLen = SMBUS_FRM_HDR_SIZE + u8Dlen + 2;
    uint8_t *pu8SBusFrame = new uint8_t[u16FrameLen];

    QByteArray pSrcAddr = QByteArray::fromHex(this->ui->txtSBusSrcAddr->text().remove(QChar(' ')).toLocal8Bit().constData());
    QByteArray pDevType = QByteArray::fromHex(this->ui->txtSBusDevType->text().remove(QChar(' ')).toLocal8Bit().constData());
    QByteArray pOpcode = QByteArray::fromHex(this->ui->txtSBusOpcode->text().remove(QChar(' ')).toLocal8Bit().constData());
    QByteArray pDstAddr = QByteArray::fromHex(this->ui->txtSBusDestAddr->text().remove(QChar(' ')).toLocal8Bit().constData());

    if(pSrcAddr.length() != 2)
        return;

    if(pDevType.length() != 2)
        return;

    if(pOpcode.length() != 2)
        return;

    if(pDstAddr.length() != 2)
        return;

    //+----------+------+----------+----------+--------+----------+------------+-------+
    //| PREAMBLE | DLEN | SRC_ADDR | DEV_TYPE | OPCODE | DST_ADDR | DATA[0..n] | CRC_D |
    //|  (2B)    | (1B) | (2B)     | (2B)     | (2B)   | (2B)     | (nB)       | (2B)  |
    //+----------+------+----------+----------+--------+----------+------------+-------+
    //	  AAAA      0F      0CFE      FFFE       0031      010C     01 64 00 01   B22F

    pu8SBusFrame[SMBUS_FRM_IDX_SOF] = SMBUS_SOF_PREAMBLE;
    pu8SBusFrame[SMBUS_FRM_IDX_SOF + 1] = SMBUS_SOF_PREAMBLE;

    pu8SBusFrame[SMBUS_FRM_IDX_DLEN] = u16FrameLen - 2;

    pu8SBusFrame[SMBUS_FRM_IDX_SRC_SUB_ID] = pSrcAddr[0];
    pu8SBusFrame[SMBUS_FRM_IDX_SRC_DEV_ID] = pSrcAddr[1];

    pu8SBusFrame[SMBUS_FRM_IDX_DEV_TYPE] = pDevType[0];
    pu8SBusFrame[SMBUS_FRM_IDX_DEV_TYPE+1] = pDevType[1];

    pu8SBusFrame[SMBUS_FRM_IDX_OP_CODE] = pOpcode[0];
    pu8SBusFrame[SMBUS_FRM_IDX_OP_CODE+1] = pOpcode[1];

    pu8SBusFrame[SMBUS_FRM_IDX_DST_SUB_ID] = pDstAddr[0];
    pu8SBusFrame[SMBUS_FRM_IDX_DST_DEV_ID] = pDstAddr[1];

    // Copy data
    memcpy(&pu8SBusFrame[SMBUS_FRM_IDX_DATA0], pData, u8Dlen);

    // Calculate the CRC_D
    uint16_t    u16CRCD = CalculateCRC16(&pu8SBusFrame[SMBUS_FRM_IDX_DLEN], u8Dlen + SMBUS_FRM_HDR_SIZE - 2);
    pu8SBusFrame[SMBUS_FRM_IDX_DATA0 + u8Dlen] = (u16CRCD >> 8) & 0xFF;
    pu8SBusFrame[SMBUS_FRM_IDX_DATA0 + u8Dlen + 1] = u16CRCD & 0xFF;

    this->ui->txtSBusFrame->setText("");

    char tmp[10] = {0};
    for(uint8_t idx = 0; idx < u16FrameLen; idx ++)
    {
        if(this->ui->chkSBusPrefixEnable->checkState() == Qt::Checked)
        {
            sprintf(tmp, "0x%.2X, ", pu8SBusFrame[idx]);
        }
        else
        {
            sprintf(tmp, "%.2X ", pu8SBusFrame[idx]);
        }

        this->ui->txtSBusFrame->setText(this->ui->txtSBusFrame->toPlainText() + QString::fromStdString(tmp) );

    }

    delete pu8SBusFrame;

}

void MainWindow::on_btnModbusFrameGen_clicked()
{
        float a = 1.23;
        uint32_t*  t = (uint32_t*)&a;
        qDebug("a = %x", *t);
    QByteArray pData    = QByteArray::fromHex(ui->txtModbusData->toPlainText().remove(QChar(' ')).toLocal8Bit().constData());

    uint8_t u8Dlen  = pData.length();

    uint16_t u16FrameLen = 4 + u8Dlen;
    uint8_t *pu8ModbusFrame = new uint8_t[u16FrameLen];

    QByteArray pAddress = QByteArray::fromHex(this->ui->txtModbusAddress->text().remove(QChar(' ')).toLocal8Bit().constData());
    QByteArray pOpcode = QByteArray::fromHex(this->ui->txtModbusFcnCode->text().remove(QChar(' ')).toLocal8Bit().constData());


    if(pAddress.length() != 1)
        return;


    if(pOpcode.length() != 1)
        return;

    pu8ModbusFrame[0] = pAddress[0];
    pu8ModbusFrame[1] = pOpcode[0];

    // Copy data
    memcpy(&pu8ModbusFrame[2], pData, u8Dlen);

    // Calculate the CRC_D
    uint16_t    u16CRCD = ModRTU_CRC(&pu8ModbusFrame[0], u8Dlen + 2);
    qDebug("CRC: %x", u16CRCD);
    pu8ModbusFrame[2 + u8Dlen + 1] = (u16CRCD >> 8) & 0xFF;
    pu8ModbusFrame[2 + u8Dlen] = u16CRCD & 0xFF;

    this->ui->txtModbusFrame->setText("");

    char tmp[10] = {0};
    for(uint8_t idx = 0; idx < u16FrameLen; idx ++)
    {
        if(this->ui->chkModbusPrefixEnable->checkState() == Qt::Checked)
        {
            sprintf(tmp, "0x%.2X, ", pu8ModbusFrame[idx]);
        }
        else
        {
            sprintf(tmp, "%.2X ", pu8ModbusFrame[idx]);
        }

        this->ui->txtModbusFrame->setText(this->ui->txtModbusFrame->toPlainText() + QString::fromStdString(tmp) );

    }

    delete pu8ModbusFrame;
}
//done
void MainWindow::on_btt_readsensorvalue_clicked()
{
        uint8_t data[] = { 0x55, 0x55, 	// sof  - idx = 0
                0x00, 0x02, 	// dlen	- idx = 2
                0x00, 			// flag - idx = 4
                0x00, 			// seq -  idx = 5
                0x00, 0x01,		// dst - idx = 6
                0x00, 0xff,		// src - idx = 8
                0x00,			// crc_h - idx = 10
                0x01, 0x02,		// Data[0] - idx = 11
                0x00, 0x00,		// crc
                0xff			// eof
                };
        data[RSP_FRM_IDX_DSTADDR] = (m_u16SensorID >> 8)&0xFF;
        data[RSP_FRM_IDX_DSTADDR+1] = m_u16SensorID & 0xFF;
        qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR]);
        qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR+1]);
//        //calculate CRC_H
        data[RSP_FRM_IDX_CRCH] = CalCRC8(&data[RSP_FRM_IDX_DLEN], 8);
        uint16_t u16Dlen = 2;
        // Calculate CRC_D
        uint16_t u16CRCD = CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1);
//		*(uint16_t*)&data[RSP_FRM_IDX_DATA0 + u16Dlen] = htons(CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1));
        data[RSP_FRM_IDX_DATA0 +  u16Dlen] = (u16CRCD >> 8) & 0xFF;
        data[RSP_FRM_IDX_DATA0 +  u16Dlen + 1] = (u16CRCD) & 0xFF;

        for(int i=0;i<16;i++)
        {
            qDebug("%.2x",data[i]);
        }
        if( pSerialPort->write((const char*)data, 16) != 16)
        {
//            qDebug("Send data failed!");
//            LogToConsole("Send data failed!\r\n");
            LREP("Send data failed!");
        }

}
//done
void MainWindow::on_btt_readRTC_clicked()
{
    qDebug("Run btt Read RTC Time");
    uint8_t data[] = { 0x55,0x55, 	// sof  - idx = 0
            0x00,0x02, 	// dlen	- idx = 2
            0x00, 			// flag - idx = 4
            0x00, 			// seq -  idx = 5
            0x00, 0x01,		// dst - idx = 6
            0x00, 0xff,		// src - idx = 8
            0x00,			// crc_h - idx = 10
            0x01, 0x05,		// Data[0] - idx = 11
            0x00, 0x00,		// crc
            0xff			// eof
            };
    data[RSP_FRM_IDX_DSTADDR] = (m_u16SensorID >> 8)&0xFF;
    data[RSP_FRM_IDX_DSTADDR+1] = m_u16SensorID & 0xFF;
    // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR]);
    // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR+1]);
    //        //calculate CRC_H
    data[RSP_FRM_IDX_CRCH] = CalCRC8(&data[RSP_FRM_IDX_DLEN], 8);
    uint16_t u16Dlen = 2;
    // Calculate CRC_D
    uint16_t u16CRCD = CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1);
//		*(uint16_t*)&data[RSP_FRM_IDX_DATA0 + u16Dlen] = htons(CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1));
    data[RSP_FRM_IDX_DATA0 +  u16Dlen] = (u16CRCD >> 8) & 0xFF;
    data[RSP_FRM_IDX_DATA0 +  u16Dlen + 1] = (u16CRCD) & 0xFF;

    for(int i=0;i<16;i++)
    {
        qDebug("%.2x",data[i]);
    }
    if( pSerialPort->write((const char*)data, 16) != 16)
    {
//            qDebug("Send data failed!");
//            LogToConsole("Send data failed!\r\n");
        LREP("Send data failed!");
    }
}
// done
void MainWindow::on_btt_writrRTC_clicked()
{
    qDebug("Run Write RTC Time");
    uint8_t data[] = { 0x55, 0x55, 	// sof  - idx = 0
            0x00, 0x05, 	// dlen	- idx = 2
            0x00, 			// flag - idx = 4
            0x00, 			// seq -  idx = 5
            0x00, 0x01,		// dst - idx = 6
            0x00, 0xff,		// src - idx = 8
            0x00,			// crc_h - idx = 10
            0x01, 0x06,		// Data[0] - idx = 11
            0x00,0x00,0x00,
            0x00, 0x00,		// crc
            0xff			// eof
            };
    data[RSP_FRM_IDX_DSTADDR] = (m_u16SensorID >> 8)&0xFF;
    data[RSP_FRM_IDX_DSTADDR+1] = m_u16SensorID & 0xFF;
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR]);
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR+1]);
//        //calculate CRC_H
    data[RSP_FRM_IDX_CRCH] = CalCRC8(&data[RSP_FRM_IDX_DLEN], 8);
    //get time to qstring.
//    QString strTime = time_text;
//    qDebug()<<strTime;
//    //lay 2 ky tu tu dau
//    qDebug()<<strTime.left(2);
//    qDebug()<<strTime.mid(3,2);
//    qDebug()<<strTime.right(2);
//    bool b;
//    uint8_t uH = strTime.left(2).toUInt(&b,24);
//    uint8_t uP = strTime.mid(3,2).toUInt(&b,24);
//    uint8_t uG = strTime.right(2).toUInt(&b,24);
    QTime time = QTime::currentTime();
    qDebug("\nH%.2x",time.hour());
    qDebug("\nP%.2x",time.minute());
    qDebug("\nG%.2x",time.second());
    uint16_t u16Dlen = 5;

    data[RSP_FRM_IDX_CRCH+3]=time.hour();
    data[RSP_FRM_IDX_CRCH+4]=time.minute();
    data[RSP_FRM_IDX_CRCH+5]=time.second();

    // Calculate CRC_D
    uint16_t u16CRCD = CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1);
//		*(uint16_t*)&data[RSP_FRM_IDX_DATA0 + u16Dlen] = htons(CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1));
    data[RSP_FRM_IDX_DATA0 +  u16Dlen] = (u16CRCD >> 8) & 0xFF;
    data[RSP_FRM_IDX_DATA0 +  u16Dlen + 1] = (u16CRCD) & 0xFF;

    for(int i=0;i<19;i++)
    {
        qDebug("%.2x",data[i]);
    }
    if( pSerialPort->write((const char*)data, 19) != 19)
    {
//            qDebug("Send data failed!");
//            LogToConsole("Send data failed!\r\n");
        LREP("Send data failed!");
    }
}
//done
void MainWindow::on_btt_change_slaveAdd_clicked()
{
    qDebug("Run Change Address");
    uint8_t data[] = { 0x55, 0x55, 	// sof  - idx = 0
            0x00, 0x04, 	// dlen	- idx = 2
            0x00, 			// flag - idx = 4
            0x00, 			// seq -  idx = 5
            0x00, 0x01,		// dst - idx = 6
            0x00, 0xff,		// src - idx = 8
            0x00,			// crc_h - idx = 10
            0x01, 0x25,     // Data[0] - idx = 11
            0x00, 0x00,     // data
            0x00, 0x00,		// crc
            0xff			// eof
            };
    data[RSP_FRM_IDX_DSTADDR] = (m_u16SensorID >> 8)&0xFF;
    data[RSP_FRM_IDX_DSTADDR+1] = m_u16SensorID & 0xFF;
// qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR+1]);
//        //calculate CRC_H
    data[RSP_FRM_IDX_CRCH] = CalCRC8(&data[RSP_FRM_IDX_DLEN], 8);
    uint16_t u16Dlen = 4;
    QString strval = ui->lineEdit_change_slaveAdd->text();
    //qDebug()<<"new slave add ";
    //qDebug()<<strval;
    // Calculate CRC_D
    data[RSP_FRM_IDX_CRCH+3]=strval.left(2).toInt();
    data[RSP_FRM_IDX_CRCH+4]=strval.right(2).toInt();
    uint16_t u16CRCD = CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1);
//		*(uint16_t*)&data[RSP_FRM_IDX_DATA0 + u16Dlen] = htons(CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1));
    data[RSP_FRM_IDX_DATA0 +  u16Dlen] = (u16CRCD >> 8) & 0xFF;
    data[RSP_FRM_IDX_DATA0 +  u16Dlen + 1] = (u16CRCD) & 0xFF;

    for(int i=0;i<18;i++)
    {
        qDebug("%.2x",data[i]);
    }
    if( pSerialPort->write((const char*)data, 18) != 18)
    {
//            qDebug("Send data failed!");
//            LogToConsole("Send data failed!\r\n");
        LREP("Send data failed!");
    }
}
//done
void MainWindow::on_btt_writrRTC_date_clicked()
{
    qDebug("Run Write RTC date");
//    QString strDate = date_text;
//    qDebug()<<strDate;
//    //lay 2 ky tu tu dau
//    qDebug()<<strDate.left(1);
//    qDebug()<<strDate.mid(2,2);
//    qDebug()<<strDate.right(4);
//    bool b;
//    uint8_t uM = strDate.left(1).toUInt(&b,12);
//    uint8_t uD = strDate.mid(2,2).toUInt(&b,31);
//    uint8_t uY = strDate.right(4).toUInt(&b,3000);
//    qDebug("\nuD%.2x",uM);
//    qDebug("\nuM%.2x",uD);
//    qDebug("\nuY%.2x",uY);
    //
    uint8_t data[] = { 0x55, 0x55, 	// sof  - idx = 0
            0x00, 0x06, 	// dlen	- idx = 2
            0x00, 			// flag - idx = 4
            0x00, 			// seq -  idx = 5
            0x00, 0x01,		// dst - idx = 6
            0x00, 0xff,		// src - idx = 8
            0x00,			// crc_h - idx = 10
            0x01, 0x08,		// Data[0] - idx = 11
            0x00,0x00,
            0x00,0x00,
            0x00, 0x00,		// crc
            0xff			// eof
            };
    data[RSP_FRM_IDX_DSTADDR] = (m_u16SensorID >> 8)&0xFF;
    data[RSP_FRM_IDX_DSTADDR+1] = m_u16SensorID & 0xFF;
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR]);
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR+1]);
//        //calculate CRC_H
    data[RSP_FRM_IDX_CRCH] = CalCRC8(&data[RSP_FRM_IDX_DLEN], 8);

    uint16_t u16Dlen = 6;
    QDate date =QDate::currentDate();
    QString datetxt = date.toString(Qt::SystemLocaleShortDate);
    //qDebug()<<datetxt;
    int wd = date.dayOfWeek();
    bool b;
    data[RSP_FRM_IDX_CRCH+3]=wd;
    data[RSP_FRM_IDX_CRCH+4]=date.day();
    data[RSP_FRM_IDX_CRCH+5]=date.month();
    data[RSP_FRM_IDX_CRCH+6]=datetxt.right(2).toInt();

    // Calculate CRC_D
    uint16_t u16CRCD = CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1);
//		*(uint16_t*)&data[RSP_FRM_IDX_DATA0 + u16Dlen] = htons(CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1));
    data[RSP_FRM_IDX_DATA0 +  u16Dlen] = (u16CRCD >> 8) & 0xFF;
    data[RSP_FRM_IDX_DATA0 +  u16Dlen + 1] = (u16CRCD) & 0xFF;

    for(int i=0;i<20;i++)
    {
        qDebug("%.2x",data[i]);
    }
    if( pSerialPort->write((const char*)data, 20) != 20)
    {
//            qDebug("Send data failed!");
//            LogToConsole("Send data failed!\r\n");
        LREP("Send");
    }



}
//done
void MainWindow::on_btt_readRTC_date_clicked()
{
    qDebug("Run Read RTC date");
    uint8_t data[] = { 0x55, 0x55, 	// sof  - idx = 0
            0x00, 0x02, 	// dlen	- idx = 2
            0x00, 			// flag - idx = 4
            0x00, 			// seq -  idx = 5
            0x00, 0x01,		// dst - idx = 6
            0x00, 0xff,		// src - idx = 8
            0x00,			// crc_h - idx = 10
            0x01, 0x07,		// Data[0] - idx = 11
            0x00, 0x00,		// crc
            0xff			// eof
            };
    data[RSP_FRM_IDX_DSTADDR] = (m_u16SensorID >> 8)&0xFF;
    data[RSP_FRM_IDX_DSTADDR+1] = m_u16SensorID & 0xFF;
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR]);
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR+1]);
//        //calculate CRC_H
    data[RSP_FRM_IDX_CRCH] = CalCRC8(&data[RSP_FRM_IDX_DLEN], 8);
    uint16_t u16Dlen = 2;
    // Calculate CRC_D
    uint16_t u16CRCD = CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1);
//		*(uint16_t*)&data[RSP_FRM_IDX_DATA0 + u16Dlen] = htons(CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1));
    data[RSP_FRM_IDX_DATA0 +  u16Dlen] = (u16CRCD >> 8) & 0xFF;
    data[RSP_FRM_IDX_DATA0 +  u16Dlen + 1] = (u16CRCD) & 0xFF;

    for(int i=0;i<16;i++)
    {
        qDebug("%.2x",data[i]);
    }
    if( pSerialPort->write((const char*)data, 16) != 16)
    {
//            qDebug("Send data failed!");
//            LogToConsole("Send data failed!\r\n");
        LREP("Send data failed!");
    }
}

void MainWindow::on_btt_getsensor_on_port_clicked()
{
    qDebug("Get Sensor On Port");
    uint8_t data[] = { 0x55, 0x55, 	// sof  - idx = 0
            0x00, 0x02, 	// dlen	- idx = 2
            0x00, 			// flag - idx = 4
            0x00, 			// seq -  idx = 5
            0x00, 0x01,		// dst - idx = 6
            0x00, 0xff,		// src - idx = 8
            0x00,			// crc_h - idx = 10
            0x01, 0x09,		// Data[0] - idx = 11
            0x00, 0x00,		// crc
            0xff			// eof
            };
    data[RSP_FRM_IDX_DSTADDR] = (m_u16SensorID >> 8)&0xFF;
    data[RSP_FRM_IDX_DSTADDR+1] = m_u16SensorID & 0xFF;
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR]);
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR+1]);
//        //calculate CRC_H
    data[RSP_FRM_IDX_CRCH] = CalCRC8(&data[RSP_FRM_IDX_DLEN], 8);
    uint16_t u16Dlen = 2;
    // Calculate CRC_D
    uint16_t u16CRCD = CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1);
//		*(uint16_t*)&data[RSP_FRM_IDX_DATA0 + u16Dlen] = htons(CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1));
    data[RSP_FRM_IDX_DATA0 +  u16Dlen] = (u16CRCD >> 8) & 0xFF;
    data[RSP_FRM_IDX_DATA0 +  u16Dlen + 1] = (u16CRCD) & 0xFF;

    for(int i=0;i<16;i++)
    {
        qDebug("%.2x",data[i]);
    }
    if( pSerialPort->write((const char*)data, 16) != 16)
    {
//            qDebug("Send data failed!");
//            LogToConsole("Send data failed!\r\n");
        LREP("Send data failed!");
    }
}

void MainWindow::on_btt_Read_offset_clicked()
{
    qDebug("Read Offset");
    uint8_t data[] = { 0x55, 0x55, 	// sof  - idx = 0
            0x00, 0x02, 	// dlen	- idx = 2
            0x00, 			// flag - idx = 4
            0x00, 			// seq -  idx = 5
            0x00, 0x01,		// dst - idx = 6
            0x00, 0xff,		// src - idx = 8
            0x00,			// crc_h - idx = 10
            0x01, 0x29,		// Data[0] - idx = 11
            0x00, 0x00,		// crc
            0xff			// eof
            };
    data[RSP_FRM_IDX_DSTADDR] = (m_u16SensorID >> 8)&0xFF;
    data[RSP_FRM_IDX_DSTADDR+1] = m_u16SensorID & 0xFF;
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR]);
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR+1]);
//        //calculate CRC_H
    data[RSP_FRM_IDX_CRCH] = CalCRC8(&data[RSP_FRM_IDX_DLEN], 8);
    uint16_t u16Dlen = 2;
    // Calculate CRC_D
    uint16_t u16CRCD = CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1);
//		*(uint16_t*)&data[RSP_FRM_IDX_DATA0 + u16Dlen] = htons(CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1));
    data[RSP_FRM_IDX_DATA0 +  u16Dlen] = (u16CRCD >> 8) & 0xFF;
    data[RSP_FRM_IDX_DATA0 +  u16Dlen + 1] = (u16CRCD) & 0xFF;

    for(int i=0;i<16;i++)
    {
        qDebug("%.2x",data[i]);
    }
    if( pSerialPort->write((const char*)data, 16) != 16)
    {
//            qDebug("Send data failed!");
//            LogToConsole("Send data failed!\r\n");
        LREP("Send data failed!");
    }
}
//done
void MainWindow::on_btt_Read_SlaveAdd_clicked()
{
    qDebug("Run Read Slave Address");
    uint8_t data[] = { 0x55, 0x55, 	// sof  - idx = 0
            0x00, 0x02, 	// dlen	- idx = 2
            0x00, 			// flag - idx = 4
            0x00, 			// seq -  idx = 5
            0x00, 0x01,		// dst - idx = 6
            0x00, 0xff,		// src - idx = 8
            0x00,			// crc_h - idx = 10
            0x01, 0x01,		// Data[0] - idx = 11
            0x00, 0x00,		// crc
            0xff			// eof
            };
    data[RSP_FRM_IDX_DSTADDR] = (m_u16SensorID >> 8)&0xFF;
    data[RSP_FRM_IDX_DSTADDR+1] = m_u16SensorID & 0xFF;
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR]);
   // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR+1]);
//        //calculate CRC_H
    data[RSP_FRM_IDX_CRCH] = CalCRC8(&data[RSP_FRM_IDX_DLEN], 8);
    uint16_t u16Dlen = 2;
    // Calculate CRC_D
    uint16_t u16CRCD = CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1);
//		*(uint16_t*)&data[RSP_FRM_IDX_DATA0 + u16Dlen] = htons(CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1));
    data[RSP_FRM_IDX_DATA0 +  u16Dlen] = (u16CRCD >> 8) & 0xFF;
    data[RSP_FRM_IDX_DATA0 +  u16Dlen + 1] = (u16CRCD) & 0xFF;

    for(int i=0;i<16;i++)
    {
        qDebug("%.2x",data[i]);
    }
    if( pSerialPort->write((const char*)data, 16) != 16)
    {
//            qDebug("Send data failed!");
//            LogToConsole("Send data failed!\r\n");
        LREP("Send data failed!");
    }
}

void MainWindow::on_btt_Write_offset_clicked()
{

}

void MainWindow::on_btt_setsensor_onport_clicked()
{

   QString strport = QString::number(ui->cboSensorPort->currentIndex()+1);
   qDebug()<<"Port";
   qDebug()<<strport;
   QString strch1  = QString::number(ui->cboChannel1->currentIndex());
   qDebug()<<"Channel 1";
   qDebug()<<strch1;
   QString strch2  = QString::number(ui->cboChannel2->currentIndex());
   qDebug()<<"Channel 2";
   qDebug()<<strch2;

   qDebug("Write Sensor On Port");
   uint8_t data[] = { 0x55, 0x55, 	// sof  - idx = 0
           0x00, 0x05, 	// dlen	- idx = 2
           0x00, 			// flag - idx = 4
           0x00, 			// seq -  idx = 5
           0x00, 0x01,		// dst - idx = 6
           0x00, 0xff,		// src - idx = 8
           0x00,			// crc_h - idx = 10
           0x01, 0x0a,		// Data[0] - idx = 11
           0x00,0x00,0x00,
           0x00, 0x00,		// crc
           0xff			// eof
           };
   data[RSP_FRM_IDX_DSTADDR] = (m_u16SensorID >> 8)&0xFF;
   data[RSP_FRM_IDX_DSTADDR+1] = m_u16SensorID & 0xFF;
  // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR]);
  // qDebug("%.2x",data[RSP_FRM_IDX_SRCADDR+1]);
//        //calculate CRC_H
   data[RSP_FRM_IDX_CRCH] = CalCRC8(&data[RSP_FRM_IDX_DLEN], 8);
   uint16_t u16Dlen = 5;
   data[RSP_FRM_IDX_CRCH+3]=strport.toInt();
   if(ui->cboChannel1->currentIndex()==0)
   {
        data[RSP_FRM_IDX_CRCH+4]=0xff;
   }
    else if(ui->cboChannel1->currentIndex()!=0)
   {
       data[RSP_FRM_IDX_CRCH+4]=strch1.toInt();
   }
   if(ui->cboChannel2->currentIndex()==0)
   {
       data[RSP_FRM_IDX_CRCH+5]=0xff;
   }
    else if(ui->cboChannel2->currentIndex()!=0)
    {
        data[RSP_FRM_IDX_CRCH+5]=strch2.toInt();
    }


   // Calculate CRC_D
   uint16_t u16CRCD = CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1);
//	*(uint16_t*)&data[RSP_FRM_IDX_DATA0 + u16Dlen] = htons(CalculateCRC16(&data[RSP_FRM_IDX_CRCH], u16Dlen + 1));
   data[RSP_FRM_IDX_DATA0 +  u16Dlen] = (u16CRCD >> 8) & 0xFF;
   data[RSP_FRM_IDX_DATA0 +  u16Dlen + 1] = (u16CRCD) & 0xFF;

   for(int i=0;i<19;i++)
   {
       qDebug("%.2x",data[i]);
   }
   if( pSerialPort->write((const char*)data, 19) != 19)
   {
//            qDebug("Send data failed!");
//            LogToConsole("Send data failed!\r\n");
       LREP("Send data failed!");
   }

}
