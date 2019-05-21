#ifndef WIDGETBUILDER_HH
#define WIDGETBUILDER_HH
#include "stream.hh"
#include "utilityprograms.hh"
#include "programinterface.hh"
#include <QWidget>
#include <QGridLayout>

namespace my_program {

class WidgetBuilder {
    public:
        WidgetBuilder();
        WidgetBuilder(my_program::interface::ProgramInterface *parentModel);
        QWidget *buildQListItem(const my_program::Stream &channel) const;
        QGridLayout *buildTopGamePage(QGridLayout *gameGrid);

    private:
        my_program::interface::ProgramInterface *parentModel_;
};

} // my_program


#endif // WIDGETBUILDER_HH
