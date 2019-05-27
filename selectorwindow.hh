#ifndef SELECTORWINDOW_HH
#define SELECTORWINDOW_HH

#include "programinterface.hh"
//#include "program.hh"
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
        void setProgram(my_program::interface::ProgramInterface *pa);
        void updateSettings();
    private slots:

        void on_fullBtn_clicked();

        void on_minimalBtn_clicked();

        void on_closeBtn_clicked();

    private:
        void changePage(int index);
        void saveSettings();
        void usernameChange(const QString &newName);
        Ui::SelectorWindow *ui;
        my_program::interface::ProgramInterface *pa_;

        bool editingFlag_;

};

#endif // SELECTORWINDOW_HH
