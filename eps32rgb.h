#ifndef EPS32RGB_H
#define EPS32RGB_H

#include <QMainWindow>
#include<QSerialPort>   //für Serielle Kommunikation
#include<QSerialPortInfo>   //Für Infos von Seriellen Kommunikation
#include<QDebug>    //Für Ausgaben in Debuger
#include<QtWidgets> //Für Warnungen (QMessageBox)

QT_BEGIN_NAMESPACE
namespace Ui { class Eps32RGB; }
QT_END_NAMESPACE

class Eps32RGB : public QMainWindow
{
    Q_OBJECT

public:
    Eps32RGB(QWidget *parent = nullptr);
    ~Eps32RGB();

private slots:
    void on_btn_Connect_clicked();

    void on_btn_Disconnect_clicked();

    void on_Sl_Red_valueChanged(int value);

    void on_Sl_Green_valueChanged(int value);

    void on_Sl_Blue_valueChanged(int value);

private:
    Ui::Eps32RGB *ui;   //Pointer auf Gui von Formual.ui
    QSerialPort *Controller; //Obj für Kommunikation
    static const quint16 Controller_Vendor_ID=4292;  //Vendor ID Wenn nicht bekannt dann folgendes: 1*
    static const quint16 Controller_Product_ID=60000; //Produkt ID
    QString Controller_PortName;    //Port of Controller
    bool Controller_PortAvailable;  //Connection State
    void UpdateLed(QString Command);    //Update Methode
};
#endif // EPS32RGB_H

//**** 1* Hilfe: ****
//Produkt info und Vendor ID rausfinden:
/*
qDebug()<<"Number of ports:"<<QSerialPortInfo::availablePorts().length();
foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
    qDebug()<<"Vendor ID: "<<serialPortInfo.hasVendorIdentifier();
    if(serialPortInfo.hasVendorIdentifier())
    {
        qDebug()<<"Vendor ID: "<<serialPortInfo.vendorIdentifier();
    }
    qDebug()<<"Product ID: "<<serialPortInfo.hasProductIdentifier();
    if(serialPortInfo.hasProductIdentifier())
    {
        qDebug()<<"Produkt ID: "<<serialPortInfo.productIdentifier();
    }
}
*/
