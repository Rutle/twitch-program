#ifndef STREAM_HH
#define STREAM_HH


#include <memory>
#include <QUrl>
#include <QJsonObject>
#include <QHash>
#include <QImage>

namespace my_program {

class Stream {

    public:
        Stream();
        Stream(const QJsonObject &json);

        void set_stream_details(const QJsonObject &json);
        QString get_channel_name() const;
        QUrl get_url_value(const QString &key) const;
        QString get_data_value(const QString &key) const;
        QString get_game() const;
        QString get_stream_start() const;
        double get_viewers() const;
        double get_followers() const;
        double get_delay() const;
        bool is_online() const;
        QImage get_logo() const;


    private:
        void set_logo() const;
        // When stream is online:
        struct Stream_details {
            QString game_;
            double viewers_;
            double delay_;
            // When the stream started:
            QString created_at_;
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
            double id_;
            //QString created_at_;
            //QString updated_at_;
            // Banner urls etc.
            QHash<QString, QUrl> urls_;
            double followers_;
            QImage logo_;
        };
        std::shared_ptr<Channel_details> channel_details_;

};
}
#endif // STREAM_HH
