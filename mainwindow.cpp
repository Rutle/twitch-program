#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "utilityprograms.hh"

#include <QLabel>
#include <QVariantList>
#include <QJsonArray>
#include <QMap>
#include <utility>
#include <QListWidgetItem>
#include <QDateTime>
#include <QDir>

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

    ui->follow_list->setFocusPolicy(Qt::NoFocus);
    QDir dir(QDir::current());
    if ( !dir.mkdir("user_pictures")) {
        qDebug() << "Directory already exists!";
    }

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
    ui->follows_make_request->setDisabled(true);
    data_retriever_.make_request("https://api.twitch.tv/kraken/users/rutle/follows/channels?client_id=kotialthf6zsygxpvqfhgbf0wvblsv5");
    json_data_follows_ = data_retriever_.retrieve_json_data();
    qDebug() << "on_follows_make_request data retrieved.";

    QJsonValue follows_value = json_data_follows_.value("follows");
    QJsonArray follows_array_qjson = follows_value.toArray();
    // Array of QJsonObjects where each one is a channel QJsonObject.
    for ( auto item :  follows_array_qjson ) {
        QJsonObject followed_channel = item.toObject();
        QJsonValue name = followed_channel["channel"].toObject().value("name");
        my_program::Stream temp_holder(followed_channel["channel"].toObject());
        followed_stream_data_.push_back(std::move(temp_holder));
        followed_online_status_[name.toString()] = false;
    }
    // Online/Offline status to streams in followed_online_status;
    check_channel_online_status();

    // for ( auto channel : follows_array_qjson ) {
    for ( auto channel : followed_stream_data_ ) {
        //QJsonObject followed_channel = channel.toObject();
        //QString channel_name = followed_channel["channel"].toObject().value("name").toString();

        // Widget to put into a QListWidgetItem:
        QWidget *widget = build_qlistwidgetitem(channel);
        QListWidgetItem *list_item = new QListWidgetItem();
        list_item->setSizeHint(QSize(140, 21));
        ui->follow_list->addItem(list_item);
        ui->follow_list->setItemWidget(list_item, widget);

        // StackedWidget page:
        QWidget *temp_page = build_channel_info_page(channel);

        ui->stackedWidget->addWidget(temp_page);
    }
    ui->follows_make_request->setDisabled(false);

}

void MainWindow::check_channel_online_status() {
    QString channels_string;
    QMap<QString, bool>::iterator qmap_iter;

    // String to add to url so that multiple channels can be looked up with
    // single network request.
    for ( qmap_iter = followed_online_status_.begin();
          qmap_iter != followed_online_status_.end(); qmap_iter++ ) {
        channels_string += qmap_iter.key()+",";
    }
    channels_string = channels_string.left(channels_string.size() - 1);

    QString url("https://api.twitch.tv/kraken/streams?channel="+channels_string);
    data_retriever_.make_request(url);
    json_data_on_followed_channels_ = data_retriever_.retrieve_json_data();
    qDebug() << "channel_online_status: data retrieved.";

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
        for ( auto stream : followed_stream_data_ ) {
            if (stream.get_channel_name() == stream_name.toString() ) {
                stream.set_stream_details(stream_object);
                qDebug() << "Channel [" << stream_name.toString() << "] online!";
            }
        }

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

QWidget* MainWindow::build_qlistwidgetitem(const my_program::Stream &stream) {
    //QString channel_name = followed_channel["channel"].toObject().value("name").toString();
    QString channel_name = stream.get_channel_name();
    QWidget *widget = new QWidget();
    QLabel *label_list = new QLabel(channel_name);
    QLabel *online_status = new QLabel();

    label_list->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    label_list->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    label_list->setFixedSize(135, 20);
    online_status->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    online_status->setFixedSize(5, 20);

    if ( followed_online_status_[channel_name] == true ) {
        //label_stacked->setStyleSheet("QLabel { background-color: #28385e; color: #304163; }");
        label_list->setStyleSheet("QLabel { background-color: #28385e; color: #dddddd; padding: 1px; font: bold 10px; }");
        online_status->setStyleSheet("QLabel { background-color: #4CAF50; }");

        qDebug() << "Channel label online: " << channel_name;

    } else if ( followed_online_status_[channel_name] == false ) {
        //label_stacked->setStyleSheet("QLabel { background-color: #516C8D; color: #DDDDDD; }");
        label_list->setStyleSheet("QLabel { background-color: #28385e; color: #DDDDDD; padding: 1px; font: bold 10px; }");
        online_status->setStyleSheet("QLabel { background-color: #FF5722; }");

        qDebug() << "Channel label offline: " << channel_name;
    }

    // QListWidgetItem:
    QGridLayout *grid_layout = new QGridLayout;
    grid_layout->setMargin(0);
    grid_layout->setSpacing(0);
    grid_layout->addWidget(online_status, 0, 0);
    grid_layout->addWidget(label_list, 0, 1);
    widget->setLayout(grid_layout);

    return widget;

}

QWidget* MainWindow::build_channel_info_page(const my_program::Stream &stream) {
    QWidget *temp_page = new QWidget;
    QHBoxLayout *layout_base_hbox = new QHBoxLayout;
    layout_base_hbox->setContentsMargins(5, 0, 0, 0);

    // Left side of stacked page:
    QVBoxLayout *layout_left_vbox = new QVBoxLayout;
    // Right side of stacked page:
    QVBoxLayout *layout_right_vbox = new QVBoxLayout;
    // Logo, display_name, game, created_at, viewers, followers, url
    QLabel *logo = new QLabel();
    logo->setPixmap(QPixmap::fromImage(stream.get_logo()).scaled(QSize(150,150)));

    QLabel *display_name = new QLabel(stream.get_data_value(QStringLiteral("display_name")));
    QLabel *game = new QLabel(stream.get_game());
    // updated_at": "2016-06-27T13:03:28Z",
    QLabel *created_at_time = new QLabel();

    if ( stream.is_online() ) {
        QDateTime stream_start = QDateTime::fromString(stream.get_stream_start(), "yyyy'-'MM'-'dd'T'hh:mm:ss'Z'");
        qDebug() << "QString: " << stream.get_stream_start();
        qDebug() << "QDateTime.toString(): " << stream_start.toString();
        created_at_time->setText(stream_start.time().toString());
    } else {

    }


    //qDebug() << QString::number(stream.get_viewers());
    QLabel *viewers = new QLabel();
    if ( stream.get_viewers() != 0 ) {
        viewers->setText(QString::number(stream.get_viewers()));
    } else {
        viewers->setText(QStringLiteral("Offline"));
    }
    //qDebug() << QString::number(stream.get_followers());
    QLabel *followers = new QLabel(QString::number(stream.get_followers()));
    QLabel *url_to_stream = new QLabel(stream.get_url_value("url").toString());

    // big preview, status
    QLabel *preview_picture = new QLabel("preview_picture:");
    QLabel *status = new QLabel(stream.get_data_value("status"));

    logo->setFixedSize(150, 150);
    display_name->setFixedSize(150, 20);
    game->setFixedSize(150, 20);
    created_at_time->setFixedSize(150, 20);
    viewers->setFixedSize(150, 20);
    followers->setFixedSize(150, 20);
    url_to_stream->setFixedSize(150, 20);

    preview_picture->setFixedSize(500, 300);
    status->setFixedSize(500, 20);

    layout_left_vbox->addWidget(logo, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(display_name, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(game, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(created_at_time, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(viewers, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(followers, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(url_to_stream, 0, Qt::AlignTop);
    layout_left_vbox->addStretch();
    layout_left_vbox->setMargin(0);
    layout_base_hbox->setSpacing(0);
    //layout_base_hbox->setMargin(0);
    layout_right_vbox->addWidget(preview_picture, 0, Qt::AlignTop);
    layout_right_vbox->addWidget(status, 0, Qt::AlignTop);
    layout_right_vbox->addStretch();
    layout_right_vbox->setMargin(0);


    layout_base_hbox->addLayout(layout_left_vbox);
    layout_base_hbox->addLayout(layout_right_vbox);

    temp_page->setLayout(layout_base_hbox);
    return temp_page;
}
