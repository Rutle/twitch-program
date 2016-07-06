#ifndef TIEDONHAKIJA_HH
#define TIEDONHAKIJA_HH

#include "utilityprograms.hh"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImage>

class Networkmanager : public QObject {
        Q_OBJECT
    public:
        explicit Networkmanager(QObject *parent = 0);
        ~Networkmanager();
        void make_api_request(QString outgoing_request);
        QJsonObject retrieve_json_data();
        void make_image_request(QUrl outgoing_request);
        QImage retrieve_image() const;

    signals:
        void network_error(QNetworkReply::NetworkError net_error);
        void data_ready_read(QByteArray);


    public slots:
        void parse_api_response();
        void parse_image_response();

    private:
        QNetworkAccessManager network_manager_;
        QJsonObject retrieved_json_data_;
        QImage retrieved_image_;
};

#endif // TIEDONHAKIJA_HH
