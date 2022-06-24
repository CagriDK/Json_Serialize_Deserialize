#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <serial_deserial.h>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    std::unique_ptr<Serial_Deserial> player;
    Serial_Deserial::Player_Config player_params_;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString pack(QStringList const& list){
        return list.join(",");
    }
private slots:
    //Select Json File to Read
    void Open_File_Explorer(){
        QString path=QDir::currentPath();
        QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),path,tr("Json Files (*.json)"));
        ui->lineEdit->setText(pack(fileNames));
        jsonpath=pack(fileNames);
    }

    void Source_Field_Items(){

        if(ui->comboBox->currentText() =="adsb")
        {
            ui->adsb_host_text->show();
            ui->adsb_host_label->show();
            ui->adsb_port_text->show();
            ui->adsb_port_label->show();

            ui->checkBox_3->hide();

            ui->telemetry_kalman_checkBox->hide();
            ui->telemetry_timeout_label->hide();
            ui->telemetry_timeout_text->hide();
        }

        else if(ui->comboBox->currentText()== "telemetry")
        {
            ui->adsb_host_text->hide();
            ui->adsb_host_label->hide();
            ui->adsb_port_text->hide();
            ui->adsb_port_label->hide();

            ui->checkBox_3->hide();

            ui->telemetry_kalman_checkBox->show();
            ui->telemetry_timeout_label->show();
            ui->telemetry_timeout_text->show();
        }

        else if(ui->comboBox->currentText()== "radar")
        {
            ui->adsb_host_text->hide();
            ui->adsb_host_label->hide();
            ui->adsb_port_text->hide();
            ui->adsb_port_label->hide();

            ui->checkBox_3->show();

            ui->telemetry_kalman_checkBox->hide();
            ui->telemetry_timeout_label->hide();
            ui->telemetry_timeout_text->hide();
        }

        else if(ui->comboBox->currentText()== "none")
        {
            ui->adsb_host_text->hide();
            ui->adsb_host_label->hide();
            ui->adsb_port_text->hide();
            ui->adsb_port_label->hide();

            ui->checkBox_3->hide();

            ui->telemetry_kalman_checkBox->hide();
            ui->telemetry_timeout_label->hide();
            ui->telemetry_timeout_text->hide();
        }

    }


    void select_com(bool com){
        if(com){
            player_params_.udp=true;
            player_params_.com_port_=ui->lineEdit_2->text();
            qDebug("UDP Selected");
            return;
        }
        player_params_.udp=false;
        player_params_.com_port_=ui->lineEdit_3->text();
        player_params_.com_baudrate=ui->lineEdit_4->text();
        qDebug("Serial Selected");
    }

    void Json_Read(){

        QMessageBox msg_Warning;
        msg_Warning.setIcon(QMessageBox::Information);
        msg_Warning.setWindowTitle("Warning Message");

        new_jsonpath=ui->lineEdit->text();

        if(ui->lineEdit->text()==""){
            msg_Warning.setText("File Not Chosen from File Explorer");
            msg_Warning.exec();
            return;
        }
        if((ui->checkBox->isChecked() || ui->checkBox_2->isChecked()) == 0  ){
            msg_Warning.setText("Neither UDP or Serial Telemetry option is Selected");
            msg_Warning.exec();
            return;
        }

        select_com((ui->checkBox->isChecked()==0) ? 0 : 1);
        player_params_.source=ui->comboBox->currentText();
        player_params_.reference=ui->comboBox_2->currentText();
        player_params_.target=ui->comboBox_3->currentText();
        player_params_.adsb_host=ui->adsb_host_text->text();
        player_params_.adsb_port=ui->adsb_port_text->text().toInt();
        player_params_.telemetry_kalman=(ui->telemetry_kalman_checkBox->isChecked()==0) ? false:true;
        player_params_.telemetry_timeout=ui->telemetry_timeout_text->text().toInt();
        player_params_.radar_kalman=(ui->checkBox_3->isChecked()==0) ? false:true;
        player->Json_Read(jsonpath,player_params_,new_jsonpath);
    }

private:
    Ui::MainWindow *ui;
    QString jsonpath;
    QString new_jsonpath;
    bool com;
};
#endif // MAINWINDOW_H
