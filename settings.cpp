#include "settings.hh"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

namespace my_program {

Settings::Settings() {
    load_from_file();
}

void Settings::read(const QJsonObject &json) {
    user_name_ = json["user_name"].toString();
}

void Settings::write(QJsonObject &json) const {
    json["user_name"] = user_name_;
}

bool Settings::load_from_file() {
    QFile load_file(QStringLiteral("settings.json"));

    if (!load_file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open settings file.");
        return false;
    }
    QByteArray save_data = load_file.readAll();

    QJsonDocument load_doc(QJsonDocument::fromJson(save_data));
    read(load_doc.object());
    return true;
}

bool Settings::save_to_file() const {
    QFile save_file(QStringLiteral("settings.json"));

    if (!save_file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open settings file.");
        return false;
    }

    QJsonObject settings_object;
    write(settings_object);

    QJsonDocument save_doc(settings_object);
    save_file.write(save_doc.toJson());
    return true;
}

void Settings::set_user_name(const QString &name) {
    user_name_ = name;

}

QString Settings::give_user_name() const{
    return user_name_;
}
}
