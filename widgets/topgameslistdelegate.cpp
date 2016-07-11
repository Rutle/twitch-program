#include "topgameslistdelegate.hh"
#include <QStyle>
#include <QApplication>
#include <QPainter>
#include <QDebug>
TopGamesListDelegate::TopGamesListDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {

}

void TopGamesListDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const {

    QStyleOptionViewItem opt = option;
    // Text font and color:
    QFont font{opt.font};
    font.setPixelSize(10);
    font.setBold(true);
    QColor text_color("#DDDDDD");

    QStringList info(index.model()->data(index.model()->index(index.row(), 0)).toStringList());
    QString name{info.at(0)};
    QString viewers{"Viewers: " + info.at(1)};
    QString channels{"Channels: "+ info.at(2)};

    // +4 is for padding:
    QRect rect_text{QRect(opt.rect.left()+4, opt.rect.top(), opt.rect.width(), 15)};
    QRect rect_text2{QRect(rect_text.left(), rect_text.bottom(), rect_text.width(), 15)};
    QRect rect_item{QRect(opt.rect.left(), opt.rect.top(), opt.rect.width(), opt.rect.height())};
    // QStyle *new_style = opt.widget ? opt.widget->style() : QApplication::style();

    // Brush to fill rectangle.
    // Pen to outline rectangle.
    // new_style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
    if ( opt.state & QStyle::State_Selected || opt.state & QStyle::State_MouseOver ) {
        QColor item_bg_color("#516C8D");
        // QColor bg_color{QColor(47, 60, 84)};
        QBrush bg_brush_normal(item_bg_color, Qt::SolidPattern);
        painter->setPen(Qt::NoPen);
        // This reduces the height by 1 so we can draw a line below.
        painter->fillRect(rect_item.adjusted(0,0,0,-1), bg_brush_normal);
        QColor bg_color{QColor(37, 50, 76)};
        QPen drawing_pen{bg_color};
        drawing_pen.setStyle(Qt::SolidLine);
        drawing_pen.setWidth(1);
        // Selected and Mouse Over:
        if ( opt.state & QStyle::State_MouseOver ) {
            // Selected, Mouse Over and Has Focus:
            painter->setFont(font);
            painter->setPen(text_color);
            if ( opt.state & QStyle::State_HasFocus ) {

                painter->drawText(rect_text, Qt::AlignTop | Qt::AlignLeft, name);
                painter->drawText(rect_text2, Qt::AlignTop | Qt::AlignLeft, viewers);
            } else {
                painter->drawText(rect_text, Qt::AlignTop | Qt::AlignLeft, name);
                painter->drawText(rect_text2, Qt::AlignTop | Qt::AlignLeft, viewers);
            }

        // Selected and NOT Mouse over:
        } else if ( !(opt.state & QStyle::State_MouseOver) ) {
            painter->setFont(font);
            painter->setPen(text_color);
            painter->drawText(rect_text, Qt::AlignTop | Qt::AlignLeft, name);
            painter->drawText(rect_text2, Qt::AlignTop | Qt::AlignLeft, viewers);
        }

        // Selected, Mouse over and Focus:
        // Solid line->
    } else {
        // QColor bg_color{"#304163"};
        QColor bg_color{QColor(37, 50, 76)};
        QPen drawing_pen{bg_color};
        drawing_pen.setStyle(Qt::SolidLine);
        drawing_pen.setWidth(1);

        //QColor item_bg_color("#28385e");
        QColor item_bg_color{QColor(47, 60, 84)};
        QBrush bg_brush_normal(item_bg_color, Qt::SolidPattern);
        painter->setPen(Qt::NoPen);
        painter->fillRect(rect_item.adjusted(0, 0, 0, -1), bg_brush_normal);

        painter->setPen(drawing_pen);
        painter->drawLine(rect_item.bottomLeft(), rect_item.bottomRight());

        painter->setFont(font);
        painter->setPen(text_color);
        // Name
        // Viewers
        painter->drawText(rect_text, Qt::AlignTop | Qt::AlignLeft, name);
        painter->drawText(rect_text2, Qt::AlignTop | Qt::AlignLeft, viewers);
    }
}

// Alloocate each item size in listview. Sadly only useful when list is
// initialized.
QSize TopGamesListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const {

    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(30);
    //result.setWidth(160);
    return result;

}
