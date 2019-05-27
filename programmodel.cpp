#include "programmodel.hh"
//#include "stream.hh"
#include "channelinfo.hh"
#include "topgameslistdelegate.hh"
#include "topgameslistmodel.hh"
#include "miniinfo.hh"

#include <QDir>
#include <QDebug>
#include <QJsonArray>
#include <QApplication>

const QString CLIENTID = "client_id=th5mqjz7wtxx0ut8cns0g22r3miuzz";
const QString API_URL = "https://api.twitch.tv/kraken/";

const QString SUMMARY = API_URL+"streams/summary?"+CLIENTID;
const QString TOPGAMES = API_URL+"games/top?"+CLIENTID;

namespace my_program {

ProgramModel::ProgramModel() {
    builder_ = std::make_shared<WidgetBuilder>(this);
    settingsData_ = std::unique_ptr<Settings>(new Settings());
    QMap<Networkmanager::Status, QString> search = {
        { Networkmanager::ContentNotFound, "Channel does not exist!" },
        { Networkmanager::ServiceUnavailable, "Service is unavailable!" },
        { Networkmanager::DefaultProblem, "There was a problem with the request to twitch API!" },
    };

    QMap<Networkmanager::Status, QString> fetchFollowed = {
        { Networkmanager::ContentNotFound, "Followed channels were not found!" },
        { Networkmanager::ServiceUnavailable, "Service is unavailable!" },
        { Networkmanager::DefaultProblem, "There was a problem with the request to twitch API!" },
    };

    QMap<Networkmanager::Status, QString> updateFollowed = {
        { Networkmanager::ContentNotFound, "There was a problem getting response for followed channel statuses!" },
        { Networkmanager::ServiceUnavailable, "Twitch API service is unavailable!" },
        { Networkmanager::DefaultProblem, "There was a problem with the request to twitch API!" },
    };

    QMap<Networkmanager::Status, QString> updateSummary = {
        { Networkmanager::ContentNotFound, "Summary not found!" },
        { Networkmanager::ServiceUnavailable, "Twitch API service is unavailable!" },
        { Networkmanager::DefaultProblem, "There was a problem with the request to twitch API!" },
    };

    QMap<Networkmanager::Status, QString> changeTop = {
        { Networkmanager::ContentNotFound, "Top games data not found!" },
        { Networkmanager::ServiceUnavailable, "Twitch API service is unavailable!" },
        { Networkmanager::DefaultProblem, "There was a problem with the request to twitch API!" },
    };

    QMap<Networkmanager::Status, QString> updateTop = {
        { Networkmanager::ContentNotFound, "Top games data not found!" },
        { Networkmanager::ServiceUnavailable, "Twitch API service is unavailable!" },
        { Networkmanager::DefaultProblem, "There was a problem with the request to twitch API!" },
    };

    QMap<Networkmanager::Status, QString> checkFollowed = {
        { Networkmanager::ContentNotFound, "Unable to check online status for followed channels!" },
        { Networkmanager::ServiceUnavailable, "Twitch API service is unavailable!" },
        { Networkmanager::DefaultProblem, "There was a problem with the request to twitch API!" },
    };
    errorMessages_ = {
        { Search, search },
        { FetchFollowed, fetchFollowed },
        { UpdateFollowed, updateFollowed },
        { UpdateSummary, updateSummary },
        { ChangeTopGame, changeTop },
        { UpdateTopGames, updateTop },
        { CheckFollowed, checkFollowed }
    };
}

ProgramModel::~ProgramModel() {

}

bool ProgramModel::searchChannel(QStackedWidget *qStack, QWidget *message, QString channel) {

    QString request_url{API_URL+"channels/"+channel+"?"+CLIENTID};
    nam_.make_api_request(request_url);
    //widgets::Channelinfo *cW{nullptr};
    QJsonObject jsonDataObj{};

    if(!getData(jsonDataObj, message, Search) ) {
        return false;
    }
    qDebug() << jsonDataObj.keys();
    Stream streamObj(&jsonDataObj);
    if (!streamObj.checkLogoStatus()) {
        retrieveChannelLogo(streamObj);
    }

    qStack->addWidget(WidgetBuilder::buildWidget(WidgetBuilder::ChannelPage, nullptr, &streamObj));
    return true;
}

bool ProgramModel::fetchFollowedChannels(QWidget *message, const UIMODE &mode) {
    QString userName{settingsData_->getUsername()};
    QString requestUrl{API_URL+"users/"+userName+"/follows/channels?"+CLIENTID};
    QJsonObject followsJsonData{};
    nam_.make_api_request(requestUrl);

    if(!getData(followsJsonData, message, FetchFollowed) ) {
        qDebug() << "Follows data not found";
        return false;
    }

    QJsonValue followsData{followsJsonData.value("follows")};

    // Array of QJsonObjects where each one is a channel QJsonObject.
    for ( auto item : followsData.toArray() ) {
        QJsonObject followed = item.toObject();
        QJsonObject chl = followed["channel"].toObject();
        QJsonValue name = followed["channel"].toObject().value("name");
        my_program::Stream tempChannel(&chl);
        if (!tempChannel.checkLogoStatus()) {
            retrieveChannelLogo(tempChannel);
        }

        followedStreamData_.push_back(std::move(tempChannel));
        followedOnlineStatus_[name.toString()] = false;
    }
    if(!checkFollowedOnlineStatus(message)) {
        // Any problem found
        return false;
    }
    std::sort(followedStreamData_.begin(), followedStreamData_.end(),
              [](my_program::Stream a, my_program::Stream b){
        return a.isOnline() ? true : false;

    });

    return true;
}

void ProgramModel::buildFollowsPage(QListWidget *qList, const UIMODE &mode, QStackedWidget *qStack) {

    switch (mode) {
        case Mini: {
            for ( auto channel : followedStreamData_ ) {
                QWidget *widget = builder_->buildQListItem(channel, Mini);
                QListWidgetItem *listItem{new QListWidgetItem()};
                //listItem->set
                listItem->setSizeHint(QSize(0, 30));

                //qList->setContentsMargins(5, 0, 0, 0);
                qList->addItem(listItem);
                qList->setItemWidget(listItem, widget);

                QApplication::sendPostedEvents();

            }
            qList->setStyleSheet("QListWidget::item:selected { background-color: #565656; }"
                                 "QListWidget::item:hover { background-color: #565656; }");

        } break;
        case Full: {
            for ( auto channel : followedStreamData_ ) {
                // Widget to put in place of a QListWidgetItem:
                QWidget *widget = builder_->buildQListItem(channel, Full);

                QListWidgetItem *listItem{new QListWidgetItem()};

                listItem->setSizeHint(QSize(140, 21));
                qList->addItem(listItem);
                qList->setItemWidget(listItem, widget);

                // StackedWidget page:
                my_program::widgets::Channelinfo *channelWidget{new my_program::widgets::Channelinfo()};

                channelWidget->setValues(&channel);
                channelWidget->setContentsMargins(10, 0, 0, 0);
                qStack->addWidget(channelWidget);

                // Process an event meaning add it to the list and display it
                QApplication::sendPostedEvents();
            }
        } break;

    }

}

const QList<Stream> &ProgramModel::getFStreamData() const {
    return followedStreamData_;
}

bool ProgramModel::getCOnlineStatus(QString channelName) const {
    return followedOnlineStatus_[channelName];
}

bool ProgramModel::updateFollowedStatus(QWidget *message, const UIMODE &mode) {
    qDebug() << "updateFollowedStatus beginning";
    if(!checkFollowedOnlineStatus(message)) {
        return false;
    }
    qDebug() << "updateFollowedStatus end";
    return true;

}

bool ProgramModel::updateSummaryLabels(QLabel *viewers, QLabel *channels,
                                       QWidget *message) {

    nam_.make_api_request(SUMMARY);
    QJsonObject jsonData{};

    if(!getData(jsonData, message, UpdateSummary) ) {
        return false;
    }

    viewers->setText(QString::number(jsonData["viewers"].toInt()));
    channels->setText(QString::number(jsonData["channels"].toInt()));
    return true;
}

bool ProgramModel::updateTopGames(QStackedWidget *qStack, QListView *topGamesList, QWidget *message) {
    nam_.make_api_request(TOPGAMES);
    QJsonObject jsonData{};

    if(!getData(jsonData, message, UpdateTopGames) ) {
        qDebug() << "Couldn't get data";
        return false;
    }

    //QJsonObject jsonData = nam_.retrieve_json_data();
    QJsonValue jsonDataValue{jsonData.value("top")};

    std::vector<my_program::Game> topGames;
    for ( auto game : jsonDataValue.toArray() ) {

        QJsonObject gameObj = game.toObject();
        my_program::Game tempGame;
        tempGame.name = gameObj["game"].toObject().value("name").toString();

        tempGame.popularity = gameObj["game"].toObject().value("popularity").toDouble();

        QString templateUrl(gameObj["game"].toObject()["box"].toObject().value("template").toString());

        templateUrl.replace(QString("{width}"), QString("40"));
        templateUrl.replace(QString("{height}"), QString("40"));

        tempGame.viewers = gameObj.value("viewers").toDouble();
        tempGame.channels = gameObj.value("channels").toDouble();

        topGames.push_back(tempGame);

        QWidget *tempWidget{new QWidget()};
        qStack->addWidget(tempWidget);

    }
    /*
    if ( topGames.size() == 0 ) {
        //qWarning() << "Top_games.size() == 0";
        return false;
    }*/

    my_program::widgets::TopGamesListModel *model{new my_program::widgets::TopGamesListModel(topGames, topGamesList)};
    my_program::widgets::TopGamesListDelegate *delegate{new my_program::widgets::TopGamesListDelegate()};
    topGamesList->setModel(model);
    topGamesList->setItemDelegate(delegate);
    topGamesList->setVisible(true);
    topGamesList->setStyleSheet("QListView {background-color: transparent;}");
    return true;

}
// Retrieves data for top streams in index (game) and creates a widget
// which contains 25 smaller widgets with minimal stream info in a QScrollArea.
bool ProgramModel::changeTopGamePage(QString name, int pageNum,
                                     QStackedWidget *qStack, QWidget *message) {

    if ( topGamesPages_.contains(name) ) {
        qStack->setCurrentIndex(pageNum);
        qDebug() << name << " page already created. Switching to page: " << pageNum;
        return true;
    }

    // -!Remember to change client_id to header instead of part of the query url!-
    // -!Client_ID works differently with streams.!-
    QString requestUrl{API_URL+"streams?game="+name+"&"+CLIENTID};

    nam_.make_api_request(requestUrl);
    QJsonObject gameJsonData{};

    if(!getData(gameJsonData, message, ChangeTopGame) ) {
        return false;
    }

    //QJsonObject gameJsonData{nam_.retrieve_json_data()};
    if(gameJsonData.isEmpty()) {
        qWarning() << "Empty data at changeTopGamePage";
        return false;
    }
    qDebug() << gameJsonData.keys();
    QJsonValue streams{gameJsonData.value("streams")};

    QList<my_program::Stream> streamList;
    QGridLayout *scrollAreaGrid{new QGridLayout()};
    QGridLayout *gameGrid = builder_->buildTopGamePage(scrollAreaGrid);

    QWidget *gameWidget{qStack->widget(pageNum)};
    //gameWidget->setStyleSheet("QWidget { background-color: #1f1f1f; }");
    gameWidget->setLayout(gameGrid);
    qStack->setCurrentIndex(pageNum);

    // -!Could change this so that the QScrollArea gets updated after     !-
    // -!5 objects has been created so that it would feel a bit smoother. !-

    // Array of QJsonObjects where each one is a channel/stream QJsonObject.
    //unsigned int counter{0};
    unsigned int row{0};
    unsigned int column{0};
    for ( auto item :  streams.toArray() ) {
        QJsonObject stream = item.toObject();
        QJsonObject streamChannel = stream["channel"].toObject();
        // QJsonValue name = stream_channel["channel"].toObject().value("name");
        my_program::Stream tempStream(&streamChannel);
        tempStream.setStreamDetails(&stream);
        if (!tempStream.checkLogoStatus()) {
            retrieveChannelLogo(tempStream);
        }

        scrollAreaGrid->addWidget(new my_program::widgets::MiniInfo(tempStream),
                                  row, column);
        ++column;
        if ( column == 5 ) {
            ++row;
            column = 0;
            // Let the Application to process the creation of a full row of items.
            QApplication::sendPostedEvents();
        }
        streamList.push_back(tempStream);
    }
    topGamesPages_[name] = streamList;

    //main_top_games_data_[game] = streamList;
    //qStack->setStyleSheet("#main_top_stacked_widget {"
    //                                           "background-color: #1f1f1f;"
    //                                           "}");
    //qStack->setStyleSheet("#main_top_stacked_widget { border: 0;"
    //                      "background-color: #1f1f1f;"
    //                      "}");
    return true;

}

Settings *ProgramModel::getSettings() {
    return settingsData_.get();

}
// Check online status of each channel retrieved.
// QStackedWidget qStack: To add possible error message.
bool ProgramModel::checkFollowedOnlineStatus(QWidget *message) {
    qDebug() << "checkFollowedOnlineStatus beginning";
    QString channels;
    QMap<QString, bool>::iterator qmapIter;

    // String to add to url so that multiple channels can be looked up with
    // single network request.
    for ( qmapIter = followedOnlineStatus_.begin();
          qmapIter != followedOnlineStatus_.end(); qmapIter++ ) {
        channels += qmapIter.key()+",";
    }

    channels = channels.left(channels.size() - 1);

    QString url(API_URL+"streams?channel="+channels+"&"+CLIENTID);
    nam_.make_api_request(url);
    QJsonObject onlineData{};

    if(!getData(onlineData, message, CheckFollowed) ) {
        return false;
    }

    QJsonValue streams{onlineData.value("streams")};

    for ( auto channel : streams.toArray() ) {
        QJsonObject streamObj{channel.toObject()};
        QJsonValue streamName{streamObj["channel"].toObject().value("name")};
        followedOnlineStatus_[streamName.toString()] = true;
        for ( auto stream : followedStreamData_ ) {
            if (stream.getChannelName() == streamName.toString() ) {
                stream.setStreamDetails(&streamObj);
            }
        }
    }
    qDebug() << "checkFollowedOnlineStatus end";
    return true;
}

bool ProgramModel::getData(QJsonObject &data, QWidget *messageInfo,
                           Functions caller) {

    switch(nam_.getApiStatus()) {
        case Networkmanager::ContentFound: {
            qDebug() << "Data found!";
            data = nam_.retrieve_json_data();
            return true;
        }
        case Networkmanager::ContentNotFound: {
            QString message = errorMessages_.value(caller).value(
                        Networkmanager::ContentNotFound);
            messageInfo = WidgetBuilder::buildWidget(WidgetBuilder::ErrorLabel,
                                                     &message, nullptr);
            //messageInfo = new widgets::Channelinfo{message};
            return false;
        }
        case Networkmanager::ServiceUnavailable: {
            QString message = errorMessages_.value(caller).value(
                        Networkmanager::ServiceUnavailable);
            messageInfo = WidgetBuilder::buildWidget(WidgetBuilder::ErrorLabel,
                                                     &message, nullptr);
            return false;
        }
        case Networkmanager::DefaultProblem: {
            QString message = errorMessages_.value(caller).value(
                        Networkmanager::DefaultProblem);
            messageInfo = WidgetBuilder::buildWidget(WidgetBuilder::ErrorLabel,
                                                     &message, nullptr);
            return false;
        }
        default: {
            // Something totally unrelated really broke.
            QString message{"Something really broke with the request!"};
            messageInfo = WidgetBuilder::buildWidget(WidgetBuilder::ErrorLabel,
                                                     &message, nullptr);
            return false;
        }
    }
}

void ProgramModel::retrieveChannelLogo(Stream &channel) {
    //Networkmanager nam;
    QString username{channel.getChannelName()};
    QString pathPNG = (QDir::currentPath()+"/user_pictures/"+username+".png");
    QString pathJPG = (QDir::currentPath()+"/user_pictures/"+username+".jpeg");
    QUrl url(channel.getUrlValue("logo"));
    nam_.make_image_request(url);
    QImage logo = nam_.retrieve_image();
    if ( logo.format() == 5 ) {
        // PNG
        // qDebug() << "current path: " << path;
        //qDebug() << "Image loaded from .png url!";
        logo.save(pathPNG);
    } else if ( logo.format() == 4 ) {
        // JPEG
        // qDebug() << "current path: " << path;
        //qDebug() << "Image loaded from .jpeg url!";
        logo.save(pathJPG);
    }
    channel.saveLogo(logo);
}

} // my_program
