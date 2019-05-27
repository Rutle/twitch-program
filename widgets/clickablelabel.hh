#ifndef CLICKABLELABEL_HH
#define CLICKABLELABEL_HH

#include <QLabel>
#include <QWidget>
#include <Qt>

/* Courtesy of https://wiki.qt.io/Clickable_QLabel */
class ClickableLabel : public QLabel
{
        Q_OBJECT
    public:
        explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
        ~ClickableLabel();

    signals:
        void clicked();

    public slots:
        void mousePressEvent(QMouseEvent* event);
};

#endif // CLICKABLELABEL_HH
