#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "utilityprograms.hh"
#include "channelinfo.hh"
#include "topgameslistmodel.hh"
#include "topgameslistdelegate.hh"
#include "miniinfo.hh"

#include <QLabel>
#include <QJsonArray>
#include <QMap>
#include <utility>
#include <QListWidgetItem>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QScrollArea>


const QString CLIENTID = "?client_id=kotialthf6zsygxpvqfhgbf0wvblsv5";
const QString API_URL = "https://api.twitch.tv/kraken/";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    /*
    connect(&data_retriever_, SIGNAL(data_ready_read(QByteArray)), this,
            SLOT(data_retrieved(QByteArray)));
    */
    settings_ = std::unique_ptr<my_program::Settings>(new my_program::Settings());

    update_settings();

    ui->follow_list->setFocusPolicy(Qt::NoFocus);
    QDir dir(QDir::current());
    if ( !dir.mkdir("user_pictures")) {
        qDebug() << "Directory already exists!";
    }

    // Follows update- and clear-buttons disabled at the start because the
    // list is empty.
    ui->clear_follows->setDisabled(true);
    ui->update_follows->setDisabled(true);

    my_program::Channelinfo *base_page = new my_program::Channelinfo();
    ui->search_stacked_widget->setContentsMargins(0, 0, 0, 0);
    ui->search_stacked_widget->addWidget(base_page);
    ui->main_top_games_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
void MainWindow::data_retrieved(QByteArray data) {

    qDebug() << "Data_retrieved [Obsolete slot]";
}
*/
void MainWindow::on_fetch_follows_clicked() {
    if ( ui->channelNameLineEdit->text().isEmpty() ) {
        qDebug() << "Username is empty! ["
                 << ui->channelNameLineEdit->text() << "]";
        return;
    }

    ui->fetch_follows->setDisabled(true);
    ui->update_follows->setDisabled(true);
    ui->clear_follows->setDisabled(true);

    QString username{ui->channelNameLineEdit->text()};
    QString request_url{API_URL+"users/"+username+"/follows/channels"+CLIENTID};

    data_retriever_.make_api_request(request_url);
    QJsonObject follows_json_data{data_retriever_.retrieve_json_data()};
    build_follows_page(follows_json_data);

    ui->fetch_follows->setDisabled(false);
    ui->update_follows->setDisabled(false);
    ui->clear_follows->setDisabled(false);

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

    QString url("https://api.twitch.tv/kraken/streams?channel="+channels_string+CLIENTID);
    data_retriever_.make_api_request(url);
    QJsonObject streams_online_json_data{data_retriever_.retrieve_json_data()};
    qDebug() << "channel_online_status: data retrieved.";
    QJsonValue streams_value{streams_online_json_data.value("streams")};

    qDebug() << streams_online_json_data["_total"];
    if ( streams_online_json_data["_total"] == 0 ) {
        qDebug() << "All channels offline";
        return;
    }
    for ( auto channel : streams_value.toArray() ) {
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

void MainWindow::clear_follows_page() {
    ui->follow_list->clear();
    followed_online_status_.clear();
    followed_stream_data_.clear();

    for ( int i = ui->follows_stacked_widget->count(); i >= 0; i-- ) {
        QWidget *widget{ui->follows_stacked_widget->widget(i)};
        ui->follows_stacked_widget->removeWidget(widget);
        if ( widget != nullptr ) {
            widget->deleteLater();
        } else {
            qWarning() << "Widget [" << i <<"] is nullptr!";
        }

    }
}

void MainWindow::build_follows_page(QJsonObject &json_data) {

    QJsonValue follows_value{json_data.value("follows")};

    // Array of QJsonObjects where each one is a channel QJsonObject.
    for ( auto item :  follows_value.toArray() ) {
        QJsonObject followed_channel = item.toObject();
        QJsonValue name = followed_channel["channel"].toObject().value("name");
        my_program::Stream temp_holder(followed_channel["channel"].toObject());
        followed_stream_data_.push_back(std::move(temp_holder));
        followed_online_status_[name.toString()] = false;
    }
    // Online/Offline status to streams in followed_online_status;
    check_channel_online_status();

    for ( auto channel : followed_stream_data_ ) {
        // Widget to put into a QListWidgetItem:
        QWidget *widget = build_qlistwidgetitem(channel);

        QListWidgetItem *list_item{new QListWidgetItem()};
        list_item->setSizeHint(QSize(140, 21));
        ui->follow_list->addItem(list_item);
        ui->follow_list->setItemWidget(list_item, widget);

        // StackedWidget page:

        my_program::Channelinfo *channel_widget{new my_program::Channelinfo(this)};
        channel_widget->set_values(channel);

        ui->follows_stacked_widget->addWidget(channel_widget);
    }

}

void MainWindow::update_summary() {
    QString url{API_URL+"streams/summary"};

    data_retriever_.make_api_request(url);
    QJsonObject json_data = data_retriever_.retrieve_json_data();

    ui->twitch_viewers_value->setText(QString::number(json_data["viewers"].toDouble()));
    ui->twitch_channels_value->setText(QString::number(json_data["channels"].toDouble()));
}

void MainWindow::update_top_games() {
    QString url{API_URL+"games/top"};

    data_retriever_.make_api_request(url);
    QJsonObject json_data = data_retriever_.retrieve_json_data();
    QJsonValue json_value_top = json_data.value("top");

    std::vector<my_program::Game> top_games;
    for ( auto game : json_value_top.toArray() ) {

        QJsonObject game_object = game.toObject();
        my_program::Game temp_game;
        temp_game.name = game_object["game"].toObject().value("name").toString();

        temp_game.popularity = game_object["game"].toObject().value("popularity").toDouble();


        QString template_url(game_object["game"].toObject()["box"].toObject().value("template").toString());

        template_url.replace(QString("{width}"), QString("40"));
        template_url.replace(QString("{height}"), QString("40"));

        temp_game.viewers = game_object.value("viewers").toDouble();
        temp_game.channels = game_object.value("channels").toDouble();

        top_games.push_back(temp_game);
        QWidget *temp_widget{new QWidget()};
        ui->main_top_stacked_widget->addWidget(temp_widget);

    }

    if ( top_games.size() == 0 ) {
        qWarning() << "Top_games.size() == 0";
        return;
    }
    TopGamesListModel *model{new TopGamesListModel(top_games, ui->main_top_games_list)};
    TopGamesListDelegate *delegate{new TopGamesListDelegate()};
    ui->main_top_games_list->setModel(model);
    ui->main_top_games_list->setItemDelegate(delegate);


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

    QString channel_name{stream.get_channel_name()};
    QWidget *widget{new QWidget()};
    QLabel *label_list{new QLabel(channel_name)};
    QLabel *online_status{new QLabel()};

    label_list->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    label_list->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    label_list->setFixedSize(135, 20);
    online_status->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    online_status->setFixedSize(5, 20);

    // Channel is online:
    if ( followed_online_status_[channel_name] == true ) {
        label_list->setStyleSheet("QLabel { background-color: #2f3c54; color: #dddddd; padding: 1px; font: bold 10px; }");
        online_status->setStyleSheet("QLabel { background-color: #4CAF50; }");

        qDebug() << "Channel label online: " << channel_name;

    // Channel is offline:
    } else if ( followed_online_status_[channel_name] == false ) {
        label_list->setStyleSheet("QLabel { background-color: #2f3c54; color: #DDDDDD; padding: 1px; font: bold 10px; }");
        online_status->setStyleSheet("QLabel { background-color: #FF5722; }");

        qDebug() << "Channel label offline: " << channel_name;
    }

    // QListWidgetItem:
    QGridLayout *grid_layout{new QGridLayout};
    grid_layout->setMargin(0);
    grid_layout->setSpacing(0);
    grid_layout->addWidget(online_status, 0, 0);
    grid_layout->addWidget(label_list, 0, 1);
    widget->setLayout(grid_layout);

    return widget;

}
/*
QWidget* MainWindow::build_channel_info_page(const my_program::Stream &stream) {
    QWidget *temp_page = new QWidget;
    QHBoxLayout *layout_base_hbox = new QHBoxLayout;

    layout_base_hbox->setSpacing(0);

    // Left side of stacked page:
    QVBoxLayout *layout_left_vbox = new QVBoxLayout;
    // Right side of stacked page:
    QVBoxLayout *layout_right_vbox = new QVBoxLayout;
    // Logo, display_name, game, created_at, viewers, followers, url
    QLabel *logo = new QLabel();
    logo->setPixmap(QPixmap::fromImage(stream.get_logo()).scaled(QSize(150,150)));

    QLabel *display_name = new QLabel(stream.get_data_value(QStringLiteral("display_name")));
    QLabel *game = new QLabel(stream.get_game());
    QLabel *created_at_time = new QLabel();

    if ( stream.is_online() ) {
        QDateTime stream_start = QDateTime::fromString(stream.get_stream_start(), "yyyy'-'MM'-'dd'T'hh:mm:ss'Z'");
        qDebug() << "QString: " << stream.get_stream_start();
        qDebug() << "QDateTime.toString(): " << stream_start.toString();
        created_at_time->setText(stream_start.time().toString());
    } else {
        created_at_time->setText(QStringLiteral("Offline"));
    }

    QLabel *viewers = new QLabel();
    if ( stream.get_viewers() != 0 ) {
        viewers->setText(QString::number(stream.get_viewers()));
    } else {
        viewers->setText(QStringLiteral("-"));
    }

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
    status->setFixedSize(500, 25);

    layout_left_vbox->addWidget(logo, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(display_name, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(game, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(created_at_time, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(viewers, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(followers, 0, Qt::AlignTop);
    layout_left_vbox->addWidget(url_to_stream, 0, Qt::AlignTop);
    layout_left_vbox->addStretch();
    layout_left_vbox->setMargin(0);

    layout_right_vbox->addWidget(preview_picture, 0, Qt::AlignTop);
    layout_right_vbox->addWidget(status, 0, Qt::AlignTop);
    layout_right_vbox->addStretch();
    layout_right_vbox->setMargin(0);

    layout_base_hbox->addLayout(layout_left_vbox);
    layout_base_hbox->addLayout(layout_right_vbox);

    temp_page->setLayout(layout_base_hbox);
    return temp_page;
}

*/
void MainWindow::on_search_button_clicked() {


    if ( ui->search_stacked_widget->count() != 0 ) {
        QWidget *temp_widget{ui->search_stacked_widget->widget(0)};
        ui->search_stacked_widget->removeWidget(temp_widget);
        delete temp_widget;
    }

    ui->search_button->setDisabled(true);
    ui->search_line_edit->setDisabled(true);

    QString channel{ui->search_line_edit->text()};
    QString request_url{API_URL+"channels/"+channel+CLIENTID};
    qDebug() << "Search url: " << request_url;
    data_retriever_.make_api_request(request_url);
    QJsonObject json_data_obj{data_retriever_.retrieve_json_data()};

    if ( json_data_obj["error"] == "Not Found" ) {
        qWarning() << "Channel [" << channel << "] does not exist!";
        ui->search_button->setDisabled(false);
        ui->search_line_edit->setDisabled(false);

        return;
    }
    my_program::Stream stream_obj(json_data_obj);

    my_program::Channelinfo *channel_widget = new my_program::Channelinfo(this);
    channel_widget->set_values(stream_obj);


    // ui->search_stacked_widget->setContentsMargins(0, 0, 0, 0);
    ui->search_stacked_widget->addWidget(channel_widget);

    ui->search_button->setDisabled(false);
    ui->search_line_edit->setDisabled(false);
    ui->search_line_edit->clear();
}

void MainWindow::on_clear_follows_clicked() {
    if ( ui->follows_stacked_widget->count() == 0 ) {
        qWarning() << "Follows_stacked_widget.count() == 0";
        ui->fetch_follows->setDisabled(false);
        return;
    }
    ui->fetch_follows->setDisabled(true);
    ui->update_follows->setDisabled(true);
    ui->clear_follows->setDisabled(true);

    clear_follows_page();

    ui->fetch_follows->setDisabled(false);
    qDebug() << "Follows page cleared!";

}

void MainWindow::on_update_follows_clicked() {
    if ( ui->channelNameLineEdit->text().isEmpty() ) {
        qDebug() << "Username is empty! ["
                 << ui->channelNameLineEdit->text() << "]";
        return;
    }
    ui->fetch_follows->setDisabled(true);
    ui->update_follows->setDisabled(true);
    ui->clear_follows->setDisabled(true);

    clear_follows_page();

    followed_stream_data_.clear();
    followed_online_status_.clear();

    QString username{ui->channelNameLineEdit->text()};
    QString request_url{API_URL+"users/"+username+"/follows/channels"+CLIENTID};

    data_retriever_.make_api_request(request_url);

    qDebug() << "Update: data retrieved.";
    QJsonObject follows_json_data{data_retriever_.retrieve_json_data()};

    QJsonValue follows_value{follows_json_data.value("follows")};

    // Array of QJsonObjects where each one is a channel QJsonObject.
    for ( auto item :  follows_value.toArray() ) {
        QJsonObject followed_channel = item.toObject();
        QJsonValue name = followed_channel["channel"].toObject().value("name");
        my_program::Stream temp_holder(followed_channel["channel"].toObject());
        followed_stream_data_.push_back(std::move(temp_holder));
        followed_online_status_[name.toString()] = false;
    }
    // Online/Offline status to streams in followed_online_status;
    check_channel_online_status();

    for ( auto channel : followed_stream_data_ ) {
        // Widget to put into a QListWidgetItem:
        QWidget *widget = build_qlistwidgetitem(channel);

        QListWidgetItem *list_item = new QListWidgetItem();
        list_item->setSizeHint(QSize(140, 21));
        ui->follow_list->addItem(list_item);
        ui->follow_list->setItemWidget(list_item, widget);

        // StackedWidget page:
        my_program::Channelinfo *channel_widget = new my_program::Channelinfo(this);
        channel_widget->set_values(channel);

        ui->follows_stacked_widget->addWidget(channel_widget);
    }
    ui->fetch_follows->setDisabled(false);
    ui->update_follows->setDisabled(false);
    ui->clear_follows->setDisabled(false);
    qDebug() << "Follows page updates!";
}

void MainWindow::on_main_update_button_clicked() {
    qDebug() << "Main: Update button clicked";
    ui->main_update_button->setDisabled(true);
    update_summary();
    update_top_games();
    ui->main_update_button->setDisabled(false);

}

void MainWindow::on_main_top_games_list_clicked(const QModelIndex &index) {
    int page_number{index.row()};
    qDebug() << "Size of stacked: " << ui->main_top_stacked_widget->count();
    qDebug() << "Row: " << page_number << " Game: " << index.data(0).toStringList().at(0);
    QString game{index.data(0).toStringList().at(0)};
    if ( main_top_games_data_.contains(game) ) {
        ui->main_top_stacked_widget->setCurrentIndex(page_number);
        qDebug() << game << " page already created. Switching to page: " << page_number;
        return;
    }

    // Create new stacked widget page only when it doesn't already exists:

    // https://api.twitch.tv/kraken/streams?game=Counter-Strike:%20Global%20Offensive?client_id=kotialthf6zsygxpvqfhgbf0wvblsv5
    // !Remember to change client_id to header instead of part of the query url!
    QString request_url{API_URL+"streams?game="+game};

    data_retriever_.make_api_request(request_url);
    QJsonObject game_json_data{data_retriever_.retrieve_json_data()};

    QJsonValue stream_value{game_json_data.value("streams")};
    QList<my_program::Stream> temp_list;
    // Array of QJsonObjects where each one is a channel/stream QJsonObject.
    for ( auto item :  stream_value.toArray() ) {
        QJsonObject stream_channel = item.toObject();
        QJsonValue name = stream_channel["channel"].toObject().value("name");
        my_program::Stream temp_holder(stream_channel["channel"].toObject());
        temp_holder.set_stream_details(stream_channel);

        temp_list.push_back(temp_holder);
    }
    main_top_games_data_[game] = temp_list;

    QScrollArea *page_scroll_area{new QScrollArea()};
    page_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    page_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QGridLayout *scroll_area_grid{new QGridLayout()};
    QWidget *scroll_area_widget{new QWidget()};

    const QList<my_program::Stream> game_list{main_top_games_data_[game]};
    unsigned int counter{0};
    for ( int i = 0; i < 5; ++i ) {
        for ( int k = 0; k < 5; ++k ) {
            const my_program::Stream stream_obj{game_list.at(counter)};
            my_program::MiniInfo *mini_info_widget{new my_program::MiniInfo(stream_obj)};

            scroll_area_grid->addWidget(mini_info_widget, i, k);
            ++counter;
        }
    }
    scroll_area_grid->setSpacing(1);
    scroll_area_grid->setSizeConstraint(QLayout::SetMinAndMaxSize);
    scroll_area_grid->setContentsMargins(0, 0, 0, 0);

    scroll_area_widget->setLayout(scroll_area_grid);
    scroll_area_widget->setMinimumWidth(760);
    page_scroll_area->setWidget(scroll_area_widget);

    QWidget *game_widget{ui->main_top_stacked_widget->widget(page_number)};
    QGridLayout *game_grid{new QGridLayout()};
    game_grid->addWidget(page_scroll_area);
    game_widget->setStyleSheet("QWidget { background-color: #25324c; }");
    game_widget->setLayout(game_grid);

    ui->main_top_stacked_widget->setCurrentIndex(page_number);

    qDebug() << "Page for game [" << game << "] created. Page number: [" << page_number << "] ";
    qDebug() << "Switching to page " << page_number;
}
