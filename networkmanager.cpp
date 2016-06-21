#include "networkmanager.hh"
#include <QDebug>
#include <QEventLoop>
#include <QUrl>

Networkmanager::Networkmanager(QObject *parent) : QObject(parent) {

    connect(&network_manager_, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(parse_network_response(QNetworkReply*)));

}

Networkmanager::~Networkmanager() {

}

void Networkmanager::parse_network_response(QNetworkReply *finished) {
    if ( finished->error() != QNetworkReply::NoError ) {
        // Tapahtui virhe. Emittoidaan network_error-signaali.
        qWarning() << "ErrorNo: " << finished->errorString();
        qDebug() << "Request failed, " << finished->errorString();
        qDebug() << "Headers:"<<  finished->rawHeaderList()<< "content:" << finished->readAll();
        //emit network_error(finished->error());
        return;
    }

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QByteArray data_from_request = finished->readAll();

    // Emit data_ready_read-signal.
    retrieved_json_data_= my_program::parse_json_data(data_from_request);
    emit(data_ready_read(data_from_request));

}

void Networkmanager::make_request(QString outgoing_request) {
    QUrl url(outgoing_request);
    QNetworkRequest request(url);

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    request.setSslConfiguration(config);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //request.setRawHeader(QByteArray("Client-ID"), QByteArray("kotialthf6zsygxpvqfhgbf0wvblsv5"));
    QNetworkReply *reply = network_manager_.get(request);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    /*
    QNetworkRequest request;
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    //config.setProtocol(QSsl::TlsV1SslV3);
    request.setSslConfiguration(config);
    request.setUrl(QUrl(outgoing_request));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    network_manager_.get(request);
    */

}

QJsonObject Networkmanager::retrieve_json_data() {
    return retrieved_json_data_;
}

