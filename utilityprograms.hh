#ifndef UTILITYPROGRAMS_HH
#define UTILITYPROGRAMS_HH

#include <QByteArray>
#include <QJsonObject>
#include <QImage>

namespace my_program {
    struct Game {
        QString name;
        double popularity;
        QImage logo;
        double viewers;
        double channels;
    };
    QJsonObject parse_json_data(const QByteArray &data_as_JSON);
    bool write_json_to_file(const QJsonObject &json_data, QString name="json_data.json");
}

#endif // UTILITYPROGRAMS_HH
