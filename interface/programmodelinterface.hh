#ifndef PROGRAMMODELINTERFACE_HH
#define PROGRAMMODELINTERFACE_HH

#include "stream.hh"
#include "settings.hh"
#include "channelinfo.hh"
#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>

enum UIMODE { Mini, Full };
namespace my_program {
namespace interface {

class ProgramModelInterface {
    public:
        ProgramModelInterface() = default;
        virtual ~ProgramModelInterface() = default;

        /**
         * @brief searchChannel Function to search a channel with given string
         * and add data to cInfo widget inherited from Channelinfo-class.
         * @param cInfo Instance of a Channelinfo class which base class is
         * QWidget.
         * @param channel Channel string.
         * @return True if channel was found, otherwise False.
         */
        virtual bool searchChannel(QStackedWidget *qStack, QWidget *message, QString channel) = 0;

        virtual bool fetchFollowedChannels(QWidget *message, const UIMODE &mode) = 0;

        virtual void buildFollowsPage(QListWidget *qList, const UIMODE &mode, QStackedWidget *qStack = nullptr) = 0;

        virtual const QList<Stream> &getFStreamData() const = 0;

        virtual bool getCOnlineStatus(QString channelName) const = 0;

        virtual bool updateFollowedStatus(QWidget *message, const UIMODE &mode) = 0;

        virtual bool updateSummaryLabels(QLabel *viewers, QLabel *channels,
                                         QWidget *message) = 0;

        virtual bool updateTopGames(QStackedWidget *qStack, QListView *topGamesList, QWidget *message) = 0;

        virtual bool changeTopGamePage(QString name, int pageNum, QStackedWidget *qStack, QWidget *message) = 0;

        virtual Settings *getSettings() = 0;
    private:
};
}
}
#endif // PROGRAMMODELINTERFACE_HH
