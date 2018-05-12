#ifndef CSENSORSIMULATOR_H
#define CSENSORSIMULATOR_H

#include <QApplication>
#include <QObject>
#include <QCheckBox>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QDebug>
#include <QProgressBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QLabel>
#include <QTextEdit>
#include <QThread>


class CSensorSimulator : public QWidget
{
    Q_OBJECT
public:
     explicit CSensorSimulator(QWidget *parent = 0);
    ~CSensorSimulator();

private:
    quint32     m_u32SensorID;

    QLabel *    m_pAirTempLabel;
    QSlider*    m_pAirTempSlider;
    QCheckBox*  m_pAirTempCheckBox;

    QLabel *    m_pAirHumidLabel;
    QSlider*    m_pAirHumidSlider;
    QCheckBox*  m_pAirHumidCheckBox;

    QLabel *    m_pSoilTempLabel;
    QSlider*    m_pSoilTempSlider;
    QCheckBox*  m_pSoilTempCheckBox;

    QLabel *    m_pSoilMoistureLabel;
    QSlider*    m_pSoilMoistureSlider;
    QCheckBox*  m_pSoilMoistureCheckBox;


    QLineEdit* pFileNameLineEdit;
    QLineEdit* pServerNameEdit;
    QLineEdit* pServerPortEdit;

    QPushButton* pBrowseButton;
    QPushButton* pProgButton;
    QProgressBar *pProgressBar;
    QLabel *pTransferSpeed;
    QLabel *pLabel;

    void InitializeLayout(void);

    QThread* pThread;
    volatile bool isProgramming;
    QString path;
    QTime m_Time;
private slots:
    void setFileName(QString strName) {m_szFileName = strName;}
    void setServerName(QString strName){m_szServerName = strName;}
    void setServerPort(QString szPortNumber);
    void setTranferSpeed(float fSpeed);
    void on_BrowseButtonClicked();
    void on_ProgFinished(int);
    void on_CurrentTabChanged(int);
    void displayErrorMessage(QString str);
public slots:
    /**
     * @brief on_ProgButtonClicked to programing
     */
    void on_ProgButtonClicked();


};

#endif // CSENSORSIMULATOR_H
