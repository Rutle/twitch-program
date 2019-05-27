#include "selectorwindow.hh"
#include "ui_selectorwindow.h"
#include <QPushButton>
#include <QDebug>
#include <QLineEdit>

SelectorWindow::SelectorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectorWindow),
    editingFlag_{false}
{
    ui->setupUi(this);
    QObject::connect(ui->switchBtn, &QPushButton::clicked,
                     this,
                     [=](){ changePage(ui->stackedWidget->currentIndex());});
    QObject::connect(ui->usernameLineEdit, &QLineEdit::textEdited,
                     [=]( const QString &newValue ) { this->usernameChange(newValue); });
    //QObject::connect(ui->usernameEdit, &QLineEdit::editingFinished,
    //                 []( ) { qDebug() << "editingFinished"; });
}

SelectorWindow::~SelectorWindow()
{
    delete ui;
}

void SelectorWindow::setProgram(my_program::interface::ProgramInterface *pa) {
    pa_ = pa;
}

void SelectorWindow::updateSettings()
{
    QString curUsername{pa_->getModel()->getSettings()->getUsername()};
    ui->usernameLineEdit->setText(curUsername);

}

void SelectorWindow::changePage(int index) {
    qDebug() << index;
    switch (index) {
        case 0: {
            ui->stackedWidget->setCurrentIndex(1);
            ui->switchBtn->setText("Menu");
        }
        break;
        case 1: {
            ui->stackedWidget->setCurrentIndex(0);
            ui->switchBtn->setText("Settings");
        }
        break;
    }
}

void SelectorWindow::saveSettings()
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

void SelectorWindow::usernameChange(const QString &newName) {
    QString curUsername{pa_->getModel()->getSettings()->getUsername()};
    if (editingFlag_ == false && curUsername != newName ) {
        ui->switchBtn->setText("Save");
        editingFlag_ = true;
        QObject::disconnect(ui->switchBtn, &QPushButton::clicked,
                            this, nullptr);
        QObject::connect(ui->switchBtn, &QPushButton::clicked,
                         this, [=](){
            this->saveSettings();
        });
    }
}

void SelectorWindow::on_fullBtn_clicked() {
    pa_->show(UI::FullUI);
}

void SelectorWindow::on_minimalBtn_clicked() {
    pa_->show(UI::MinimalUI);
}

void SelectorWindow::on_closeBtn_clicked() {
    pa_->show(UI::Exit);

}
