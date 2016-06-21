#ifndef APUOHJELMAT_HH
#define APUOHJELMAT_HH

#include <QByteArray>
#include <QJsonObject>
namespace my_program {

    QJsonObject parse_json_data(const QByteArray &data_as_JSON);
    bool write_json_to_file(const QJsonObject &json_data);
}

#endif // APUOHJELMAT_HH
