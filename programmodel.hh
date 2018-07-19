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
        bool searchChannel(QWidget* cInfo, QString channel);
        bool fetchFollowedChannels();
        void buildFollowsPage(QListWidget *qList, QStackedWidget *qStack);
        const QList<Stream> &getFStreamData() const;


        bool getCOnlineStatus(QString channelName) const;
    private:
        void checkOnlineStatus();


        Networkmanager nam_;
        Settings *settingsData_;

        QMap<QString, bool> followedOnlineStatus_;
        QList<Stream> followedStreamData_;
        std::shared_ptr<WidgetBuilder> builder_;

};
}
#endif // PROGRAM_HH
