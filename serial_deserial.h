#ifndef SERIAL_DESERIAL_H
#define SERIAL_DESERIAL_H
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class Serial_Deserial
{
public:
    Serial_Deserial();

    //Json Struct to pass the variables from UI
    struct Player_Config{
        QString source;
        QString reference;
        QString target;
        QString com_port_;
        QString com_baudrate;
        QString adsb_host;
        qint16 adsb_port;
        qint16 telemetry_timeout;
        bool telemetry_kalman;
        bool radar_kalman;
        bool udp;
    };

    void Json_Read(QString const path,Serial_Deserial::Player_Config player_params_,QString const new_path){
        QFile file(path);
        //To Read JsonFiles
        if(file.open(QIODevice::ReadOnly)){
            QJsonParseError JsonParseError;
            QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(),&JsonParseError);
            file.close();

            //Create JsonObjects or Arrays to modify
            QJsonObject player_json=JsonDocument.object();
            QJsonObject external_data=(player_json.value("external_data")).toObject();

            QJsonValue reference=external_data.value("reference");
            QJsonValue source=external_data.value("source");

            QJsonObject adsb=(external_data.value("adsb").toObject());
            QJsonObject airnav(adsb.value("airnav").toObject());

            QJsonValue host=airnav.value("host");
            QJsonValue port=airnav.value("port");

            QJsonObject radar=(external_data.value("radar")).toObject();
            QJsonValue r_kalman_filter=radar.value("filter");

            QJsonObject telemetry=(external_data.value("telemetry")).toObject();
            QJsonValue t_kalman_filter=telemetry.value("filter");
            QJsonValue t_time_out=telemetry.value("timeout");

            if(player_params_.source == "adsb"){
                host=player_params_.adsb_host;
                port=player_params_.adsb_port;
                airnav["port"]=port;
                airnav["host"]=host;
                adsb["airnav"]=airnav;
                external_data["adsb"]=adsb;

            }
            if(player_params_.source == "telemetry"){
                qDebug()<<"telemetry is selected";
                t_kalman_filter=player_params_.telemetry_kalman;
                t_time_out=player_params_.telemetry_timeout;

                QJsonArray comm=telemetry["comm"].toArray();
                [player_params_,&comm]{
                    if(player_params_.udp)
                    {
                    if(comm.count()>2) {comm.pop_back();}
                    comm[0]="udp";
                    comm[1]=player_params_.com_port_;
                       return;
                    }
                    if(comm.count()<3){comm.append(player_params_.com_baudrate);}
                    comm[0]="serial";
                    comm[1]=player_params_.com_port_;
                    comm[2]=player_params_.com_baudrate;
                    return;
                         }();

                telemetry["comm"]=comm;
                telemetry["filter"]=t_kalman_filter;
                telemetry["timeout"]=t_time_out;
                external_data["telemetry"]=telemetry;
            } 
            if(player_params_.source == "radar"){
                qDebug()<<"radar is selected";
                r_kalman_filter=player_params_.radar_kalman;
                radar["use_kalman"]=r_kalman_filter;
                external_data["radar"]=radar;
            }
            if(player_params_.source == "none"){
                qDebug()<<"none is selected";
            }

            //modify JsonArray inorder to have udp or serial type
            //["udp","port"] - ["serial","port","baudrate"]


            //Assign to modified values to Json variables
            reference=player_params_.reference;
            external_data["reference"]=reference;

            source=player_params_.source;
            external_data["source"]=source;

            player_json["external_data"]=external_data;
            JsonDocument.setObject(player_json);

            //Write to json files or create new ones
            QFile file(new_path);
            file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            file.write(JsonDocument.toJson());
            file.close();
        }

    }

private:

};

#endif // SERIAL_DESERIAL_H
