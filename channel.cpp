#include "channel.hh"

namespace my_program {

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
// "created_at": "2016-06-21T14:37:24Z",
// QDate date = QDate::fromString("1MM12car2003", "d'MM'MMcaryyyy");
// date is 1 December 2003
Channel::~Channel() {

}
}
