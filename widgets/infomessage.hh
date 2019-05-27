#ifndef INFOMESSAGE_HH
#define INFOMESSAGE_HH

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
class InfoMessage : public QWidget
{
        Q_OBJECT
    public:
        explicit InfoMessage(QWidget *parent = nullptr);
        ~InfoMessage();
        enum InfoType { Error, Info };
        InfoMessage(InfoType type, QString *msg, const int &width,
                    const int &height);
        void setText(const QString &msg);
    signals:

    public slots:

    private:
        QLabel *label_;
};

#endif // INFOMESSAGE_HH
