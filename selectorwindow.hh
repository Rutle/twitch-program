#ifndef SELECTORWINDOW_HH
#define SELECTORWINDOW_HH

#include "mainwindow.hh"
#include "mainwindowminimal.hh"
#include <QDialog>

namespace Ui {
class SelectorWindow;
}

class SelectorWindow : public QDialog
{
        Q_OBJECT

    public:
        explicit SelectorWindow(QWidget *parent = nullptr);
        ~SelectorWindow();

    private slots:
        void on_fullBtn_clicked();

        void on_minimalBtn_clicked();


    private:
        Ui::SelectorWindow *ui;

        MainWindow *uiFull_;

        MainWindowMinimal *uiMin_;
};

#endif // SELECTORWINDOW_HH
