#ifndef PROGRAMMODEL_HH
#define PROGRAMMODEL_HH

#include "programmodelinterface.hh"
#include "networkmanager.hh"
#include "stream.hh"
#include "settings.hh"
#include "widgetbuilder.hh"

namespace my_program {
class WidgetBuilder;

class ProgramModel : public interface::ProgramModelInterface {
    public:
        ProgramModel();
        ~ProgramModel();

        bool searchChannel(QStackedWidget *qStack, QWidget *message, QString channel);
        bool fetchFollowedChannels(QWidget *message, const UIMODE &mode);
        void buildFollowsPage(QListWidget *qList, const UIMODE &mode, QStackedWidget *qStack = nullptr);
        const QList<Stream> &getFStreamData() const;
        bool getCOnlineStatus(QString channelName) const;
        bool updateFollowedStatus(QWidget *message, const UIMODE &mode);
        bool updateSummaryLabels(QLabel *viewers, QLabel *channels,
                                 QWidget *message);
        bool updateTopGames(QStackedWidget *qStack, QListView *topGamesList, QWidget *message);
        bool changeTopGamePage(QString name, int pageNum, QStackedWidget *qStack, QWidget *message);
        Settings *getSettings();
    private:
        bool checkFollowedOnlineStatus(QWidget *qStack);

        Networkmanager nam_;
        //Settings *settingsData_;
        std::shared_ptr<Settings> settingsData_;
        QMap<QString, bool> followedOnlineStatus_;
        QList<Stream> followedStreamData_;
        std::shared_ptr<WidgetBuilder> builder_;
        QHash<QString, QList<my_program::Stream>> topGamesPages_;
        enum Functions { Search, FetchFollowed, UpdateFollowed, UpdateSummary ,
                         ChangeTopGame, UpdateTopGames, CheckFollowed};
        QMap<Functions, QMap<Networkmanager::Status, QString>> errorMessages_;
        bool getData(QJsonObject &data, QWidget *message, Functions caller);
        void retrieveChannelLogo(my_program::Stream& channel);

};
}
#endif // PROGRAM_HH
