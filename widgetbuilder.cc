#include "widgetbuilder.hh"
#include "channelinfo.hh"
#include <QLabel>
#include <QDebug>
#include <QScrollArea>
#include <QDesktopServices>
#include "clickablelabel.hh"


namespace my_program {

WidgetBuilder::WidgetBuilder() {

}

WidgetBuilder::WidgetBuilder(interface::ProgramModelInterface *parentModel) {
    parentModel_ = parentModel;
    qDebug() << "Widgetbuilder built";
}

/**
 * @brief WidgetBuilder::buildQListItem Builds an item for Follow tab's QListWidget.
 * @param channel A reference to a channel instance.
 * @return A QWidget pointer to a built widget for QListWidget.
 */
QWidget *WidgetBuilder::buildQListItem(const Stream &channel, const UIMODE &mode) const {

    QString channelName{channel.getChannelName()};
    QWidget *widget{new QWidget()};
    QLabel *nameLabel{new QLabel(channelName)};
    QLabel *onlineStatus{new QLabel()};


    switch (mode) {
        case Mini: {

            nameLabel->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Minimum);
            nameLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            nameLabel->setMinimumHeight(30);
            nameLabel->setContentsMargins(0, 0, 0, 0);

            QLabel *gameLabel{new QLabel(channel.getGame())};
            gameLabel->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Minimum);
            gameLabel->setAlignment(Qt::AlignCenter | Qt::AlignRight);
            gameLabel->setMinimumHeight(30);
            gameLabel->setContentsMargins(0, 0, 0, 0);
            //onlineStatus->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
            onlineStatus->setFixedSize(10, 30);
            onlineStatus->setSizePolicy(QSizePolicy::Fixed,
                                        QSizePolicy::Fixed);
            onlineStatus->setContentsMargins(0, 0, 0, 0);
            ClickableLabel *btn{new ClickableLabel};
            btn->setText(">");

            btn->setFixedSize(15, 30);
            QObject::connect(btn, &ClickableLabel::clicked,
                             [=](){
                QDesktopServices::openUrl(channel.getUrlValue("url")); });
            // Channel is online:
            if ( parentModel_->getCOnlineStatus(channelName) ) {
                nameLabel->setStyleSheet("QLabel {"
                                         "padding: 1px;"
                                         "font: bold 10px;"
                                         "background-color: rgba(48, 48, 48, 0.4);"
                                         "border-bottom: 1px solid #5b5b5b;"

                                         "}");
                gameLabel->setStyleSheet("QLabel {"
                                         "padding: 2px;"
                                         "font: bold 10px;"
                                         "background-color: rgba(48, 48, 48, 0.4);"
                                         "border-bottom: 1px solid #5b5b5b;"
                                         "}");
                onlineStatus->setStyleSheet("QLabel { background-color: #4CAF50; }");
                btn->setStyleSheet("QLabel {"
                                   "font: bold 10px;"
                                   "background-color: rgba(48, 48, 48, 0.4);"
                                   "border-bottom: 1px solid #5b5b5b;"
                                   "text-decoration: underline;"
                                   "}");
            // Channel is offline:
            } else {
                nameLabel->setStyleSheet("QLabel {"
                                         "padding: 2px;"
                                         "font: bold 10px;"
                                         "background-color: rgba(46, 32, 32, 0.4);"
                                         "border-bottom: 1px solid #5b5b5b;"
                                         "}");
                gameLabel->setStyleSheet("QLabel {"
                                         "padding: 2px;"
                                         "font: bold 10px;"
                                         "background-color: rgba(46, 32, 32, 0.4);"
                                         "border-bottom: 1px solid #5b5b5b;"
                                         "}");
                onlineStatus->setStyleSheet("QLabel { background-color: #FF5722; }");
                btn->setStyleSheet("QLabel {"
                                   "font: bold 10px;"
                                   "background-color: rgba(46, 32, 32, 0.4);"
                                   "border-bottom: 1px solid #5b5b5b;"
                                   "text-decoration: underline;"
                                   "}");
            }

            // QListWidgetItem:
            QGridLayout *gridLayout{new QGridLayout};
            gridLayout->setSpacing(0);
            gridLayout->setContentsMargins(0, 0, 0, 0);
            gridLayout->addWidget(onlineStatus, 0, 0);
            gridLayout->addWidget(nameLabel, 0, 1);
            gridLayout->addWidget(gameLabel, 0, 2);
            gridLayout->addWidget(btn, 0, 3);
            //widget->setMinimumHeight(33);
            widget->setContentsMargins(0, 0, 0, 1);
            widget->setLayout(gridLayout);

        } break;
        case Full: {
            nameLabel->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
            nameLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);
            nameLabel->setFixedSize(135, 20);
            onlineStatus->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
            onlineStatus->setFixedSize(5, 20);

            // Channel is online:
            if ( parentModel_->getCOnlineStatus(channelName) ) {
                nameLabel->setStyleSheet("QLabel {"
                                          "padding: 1px;"
                                          "}");
                onlineStatus->setStyleSheet("QLabel { background-color: #4CAF50; }");
            // Channel is offline:
            } else {
                nameLabel->setStyleSheet("QLabel {"
                                          "padding: 1px "
                                          "}");
                onlineStatus->setStyleSheet("QLabel { background-color: #FF5722; }");
            }

            // QListWidgetItem:
            QGridLayout *gridLayout{new QGridLayout};
            gridLayout->setMargin(0);
            gridLayout->setSpacing(0);
            gridLayout->addWidget(onlineStatus, 0, 0);
            gridLayout->addWidget(nameLabel, 0, 1);
            widget->setLayout(gridLayout);
        } break;

    }

    return widget;
}

QGridLayout *WidgetBuilder::buildTopGamePage(QGridLayout *gameGrid) {
    QScrollArea *pageScrollArea{new QScrollArea()};
    pageScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pageScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pageScrollArea->setStyleSheet("QScrollArea { border: 0; background-color: transparent; }");

    QWidget *scrollAreaWidget{new QWidget()};

    gameGrid->setHorizontalSpacing(10);
    gameGrid->setVerticalSpacing(10);

    gameGrid->setSizeConstraint(QLayout::SetMinAndMaxSize);
    gameGrid->setContentsMargins(0, 0, 0, 0);
    scrollAreaWidget->setLayout(gameGrid);
    //scrollAreaWidget->setStyleSheet("QWidget { border: 0; background-color: transparent; }");
    scrollAreaWidget->setMinimumWidth(850);
    pageScrollArea->setWidget(scrollAreaWidget);

    // game_grid-layout is the base grid where QScrollArea is put in.
    QGridLayout *baseGameGrid{new QGridLayout()};
    baseGameGrid->setContentsMargins(10, 0, 10, 0);
    baseGameGrid->addWidget(pageScrollArea);
    return baseGameGrid;
}

QWidget *WidgetBuilder::buildWidget(WidgetBuilder::WidgetType type,
                                    QString *message,
                                    Stream *streamObj,
                                    const int &width, const int &height)

{
    switch (type) {
        case ErrorLabel:
            return new InfoMessage(InfoMessage::Error, message, width, height);
        case InfoLabel:
            return new InfoMessage(InfoMessage::Info, message, width, height);
        case ChannelPage:
            widgets::Channelinfo *tmp{new widgets::Channelinfo()};
            tmp->setValues(streamObj);
            return tmp;
    }

}

QWidget *WidgetBuilder::buildLabel(WidgetBuilder::WidgetType type, const QString &msg, const int &width, const int &height)
{

}

} // my_program

