#include "channelinfo.hh"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>

namespace my_program {
namespace widgets {

Channelinfo::Channelinfo(QWidget *parent) : QWidget(parent) {
    labels_["logo"] = nullptr;
    labels_["display_name"] = nullptr;
    labels_["game"] = nullptr;
    labels_["creation_time"] = nullptr;
    labels_["viewers"] = nullptr;
    labels_["followers"] = nullptr;
    labels_["url_to_stream"] = nullptr;
    labels_["preview_picture"] = nullptr;
    labels_["status"] = nullptr;

    urlBtn_ = new QPushButton("-> To Stream");
    urlBtn_->setObjectName("url_button");
    connect(urlBtn_, SIGNAL(clicked()), this, SLOT(on_url_button_clicked()));
    buildEmptyPage();

}

Channelinfo::~Channelinfo() {
    for ( auto item : labels_ ) {
        delete item.second;
    }
    delete urlBtn_;

}

void Channelinfo::setValues(my_program::Stream *stream) {
    labels_.at("logo")->setPixmap(QPixmap::fromImage(stream->getLogo().scaled(QSize(200, 200))));
    if(labels_.at("logo") == nullptr) {
        qDebug() << "null";
    }
    labels_.at("logo")->setAlignment(Qt::AlignHCenter);
    QString name(QStringLiteral("Name: ") + stream->getDataValue(QStringLiteral("display_name")));
    labels_.at("display_name")->setText(name);

    labels_.at("game")->setText("Game: " + stream->getGame());

    if ( stream->isOnline() ) {
        QDateTime stream_start = QDateTime::fromString(stream->getStreamStart(), "yyyy'-'MM'-'dd'T'hh:mm:ss'Z'");
        //qDebug() << "QString: " << stream.getStreamStart();
        //qDebug() << "QDateTime.toString(): " << stream_start.toString();
        labels_.at("creation_time")->setText("Started: " + stream_start.time().toString());
    } else {
        labels_.at("creation_time")->setText(QStringLiteral("Offline"));
    }

    if ( stream->getViewers() != 0 ) {
        labels_.at("viewers")->setText("Viewers: " + QString::number(stream->getViewers()));
    } else {
        labels_.at("viewers")->setText("Viewers: " + QStringLiteral("-"));
    }

    labels_.at("followers")->setText("Followers: " + QString::number(stream->getFollowersCount()));
    //labels_.at("url_to_stream")->setText("Url: " + stream.getUrlValue("url").toString());

    url_ = stream->getUrlValue("url");
    // labels_.value("preview_picture");
    labels_.at("status")->setText("Status: " + stream->getDataValue("status"));

}

void Channelinfo::on_url_button_clicked() {
    qDebug() << "Url button clicked!";
    QDesktopServices::openUrl(url_);

}

void Channelinfo::buildEmptyPage() {
    QHBoxLayout *layout_base_hbox = new QHBoxLayout;
    layout_base_hbox->setContentsMargins(0, 0, 0, 0);
    layout_base_hbox->setSpacing(0);

    // Left side of stacked page:
    QVBoxLayout *layout_left_vbox = new QVBoxLayout;
    // Right side of stacked page:
    QVBoxLayout *layout_right_vbox = new QVBoxLayout;
    // Logo, display_name, game, created_at, viewers, followers, url
    QLabel *logo = new QLabel(QStringLiteral("Logo: "));
    labels_["logo"] = logo;

    QLabel *display_name = new QLabel(QStringLiteral("Name: "));
    labels_["display_name"] = display_name;

    QLabel *game = new QLabel();
    game->setFixedSize(200, 22);
    game->setText("Game: ");
    labels_["game"] = game;

    QLabel *created_at_time = new QLabel(QStringLiteral("Started: "));
    labels_["creation_time"] = created_at_time;

    QLabel *viewers = new QLabel();
    labels_["viewers"] = viewers;

    QLabel *followers = new QLabel(QStringLiteral("Followers: "));
    labels_["followers"] = followers;

    //QLabel *url_to_stream = new QLabel(QStringLiteral("Url: "));
    //labels_["url_to_stream"] = url_to_stream;

    // big preview, status
    QLabel *preview_picture = new QLabel(QStringLiteral("Preview_picture: "));
    labels_["preview_picture"] = preview_picture;

    QLabel *status = new QLabel(QStringLiteral("Status: "));
    labels_["status"] = status;

    logo->setFixedSize(200, 200);
    display_name->setFixedSize(200, 22);
    created_at_time->setFixedSize(200, 22);
    viewers->setFixedSize(200, 22);
    followers->setFixedSize(200, 22);
    urlBtn_->setFixedSize(200, 22);

    preview_picture->setFixedSize(500, 300);
    status->setFixedSize(500, 22);

    layout_left_vbox->addWidget(logo);
    layout_left_vbox->addWidget(display_name);
    layout_left_vbox->addWidget(game);
    layout_left_vbox->addWidget(created_at_time);
    layout_left_vbox->addWidget(viewers);
    layout_left_vbox->addWidget(followers);
    layout_left_vbox->addWidget(urlBtn_);
    layout_left_vbox->addStretch();
    layout_left_vbox->setMargin(0);
    layout_left_vbox->setContentsMargins(0, 0, 0, 0);
    layout_left_vbox->setSpacing(0);

    layout_right_vbox->addWidget(preview_picture);
    layout_right_vbox->addWidget(status);
    layout_right_vbox->addStretch();
    layout_right_vbox->setMargin(0);
    layout_right_vbox->setContentsMargins(10, 0, 0, 0);


    layout_base_hbox->addLayout(layout_left_vbox);
    layout_base_hbox->addLayout(layout_right_vbox);
    layout_base_hbox->addStretch();

    QString stylesheet{"QLabel { margin-bottom: 1px;"
                       "border: 1px solid #5b5b5b; padding: 1px; }"};
    this->setStyleSheet(stylesheet);
    this->setLayout(layout_base_hbox);
}

} // widgets
} // my_program
