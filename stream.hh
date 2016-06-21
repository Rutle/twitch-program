#ifndef STREAM_HH
#define STREAM_HH
#include "channel.hh"
#include <QString>
#include <QDate>
#include <memory>
#include <QUrl>

namespace my_program {

class Channel{};
class Stream {
    public:
        Stream();
        ~Stream();
    private:
        std::unique_ptr<Channel> channel_;
        QString game_;
        unsigned int viewers_;
        unsigned int delay_;
        QDate created_at_;
        QUrl preview_medium_;



};
}
#endif // STREAM_HH
