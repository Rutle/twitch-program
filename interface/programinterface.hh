#ifndef PROGRAMINTERFACE_HH
#define PROGRAMINTERFACE_HH

#include "stream.hh"
#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>

namespace my_program {
namespace interface {

class ProgramInterface {
    public:
        ProgramInterface() = default;
        virtual ~ProgramInterface() = default;

        /**
         * @brief searchChannel Function to search a channel with given string
         * and add data to cInfo widget inherited from Channelinfo-class.
         * @param cInfo Instance of a Channelinfo class which base class is
         * QWidget.
         * @param channel Channel string.
         * @return True if channel was found, otherwise False.
         */
        virtual bool searchChannel(QWidget* cInfo, QString channel) = 0;

        virtual bool fetchFollowedChannels() = 0;

        virtual void buildFollowsPage(QListWidget *qList, QStackedWidget *qStack) = 0;

        virtual const QList<Stream> &getFStreamData() const = 0;

        virtual bool getCOnlineStatus(QString channelName) const = 0;

        virtual void updateFollowedStatus() = 0;
    private:
};
}
}
#endif // PROGRAMINTERFACE_HH
