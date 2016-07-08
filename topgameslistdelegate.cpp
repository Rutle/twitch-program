#include "topgameslistdelegate.hh"
#include <QStyle>
#include <QApplication>
#include <QPainter>
#include <QDebug>
TopGamesListDelegate::TopGamesListDelegate(QObject *parent)
    : QStyledItemDelegate(parent){

}

void TopGamesListDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const {

    QStyleOptionViewItem opt = option;
    // Text font and color:
    QFont font(opt.font);
    font.setPixelSize(10);
    font.setBold(true);
    QColor text_color("#dddddd");

    QStringList info(index.model()->data(index.model()->index(index.row(), 0)).toStringList());
    QString name = info.at(0);
    QString viewers = info.at(1);
    viewers = QString("Viewers: " + viewers);
    QRect rect;
    QRect rect_text{QRect(opt.rect.left()+4, opt.rect.top(), opt.rect.width(), 15)};
    QRect rect_text2{QRect(rect_text.left(), rect_text.bottom(), rect_text.width(), 15)};
    QRect rect_item{QRect(opt.rect.left(), opt.rect.top(), opt.rect.width(), opt.rect.height())};
    // rect = QRect(opt.rect.left(), opt.rect.top(), opt.rect.width(), opt.rect.height());
    QStyle *new_style = opt.widget ? opt.widget->style() :
                                            QApplication::style();

    // Brush to fill rectangle.
    // Pen to outline rectangle.
    // new_style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
    // selected: #516C8D
    if ( opt.state & QStyle::State_Selected || opt.state & QStyle::State_MouseOver ) {
    // if ( (opt.state & QStyle::State_Enabled) || (opt.state & QStyle::State_Active) ) {
        QColor item_bg_color("#516C8D");
        // QColor bg_color{QColor(47, 60, 84)};
        QBrush bg_brush_normal(item_bg_color, Qt::SolidPattern);
        // qDebug() << "Enabled, Selected and Active";
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
                // padding by adding 4 pixels to the left.
                rect = QRect(opt.rect.left()+4, opt.rect.top(), opt.rect.width(), 30);
                painter->drawText(rect_text, Qt::AlignTop | Qt::AlignLeft, name);
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
        // padding by adding 4 pixels to the left.
        rect = QRect(opt.rect.left()+4, opt.rect.top(), opt.rect.width(), 30);
        painter->drawText(rect, Qt::AlignTop | Qt::AlignLeft, name);
        // qDebug() << "Enabled and Active";
    }




    // Verrataan styleoptionin arvoa enum arvoihin ja jos mätsää, niin sitten tehdään sen mukaan
    /*
    QStyleOptionViewItem myoption = option;
    myoption.text = index.data().toString();

    if ( option.showDecorationSelected && (option.state & QStyle::State_Selected) ) {
        if ( option.state & QStyle::State_Active ) {
            // row selected and active
            myoption.font.setBold(true);
            painter->fillRect(option.rect, option.palette.highlight().color());
        } else {
            // selected but not activate
            myoption.font.setBold(false);
            QPalette p=option.palette;
            painter->fillRect(option.rect, p.color(QPalette::Inactive, QPalette::Background));

        }
    }
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myoption, painter);
    */
    /*
    // Data from model (QModelIndex, role)
    QString line01 = index.model()->data(index.model()->index(index.row(), 0)).toString();
    QStyleOptionViewItem new_option{option};
    //initStyleOption(&new_option, index);

    // draw correct background
    // new_option.text = "";
    QStyle *new_style = new_option.widget ? new_option.widget->style() :
                                            QApplication::style();
    new_style->drawControl(QStyle::CE_ItemViewItem, &new_option, painter,
                           new_option.widget);
    QRect rect = new_option.rect;
    rect = QRect(rect.left(), rect.top(), 140, 25);
    QPalette::ColorGroup cg = new_option.state & QStyle::State_Enabled ?
                QPalette::Normal : QPalette::Disabled;
    if ( cg == QPalette::Normal && !(new_option.state & QStyle::State_Active)) {
        cg = QPalette::Inactive;
    }

    // set pen color
    if ( new_option.state & QStyle::State_Selected ) {
        painter->setPen(new_option.palette.color(cg, QPalette::HighlightedText));
    } else {
        painter->setPen(new_option.palette.color(cg, QPalette::Text));
    }
    // draw lines of text;

    painter->drawText(rect, Qt::AlignCenter, line01);
    */
    // Katso myös se QStyle examples.
    /*
     *     switch (element) {
                case (PE_PanelItemViewItem): {
            painter->save();

            QPoint topLeft = option->rect.topLeft();
            QPoint bottomRight = option->rect.topRight();
            QLinearGradient backgroundGradient(topLeft, bottomRight);
            backgroundGradient.setColorAt(0.0, QColor(Qt::yellow).lighter(190));
            backgroundGradient.setColorAt(1.0, Qt::white);
            painter->fillRect(option->rect, QBrush(backgroundGradient));

            painter->restore();
        break;
        }
        default:
            QProxyStyle::drawPrimitive(element, option, painter, widget);
    }*/
    /*
     * void StatusWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString text = index.data(MessageRole).toString();
    QRectF r = option.rect;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(220,220,250));
    painter->drawRect(r);
    painter->restore();
    painter->drawText(r, Qt::AlignJustify | Qt::AlignVCenter | Qt::TextWordWrap, text);
}*/
    /*
     *     void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setPen(option.palette.highlightedText().color());
        } else {
            painter->setPen(option.palette.text().color());
        }
        painter->drawText(option.rect, Qt::TextWordWrap, index.data().toString());
    }
};
 */
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
