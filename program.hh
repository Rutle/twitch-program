#ifndef PROGRAM_HH
#define PROGRAM_HH

#include <QDialog>
#include <QApplication>
#include "programinterface.hh"
#include "programmodelinterface.hh"
#include "settings.hh"
#include "selectorwindow.hh"
#include "mainwindow.hh"
#include "mainwindowminimal.hh"

namespace my_program {

class Program: public interface::ProgramInterface {

    public:
        Program();
        Program(QWidget *ui);
        ~Program();
        void show(UI selection);
        //void closeProgram();
        interface::ProgramModelInterface *getModel();

    private:

        void startUI(UI selection);
        interface::ProgramModelInterface *model_;
        SelectorWindow *selectionUi_;
        MainWindow *fullUi_;
        MainWindowMinimal *minimalUi_;
        UI state_;

};

}
#endif // PROGRAM_HH
