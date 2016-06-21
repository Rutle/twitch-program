#ifndef STREAM_HH
#define STREAM_HH

#include <QString>
#include <QHash>
#include <QUrl>
#include <QDate>

namespace my_program {

class Channel {
    public:
        Channel();
        ~Channel();
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

}
#endif // STREAM_HH
