#ifndef TOPGAMESLISTDELEGATE_HH
#define TOPGAMESLISTDELEGATE_HH
#include <QStyledItemDelegate>
namespace my_program {
namespace widgets {

class TopGamesListDelegate : public QStyledItemDelegate {
    Q_OBJECT
    public:
        TopGamesListDelegate(QObject *parent = 0);
        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

};
}
}
#endif // TOPGAMESLISTDELEGATE_HH
