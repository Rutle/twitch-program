#include "networkmanager.hh"
#include <QDebug>
#include <QEventLoop>
#include <QUrl>
#include <QImageReader>
#include <QDir>

Networkmanager::Networkmanager(QObject *parent) : QObject(parent) {

}

Networkmanager::~Networkmanager() {

}

void Networkmanager::parse_api_response() {
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    qDebug() << "-> Parse_api_response <-";

    if ( reply->error() != QNetworkReply::NoError ) {
        // Error happened.
        qWarning() << "ErrorNo: " << reply->errorString();
        qDebug() << "Request failed, " << reply->errorString();
        qDebug() << "Headers:" << reply->rawHeaderList() << "content:" << reply->readAll();
        reply->deleteLater();
        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data_from_request = reply->readAll();

    retrieved_json_data_= my_program::parse_json_data(data_from_request);

    if ( retrieve_json_data().keys().size() == 0 ) {
        qDebug() << retrieve_json_data();
    }
    reply->deleteLater();
}

void Networkmanager::parse_image_response() {

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    qDebug() << "-> Parse_image_response <-";

    if ( reply->error() != QNetworkReply::NoError ) {
        // Error happened.
        qWarning() << "ErrorNo: " << reply->errorString();
        qDebug() << "Request failed, " << reply->errorString();
        qDebug() << "Headers: "<< reply->rawHeaderList() << "content:" << reply->readAll();

        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QImageReader image_reader(reply);
    //image_reader.setAutoDetectImageFormat(false);

    QImage data_from_request = image_reader.read();
    retrieved_image_ = data_from_request;
    reply->deleteLater();

}

void Networkmanager::make_api_request(QString outgoing_request) {
    QUrl url(outgoing_request);
    QNetworkRequest request(url);

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    request.setSslConfiguration(config);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //request.setRawHeader(QByteArray("Client-ID"), QByteArray("kotialthf6zsygxpvqfhgbf0wvblsv5"));
    QNetworkReply *reply = network_manager_.get(request);

    // Had to connect to reply's signal "finished()" and not network_manager's
    // signal "finished(reply)" because "listening" to network_manager's signal
    // started to multiply requests and replies.
    // Using eventloop to force the waiting for finished() signal.
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parse_api_response()));
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

}

QJsonObject Networkmanager::retrieve_json_data() {
    return retrieved_json_data_;
}

void Networkmanager::make_image_request(QUrl outgoing_request) {
    //QUrl url(outgoing_request);
    qDebug() << "Image request started: " << outgoing_request.toString();
    QNetworkRequest request(outgoing_request);
    if ( outgoing_request.toString() == "" ) {
        qWarning() << "Empty URL string!";
        QString logo_dir_jpeg(QDir::currentPath()+"/user_pictures/no-avatar.jpg");
        QImage image{QImage(logo_dir_jpeg)};
        retrieved_image_ = image;
        return;
    }
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    request.setSslConfiguration(config);
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //request.setRawHeader(QByteArray("Client-ID"), QByteArray("kotialthf6zsygxpvqfhgbf0wvblsv5"));
    QNetworkReply *reply = network_manager_.get(request);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(parse_image_response()));
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

}

QImage Networkmanager::retrieve_image() const {
    return retrieved_image_;
}

