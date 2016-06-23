#include "stream.hh"

#include <memory>
#include <utility>
#include <QDebug>
namespace my_program {
/*
class Stream::Channel {
    public:
        ~Channel() {}
        void set_details(const QJsonObject &json) {
            mature_ = json.value("mature").toBool();
            partner_ = json.value("partner").toBool();
            id_ = json.value("_id").toInt();
            QString temp_date{json.value("created_at").toString().left(10)};
            QString temp_time{json.value("created_at").toString().right(11)};
            created_at_ = QDate::fromString(temp_date, "yyyy'-'mm'-'dd");
            created_at_time_ = QTime::fromString(temp_time, "hh':'mm':'ss'Z'");
            temp_date = json.value("updated_at").toString().left(10);
            temp_time = json.value("updated_at").toString().right(11);
            updated_at_ = QDate::fromString(temp_date, "yyyy'-'mm'-'dd");
            updated_at_time_ = QTime::fromString(temp_time, "hh':'mm':'ss'Z'");

            //QUrl logo_;
            //QUrl view_banner_;
            //QUrl profile_banner_;
            //QUrl url_;


            urls_["logo"] = QUrl(json.value("logo").toString());
            urls_["video_banner"] = QUrl(json.value("video_banner").toString());
            urls_["profile_banner"] = QUrl(json.value("profile_banner").toString());
            urls_["url"] = QUrl(json.value("url").toString());

            data_["display_name"] = json.value("display_name").toString();
            data_["broadcaster_language"] = json.value("broadcaster_language").toString();
            data_["status"] = json.value("status").toString();
            // Name under which the channel was created.
            data_["name"] = json.value("name").toString();
            data_["language"] = json.value("language").toString();
        }
        QUrl get_url_value(const QString &key) const {
            if ( urls_[key].isEmpty() ) {
                qWarning() << "Value of [" << key << "] is empty!";
            }
            return urls_[key];
        }
        QString get_data_value(const QString &key) const {
            if ( urls_[key].isEmpty() ) {
                qWarning() << "Value of [" << key << "] is empty!";
            }
            return data_[key];
        }


    private:
        bool mature_;
        bool partner_;
        // Name, status etc.
        QHash<QString, QString> data_;
        unsigned int id_;
        QDate created_at_;
        QTime created_at_time_;
        QDate updated_at_;
        QTime updated_at_time_;
        // Banner urls etc.
        QHash<QString, QUrl> urls_;
        unsigned int followers_;

};
*/
/*
Channel::Channel() :
    mature_{false},
    partner_{false},
    data_{QHash<QString, QString>()},
    id_{0},
    created_at_{QDate()},
    updated_at_{QDate()},
    urls_{QHash<QString, QUrl>()},
    followers_{0} {

}

void Channel::set_details(const QJsonObject &json) {


}

Stream::Stream() :
    channel_(new Channel()),
    game_{QStringLiteral("None")},
    viewers_{0},
    delay_{0},
    created_at_{QDate()},
    preview_medium_{QUrl()} {

}

Stream::~Stream() {

}
*/
Stream::Stream(const QJsonObject &json) :
    stream_details_{new Stream_details{}},
    channel_details_{new Channel_details{}}
    {
    stream_details_->game_ = json.value("game").toString();
    stream_details_->viewers_ = 0;
    stream_details_->delay_ = 0;
    stream_details_->created_at_ = QDate();
    stream_details_->preview_medium_url_ = QUrl();
    stream_details_->online_status_ = false;

    channel_details_->mature_ = json.value("mature").toBool();
    channel_details_->partner_ = json.value("partner").toBool();
    channel_details_->id_ = json.value("_id").toInt();
    QString temp_date{json.value("created_at").toString().left(10)};
    QString temp_time{json.value("created_at").toString().right(11)};
    channel_details_->created_at_ = QDate::fromString(temp_date, "yyyy'-'mm'-'dd");
    channel_details_->created_at_time_ = QTime::fromString(temp_time, "hh':'mm':'ss'Z'");
    temp_date = json.value("updated_at").toString().left(10);
    temp_time = json.value("updated_at").toString().right(11);
    channel_details_->updated_at_ = QDate::fromString(temp_date, "yyyy'-'mm'-'dd");
    channel_details_->updated_at_time_ = QTime::fromString(temp_time, "hh':'mm':'ss'Z'");

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

}

void Stream::set_stream_details(const QJsonObject &json) {
    stream_details_->viewers_ = json.value("viewers").toInt();
    stream_details_->delay_ = json.value("delay").toInt();
    QString temp_date{json.value("created_at").toString().left(10)};
    stream_details_->created_at_ = QDate::fromString(temp_date, "yyyy'-'mm'-'dd");
    stream_details_->preview_medium_url_ = QUrl(json.value("preview").toObject().value("medium").toString());
    stream_details_->online_status_ = true;
}

QString Stream::get_channel_name() const {
    return channel_details_->data_["name"];
}

QUrl Stream::get_url_value(const QString &key) const {
    if ( channel_details_->urls_[key].isEmpty() ) {
        qWarning() << "Value of [" << key << "] is empty!";
    }

    return channel_details_->urls_[key];
}
QString Stream::get_data_value(const QString &key) const {
    if ( channel_details_->data_[key].isEmpty() ) {
        qWarning() << "Value of [" << key << "] is empty!";
    }
    return channel_details_->data_[key];
}

// "created_at": "2016-06-21T14:37:24Z",
// QDate date = QDate::fromString("1MM12car2003", "d'MM'MMcaryyyy");
// date is 1 December 2003
// QDate data = QDate::fromString(date, "yyyy'-'mm'-'dd);
// QTime time = QTime::fromString("1mm12car00", "m'mm'hcarss");
// time is 12:01.00
// QTime time = QTime::fromString(time, "hh':'mm':'ss'Z'");

}
