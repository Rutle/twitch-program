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
        void set_stream_details(const QJsonObject &json);

        QString get_channel_name() const;
        QUrl get_url_value(const QString &key) const;
        QString get_data_value(const QString &key) const;


private:
        /*
        class Channel;
        std::shared_ptr<Channel> channel_;

        QString game_;
        unsigned int viewers_;
        unsigned int delay_;
        // When the stream started:
        QDate created_at_;
        QUrl preview_medium_url_;
        bool online_status_;
        */
        // http://www.learncpp.com/cpp-tutorial/47-structs/ towards the end
        // When stream is online:
        struct Stream_details {
            QString game_;
            unsigned int viewers_;
            unsigned int delay_;
            // When the stream started:
            QDate created_at_;
            QUrl preview_medium_url_;
            bool online_status_;
        };
        std::shared_ptr<Stream_details> stream_details_;

        // General Channel/Stream details:
        struct Channel_details {
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
        std::shared_ptr<Channel_details> channel_details_;

};
}
#endif // STREAM_HH
