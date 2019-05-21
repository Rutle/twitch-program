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

    //my_program::widgets::Channelinfo *base_page{new my_program::widgets::Channelinfo()};
    my_program::widgets::Channelinfo *test{new my_program::widgets::Channelinfo("Testi error page")};
    ui->search_stacked_widget->setContentsMargins(0, 0, 0, 0);
    ui->search_stacked_widget->addWidget(test);
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

    if(programModel_->fetchFollowedChannels(ui->follows_stacked_widget)) {
        programModel_->buildFollowsPage(ui->follow_list, ui->follows_stacked_widget);
    }

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

    //my_program::widgets::Channelinfo *channel_widget{new my_program::widgets::Channelinfo(this)};

    programModel_->searchChannel(ui->search_stacked_widget, channel);

    //ui->search_stacked_widget->addWidget(channel_widget);

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

    programModel_->updateFollowedStatus(ui->follows_stacked_widget);
    clear_follows_page();
    programModel_->buildFollowsPage(ui->follow_list, ui->follows_stacked_widget);

    ui->fetch_follows->setDisabled(false);
    ui->update_follows->setDisabled(false);
    ui->clear_follows->setDisabled(false);

}

void MainWindow::on_main_update_button_clicked() {
    ui->main_update_button->setDisabled(true);
    if(!programModel_->updateSummaryLabels(ui->twitch_viewers_value,
                                           ui->twitch_channels_value,
                                           ui->main_top_stacked_widget)) {
        ui->main_update_button->setDisabled(false);
        return;
    }
    if(!programModel_->updateTopGames(ui->main_top_stacked_widget,
                                      ui->main_top_games_list) ) {
        ui->main_update_button->setDisabled(false);
        return;

    }

    ui->main_update_button->setDisabled(false);

}

// Retrieves data for top streams in index (game) and creates a widget
// which contains 25 smaller widgets with minimal stream info in a QScrollArea.
void MainWindow::on_main_top_games_list_clicked(const QModelIndex &index) {
    int page_number{index.row()};
    //qDebug() << "Row: " << page_number << " Game: " << index.data(0).toStringList().at(0);
    QString game{index.data(0).toStringList().at(0)};
    ui->main_top_games_list->setDisabled(true);
    programModel_->changeTopGamePage(game, page_number, ui->main_top_stacked_widget);
    ui->main_top_games_list->setDisabled(false);

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
