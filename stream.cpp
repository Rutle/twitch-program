#include "stream.hh"
namespace my_program {

Stream::Stream() :
    channel_{nullptr},
    game_{QStringLiteral("None")},
    viewers_{0},
    delay_{0},
    created_at_{QDate()},
    preview_medium_{QUrl()}
{
    channel_ = std::unique_ptr<my_program::Channel>(new my_program::Channel());
}
// "created_at": "2016-06-21T14:37:24Z",
// QDate date = QDate::fromString("1MM12car2003", "d'MM'MMcaryyyy");
// date is 1 December 2003
}
