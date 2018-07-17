#ifndef PROGRAM_HH
#define PROGRAM_HH

#include "programinterface.hh"
#include "networkmanager.hh"

namespace my_program {

class Program : public interface::ProgramInterface {
    public:
        Program();
        bool searchChannel(QWidget* cInfo, QString channel);
    private:
        Networkmanager nam_;

};
}
#endif // PROGRAM_HH
