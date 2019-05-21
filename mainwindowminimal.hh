#ifndef MAINWINDOWMINIMAL_HH
#define MAINWINDOWMINIMAL_HH

#include <QMainWindow>

namespace Ui {
class MainWindowMinimal;
}

class MainWindowMinimal : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindowMinimal(QWidget *parent = nullptr);
        ~MainWindowMinimal();

    private:
        Ui::MainWindowMinimal *ui;
};

#endif // MAINWINDOWMINIMAL_HH
