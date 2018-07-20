#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "utilityprograms.hh"
#include "channelinfo.hh"
#include "topgameslistmodel.hh"
#include "topgameslistdelegate.hh"
#include "miniinfo.hh"
#include "programmodel.hh"

#include <QLabel>
#include <QJsonArray>
#include <QMap>
#include <utility>
#include <QListWidgetItem>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QScrollArea>

const QString CLIENTID = "client_id=th5mqjz7wtxx0ut8cns0g22r3miuzz";
const QString API_URL = "https://api.twitch.tv/kraken/";

const QString SUMMARY = API_URL+"streams/summary?"+CLIENTID;
const QString TOPGAMES = API_URL+"games/top?"+CLIENTID;
//const QString FOLLOWS = API_URL+"users/"+username+"/follows/channels"+CLIENTID;
const std::pair<QString, QString> USERNAMEINFO = std::make_pair("username", "Please set username in the settings tab.");
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //this->setStyleSheet("QWidget#centralWidget {background-color: #d9d9d9;}");
    //settings_ == std::make_shared<my_program::Settings>(my_program::Settings());
    settings_ = std::unique_ptr<my_program::Settings>(new my_program::Settings());
    QObject::connect(ui->channelNameLineEdit, SIGNAL(editingFinished()), this, SLOT(checkUsernameEdit()));
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

    my_program::widgets::Channelinfo *base_page{new my_program::widgets::Channelinfo()};
    ui->search_stacked_widget->setContentsMargins(0, 0, 0, 0);
    ui->search_stacked_widget->addWidget(base_page);
    ui->main_top_games_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->main_top_games_list->setVisible(false);

    // Get a raw pointer from settings_ shared_ptr:
    programModel_ = new my_program::ProgramModel(settings_.get());

    QLabel *username = new QLabel();
    username->setText("Please set username in the settings tab.");
    infoLabelContent_.append(username);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_fetch_follows_clicked() {
    if(!isUserName()) {
        qWarning() << "Please set username.";
        return;
    }

    ui->fetch_follows->setDisabled(true);
    ui->update_follows->setDisabled(true);
    ui->clear_follows->setDisabled(true);

    programModel_->fetchFollowedChannels();

    programModel_->buildFollowsPage(ui->follow_list, ui->follows_stacked_widget);

    ui->fetch_follows->setDisabled(false);
    ui->update_follows->setDisabled(false);
    ui->clear_follows->setDisabled(false);

}


void MainWindow::update_settings() {
    ui->channelNameLineEdit->setText(settings_->give_user_name());
}

void MainWindow::clear_follows_page() {
    ui->follow_list->clear();

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

bool MainWindow::isUserName() const {
    if ( ui->channelNameLineEdit->text().isEmpty() ) {
        qWarning() << "Empty user name LineEdit!";
        return false;
    }
    return true;
}

void MainWindow::on_save_settings_button_clicked() {
    if(!isUserName()) {
        qWarning() << "Please set username before saving";
        return;
    }

    settings_->set_user_name(ui->channelNameLineEdit->text());
    settings_->save_to_file();

}

void MainWindow::on_search_button_clicked() {
    if ( ui->search_stacked_widget->count() != 0 ) {
        QWidget *temp_widget{ui->search_stacked_widget->widget(0)};
        ui->search_stacked_widget->removeWidget(temp_widget);
        delete temp_widget;
    }

    ui->search_button->setDisabled(true);
    ui->search_line_edit->setDisabled(true);

    QString channel{ui->search_line_edit->text()};

    my_program::widgets::Channelinfo *channel_widget{new my_program::widgets::Channelinfo(this)};

    if(!programModel_->searchChannel(channel_widget, channel)) {
      return;
    }

    ui->search_stacked_widget->addWidget(channel_widget);

    ui->search_button->setDisabled(false);
    ui->search_line_edit->setDisabled(false);
    ui->search_line_edit->clear();
}

void MainWindow::on_clear_follows_clicked() {
    if ( ui->follows_stacked_widget->count() == 0 ) {
        //qWarning() << "Follows_stacked_widget.count() == 0";
        ui->fetch_follows->setDisabled(false);
        return;
    }
    ui->fetch_follows->setDisabled(true);
    ui->update_follows->setDisabled(true);
    ui->clear_follows->setDisabled(true);

    clear_follows_page();

    ui->fetch_follows->setDisabled(false);
    //qDebug() << "Follows page cleared!";

}

void MainWindow::on_update_follows_clicked() {
    if(!isUserName()) {
        qWarning() << "Please set username.";
        return;
    }
    ui->fetch_follows->setDisabled(true);
    ui->update_follows->setDisabled(true);
    ui->clear_follows->setDisabled(true);


    programModel_->updateFollowedStatus();
    clear_follows_page();
    programModel_->buildFollowsPage(ui->follow_list, ui->follows_stacked_widget);

    ui->fetch_follows->setDisabled(false);
    ui->update_follows->setDisabled(false);
    ui->clear_follows->setDisabled(false);

}

void MainWindow::on_main_update_button_clicked() {
    ui->main_update_button->setDisabled(true);
    programModel_->updateSummaryLabels(ui->twitch_viewers_value,
                                       ui->twitch_channels_value);
    programModel_->updateTopGames(ui->main_top_stacked_widget,
                                  ui->main_top_games_list);

    ui->main_update_button->setDisabled(false);

}
// Retrieves data for top streams in index (game) and creates a widget
// which contains 25 smaller widgets with minimal stream info in a QScrollArea.
void MainWindow::on_main_top_games_list_clicked(const QModelIndex &index) {
    int page_number{index.row()};
    //qDebug() << "Row: " << page_number << " Game: " << index.data(0).toStringList().at(0);
    QString game{index.data(0).toStringList().at(0)};

    if ( main_top_games_data_.contains(game) ) {
        ui->main_top_stacked_widget->setCurrentIndex(page_number);
        qDebug() << game << " page already created. Switching to page: " << page_number;
        return;
    }


    // -!Remember to change client_id to header instead of part of the query url!-
    // -!Client_ID works differently with streams.!-
    QString request_url{API_URL+"streams?game="+game+"&"+CLIENTID};

    data_retriever_.make_api_request(request_url);
    QJsonObject game_json_data{data_retriever_.retrieve_json_data()};

    QJsonValue stream_value{game_json_data.value("streams")};
    QList<my_program::Stream> stream_list;

    QScrollArea *page_scroll_area{new QScrollArea()};
    page_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    page_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QGridLayout *scroll_area_grid{new QGridLayout()};
    QWidget *scroll_area_widget{new QWidget()};

    scroll_area_grid->setHorizontalSpacing(10);
    scroll_area_grid->setVerticalSpacing(10);

    scroll_area_grid->setSizeConstraint(QLayout::SetMinAndMaxSize);
    scroll_area_grid->setContentsMargins(0, 0, 0, 0);

    scroll_area_widget->setLayout(scroll_area_grid);
    scroll_area_widget->setMinimumWidth(850);
    page_scroll_area->setWidget(scroll_area_widget);

    QWidget *game_widget{ui->main_top_stacked_widget->widget(page_number)};

    // game_grid-layout is the base grid where QScrollArea is put in.
    QGridLayout *game_grid{new QGridLayout()};
    game_grid->setContentsMargins(10, 0, 10, 0);
    game_grid->addWidget(page_scroll_area);
    game_widget->setStyleSheet("QWidget { background-color: #1f1f1f; }");
    game_widget->setLayout(game_grid);
    ui->main_top_stacked_widget->setCurrentIndex(page_number);

    // -!Could change this so that the QScrollArea gets updated after     !-
    // -!5 objects has been created so that it would feel a bit smoother. !-

    // Array of QJsonObjects where each one is a channel/stream QJsonObject.
    //unsigned int counter{0};
    unsigned int row{0};
    unsigned int column{0};
    for ( auto item :  stream_value.toArray() ) {
        QJsonObject stream_channel = item.toObject();
        // QJsonValue name = stream_channel["channel"].toObject().value("name");
        my_program::Stream temp_stream_holder(stream_channel["channel"].toObject());
        temp_stream_holder.set_stream_details(stream_channel);

        my_program::widgets::MiniInfo *mini_info_widget{
            new my_program::widgets::MiniInfo(temp_stream_holder)};
        scroll_area_grid->addWidget(mini_info_widget, row, column);
        ++column;
        if ( column == 5 ) {
            ++row;
            column = 0;
            // Let the Application to process the creation of a full row of items.
            QApplication::sendPostedEvents();
        }
        stream_list.push_back(temp_stream_holder);
    }
    main_top_games_data_[game] = stream_list;
    ui->main_top_stacked_widget->setStyleSheet("#main_top_stacked_widget {"
                                               "background-color: #1f1f1f;"
                                               "}");

    /*
    const QList<my_program::Stream> game_list{main_top_games_data_[game]};
    if ( game_list.size() < 25 ) {
        qDebug() << "Game list size: [" << game_list.size() << "]";
    }
    // Go through a list of stream objects in [game] key value and create
    // a widget for each stream.
    // unsigned int row{0};
    QList<my_program::Stream>::size_type counter{0};
    while ( counter < game_list.size() ) {
        for ( int k = 0; k < 5; ++k ) {
            const my_program::Stream stream_obj{game_list.at(counter)};
            my_program::widgets::MiniInfo *mini_info_widget{new my_program::widgets::MiniInfo(stream_obj)};
            scroll_area_grid->addWidget(mini_info_widget, row, k);
            ++counter;
            // Game list has less than 25 stream objects.
            if ( counter >= game_list.size() ) {
                QApplication::sendPostedEvents();
                break;
            }
        }
        ++row;
        // Process the added widgets and update the scrollarea with these
        // widgets.
        QApplication::sendPostedEvents();

    }
    */
    //qDebug() << "Page for game " << game << " created. Page number: [" << page_number << "] ";
}

void MainWindow::checkUsernameEdit() {
    if(!isUserName()) {
        ui->infoLayout->addWidget(infoLabelContent_.at(0));
        int idx = ui->infoLayout->indexOf(infoLabelContent_.at(0));
        ui->infoLayout->itemAt(idx)->widget()->setVisible(true);
    } else {
        if(ui->infoLayout->indexOf(infoLabelContent_.at(0)) == -1) {
            return;
        } else {
            int idx = ui->infoLayout->indexOf(infoLabelContent_.at(0));
            ui->infoLayout->itemAt(idx)->widget()->setVisible(false);
        }

    }

}
