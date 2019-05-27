#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "utilityprograms.hh"
#include "channelinfo.hh"
#include "topgameslistmodel.hh"
#include "topgameslistdelegate.hh"
#include "miniinfo.hh"
#include "programmodel.hh"
#include "widgetbuilder.hh"

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

using WB = my_program::WidgetBuilder;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), pa_{nullptr} {
    ui->setupUi(this);

    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //this->setStyleSheet("QWidget#centralWidget {background-color: #d9d9d9;}");
    //settings_ == std::make_shared<my_program::Settings>(my_program::Settings());
    //settings_ = std::unique_ptr<my_program::Settings>(new my_program::Settings());
    //QObject::connect(ui->channelNameLineEdit, SIGNAL(editingFinished()), this, SLOT(checkUsernameEdit()));
    //updateSettings();
    QObject::connect(ui->channelNameLineEdit, &QLineEdit::textEdited,
                     [=]( const QString &newValue ) { this->usernameChanged(newValue); });
    ui->save_settings_button->setDisabled(true);

    ui->follow_list->setFocusPolicy(Qt::NoFocus);
    QDir dir(QDir::current());
    if ( !dir.mkdir("user_pictures")) {
        qDebug() << "Directory already exists!";
    }

    // Follows update- and clear-buttons disabled at the start because the
    // list is empty.
    ui->clear_follows->setDisabled(true);
    ui->update_follows->setDisabled(true);
    infoLabel_ = static_cast<InfoMessage *> (
                            WB::buildWidget(
                            WB::InfoLabel,
                            new QString{""}, nullptr, 100));

    ui->infoLayout->addWidget(infoLabel_);
    infoLabel_->hide();

    saveLabel_ = static_cast<InfoMessage *> (
                            WB::buildWidget(
                            WB::InfoLabel,
                            new QString{"Saved"}, nullptr, 100));

    ui->infoLayout->addWidget(saveLabel_);
    saveLabel_->hide();
    QObject::connect(ui->save_settings_button, &QPushButton::clicked,
                     [=]( ) {
        ui->save_settings_button->setDisabled(true);
        saveLabel_->setText("Saved");
        saveLabel_->show();

        QTimer::singleShot(5000, saveLabel_, SLOT(hide()));

    });
    for (int i = 0; i < 4; ++i) {
        QString str = QString{"Testi "} + QString::number(i);
        messageLabelContent_.append(WB::buildWidget(
                                    WB::InfoLabel,
                                    &str, nullptr, 100));
        ui->infoLayout->addWidget(messageLabelContent_.at(i));
        messageLabelContent_.at(i)->hide();
    }
    messageLabelContent_.append(WB::buildWidget(
                                WB::InfoLabel,
                                new QString{"Please set username in settings."},
                                nullptr, 300));
    ui->infoLayout->addWidget(messageLabelContent_.last());
    messageLabelContent_.last()->hide();

    ui->main_top_games_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->main_top_games_list->setVisible(false);
    QObject::connect(ui->tabWidget, &QTabWidget::tabBarClicked,
                     this, [=]( const int &newValue ) {
                                    this->checkErrorLabel(newValue); });

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setProgram(my_program::interface::ProgramInterface *pa)
{
    pa_ = pa;
}

void MainWindow::on_fetch_follows_clicked() {
    if(!isUserName()) {
        qWarning() << "Please set username.";
        return;
    }

    ui->fetch_follows->setDisabled(true);
    ui->update_follows->setDisabled(true);
    ui->clear_follows->setDisabled(true);

    QWidget *msg = new QWidget;
    if (pa_->getModel()->fetchFollowedChannels(msg, Full)) {
        pa_->getModel()->buildFollowsPage(ui->follow_list, Full,
                                          ui->follows_stacked_widget);
    } else {
        ui->follows_stacked_widget->addWidget(msg);
    }
    //if(programModel_->fetchFollowedChannels(ui->follows_stacked_widget)) {
    //    programModel_->buildFollowsPage(ui->follow_list, ui->follows_stacked_widget);
    //}

    ui->fetch_follows->setDisabled(false);
    ui->update_follows->setDisabled(false);
    ui->clear_follows->setDisabled(false);

}


void MainWindow::updateSettings() {
    QString username{pa_->getModel()->getSettings()->getUsername()};
    ui->channelNameLineEdit->setText(username);
    isUserName();
    //ui->channelNameLineEdit->setText(settings_->give_user_name());
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
        messageLabelContent_.last()->show();

        return false;
    }
    messageLabelContent_.last()->hide();
    return true;
}

void MainWindow::usernameChanged(const QString &newValue)
{
    ui->save_settings_button->setDisabled(false);
}

void MainWindow::on_save_settings_button_clicked() {
    if(!isUserName()) {
        qWarning() << "Please set username before saving";
        return;
    }

    //settings_->set_user_name(ui->channelNameLineEdit->text());
    //settings_->save_to_file();
    QString username{ui->channelNameLineEdit->text()};
    pa_->getModel()->getSettings()->setUsername(username);
    pa_->getModel()->getSettings()->saveFile();

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

    QWidget *msg = new QWidget;

    if(!pa_->getModel()->searchChannel(ui->search_stacked_widget, msg, channel)) {
        ui->search_stacked_widget->addWidget(msg);
    }
    //programModel_->searchChannel(ui->search_stacked_widget, channel);

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
    clear_follows_page();
    QWidget *msg = new QWidget;
    if(!pa_->getModel()->updateFollowedStatus(msg, Full)) {
        ui->follows_stacked_widget->addWidget(msg);
        return;
    }
    //programModel_->updateFollowedStatus(ui->follows_stacked_widget);

    pa_->getModel()->buildFollowsPage(ui->follow_list, Full,
                                      ui->follows_stacked_widget);
    //programModel_->buildFollowsPage(ui->follow_list, ui->follows_stacked_widget);

    ui->fetch_follows->setDisabled(false);
    ui->update_follows->setDisabled(false);
    ui->clear_follows->setDisabled(false);

}

void MainWindow::on_main_update_button_clicked() {
    QWidget *msg = new QWidget;
    ui->main_update_button->setDisabled(true);
    //if(!programModel_->updateSummaryLabels(ui->twitch_viewers_value,
    if(!pa_->getModel()->updateSummaryLabels(ui->twitch_viewers_value,
                                             ui->twitch_channels_value,
                                             msg)) {
        ui->main_top_stacked_widget->addWidget(msg);
        ui->main_update_button->setDisabled(false);
        return;
    }
    QWidget *msg2 = new QWidget;
    //if(!programModel_->updateTopGames(ui->main_top_stacked_widget,
    if(!pa_->getModel()->updateTopGames(ui->main_top_stacked_widget,
                                        ui->main_top_games_list, msg2) ) {
        ui->main_top_stacked_widget->addWidget(msg2);
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
    QWidget *msg = new QWidget;
    if(!pa_->getModel()->changeTopGamePage(game, page_number, ui->main_top_stacked_widget, msg)) {
        ui->main_top_stacked_widget->addWidget(msg);
    }
    //programModel_->changeTopGamePage(game, page_number, ui->main_top_stacked_widget);
    ui->main_top_games_list->setDisabled(false);

}

void MainWindow::checkUsernameEdit() {
    /*
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
    */
}

void MainWindow::checkErrorLabel(const int &newValue)
{
    if(ui->tabWidget->currentIndex() == ui->tabWidget->count()-1) {
        messageLabelContent_.at(newValue)->show();
        return;
    } else if (newValue == ui->tabWidget->count()-1) {
        messageLabelContent_.at(ui->tabWidget->currentIndex())->hide();
        return;
    }
    if(!(messageLabelContent_.at(newValue) == nullptr)) {
        messageLabelContent_.at(ui->tabWidget->currentIndex())->hide();
        messageLabelContent_.at(newValue)->show();
    }
}
