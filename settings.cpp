#include "settings.hh"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

namespace my_program {

Settings::Settings() {
    loadFile();
}

void Settings::read(const QJsonObject &json) {
    userName_ = json["user_name"].toString();
}

void Settings::write(QJsonObject &json) const {
    json["user_name"] = userName_;
}

bool Settings::loadFile() {
    QFile file(QStringLiteral("settings.json"));

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open settings file.");
        return false;
    }
    QByteArray saveData = file.readAll();

    QJsonDocument doc(QJsonDocument::fromJson(saveData));
    read(doc.object());
    return true;
}

bool Settings::saveFile() const {
    QFile file(QStringLiteral("settings.json"));

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open settings file.");
        return false;
    }

    QJsonObject settingsObj;
    write(settingsObj);

    QJsonDocument doc(settingsObj);
    file.write(doc.toJson());
    return true;
}

void Settings::setUsername(const QString &name) {
    userName_ = name;

}

QString Settings::getUsername() const {
    return userName_;
}
}
