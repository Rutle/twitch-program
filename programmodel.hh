#ifndef PROGRAMMODEL_HH
#define PROGRAMMODEL_HH

#include "programinterface.hh"
#include "networkmanager.hh"
#include "stream.hh"
#include "settings.hh"
#include "widgetbuilder.hh"

namespace my_program {
class WidgetBuilder;

class ProgramModel : public interface::ProgramInterface {
    public:
        ProgramModel();
        ProgramModel(Settings *settings);
        void searchChannel(QStackedWidget *qStack, QString channel);
        bool fetchFollowedChannels(QStackedWidget *qStack);
        void buildFollowsPage(QListWidget *qList, QStackedWidget *qStack);
        const QList<Stream> &getFStreamData() const;
        bool getCOnlineStatus(QString channelName) const;
        void updateFollowedStatus(QStackedWidget *qStack);
        bool updateSummaryLabels(QLabel *viewers, QLabel *channels,
                                 QStackedWidget *qStack);
        bool updateTopGames(QStackedWidget *qStack, QListView *topGamesList);
        bool changeTopGamePage(QString name, int pageNum, QStackedWidget *qStack);
    private:
        bool checkFollowedOnlineStatus(QStackedWidget *qStack);

        Networkmanager nam_;
        Settings *settingsData_;

        QMap<QString, bool> followedOnlineStatus_;
        QList<Stream> followedStreamData_;
        std::shared_ptr<WidgetBuilder> builder_;
        QHash<QString, QList<my_program::Stream>> topGamesPages_;
        enum Functions { Search, FetchFollowed, UpdateFollowed, UpdateSummary ,
                         ChangeTopGame, UpdateTopGames, CheckFollowed};
        QMap<Functions, QMap<Networkmanager::Status, QString>> errorMessages_;
        bool getData(QJsonObject &data, QStackedWidget *qStack, Functions caller);
        void retrieveChannelLogo(my_program::Stream& channel);

};
}
#endif // PROGRAM_HH
