#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "utilityprograms.hh"

#include <QLabel>
#include <QVariantList>
#include <QJsonArray>
#include <QMap>

const QString CLIENTID = "kotialthf6zsygxpvqfhgbf0wvblsv5";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&data_retriever_, SIGNAL(data_ready_read(QByteArray)), this,
            SLOT(data_retrieved(QByteArray)));
    settings_ = std::unique_ptr<my_program::Settings>(new my_program::Settings());
    update_settings();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::data_retrieved(QByteArray data) {

    qDebug() << "Data_retrieved.";
    //QJsonObject temp_json;
    //temp_json = Apuohjelmat::parse_json_data(data);
    //qDebug() << temp_json;
    //Apuohjelmat::write_json_to_file(temp_json);
}

void MainWindow::on_follows_make_request_clicked() {
    data_retriever_.make_request("https://api.twitch.tv/kraken/users/rutle/follows/channels?client_id=kotialthf6zsygxpvqfhgbf0wvblsv5");
    json_data_follows_ = data_retriever_.retrieve_json_data();
    qDebug() << "on_follows_make_request data retrieved.";

    QJsonValue follows_value = json_data_follows_.value("follows");
    QJsonArray follows_array_qjson = follows_value.toArray();

    for ( auto item :  follows_array_qjson ) {
        QJsonObject followed_channel = item.toObject();
        QJsonValue name = followed_channel["channel"].toObject().value("name");
        followed_online_status_[name.toString()] = false;
    }
    check_channel_online_status();

    for ( auto channel : follows_array_qjson ) {
        QJsonObject followed_channel = channel.toObject();
        QString channel_name = followed_channel["channel"].toObject().value("name").toString();
        QLabel *label = new QLabel(this);
        label->setText(channel_name);
        label->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
        label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
        label->setMinimumHeight(20);
        label->setMaximumWidth(120);
        //label->setLineWidth(2);

        if ( followed_online_status_[channel_name] == true ) {
            label->setStyleSheet("QLabel { background-color : #e3f2fd; color : black; }");
            qDebug() << "Channel label online: " << channel_name;
        } else if ( followed_online_status_[channel_name] == false ) {
            label->setStyleSheet("QLabel { background-color : #F44336; color : black; }");
            qDebug() << "Channel label offline: " << channel_name;
        }
        ui->follows_vert_list->addWidget(label);
    }

}

void MainWindow::check_channel_online_status() {
    QString channels_string;
    QMap<QString, bool>::iterator qmap_iter;

    for ( qmap_iter = followed_online_status_.begin();
          qmap_iter != followed_online_status_.end(); qmap_iter++ ) {
        channels_string += qmap_iter.key()+",";
    }
    channels_string = channels_string.left(channels_string.size() - 1);

    QString url("https://api.twitch.tv/kraken/streams?channel="+channels_string);
    data_retriever_.make_request(url);
    json_data_on_followed_channels_ = data_retriever_.retrieve_json_data();
    qDebug() << "channel_online_status data retrieved.";

    QJsonValue streams_value = json_data_on_followed_channels_.value("streams");
    QJsonArray streams_array_qjson = streams_value.toArray();

    if ( json_data_on_followed_channels_["_total"] == 0 ) {
        qDebug() << "All channels offline";
        return;
    }
    for ( auto channel : streams_array_qjson ) {
        QJsonObject stream_object{channel.toObject()};
        QJsonValue stream_name{stream_object["channel"].toObject().value("name")};
        followed_online_status_[stream_name.toString()] = true;
        qDebug() << "Channel: " << stream_name.toString();
    }

}

void MainWindow::update_settings() {

    ui->channelNameLineEdit->setText(settings_->give_user_name());
    qDebug() << "Update settings.";
    qDebug() << "User name: " << settings_->give_user_name();

}

void MainWindow::on_save_settings_button_clicked() {
    if ( ui->channelNameLineEdit->text().isEmpty() ) {
        qWarning() << "Empty user name LineEdit!";
    }
    settings_->set_user_name(ui->channelNameLineEdit->text());
    qDebug() << "Save settings.";
    qDebug() << "User name: " << ui->channelNameLineEdit->text();
    settings_->save_to_file();

}
