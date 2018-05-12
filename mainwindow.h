<<<<<<< HEAD
<<<<<<< HEAD
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "QRingBuffer.h"
#include <QTimer>
#include <QMutex>
#include <ahg.h>
#include <clsTransSmartBus.h>
#include <clsTransRSP.h>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sldAirTemp_valueChanged(int value);

    void on_sldAirHumid_valueChanged(int value);

    void on_sldSoilTemp_valueChanged(int value);

    void on_sldSoilMoisture_valueChanged(int value);

    void on_sldLightIntensity_valueChanged(int value);

    void on_sldWaterEC_valueChanged(int value);

    void on_sldWaterPH_valueChanged(int value);

    void on_sldWaterLevel_valueChanged(int value);

    void on_sldSoilEC_valueChanged(int value);

    void on_sldSoidPH_valueChanged(int value);

    void on_sldWaterTemp_valueChanged(int value);

    void on_btnRefreshPorts_clicked();

    void on_SerialportDataReceived();

    void on_txtSensorAddress_textChanged();

    void on_btnStartSimulation_clicked();

    void onTimerTicked();


    void on_btnGenerate_clicked();

    void on_btnSBusGenFrame_clicked();

    void on_btnModbusFrameGen_clicked();

    void on_btt_readsensorvalue_clicked();


    void on_btt_readRTC_clicked();

    void on_btt_writrRTC_clicked();

    void on_btt_change_slaveAdd_clicked();
    void ShowTime();

    void on_btt_writrRTC_date_clicked();

    void on_btt_readRTC_date_clicked();

    void on_btt_getsensor_on_port_clicked();

    void on_btt_Read_offset_clicked();

    void on_btt_Read_SlaveAdd_clicked();

    void on_btt_Write_offset_clicked();

    void on_btt_setsensor_onport_clicked();

private:
    Ui::MainWindow *ui;
    QMutex          *pRxBuffMutex;
    QRingBuffer     *pRxBuffer;
    QSerialPort     *pSerialPort;
    QTimer         *pTimer;
    uint16_t        m_u16SensorID = 0;
    uint16_t        m_u16SensorIDchange = 0;
    bool            m_bRunning = false;
    QString         time_text;
    QString         date_text ;
    void RunSimulation(bool bIsterminated);

    void ProcessRawData();

    void ProcessRSPFrame(void *pvFrame);

    void ProcessRSPFrameReaddata(void *pvFrame);
    void ProcessRSPFrameReadRTCTime(void *pvFrame);
    void ProcessRSPFrameWriteRTCTime(void *pvFrame);
    void ProcessRSPFrameReadRTCDate(void *pvFrame);
    void ProcessRSPFrameWriteRTCDate(void *pvFrame);
    void ProcessRSPFrameChangeAddress(void *pvFrame);
    void ProcessRSPFrameGetSlaveAddress(void *pvFrame);
    void LogToConsole(const char* string, ...);
    void LogToConsoleTime(const char* string, ...);

};

#endif // MAINWINDOW_H
=======
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "QRingBuffer.h"
#include <QTimer>
#include <QMutex>
#include <ahg.h>
#include <clsTransSmartBus.h>
#include <clsTransRSP.h>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sldAirTemp_valueChanged(int value);

    void on_sldAirHumid_valueChanged(int value);

    void on_sldSoilTemp_valueChanged(int value);

    void on_sldSoilMoisture_valueChanged(int value);

    void on_sldLightIntensity_valueChanged(int value);

    void on_sldWaterEC_valueChanged(int value);

    void on_sldWaterPH_valueChanged(int value);

    void on_sldWaterLevel_valueChanged(int value);

    void on_sldSoilEC_valueChanged(int value);

    void on_sldSoidPH_valueChanged(int value);

    void on_sldWaterTemp_valueChanged(int value);

    void on_btnRefreshPorts_clicked();

    void on_SerialportDataReceived();

    void on_txtSensorAddress_textChanged();

    void on_btnStartSimulation_clicked();

    void onTimerTicked();


    void on_btnGenerate_clicked();

    void on_btnSBusGenFrame_clicked();

    void on_btnModbusFrameGen_clicked();

    void on_btt_readsensorvalue_clicked();


    void on_btt_readRTC_clicked();

    void on_btt_writrRTC_clicked();

    void on_btt_change_slaveAdd_clicked();
    void ShowTime();

    void on_btt_writrRTC_date_clicked();

    void on_btt_readRTC_date_clicked();

    void on_btt_getsensor_on_port_clicked();

    void on_btt_Read_offset_clicked();

    void on_btt_Read_SlaveAdd_clicked();

    void on_btt_Write_offset_clicked();

    void on_btt_setsensor_onport_clicked();

private:
    Ui::MainWindow *ui;
    QMutex          *pRxBuffMutex;
    QRingBuffer     *pRxBuffer;
    QSerialPort     *pSerialPort;
    QTimer         *pTimer;
    uint16_t        m_u16SensorID = 0;
    uint16_t        m_u16SensorIDchange = 0;
    bool            m_bRunning = false;
    QString         time_text;
    QString         date_text ;
    void RunSimulation(bool bIsterminated);

    void ProcessRawData();

    void ProcessRSPFrame(void *pvFrame);

    void ProcessRSPFrameReaddata(void *pvFrame);
    void ProcessRSPFrameReadRTCTime(void *pvFrame);
    void ProcessRSPFrameWriteRTCTime(void *pvFrame);
    void ProcessRSPFrameReadRTCDate(void *pvFrame);
    void ProcessRSPFrameWriteRTCDate(void *pvFrame);
    void ProcessRSPFrameChangeAddress(void *pvFrame);
    void ProcessRSPFrameGetSlaveAddress(void *pvFrame);
    void LogToConsole(const char* string, ...);
    void LogToConsoleTime(const char* string, ...);

};

#endif // MAINWINDOW_H
>>>>>>> 15491401a593fcf951f6b05586a0e95e9b6d9d1a
=======
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "QRingBuffer.h"
#include <QTimer>
#include <QMutex>
#include <ahg.h>
#include <clsTransSmartBus.h>
#include <clsTransRSP.h>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sldAirTemp_valueChanged(int value);

    void on_sldAirHumid_valueChanged(int value);

    void on_sldSoilTemp_valueChanged(int value);

    void on_sldSoilMoisture_valueChanged(int value);

    void on_sldLightIntensity_valueChanged(int value);

    void on_sldWaterEC_valueChanged(int value);

    void on_sldWaterPH_valueChanged(int value);

    void on_sldWaterLevel_valueChanged(int value);

    void on_sldSoilEC_valueChanged(int value);

    void on_sldSoidPH_valueChanged(int value);

    void on_sldWaterTemp_valueChanged(int value);

    void on_btnRefreshPorts_clicked();

    void on_SerialportDataReceived();

    void on_txtSensorAddress_textChanged();

    void on_btnStartSimulation_clicked();

    void onTimerTicked();


    void on_btnGenerate_clicked();

    void on_btnSBusGenFrame_clicked();

    void on_btnModbusFrameGen_clicked();

    void on_btt_readsensorvalue_clicked();


    void on_btt_readRTC_clicked();

    void on_btt_writrRTC_clicked();

    void on_btt_change_slaveAdd_clicked();
    void ShowTime();

    void on_btt_writrRTC_date_clicked();

    void on_btt_readRTC_date_clicked();

    void on_btt_getsensor_on_port_clicked();

    void on_btt_Read_offset_clicked();

    void on_btt_Read_SlaveAdd_clicked();

    void on_btt_Write_offset_clicked();

    void on_btt_setsensor_onport_clicked();

private:
    Ui::MainWindow *ui;
    QMutex          *pRxBuffMutex;
    QRingBuffer     *pRxBuffer;
    QSerialPort     *pSerialPort;
    QTimer         *pTimer;
    uint16_t        m_u16SensorID = 0;
    uint16_t        m_u16SensorIDchange = 0;
    bool            m_bRunning = false;
    QString         time_text;
    QString         date_text ;
    void RunSimulation(bool bIsterminated);

    void ProcessRawData();

    void ProcessRSPFrame(void *pvFrame);

    void ProcessRSPFrameReaddata(void *pvFrame);
    void ProcessRSPFrameReadRTCTime(void *pvFrame);
    void ProcessRSPFrameWriteRTCTime(void *pvFrame);
    void ProcessRSPFrameReadRTCDate(void *pvFrame);
    void ProcessRSPFrameWriteRTCDate(void *pvFrame);
    void ProcessRSPFrameChangeAddress(void *pvFrame);
    void ProcessRSPFrameGetSlaveAddress(void *pvFrame);
    void LogToConsole(const char* string, ...);
    void LogToConsoleTime(const char* string, ...);

};

#endif // MAINWINDOW_H
>>>>>>> 15491401a593fcf951f6b05586a0e95e9b6d9d1a
