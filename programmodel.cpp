#include "programmodel.hh"
//#include "stream.hh"
#include "channelinfo.hh"

#include <QDebug>
#include <QJsonArray>
#include <QApplication>

const QString CLIENTID = "client_id=th5mqjz7wtxx0ut8cns0g22r3miuzz";
const QString API_URL = "https://api.twitch.tv/kraken/";

const QString SUMMARY = API_URL+"streams/summary?"+CLIENTID;
const QString TOPGAMES = API_URL+"games/top?"+CLIENTID;

namespace my_program {

ProgramModel::ProgramModel() {
    qDebug() << "Program created";
}

ProgramModel::ProgramModel(Settings *settings) {
    settingsData_ = settings;
    qDebug() << "Model for user: " << settingsData_->give_user_name() << " created.";
    builder_ = std::make_shared<WidgetBuilder>(this);
    qDebug() << "Program created";
    //qDebug() << settingsData_->give_user_name()

}

bool ProgramModel::searchChannel(QWidget* cInfo, QString channel) {

    QString request_url{API_URL+"channels/"+channel+"?"+CLIENTID};
    nam_.make_api_request(request_url);
    QJsonObject json_data_obj{nam_.retrieve_json_data()};

    if ( json_data_obj["error"] == "Not Found" ) {
        qWarning() << "Channel [" << channel << "] does not exist!";
        return false;
    }
    Stream stream_obj(json_data_obj);
    widgets::Channelinfo* cW = dynamic_cast<widgets::Channelinfo *>(cInfo);

    cW->set_values(stream_obj);

    return true;
}

bool ProgramModel::fetchFollowedChannels() {
    QString userName{settingsData_->give_user_name()};
    QString requestUrl{API_URL+"users/"+userName+"/follows/channels?"+CLIENTID};

    nam_.make_api_request(requestUrl);
    QJsonObject followsJsonData{nam_.retrieve_json_data()};

    qDebug() << followsJsonData["_total"];
    if ( followsJsonData["_total"] == 0 ) {
        qDebug() << "No followed channels";
        return false;
    }

    QJsonValue followsData{followsJsonData.value("follows")};

    // Array of QJsonObjects where each one is a channel QJsonObject.
    for ( auto item : followsData.toArray() ) {
        QJsonObject followedChannel = item.toObject();
        QJsonValue name = followedChannel["channel"].toObject().value("name");
        my_program::Stream tempChannel(followedChannel["channel"].toObject());
        followedStreamData_.push_back(std::move(tempChannel));
        followedOnlineStatus_[name.toString()] = false;
    }
    checkFollowedOnlineStatus();
    return true;
}

void ProgramModel::buildFollowsPage(QListWidget *qList, QStackedWidget *qStack) {

    for ( auto channel : followedStreamData_ ) {
        // Widget to put in place of a QListWidgetItem:
        QWidget *widget = builder_->buildQListItem(channel);

        QListWidgetItem *listItem{new QListWidgetItem()};

        listItem->setSizeHint(QSize(140, 21));
        qList->addItem(listItem);
        qList->setItemWidget(listItem, widget);

        // StackedWidget page:
        my_program::widgets::Channelinfo *channelWidget{new my_program::widgets::Channelinfo()};

        channelWidget->set_values(channel);
        channelWidget->setContentsMargins(10, 0, 0, 0);
        qStack->addWidget(channelWidget);

        // Process an event meaning add it to the list and display it
        QApplication::sendPostedEvents();
    }

}

const QList<Stream> &ProgramModel::getFStreamData() const {
    return followedStreamData_;
}

bool ProgramModel::getCOnlineStatus(QString channelName) const {
    return followedOnlineStatus_[channelName];
}

void ProgramModel::updateFollowedStatus() {
    qDebug() << "updateFollowedStatus beginning";
    checkFollowedOnlineStatus();
    qDebug() << "updateFollowedStatus end";
}

void ProgramModel::checkFollowedOnlineStatus() {
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

    QJsonObject onlineData{nam_.retrieve_json_data()};
    //qDebug() << "channel_online_status: data retrieved.";
    QJsonValue streams{onlineData.value("streams")};

    qDebug() << onlineData["_total"];
    if ( onlineData["_total"] == 0 ) {
        qDebug() << "All channels offline";
        return;
    }
    for ( auto channel : streams.toArray() ) {
        QJsonObject streamObj{channel.toObject()};
        QJsonValue streamName{streamObj["channel"].toObject().value("name")};
        followedOnlineStatus_[streamName.toString()] = true;
        for ( auto stream : followedStreamData_ ) {
            if (stream.get_channel_name() == streamName.toString() ) {
                stream.set_stream_details(streamObj);
                //qDebug() << "Channel [" << stream_name.toString() << "] online!";
            }
        }
    }
    qDebug() << "checkFollowedOnlineStatus end";
}
} // my_program
