#include "stream.hh"
#include <QDebug>

namespace my_program {

Stream::Stream() :
    stream_details_{std::make_shared<Stream_details>(Stream_details{})},
    channel_details_{std::make_shared<Channel_details>(Channel_details{})}
    {

}

Stream::Stream(const QJsonObject &json) :
    stream_details_{std::make_shared<Stream_details>(Stream_details{})},
    channel_details_{std::make_shared<Channel_details>(Channel_details{})}
    {
    stream_details_->game_ = json.value("game").toString();
    stream_details_->viewers_ = 0;
    stream_details_->delay_ = 0;
    stream_details_->created_at_ = QString("Offline");
    stream_details_->preview_medium_url_ = QUrl();
    stream_details_->online_status_ = false;

    channel_details_->mature_ = json.value("mature").toBool();
    channel_details_->partner_ = json.value("partner").toBool();
    channel_details_->id_ = json.value("_id").toDouble();
    //QString temp_date{json.value("created_at").toString().left(10)};
    //QString temp_time{json.value("created_at").toString().right(11)};
    //channel_details_->created_at_ = QDate::fromString(temp_date, "yyyy'-'mm'-'dd");
    //channel_details_->created_at_time_ = QTime::fromString(temp_time, "hh':'mm':'ss'Z'");
    //temp_date = json.value("updated_at").toString().left(10);
    //temp_time = json.value("updated_at").toString().right(11);
    //channel_details_->updated_at_ = QDate::fromString(temp_date, "yyyy'-'mm'-'dd");
    //channel_details_->updated_at_time_ = QTime::fromString(temp_time, "hh':'mm':'ss'Z'");

    channel_details_->urls_["logo"] = QUrl(json.value("logo").toString());
    channel_details_->urls_["video_banner"] = QUrl(json.value("video_banner").toString());
    channel_details_->urls_["profile_banner"] = QUrl(json.value("profile_banner").toString());
    channel_details_->urls_["url"] = QUrl(json.value("url").toString());

    channel_details_->data_["display_name"] = json.value("display_name").toString();
    channel_details_->data_["broadcaster_language"] = json.value("broadcaster_language").toString();
    channel_details_->data_["status"] = json.value("status").toString();
    // Name under which the channel was created aka. username.
    channel_details_->data_["name"] = json.value("name").toString();
    channel_details_->data_["language"] = json.value("language").toString();
    channel_details_->data_["created_at"] = json.value("created_at").toString();
    channel_details_->data_["updated_at"] = json.value("updated_at").toString();
    channel_details_->followers_ = json.value("followers").toDouble();

}

void Stream::set_stream_details(const QJsonObject &json) {

    qDebug() << "viewers type: " <<json.value("viewers").type();
    stream_details_->viewers_ = json.value("viewers").toDouble();
    qDebug() << "vdelay type: " <<json.value("viewers").type();
    stream_details_->delay_ = json.value("delay").toDouble();
    //QString temp_date{json.value("created_at").toString().left(10)};
    stream_details_->created_at_ = json.value("created_at").toString();
    //temp_date = json.value("created_at").toString().right(11);
    //stream_details_->created_at_time_ = QDate::fromString(temp_date, "hh':'mm':'ss'Z'");
    stream_details_->preview_medium_url_ = QUrl(json.value("preview").toObject().value("medium").toString());
    stream_details_->online_status_ = true;
}

QString Stream::get_channel_name() const {
    if ( channel_details_->data_["name"].isEmpty() ) {
        qWarning() << "Value of data_[name] is empty!";
    }
    return channel_details_->data_["name"];
}

QUrl Stream::get_url_value(const QString &key) const {
    if ( channel_details_->urls_[key].isEmpty() ) {
        qWarning() << "Value of urls_[" << key << "] is empty!";
    }

    return channel_details_->urls_[key];
}
QString Stream::get_data_value(const QString &key) const {
    if ( channel_details_->data_[key].isEmpty() ) {
        qWarning() << "Value of [" << key << "] is empty!";
    }
    return channel_details_->data_[key];
}

QString Stream::get_game() const {
    if ( stream_details_->game_.isEmpty() ) {
        qWarning() << "Value of [game_] is empty!";
    }
    return stream_details_->game_;
}

QString Stream::get_stream_start() const {
    if ( stream_details_->created_at_.isEmpty() ) {
        qWarning() << "Value of [created_] is empty!";
    }
    return stream_details_->created_at_;

}

double Stream::get_viewers() const {
    if ( stream_details_->viewers_ == 0) {
        qWarning() << "Value of [viewers_] is empty!";
        return 0;
    }
    return stream_details_->viewers_;
}

double Stream::get_followers() const {
    if ( channel_details_->followers_ == 0 ) {
        qWarning() << "Value of [followers_] is empty!";
        return 0;
    }
    return channel_details_->followers_;
}

bool Stream::is_online() const {
    if (stream_details_->online_status_ == false ) {
        return false;
    }
    return true;
}

// "created_at": "2016-06-21T14:37:24Z",
// QDate date = QDate::fromString("1MM12car2003", "d'MM'MMcaryyyy");
// date is 1 December 2003
// QDate data = QDate::fromString(date, "yyyy'-'mm'-'dd);
// QTime time = QTime::fromString("1mm12car00", "m'mm'hcarss");
// time is 12:01.00
// QTime time = QTime::fromString(time, "hh':'mm':'ss'Z'");

}
