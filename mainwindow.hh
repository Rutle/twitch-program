#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

/*      ****This program uses data from Twitch.tv API.****
 * *This is program that uses Twitch.tv's API to retrieve channel information,
 *  stream information (if online) based on users followed channel list.
 *
 * *This program only saves avatar picture of a channel but other information
 *  is removed when the program is closed. Shown information includes list of
 *  channels the user has.
 *
 * *Channel information that is being shown includes channel name, avatar
 *  picture, total views, online status, number of followers, channel url,
 *  partner status and maturity, creation date for channel and stream.
 * */

#include "networkmanager.hh"
#include "settings.hh"
#include "stream.hh"
#include "programinterface.hh"
#include "widgetbuilder.hh"

#include <QJsonObject>
#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_fetch_follows_clicked();
        void on_save_settings_button_clicked();
        void on_search_button_clicked();
        void on_clear_follows_clicked();
        void on_update_follows_clicked();
        void on_main_update_button_clicked();
        void on_main_top_games_list_clicked(const QModelIndex &index);

private:
        QWidget* build_qlistwidgetitem(const my_program::Stream &stream);
        void check_channel_online_status();
        void update_settings();
        void clear_follows_page();
        void build_follows_page(QJsonObject &json_data);
        void update_summary();
        void update_top_games();
        void set_follow_list_style();
        Ui::MainWindow *ui;
        Networkmanager data_retriever_;

        // Followed channel online status [channel_name]:[true/false]:
        // Might be able to have one less container by having pointer in key-value
        // to a Stream-object?
        QMap<QString, bool> followed_online_status_;
        std::shared_ptr<my_program::Settings> settings_;

        //Contains Stream-objects with extracted data from Twitch.tv API-request.
        QList<my_program::Stream> followed_stream_data_;
        QHash<QString, QList<my_program::Stream>> main_top_games_data_;
        my_program::interface::ProgramInterface *programModel_;



};

#endif // MAINWINDOW_HH
