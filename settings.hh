#ifndef SETTINGS_HH
#define SETTINGS_HH

#include <QJsonObject>

namespace my_program {
class Settings {
    public:
        Settings();
        bool loadFile();
        bool saveFile() const;
        void setUsername(const QString &name);
        QString getUsername() const;
    private:
        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;

        QJsonObject settings_;
        QString userName_;

};
}
#endif // SETTINGS_HH
