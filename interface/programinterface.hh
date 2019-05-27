#ifndef PROGRAMINTERFACE_HH
#define PROGRAMINTERFACE_HH

//#include "stream.hh"
#include "programmodelinterface.hh"
#include <QWidget>
//#include <QListWidget>
//#include <QStackedWidget>
//#include <QLabel>
enum UI { SelectionUI, MinimalUI, FullUI, Exit };
namespace my_program {
namespace interface {

class ProgramInterface {
    public:
        ProgramInterface() = default;
        virtual ~ProgramInterface() = default;

        //virtual void searchChannel(QStackedWidget *qStack, QString channel) = 0;

        virtual void show(UI selection) = 0;
        //virtual void closeProgram() = 0;
        virtual ProgramModelInterface *getModel() = 0;

    private:
};
}
}
#endif // PROGRAMINTERFACE_HH
