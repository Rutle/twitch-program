#ifndef MINIINFO_HH
#define MINIINFO_HH

#include "stream.hh"
#include <QWidget>
#include <QPushButton>
#include <QUrl>
#include <QObject>

namespace my_program {
class MiniInfo : public QWidget {
        Q_OBJECT
    public:
        explicit MiniInfo(const my_program::Stream &stream_obj, QWidget *parent = 0);
        // To give support for StyleSheets:
        void paintEvent(QPaintEvent *paint_event);

    signals:

    public slots:
        void on_url_button_clicked();
    private:
        QUrl url_;
};
}
#endif // MINIINFO_HH
