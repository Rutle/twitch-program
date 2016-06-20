#ifndef TIEDONHAKIJA_HH
#define TIEDONHAKIJA_HH
#include "utilityprograms.hh"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class Networkmanager : public QObject {
        Q_OBJECT
    public:
        explicit Networkmanager(QObject *parent = 0);
        ~Networkmanager();
        void make_request(QString outgoing_request);
        QJsonObject retrieve_json_data();

    signals:
        void network_error(QNetworkReply::NetworkError net_error);
        void data_ready_read(QByteArray);


    public slots:
        void parse_network_response(QNetworkReply *finished);

    private:

        QNetworkAccessManager network_manager_;
        QJsonObject retrieved_json_data_;
};

#endif // TIEDONHAKIJA_HH
