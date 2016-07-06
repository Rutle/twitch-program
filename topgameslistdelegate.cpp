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

    // Ei ole klikattu, ei ole hiiri päällä: State_Enabled | State_Active
    // Kursori itemin päällä State_Enabled | State_MouseOver | State_Active
    // Itemiä klikattu, mutta hiiri ei päällä:
    // State_Enabled | State_HasFocus | State_Selected | State_Active
    QString line01 = index.model()->data(index.model()->index(index.row(), 0)).toString();
    QRect rect;
    qDebug() << "Text: " << line01 << "[" << opt.state << "]";
    rect = QRect(opt.rect.left(), opt.rect.top(), opt.rect.width(), 25);

    QStyle *new_style = opt.widget ? opt.widget->style() :
                                            QApplication::style();

    // painter->fillRect(option.rect.adjusted(1, 1, -1, -1), Qt::SolidPattern);
    // Brush to fill rectangle.
    // Pen to outline rectangle.
    // opt.palette.setColor();
    // QPalette palette{opt.palette};
    // palette.setColor();
    // new_style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    // Could do: Selected and NOT mouse over / Selected and mouse over

    if ( opt.state & QStyle::State_Selected || opt.state & QStyle::State_MouseOver ) {
    // if ( (opt.state & QStyle::State_Enabled) || (opt.state & QStyle::State_Active) ) {
        QColor bg_color("#516C8D");
        QBrush bg_brush_normal(bg_color, Qt::SolidPattern);
        qDebug() << "Enabled, Selected and Active";
        painter->setPen(Qt::NoPen);
        // This reduces the height by 1.
        painter->fillRect(rect.adjusted(0,0,0,-1), bg_brush_normal);
        // Selected and Mouse Over
        if ( opt.state & QStyle::State_MouseOver ) {
            QPen drawing_pen(Qt::red);
            drawing_pen.setStyle(Qt::DashLine);
            drawing_pen.setWidth(1);
            painter->setPen(drawing_pen);
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
        // Selected and NOT Mouse over:
        } else if ( !(opt.state & QStyle::State_MouseOver) ) {
            QPen drawing_pen(Qt::red);
            drawing_pen.setStyle(Qt::SolidLine);
            drawing_pen.setWidth(1);
            painter->setPen(drawing_pen);
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
        }
        // Selected, Mouse over and Focus:
        // Solid line->
        /*
        QColor bg_color("#516C8D");
        QBrush bg_brush_normal(bg_color, Qt::SolidPattern);
        qDebug() << "Enabled, Selected and Active";
        painter->setPen(Qt::NoPen);
        // This reduces the height by 1.
        painter->fillRect(rect.adjusted(0,0,0,-1), bg_brush_normal);
        */
    /*
    } else if ( (opt.state & QStyle::State_Selected) && (opt.state & QStyle::State_MouseOver) ) {

    */
    } else {
        QColor bg_color("#28385e");
        QBrush bg_brush_normal(bg_color, Qt::SolidPattern);
        painter->setPen(Qt::NoPen);
        painter->fillRect(rect, bg_brush_normal);
        qDebug() << "Enabled and Active";
    }

    // painter->
    painter->setFont(font);
    painter->setPen(text_color);
    // padding by reducing the rect width size by 4.
    rect = QRect(opt.rect.left(), opt.rect.top(), opt.rect.width()-4, 25);
    painter->drawText(rect, Qt::AlignVCenter | Qt::AlignRight, line01);
    // Verrataan styleoptionin arvoa enum arvoihin ja jos mätsää, niin sitten tehdään sen mukaan
    //
    // QStyle::State_Active	0x00010000	Indicates that the widget is active.
    // QStyle::State_MouseOver	0x00002000	Used to indicate if the widget is under the mouse.
    // QStyle::State_Selected	0x00008000	Used to indicate if a widget is selected.
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

//alocate each item size in listview.
QSize TopGamesListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const {
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(25);
    //result.setWidth(160);
    return result;

}
