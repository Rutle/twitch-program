#include "miniinfo.hh"
#include <QGridLayout>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QDesktopServices>
#include <QDir>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>

namespace my_program {
namespace widgets {

MiniInfo::MiniInfo(const my_program::Stream &stream_obj, QWidget *parent) :
    QWidget(parent) {

    // this->setFixedSize(150, 220);
    // this->setSizePolicy(QSizePolicy::MinimumExpanding,
    //                      QSizePolicy::MinimumExpanding);
    QSize label_sizes(50, 25);
    QSize detail_label_sizes(100, 25);
    QGridLayout *base_layout = new QGridLayout;
    base_layout->setContentsMargins(0, 0, 0, 0);
    base_layout->setSpacing(0);


    QLabel *logo = new QLabel();
    logo->setObjectName("logo");
    QImage logo__;

    if ( stream_obj.getLogo().isNull() ) {
        qWarning() << "my_program::MiniInfo stream_obj.getLogo() is null! Name: ["
                   << stream_obj.getChannelName() << "]";
        QString logo_dir_jpeg(QDir::currentPath()+"/user_pictures/no-avatar.jpg");
        QImage image{QImage(logo_dir_jpeg)};
        logo__ = image;
    } else {
        logo__ = stream_obj.getLogo();
    }
    logo__ = logo__.scaled(150, 150, Qt::IgnoreAspectRatio);
    logo->setPixmap(QPixmap::fromImage(logo__));
    logo->setFixedSize(150, 150);

    base_layout->addWidget(logo, 0, 0, 1, 2);


    QLabel *display_name_label = new QLabel(QStringLiteral("Name:"));
    QLabel *display_name = new QLabel(stream_obj.getChannelName());

    display_name_label->setFixedSize(label_sizes);
    display_name->setFixedSize(detail_label_sizes);

    base_layout->addWidget(display_name_label, 1, 0);
    base_layout->addWidget(display_name, 1, 1);

    QLabel *viewers_label = new QLabel(QStringLiteral("Viewers:"));
    QLabel *viewers = new QLabel(QString::number(stream_obj.getViewers()));
    viewers_label->setFixedSize(label_sizes);
    viewers->setFixedSize(detail_label_sizes);

    base_layout->addWidget(viewers_label, 2, 0);
    base_layout->addWidget(viewers, 2, 1);

    QLabel *url_label = new QLabel(QStringLiteral("Url:"));
    QPushButton *url_button = new QPushButton(QStringLiteral("-> To Stream"));

    url_button->setObjectName("url_button");
    connect(url_button, SIGNAL(clicked()), this, SLOT(on_url_button_clicked()));
    url_ = stream_obj.getUrlValue("url");

    url_label->setFixedSize(label_sizes);
    url_button->setFixedSize(detail_label_sizes);
//background-color: #1f1f1f;
    base_layout->addWidget(url_label, 3, 0);
    base_layout->addWidget(url_button, 3, 1);
    this->setObjectName("MiniWidget");
    QString stylesheet{"QLabel { background-color: #2f3c54; color: #DDDDDD; border: 0px; font: bold 10px; padding: 1px; "
                       "margin-bottom: 1px; margin-right: 1px; }"
                       "QLabel#logo { background-color: #2f3c54; margin-bottom: 2px; }"
                       "QPushButton#url_button { background-color: #2f3c54; margin-bottom: 1px; margin-right: 1px;"
                       "color: #DDDDDD; border: 1px; font: bold 10px; padding 1px; text-align: left; }"
                       "QPushButton#url_button:hover { background-color: #516C8D; border: 1px solid #304163; padding: 1px;"
                       "font: bold 10px; color: #DDDDDD; margin-bottom: 1px; margin-right: 1px; text-align:left; }"
                       "QPushButton#url_button:!enabled { background-color:rgb(61, 63, 94); border: 1px solid #304163;"
                       "padding: 1px; font: bold 10px; color: #DDDDDD; margin-bottom: 1px; margin-right: 1px; text-align: left;}"
                       "QWidget#MiniWidget { background-color: #25324c; margin: 1px; }"};
    this->setStyleSheet(stylesheet);
    this->setLayout(base_layout);


}
// Had to do this to make subclassed QWidget able to support StyleSheets.
void MiniInfo::paintEvent(QPaintEvent *paint_event) {
    QStyleOption option;
    option.initFrom(this);
    QPainter new_painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &new_painter, this);

}

void MiniInfo::on_url_button_clicked() {
    qDebug() << "Url button clicked!";
    QDesktopServices::openUrl(url_);

}
}
}
