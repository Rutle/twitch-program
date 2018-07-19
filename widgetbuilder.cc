#include "widgetbuilder.hh"
#include <QLabel>
#include <QDebug>
#include <QGridLayout>

namespace my_program {

WidgetBuilder::WidgetBuilder() {

}

WidgetBuilder::WidgetBuilder(my_program::interface::ProgramInterface *parentModel) {
    parentModel_ = parentModel;
    qDebug() << "Widgetbuilder built";
}

QWidget *WidgetBuilder::buildQListItem(const Stream &channel) const {

    QString channelName{channel.get_channel_name()};
    QWidget *widget{new QWidget()};
    QLabel *labelList{new QLabel(channelName)};
    QLabel *onlineStatus{new QLabel()};

    labelList->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    labelList->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    labelList->setFixedSize(135, 20);
    onlineStatus->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    onlineStatus->setFixedSize(5, 20);

    // Channel is online:
    if ( parentModel_->getCOnlineStatus(channelName) ) {
        labelList->setStyleSheet("QLabel { background-color: #2f3c54; "
                                  "color: #dddddd; padding: 1px; "
                                  "font: bold 10px; }");
        onlineStatus->setStyleSheet("QLabel { background-color: #4CAF50; }");

        qDebug() << "Channel label online: " << channelName;

    // Channel is offline:
    } else {
        labelList->setStyleSheet("QLabel { background-color: #2f3c54; "
                                  "color: #DDDDDD; padding: 1px; "
                                  "font: bold 10px; }");
        onlineStatus->setStyleSheet("QLabel { background-color: #FF5722; }");

        qDebug() << "Channel label offline: " << channelName;
    }

    // QListWidgetItem:
    QGridLayout *gridLayout{new QGridLayout};
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);
    gridLayout->addWidget(onlineStatus, 0, 0);
    gridLayout->addWidget(labelList, 0, 1);
    widget->setLayout(gridLayout);

    return widget;
}

} // my_program

