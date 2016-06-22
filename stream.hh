#ifndef STREAM_HH
#define STREAM_HH

#include <QString>
#include <QDate>
#include <memory>
#include <QUrl>
#include <QJsonObject>
#include <QHash>

namespace my_program {

class Stream {

    public:
        Stream(const QJsonObject &json);
        void set_channel_details(const QJsonObject &json);
    private:
        class Channel;
        std::shared_ptr<Channel> channel_;
        QString game_;
        unsigned int viewers_;
        unsigned int delay_;
        QDate created_at_;
        QUrl preview_medium_;

};
}
#endif // STREAM_HH
