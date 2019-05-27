#include "infomessage.hh"

InfoMessage::InfoMessage(QWidget *parent) : QWidget(parent)
{

}

InfoMessage::~InfoMessage()
{
    label_->deleteLater();
}

InfoMessage::InfoMessage(InfoMessage::InfoType type, QString *msg,
                         const int &width, const int &height)
{

    label_ = new QLabel(*msg);
    label_->setFixedSize(width, height);
    label_->setAlignment(Qt::AlignHCenter);
    label_->setAlignment(Qt::AlignCenter);
    QVBoxLayout *baseLayout = new QVBoxLayout;
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setAlignment(Qt::AlignHCenter);
    baseLayout->setAlignment(Qt::AlignCenter);
    baseLayout->setSpacing(0);
    baseLayout->addWidget(label_);
    QString style{};
    switch (type) {
        case Error:
            style = QString{"QLabel {"
                    "background-color: #303030;"
                    "border-radius: 2px;"
                    "padding: 1px;"
                    "font: bold 10px;"
                    "border: 1px solid #e26a61;}"};
            break;
        case Info:
            style = QString{"QLabel {"
                    "background-color: #303030;"
                    "border-radius: 2px;"
                    "padding: 1px;"
                    "font: bold 10px;"
                    "border: 1px solid #5b5b5b;}"};
            break;
    }

    this->setStyleSheet(style);
    this->setLayout(baseLayout);
}

void InfoMessage::setText(const QString &msg)
{
    label_->setText(msg);
}
