#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "utilityprograms.hh"

#include <QLabel>
#include <QVariantList>
#include <QJsonArray>
#include <QMap>
#include <utility>
#include <QListWidgetItem>


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
        my_program::Stream temp_holder(followed_channel["channel"].toObject());
        followed_stream_data_.push_back(std::move(temp_holder));
        followed_online_status_[name.toString()] = false;
    }
    check_channel_online_status(); // Tässä sitten lisätään sinne se online/offline-status.

    for ( auto channel : follows_array_qjson ) {
        QJsonObject followed_channel = channel.toObject();
        QString channel_name = followed_channel["channel"].toObject().value("name").toString();

        QLabel *label_stacked = new QLabel(channel_name);
        QLabel *label_list = new QLabel(channel_name);

        QWidget *widget = new QWidget;
        QListWidgetItem *list_item = new QListWidgetItem();

        // Käytä setItemWidgettiä, että voit laittaa vaikkapa labelin itemiksi.
        // -------------------------------------------------------------------
        // Widget on kuin säiliö johon tulee layout
        // layout on taas säiliö esim. labeleille.
        // Eli labelit layouttiin ja sitten layout widgettiin.

        label_list->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
        label_list->setAlignment(Qt::AlignBottom | Qt::AlignRight);
        label_list->setFixedSize(140, 25);

        if ( followed_online_status_[channel_name] == true ) {
            //label_stacked->setStyleSheet("QLabel { background-color: #28385e; color: #304163; }");
            label_list->setStyleSheet("QLabel { background-color: #e3f2fd; color: #304163; padding: 1px; }");
            //widget->setStyleSheet("QLabel { background-color: #e3f2fd; color: #304163; }");
            qDebug() << "Channel label online: " << channel_name;

        } else if ( followed_online_status_[channel_name] == false ) {
            //label_stacked->setStyleSheet("QLabel { background-color: #516C8D; color: #DDDDDD; }");
            label_list->setStyleSheet("QLabel { background-color: #F44336; color: #DDDDDD; padding: 1px; }");
            //widget->setStyleSheet("QLabel { background-color: #F44336; color: #DDDDDD; }");
            qDebug() << "Channel label offline: " << channel_name;
        }


        /*
        //list_item->setData(Qt::UserRole, 1);
        //list_item->setTextAlignment(Qt::AlignBottom | Qt::AlignRight);
        //list_item->setSizeHint(QSize(list_item->sizeHint().height(), 25));
        //list_item->setSizeHint(QSize(list_item->sizeHint().width(), 150));

        // background: #516C8D
        // font: #DDDDDD
        // list_item->setTextColor(QColor::fromRgba(QRgb("#DDDDDD")));
        // list_item->setBackgroundColor(QColor::fromRgba(QRgb("#516C8D")));

        */
        // QListWidgetItem:
        // Gridlayout koska se toimii erilailla.
        QGridLayout *grid_layout = new QGridLayout;
        grid_layout->setMargin(0);

        grid_layout->addWidget(label_list);
        widget->setLayout(grid_layout);

        list_item->setSizeHint(QSize(140, 25));


        ui->follow_list->addItem(list_item);
        ui->follow_list->setItemWidget(list_item, widget);

        // StackedWidget page:
        QWidget *temp_page = new QWidget;
        QHBoxLayout *layout_base_hbox = new QHBoxLayout;
        layout_base_hbox->setMargin(1);

        // Left side of stacked page:
        QVBoxLayout *layout_left_vbox = new QVBoxLayout;
        // Right side of stacked page:
        QVBoxLayout *layout_right_vbox = new QVBoxLayout;

        // Logo, display_name, game, created_at, viewers, followers, url
        QLabel *logo = new QLabel("logo:");

        QLabel *display_name = new QLabel("display_name:");
        QLabel *game = new QLabel("game:");
        QLabel *created_at_time = new QLabel("created_at_time:");
        QLabel *viewers = new QLabel("viewers:");
        QLabel *followers = new QLabel("followers:");
        QLabel *url_to_stream = new QLabel("url:");

        // big preview, status
        QLabel *preview_picture = new QLabel("preview_picture:");
        QLabel *status = new QLabel("status:");

        logo->setFixedSize(150, 150);
        display_name->setFixedSize(150, 25);
        game->setFixedSize(150, 25);
        created_at_time->setFixedSize(150, 25);
        viewers->setFixedSize(150, 25);
        followers->setFixedSize(150, 25);
        url_to_stream->setFixedSize(150, 25);

        preview_picture->setFixedSize(500, 300);
        status->setFixedSize(500, 25);

        layout_left_vbox->addWidget(logo, 0, Qt::AlignTop);
        layout_left_vbox->addWidget(display_name, 0, Qt::AlignTop);
        layout_left_vbox->addWidget(game, 0, Qt::AlignTop);
        layout_left_vbox->addWidget(created_at_time, 0, Qt::AlignTop);
        layout_left_vbox->addWidget(viewers, 0, Qt::AlignTop);
        layout_left_vbox->addWidget(followers, 0, Qt::AlignTop);
        layout_left_vbox->addWidget(url_to_stream, 0, Qt::AlignTop);
        layout_left_vbox->addStretch();
        layout_left_vbox->setMargin(1);
        layout_base_hbox->setSpacing(0);
        //layout_base_hbox->setMargin(0);
        layout_right_vbox->addWidget(preview_picture, 0, Qt::AlignTop);
        layout_right_vbox->addWidget(status, 0, Qt::AlignTop);
        layout_right_vbox->addStretch();
        layout_right_vbox->setMargin(1);


        layout_base_hbox->addLayout(layout_left_vbox);
        layout_base_hbox->addLayout(layout_right_vbox);

        //layout_base_hbox->addWidget(label_stacked);
        //temp_page->setFixedHeight(325);

        temp_page->setLayout(layout_base_hbox);


        ui->stackedWidget->addWidget(temp_page);
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
