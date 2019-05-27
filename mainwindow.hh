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
#include "programmodelinterface.hh"
#include "widgetbuilder.hh"
#include "programinterface.hh"

#include <QJsonObject>
#include <QMainWindow>
#include <memory>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void setProgram(my_program::interface::ProgramInterface *pa);
        void updateSettings();
    private slots:
        void on_fetch_follows_clicked();
        void on_save_settings_button_clicked();
        void on_search_button_clicked();
        void on_clear_follows_clicked();
        void on_update_follows_clicked();
        void on_main_update_button_clicked();
        void on_main_top_games_list_clicked(const QModelIndex &index);
        void checkUsernameEdit();

    private:
        void checkErrorLabel(const int &newValue);
        void clear_follows_page();
        void set_follow_list_style();
        bool isUserName() const;
        void usernameChanged(const QString &newValue);
        Ui::MainWindow *ui;
        Networkmanager data_retriever_;

        // Info label:
        // <ID, Text>

        QVector<QWidget *> messageLabelContent_;
        InfoMessage *infoLabel_;
        InfoMessage *saveLabel_;

        my_program::interface::ProgramInterface *pa_;
};

#endif // MAINWINDOW_HH
