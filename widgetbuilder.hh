#ifndef WIDGETBUILDER_HH
#define WIDGETBUILDER_HH

#include "stream.hh"
#include "utilityprograms.hh"
#include "programmodelinterface.hh"
#include "channelinfo.hh"
#include "infomessage.hh"
#include <QWidget>
#include <QGridLayout>

namespace my_program {

class WidgetBuilder {
    public:
        enum WidgetType { ErrorLabel, InfoLabel, ChannelPage };
        WidgetBuilder();
        WidgetBuilder(interface::ProgramModelInterface *parentModel);
        QWidget *buildQListItem(const Stream &channel, const UIMODE &mode) const;
        QGridLayout *buildTopGamePage(QGridLayout *gameGrid);
        static QWidget *buildWidget(WidgetType type, QString *message = nullptr,
                                    Stream *stream = nullptr,
                                    const int &width = 300,
                                    const int &height = 25);

    private:
        QWidget *buildLabel(WidgetType type, const QString &msg,
                            const int &width, const int &height);
        my_program::interface::ProgramModelInterface *parentModel_;
};

} // my_program


#endif // WIDGETBUILDER_HH
