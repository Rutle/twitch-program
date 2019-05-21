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
#include <QPushButton>
#include <QUrl>

namespace my_program {
namespace widgets {

class Channelinfo : public QWidget {
    Q_OBJECT
    public:
        explicit Channelinfo(QWidget *parent = 0);
        Channelinfo(QString errorMessage);
        ~Channelinfo();
        void set_values(const my_program::Stream &stream);

    signals:

    public slots:
        void on_url_button_clicked();
    private:
        void build_empty_page();
        void buildErrorPage(QString message);
        QUrl url_;
        std::map<QString, QLabel *> labels_;
        QPushButton *url_button;
};
}
}

#endif // CHANNELINFO_HH
