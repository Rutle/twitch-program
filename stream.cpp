#include "networkmanager.hh"
#include "stream.hh"
#include <QDebug>
#include <QImageReader>
#include <QFileDialog>

namespace my_program {

Stream::Stream() //:
    //stream_details_{std::make_shared<Stream_details>(Stream_details{})},
    //channel_details_{std::make_shared<Channel_details>(Channel_details{})}
    {

}

Stream::Stream(QJsonObject *json) :
    stream_details_{std::make_shared<Stream_details>(Stream_details{})},
    channel_details_{std::make_shared<Channel_details>(Channel_details{})}
    {
    stream_details_->game_ = json->value("game").toString();
    stream_details_->viewers_ = 0;
    stream_details_->delay_ = 0;
    stream_details_->created_at_ = QString("Offline");
    stream_details_->preview_medium_url_ = QUrl();
    stream_details_->online_status_ = false;

    channel_details_->mature_ = json->value("mature").toBool();
    channel_details_->partner_ = json->value("partner").toBool();
    channel_details_->id_ = json->value("_id").toDouble();

    channel_details_->urls_["logo"] = QUrl(json->value("logo").toString());
    channel_details_->urls_["video_banner"] = QUrl(json->value("video_banner").toString());
    channel_details_->urls_["profile_banner"] = QUrl(json->value("profile_banner").toString());
    channel_details_->urls_["url"] = QUrl(json->value("url").toString());

    channel_details_->data_["display_name"] = json->value("display_name").toString();
    channel_details_->data_["broadcaster_language"] = json->value("broadcaster_language").toString();
    channel_details_->data_["status"] = json->value("status").toString();
    // Name under which the channel was created aka. username.
    channel_details_->data_["name"] = json->value("name").toString();
    channel_details_->data_["language"] = json->value("language").toString();
    channel_details_->data_["created_at"] = json->value("created_at").toString();
    channel_details_->data_["updated_at"] = json->value("updated_at").toString();
    channel_details_->followers_ = json->value("followers").toDouble();
    //setLogo();



}

void Stream::setStreamDetails(QJsonObject *json) {
    stream_details_->viewers_ = json->value("viewers").toDouble();
    stream_details_->delay_ = json->value("delay").toDouble();
    stream_details_->created_at_ = json->value("created_at").toString();
    stream_details_->preview_medium_url_ = QUrl(json->value("preview").toObject().value("medium").toString());
    stream_details_->online_status_ = true;
}

QString Stream::getChannelName() const {
    if ( channel_details_->data_["name"].isEmpty() ) {
        //qWarning() << "Value of data_[name] is empty!";
    }
    return channel_details_->data_["name"];
}

QUrl Stream::getUrlValue(const QString &key) const {
    if ( channel_details_->urls_[key].isEmpty() ) {
        //qWarning() << "Value of urls_[" << key << "] is empty!";
    }

    return channel_details_->urls_[key];
}
QString Stream::getDataValue(const QString &key) const {
    if ( channel_details_->data_[key].isEmpty() ) {
        //qWarning() << "Value of [" << key << "] is empty!";
    }
    return channel_details_->data_[key];
}

QString Stream::getGame() const {
    if ( stream_details_->game_.isEmpty() ) {
        //qWarning() << "Value of [game_] is empty!";
    }
    return stream_details_->game_;
}

QString Stream::getStreamStart() const {
    if ( stream_details_->created_at_.isEmpty() ) {
        //qWarning() << "Value of [created_] is empty!";
    }
    return stream_details_->created_at_;

}

double Stream::getViewers() const {
    if ( stream_details_->viewers_ == 0) {
        //qWarning() << "Value of [viewers_] is empty!";
        return 0;
    }
    return stream_details_->viewers_;
}

double Stream::getFollowersCount() const {
    if ( channel_details_->followers_ == 0 ) {
        //qWarning() << "Value of [followers_] is empty!";
        return 0;
    }
    return channel_details_->followers_;
}

bool Stream::isOnline() const {
    if (stream_details_->online_status_ == false ) {
        return false;
    }
    return true;
}

QImage Stream::getLogo() const {
    if ( channel_details_->logo_.isNull() ) {
        qWarning() << "No logo image!";
    }
    return channel_details_->logo_;

}
// Check if logo has been downloaded previously.
bool Stream::checkLogoStatus() const {
    QString username(channel_details_->data_["name"]);
    QString logo_dir_png(QDir::currentPath()+"/user_pictures/"+username+".png");
    QString logo_dir_jpeg(QDir::currentPath()+"/user_pictures/"+username+".jpeg");

    if ( QFile::exists(logo_dir_png) ) {
        channel_details_->logo_ = QImage(logo_dir_png);
        //qDebug() << "Image loaded from .png file!";
        return true;
    } else if ( QFile::exists(logo_dir_jpeg) ) {
        channel_details_->logo_ = QImage(logo_dir_jpeg);
        //qDebug() << "Image loaded from .jpeg file!";
        return true;
    } else {
        qDebug() << "Fetch image.";
    }
    return false;
}

void Stream::saveLogo(QImage &logo) {
    channel_details_->logo_ = std::move(logo);
}

}
