#include "utilityprograms.hh"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QIODevice>


namespace my_program {

QJsonObject parse_json_data(const QByteArray &data_as_JSON) {

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data_as_JSON);
    QJsonObject jsonObj = jsonDoc.object();

    return jsonObj;
}

bool write_json_to_file(const QJsonObject &json_data) {
    QFile jsonsave_file(QStringLiteral("json_data.json"));
    if (!jsonsave_file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonDocument save_doc(json_data);
    jsonsave_file.write(save_doc.toJson());

    return true;
}
}

