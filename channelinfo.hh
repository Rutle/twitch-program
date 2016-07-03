#ifndef CHANNELINFO_HH
#define CHANNELINFO_HH

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

    signals:

    public slots:

    private:
        void build_empty_page();
        std::map<QString, QLabel *> labels_;
        /*
        std::shared_ptr<QLabel> logo_;
        std::shared_ptr<QLabel> display_name_;
        std::shared_ptr<QLabel> game_;
        std::shared_ptr<QLabel> creation_time_;
        std::shared_ptr<QLabel> viewers_;
        std::shared_ptr<QLabel> followers_;
        std::shared_ptr<QLabel> url_to_stream_;
        std::shared_ptr<QLabel> preview_picture_;
        std::shared_ptr<QLabel> status_;
        */
};
}
#endif // CHANNELINFO_HH
