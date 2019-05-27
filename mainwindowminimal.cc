#include "mainwindowminimal.hh"
#include "ui_mainwindowminimal.h"
#include "widgetbuilder.hh"

#include <QDebug>
#include <QDesktopServices>

using WB = my_program::WidgetBuilder;

MainWindowMinimal::MainWindowMinimal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowMinimal), pa_{nullptr}, editingFlag_{false}
{
    ui->setupUi(this);
    MainWindowMinimal::setWindowTitle("Minimal UI for followed channels");
    QObject::connect(ui->twitchBtn, &QPushButton::clicked,
                     [=](){
        QDesktopServices::openUrl(QUrl{"https://www.twitch.tv"});
    });
    ui->stackedWidget->setCurrentIndex(0);
    QObject::connect(ui->switchBtn, &QPushButton::clicked,
                     this,
                     [=](){ changePage(ui->stackedWidget->currentIndex());});

    QObject::connect(ui->usernameLineEdit, &QLineEdit::textEdited,
                     [=]( const QString &newValue ) { this->usernameChange(newValue); });
    addMessage(new QString{"Followed channels"});
}

MainWindowMinimal::~MainWindowMinimal()
{
    delete ui;
}

void MainWindowMinimal::setProgram(my_program::interface::ProgramInterface *pa)
{
    pa_ = pa;
}

void MainWindowMinimal::updateSettings()
{
    QString username{pa_->getModel()->getSettings()->getUsername()};
    ui->usernameLineEdit->setText(username);
    /*
     * QLabel {
    background-color: #303030;
    border-radius: 2px;
    padding: 1px;
    font: bold 10px;
    border: 1px solid #5b5b5b;
}
     *
     * */
}

void MainWindowMinimal::on_updateBtn_clicked() {
    if(!isUserName()) {
        addMessage(new QString{"Please add username"});
        return;
    }
    qDebug() << MainWindowMinimal::height() << " " << MainWindowMinimal::width();
    qDebug() << ui->followedChannelsList->height() << " " << ui->followedChannelsList->width();
    ui->switchBtn->setDisabled(true);
    ui->twitchBtn->setDisabled(true);
    ui->updateBtn->setDisabled(true);

    QWidget *msg = new QWidget;
    if (pa_->getModel()->fetchFollowedChannels(msg, Mini)) {
        pa_->getModel()->buildFollowsPage(ui->followedChannelsList, Mini,
                                          ui->stackedWidget);
    } else {
        // Error happened.
        //ui->stackedWidget->setCurrentIndex(2);
    }

    ui->switchBtn->setDisabled(false);
    ui->twitchBtn->setDisabled(false);
    ui->updateBtn->setDisabled(false);
}

void MainWindowMinimal::addMessage(QString *message)
{
    ui->message->removeWidget(message_);
    message_ = static_cast<InfoMessage *>(
                WB::buildWidget(WB::InfoLabel, message, nullptr, 200));
    message_->setStyleSheet("QLabel {"
                            "background-color: #303030;"
                            "border-radius: 2px;"
                            "padding: 1px;"
                            "font: bold 10px;"
                            "border: 1px solid #5b5b5b;"
                            "}");
    ui->message->addWidget(message_);
}

bool MainWindowMinimal::isUserName() const {
    if (ui->usernameLineEdit->text().isEmpty() ) {
        return false;
    }
    return true;
}

void MainWindowMinimal::saveSettings()
{
    QString curUsername{ui->usernameLineEdit->text()};
    pa_->getModel()->getSettings()->setUsername(curUsername);
    pa_->getModel()->getSettings()->saveFile();
    editingFlag_ = false;
    QObject::disconnect(ui->switchBtn, &QPushButton::clicked,
                        this, nullptr);
    QObject::connect(ui->switchBtn, &QPushButton::clicked,
                     this,
                     [=](){ changePage(ui->stackedWidget->currentIndex());});
    ui->switchBtn->setText("Menu");
}

void MainWindowMinimal::changePage(int index)
{
    qDebug() << index;
    switch (index) {
        case 0: { // Currently on follows page
            ui->stackedWidget->setCurrentIndex(1);
            ui->switchBtn->setText("Follows");
            ui->updateBtn->setDisabled(true);
            addMessage(new QString{"Settings"});
        }
        break;
        case 1: {
            ui->stackedWidget->setCurrentIndex(0);
            ui->switchBtn->setText("Settings");
            ui->updateBtn->setDisabled(false);
            addMessage(new QString{"Followed channels"});
        }
        break;
    }
}

void MainWindowMinimal::usernameChange(const QString &newName)
{
    QString curUsername{pa_->getModel()->getSettings()->getUsername()};
    if (editingFlag_ == false && curUsername != newName ) {
        ui->switchBtn->setText("Save");
        editingFlag_ = true;
        QObject::disconnect(ui->switchBtn, &QPushButton::clicked,
                            this, nullptr);
        QObject::connect(ui->switchBtn, &QPushButton::clicked,
                         this, [=](){ this->saveSettings();});
    }
}
