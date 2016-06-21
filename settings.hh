#ifndef SETTINGS_HH
#define SETTINGS_HH

#include <QJsonObject>

namespace my_program {
class Settings {
    public:
        Settings();
        ~Settings();
        bool load_from_file();
        bool save_to_file() const;
        void set_user_name(const QString &name);
        QString give_user_name() const;
    private:
        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;

        QJsonObject settings_;
        QString user_name_;

};
}
#endif // SETTINGS_HH
