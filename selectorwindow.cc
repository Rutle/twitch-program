#include "selectorwindow.hh"
#include "ui_selectorwindow.h"


SelectorWindow::SelectorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectorWindow)
{
    ui->setupUi(this);
}

SelectorWindow::~SelectorWindow()
{
    delete ui;
}

void SelectorWindow::on_fullBtn_clicked() {

    uiFull_ = new MainWindow;
    uiFull_->show();
    this->hide();
}

void SelectorWindow::on_minimalBtn_clicked() {
    //uiMin_ = new MainWindow;
    //uiMin_->show();
    this->hide();
}
