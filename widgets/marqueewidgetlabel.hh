#ifndef MARQUEEWIDGETLABEL_HH
#define MARQUEEWIDGETLABEL_HH
/* Source: "https://www.linux-apps.com/p/1132075/"
 * and "http://www.qtcentre.org/threads/45567-PyQt4-Smooth-text-scrolling-in-
 * QLabel-(again)?p=208197#post208197"
 * */
#include <QWidget>
#include <QLabel>
#include <QTimer>
namespace my_program {
namespace widgets {

class MarqueeWidgetLabel : public QLabel {
    Q_OBJECT
    public: //Member Functions
        enum Direction{LeftToRight,RightToLeft};
        MarqueeWidgetLabel(QWidget *parent = 0, int spd = 50,
                           int dir = MarqueeWidgetLabel::RightToLeft,
                           bool start = false);
        ~MarqueeWidgetLabel();
        void show();
        void setAlignment(Qt::Alignment);
        int getSpeed();

    public slots: //Public Member Slots
        void setSpeed(int s);
        void setDirection(int d);
        void updateCoordinates();
    protected: //Member Functions
        void paintEvent(QPaintEvent *evt);
        void resizeEvent(QResizeEvent *evt);
    private: //Data Members
        bool autostart;
        int px;
        int py;
        QTimer timer;
        Qt::Alignment m_align;
        int speed;
        int direction;
        int fontPointSize;
        int textLength;

    private slots: //Private Member Slots
        void refreshLabel();
};
}
}
#endif // MARQUEEWIDGETLABEL_HH
