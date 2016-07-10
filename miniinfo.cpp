#include "miniinfo.hh"
#include <QGridLayout>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QDesktopServices>
#include <QDir>
#include <QDebug>

namespace my_program {
MiniInfo::MiniInfo(const my_program::Stream &stream_obj, QWidget *parent) :
    QWidget(parent) {

    this->setFixedSize(150, 210);
    this->setSizePolicy(QSizePolicy::MinimumExpanding,
                          QSizePolicy::MinimumExpanding);
    QSize label_sizes(50, 20);
    QSize detail_label_sizes(100, 20);
    QGridLayout *base_layout = new QGridLayout;
    base_layout->setContentsMargins(0, 0, 0, 0);
    base_layout->setSpacing(0);


    QLabel *logo = new QLabel();
    QImage logo__;

    if ( stream_obj.get_logo().isNull() ) {
        QString logo_dir_jpeg(QDir::currentPath()+"/user_pictures/no-avatar.jpg");
        QImage image{QImage(logo_dir_jpeg)};
        logo__ = image;
    } else {
        logo__ = stream_obj.get_logo();
    }
    logo__ = logo__.scaled(150, 150, Qt::IgnoreAspectRatio);
    logo->setPixmap(QPixmap::fromImage(logo__));
    logo->setFixedSize(150, 150);

    base_layout->addWidget(logo, 0, 0, 1, 2);


    QLabel *display_name_label = new QLabel(QStringLiteral("Name:"));
    QLabel *display_name = new QLabel(stream_obj.get_channel_name());

    display_name_label->setFixedSize(label_sizes);
    display_name->setFixedSize(detail_label_sizes);

    base_layout->addWidget(display_name_label, 1, 0);
    base_layout->addWidget(display_name, 1, 1);

    QLabel *viewers_label = new QLabel(QStringLiteral("Viewers:"));
    QLabel *viewers = new QLabel(QString::number(stream_obj.get_viewers()));
    viewers_label->setFixedSize(label_sizes);
    viewers->setFixedSize(detail_label_sizes);

    base_layout->addWidget(viewers_label, 2, 0);
    base_layout->addWidget(viewers, 2, 1);

    QLabel *url_label = new QLabel(QStringLiteral("Url:"));
    QPushButton *url_button = new QPushButton(QStringLiteral("-> To Stream"));

    url_button->setObjectName(QStringLiteral("url_button"));
    connect(url_button, SIGNAL(clicked()), this, SLOT(on_url_button_clicked()));
    url_ = stream_obj.get_url_value("url");

    url_label->setFixedSize(label_sizes);
    url_button->setFixedSize(detail_label_sizes);

    base_layout->addWidget(url_label, 3, 0);
    base_layout->addWidget(url_button, 3, 1);

    QString stylesheet{"QLabel { background-color: #2f3c54; color: #DDDDDD; border: 0px; font: bold 10px; padding: 1px; "
                       "margin-bottom: 1px; margin-right: 1px; }"
                       "QPushButton#url_button { background-color: #2f3c54; margin-bottom: 1px; margin-right: 1px;"
                       "color: #DDDDDD; border: 0px; font: bold 10px; padding 1px; }"};
    this->setStyleSheet(stylesheet);
    this->setLayout(base_layout);

}

void MiniInfo::on_url_button_clicked() {
    qDebug() << "Url button clicked!";
    QDesktopServices::openUrl(url_);

}
}
