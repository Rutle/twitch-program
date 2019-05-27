#include "program.hh"
#include "settings.hh"
#include "programmodel.hh"


namespace my_program {

Program::Program() {
    model_ = new ProgramModel;
    fullUi_ = nullptr;
    minimalUi_ = nullptr;
    selectionUi_ = nullptr;

}


Program::~Program() {
    delete model_;

}

void Program::show(UI selection) {
    state_ = selection;
    startUI(selection);
}
/*
void Program::closeProgram()
{
    qDebug() << "Uusi Exit";
    QApplication::quit();
}
*/
interface::ProgramModelInterface *Program::getModel() {
    return model_;

}

void Program::startUI(UI selection) {

    if (selectionUi_ != nullptr) {
        selectionUi_->deleteLater();
    }
    switch (selection) {
        case SelectionUI:
            selectionUi_ = new SelectorWindow;
            selectionUi_->setProgram(this);
            selectionUi_->updateSettings();
            selectionUi_->show();
            break;
        case MinimalUI:
            minimalUi_ = new MainWindowMinimal;
            minimalUi_->setProgram(this);
            minimalUi_->updateSettings();
            minimalUi_->show();
            break;
        case FullUI:
            fullUi_ = new MainWindow;
            fullUi_->setProgram(this);
            fullUi_->updateSettings();
            fullUi_->show();
            break;
        case Exit:
            qDebug() << "Doh";
            QApplication::quit();
            break;
    }

}

} // my_program
