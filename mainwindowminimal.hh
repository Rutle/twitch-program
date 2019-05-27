#ifndef MAINWINDOWMINIMAL_HH
#define MAINWINDOWMINIMAL_HH

#include "programinterface.hh"
#include "infomessage.hh"

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
        void setProgram(my_program::interface::ProgramInterface *pa);
        void updateSettings();
    private slots:
        void on_updateBtn_clicked();

        //void on_closeBtn_clicked();

     private:
        void addMessage(QString *message);
        bool isUserName() const;
        void saveSettings();
        void changePage(int index);
        void usernameChange(const QString &newName);
        Ui::MainWindowMinimal *ui;
        my_program::interface::ProgramInterface *pa_;
        bool editingFlag_;
        InfoMessage *message_;
};

#endif // MAINWINDOWMINIMAL_HH
