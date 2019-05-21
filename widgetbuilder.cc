#include "widgetbuilder.hh"
#include <QLabel>
#include <QDebug>
#include <QScrollArea>

namespace my_program {

WidgetBuilder::WidgetBuilder() {

}

WidgetBuilder::WidgetBuilder(my_program::interface::ProgramInterface *parentModel) {
    parentModel_ = parentModel;
    qDebug() << "Widgetbuilder built";
}

/**
 * @brief WidgetBuilder::buildQListItem Builds an item for Follow tab's QListWidget.
 * @param channel A reference to a channel instance.
 * @return A QWidget pointer to a built widget for QListWidget.
 */
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
        labelList->setStyleSheet("QLabel {"
                                  "padding: 1px;"
                                  "}");
        onlineStatus->setStyleSheet("QLabel { background-color: #4CAF50; }");
    // Channel is offline:
    } else {
        labelList->setStyleSheet("QLabel {"
                                  "padding: 1px "
                                  "}");
        onlineStatus->setStyleSheet("QLabel { background-color: #FF5722; }");
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

} // my_program

