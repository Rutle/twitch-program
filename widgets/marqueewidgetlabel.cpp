#include "marqueewidgetlabel.hh"

#include <QPainter>
#include <QDebug>
namespace my_program {
namespace widgets {

MarqueeWidgetLabel::MarqueeWidgetLabel(QWidget *parent, int spd , int dir, bool start)
    : QLabel(parent) {
    px = 8;
    py = 15;
    speed = spd;
    direction = dir;
    autostart = start;
    connect(&timer, SIGNAL(timeout()), this, SLOT(refreshLabel()));
    if ( autostart == false ) {
        if ( fontMetrics().width(text()) > width() ) {
            timer.start(10);
        }

    }
    else {
        timer.start(10);
    }
}

void MarqueeWidgetLabel::refreshLabel() {

    if ( direction == RightToLeft ) {
        px -= 1;
        if ( px - width() == -textLength ) {
            direction = LeftToRight;
        }

        if( px <= (-textLength) ) {
            px = width();
        }

    } else {
        px += 1;
        if ( px == 8 ) {
            direction = RightToLeft;
        }
        if ( px >= width() ) {
            px = - textLength;

        }
    }
    timer.start( speed );
    //repaint();
    update();
}

MarqueeWidgetLabel::~MarqueeWidgetLabel() {

}

void MarqueeWidgetLabel::show() {
    QLabel::show();
}

void MarqueeWidgetLabel::setAlignment(Qt::Alignment al) {
    m_align = al;
    updateCoordinates();
    QLabel::setAlignment(al);
}

void MarqueeWidgetLabel::paintEvent(QPaintEvent *evt) {
    QPainter p(this);

    p.drawText(px, py + fontPointSize, text());
    p.translate(px, 0);
}

void MarqueeWidgetLabel::resizeEvent(QResizeEvent *evt) {
    updateCoordinates();
    QLabel::resizeEvent(evt);
}

void MarqueeWidgetLabel::updateCoordinates() {
    px=0;
    switch(m_align) {
    case Qt::AlignTop:
        py = 10;
        break;
    case Qt::AlignBottom:
        py = height()-10;
        break;
    case Qt::AlignVCenter:
        py = height()/2;
        break;
    }
    fontPointSize = font().pointSize()/2;
    textLength = fontMetrics().width(text());

    if ( textLength>width() ) {
        timer.start(10);
    } else {
        timer.stop();
        update();
    }
}

void MarqueeWidgetLabel::setSpeed(int s) {
    speed = s;
}

int MarqueeWidgetLabel::getSpeed() {
    return speed;
}

void MarqueeWidgetLabel::setDirection(int d) {
    direction = d;
    if ( direction == RightToLeft ) {
        px = width() - textLength;
    } else {
        px = 8;
    }
    refreshLabel();
}
}
}
