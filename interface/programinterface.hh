#ifndef PROGRAMINTERFACE_HH
#define PROGRAMINTERFACE_HH

//#include "channelinfo.hh"
#include <QWidget>

namespace my_program {
namespace interface {

class ProgramInterface {
    public:
        ProgramInterface() = default;
        virtual ~ProgramInterface() = default;
        // A pure virtual function for channel search.
        /**
         * @brief searchChannel Function to search a channel with given string
         * and add data to cInfo widget inherited from Channelinfo-class.
         * @param cInfo Instance of a Channelinfo class which base class is
         * QWidget.
         * @param channel Channel string.
         * @return True if channel was found, otherwise False.
         */
        virtual bool searchChannel(QWidget* cInfo, QString channel) = 0;
    private:
};
}
}
#endif // PROGRAMINTERFACE_HH
