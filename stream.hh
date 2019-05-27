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
        Stream(QJsonObject *json);

        void setStreamDetails(QJsonObject *json);
        QString getChannelName() const;
        QUrl getUrlValue(const QString &key) const;
        QString getDataValue(const QString &key) const;
        QString getGame() const;
        QString getStreamStart() const;
        double getViewers() const;
        double getFollowersCount() const;
        double getDelay() const;
        bool isOnline() const;
        QImage getLogo() const;
        bool checkLogoStatus() const;
        void saveLogo(QImage &logo);


    private:
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
