#include "program.hh"
#include "stream.hh"
#include "channelinfo.hh"

#include <QDebug>

const QString CLIENTID = "client_id=th5mqjz7wtxx0ut8cns0g22r3miuzz";
const QString API_URL = "https://api.twitch.tv/kraken/";

const QString SUMMARY = API_URL+"streams/summary?"+CLIENTID;
const QString TOPGAMES = API_URL+"games/top?"+CLIENTID;

namespace my_program {

namespace mp = my_program;
namespace mpW = my_program::widgets;

Program::Program() {
    qDebug() << "Program created";
}

bool Program::searchChannel(QWidget* cInfo, QString channel)
{
    //QString channel{ui->search_line_edit->text()};
    QString request_url{API_URL+"channels/"+channel+"?"+CLIENTID};
    //qDebug() << "Search url: " << request_url;
    nam_.make_api_request(request_url);
    QJsonObject json_data_obj{nam_.retrieve_json_data()};

    if ( json_data_obj["error"] == "Not Found" ) {
        qWarning() << "Channel [" << channel << "] does not exist!";
        //ui->search_button->setDisabled(false);
        //ui->search_line_edit->setDisabled(false);

        return false;
    }
    mp::Stream stream_obj(json_data_obj);
    mpW::Channelinfo* cW = dynamic_cast<mpW::Channelinfo *>(cInfo);

    cW->set_values(stream_obj);
    //dynamic_cast<RGame *>(game)
    //mpW::Channelinfo *channel_widget{new my_program::widgets::Channelinfo(this)};
    //channel_widget->set_values(stream_obj);

    return true;
}
} // my_program
