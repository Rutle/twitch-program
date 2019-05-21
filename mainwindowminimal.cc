#include "mainwindowminimal.hh"
#include "ui_mainwindowminimal.h"

MainWindowMinimal::MainWindowMinimal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowMinimal)
{
    ui->setupUi(this);
}

MainWindowMinimal::~MainWindowMinimal()
{
    delete ui;
}
