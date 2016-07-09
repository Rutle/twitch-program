#ifndef CHANNELINFO_HH
#define CHANNELINFO_HH
/* Channelinfo:
 * This class is used to build a page containing channel information provided
 * by the Twitch API.
 *
 *
 *
*/
#include "stream.hh"
#include <QWidget>
#include <memory>
#include <QLabel>
#include <QString>
#include <map>

namespace my_program {
class Channelinfo : public QWidget {
    Q_OBJECT
    public:
        explicit Channelinfo(QWidget *parent = 0);
        ~Channelinfo();
        void set_values(const my_program::Stream &stream);
        static QWidget* from_json_mini_info();

    signals:

    public slots:

    private:
        void build_empty_page();
        std::map<QString, QLabel *> labels_;
};
}
#endif // CHANNELINFO_HH
