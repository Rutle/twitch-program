#include "stream.hh"
#include "channel.hh"
#include <memory>
#include <utility>
#include <QDebug>
namespace my_program {

class Stream::Channel {
    public:
        ~Channel() {}
        void set_details(const QJsonObject &json) {
            mature_ = json.value("mature").toBool();
            partner_ = json.value("partner").toBool();
            id_ = json.value("_id").toInt();
            QString temp_date{json.value("created_at").toString().left(10)};
            created_at_ = QDate::fromString(temp_date, "yyyy'-'mm'-'dd");
            temp_date = json.value("updated_at").toString().left(10);
            updated_at_ = QDate::fromString(temp_date, "yyyy'-'mm'-'dd");
            /*
            QUrl logo_;
            QUrl view_banner_;
            QUrl profile_banner_;
            QUrl url_;
            */

            urls_["logo"] = QUrl(json.value("logo").toString());
            urls_["video_banner"] = QUrl(json.value("video_banner").toString());
            urls_["profile_banner"] = QUrl(json.value("profile_banner").toString());
            urls_["url"] = QUrl(json.value("url").toString());
        }

    private:
        bool mature_;
        bool partner_;
        QHash<QString, QString> data_;
        unsigned int id_;
        QDate created_at_;
        QDate updated_at_;
        QHash<QString, QUrl> urls_;
        /*
        QUrl logo_;
        QUrl view_banner_;
        QUrl profile_banner_;
        QUrl url_;
        */
        unsigned int followers_;

};
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
    channel_(new Channel()),
    game_{json.value("game").toString()},
    viewers_{0},
    delay_{0},
    created_at_{QDate()},
    preview_medium_{QUrl()}{
    set_channel_details(json);
    /*
    QJsonValue name = followed_channel["channel"].toObject().value("name");
    followed_stream_data_.push_back(my_program::Stream(followed_channel["channel"].toObject()));
    */

}

void Stream::set_channel_details(const QJsonObject &json) {
    channel_->set_details(json);
}



// "created_at": "2016-06-21T14:37:24Z",
// QDate date = QDate::fromString("1MM12car2003", "d'MM'MMcaryyyy");
// date is 1 December 2003
// QDate data = QDate::fromString(date, "yyyy'-'mm'-'dd);
// QTime time = QTime::fromString("1mm12car00", "m'mm'hcarss");
// time is 12:01.00
// QTime time = QTime::fromString(time, "'T'hh':'mm':'ss'Z'");

}
