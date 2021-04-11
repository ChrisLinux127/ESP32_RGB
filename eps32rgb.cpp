#include "eps32rgb.h"
#include "ui_eps32rgb.h"


//Konstruktor:
Eps32RGB::Eps32RGB(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Eps32RGB)
{
    ui->setupUi(this); //Erzeugt Fenster
    //Serielle Kommunikation Setup:
    Controller_PortAvailable=false;
    Controller_PortName="";
    Controller=new QSerialPort;
}
//Destruktor:
Eps32RGB::~Eps32RGB()
{
    //Close Connection of UART:
    if(Controller->isOpen())
    {
        Controller->close();
    }
    delete ui;
}


void Eps32RGB::on_btn_Connect_clicked()
{
    //Connect to Controller:
    //Portsfinden und zuweisen in foreach schleife:
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier()&&serialPortInfo.hasProductIdentifier())
        {
            if(serialPortInfo.vendorIdentifier()==Controller_Vendor_ID)
            {
                if(serialPortInfo.productIdentifier()==Controller_Product_ID)
                {
                    //Erst wenn Kontroller gefunden wurde, dann zuweisen:
                    Controller_PortName=serialPortInfo.portName();
                    Controller_PortAvailable=true;
                }
            }
        }
    }
    if(Controller_PortAvailable)
    {
        //Konfiguration of UART
        //Öffnen und Einstellen des Seriellen Ports:
        Controller->setPortName(Controller_PortName);   //Namen zuweisen
        Controller->open(QSerialPort::ReadWrite); //Bidirektionale Kommunikation
        Controller->setBaudRate(QSerialPort::Baud9600); //9600 Boud
        Controller->setDataBits(QSerialPort::Data8);    //8 Bits Übertragen
        Controller->setParity(QSerialPort::NoParity); //Kein Paritätsbit
        Controller->setStopBits(QSerialPort::OneStop);  //1 Stopbit
        Controller->setFlowControl(QSerialPort::NoFlowControl);     //??
        ui->btn_connection_state->setText("Erfolgreich Vebunden!");
        ui->lst_Output->addItem("Erfolgreich Verbundnen!");
        ui->btn_Connect->setDisabled(true);
        ui->btn_Disconnect->setEnabled(true);
    }
    else
    {
        //Fehler ausgeben:
        QMessageBox::warning(this, "Vebindung Fehlgeschlagen!","ESP32 konnte nicht gefunden werden!");
        ui->btn_connection_state->setText("Verbindung Fehlgeschlagen...");
    }
}

void Eps32RGB::on_btn_Disconnect_clicked()
{
    if(Controller->isWritable())
    {
        QString str="k";
        Controller->write(str.toStdString().c_str()); //Umwandlung von QString in Ascii Zeichen
    }
    //Close Connection of UART:
    if(Controller->isOpen())
    {
        ui->btn_connection_state->setText("Verbindung wurde getrennt...");
        ui->lst_Output->addItem("Vebindung zum Kontroller vom User entfernt.");
        Controller->close();
        ui->btn_Disconnect->setDisabled(true);
        ui->btn_Connect->setEnabled(true);
    }
}

void Eps32RGB::UpdateLed(QString command)
{
    if(Controller->isWritable())
    {
       Controller->write(command.toStdString().c_str());
    }
    else
    {
       qDebug()<<"Uebertragung Fehlgeschlagen!\n";
    }
}

void Eps32RGB::on_Sl_Red_valueChanged(int value)
{
    ui->L_Red->setText(QString("<p><span style=\" font-size:24pt; font-weight:600;\">%1</span></p>").arg(value));
    Eps32RGB::UpdateLed(QString("r%1").arg(value));
    if(Controller->isReadable())
    {
        if(ui->lst_Output->count()==10)
        {
            ui->lst_Output->clear();
        }
       QByteArray Data = Controller->readLine();    //Lese Byte ein
       ui->lst_Output->addItem(QTextCodec::codecForMib(106)->toUnicode(Data));  //Umwandlung von Byte in QString (Unicode UTF8=106 mit Codiererklasse)
    }

}

void Eps32RGB::on_Sl_Green_valueChanged(int value)
{
    ui->L_Green->setText(QString("<p><span style=\" font-size:24pt; font-weight:600;\">%1</span></p>").arg(value));
    Eps32RGB::UpdateLed(QString("g%1").arg(value));
}

void Eps32RGB::on_Sl_Blue_valueChanged(int value)
{
    ui->L_Blue->setText(QString("<p><span style=\" font-size:24pt; font-weight:600;\">%1</span></p>").arg(value));
    Eps32RGB::UpdateLed(QString("b%1").arg(value));
}
