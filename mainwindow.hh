#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "networkmanager.hh"
#include "settings.hh"
#include "stream.hh"

#include <QJsonObject>
#include <QMainWindow>
#include <memory>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void data_retrieved(QByteArray data);
    void on_fetch_follows_clicked();
    void on_save_settings_button_clicked();
    void on_search_button_clicked();
    void on_clear_follows_clicked();
    void on_update_follows_clicked();

private:
    QWidget* build_qlistwidgetitem(const my_program::Stream &stream);
    QWidget* build_channel_info_page(const my_program::Stream &stream);
    void check_channel_online_status();
    void update_settings();
    void clear_follows_page();
    void build_follows_page(QJsonObject &json_data);

    Ui::MainWindow *ui;
    Networkmanager data_retriever_;

    // QJsonObject json_data_follows_;
    // QJsonObject json_data_on_followed_channels_;

    // Followed channel online status [channel_name]:[true/false]:
    // Might be able to have one less container by having pointer in key-value
    // to a Stream-object?
    QMap<QString, bool> followed_online_status_;
    std::unique_ptr<my_program::Settings> settings_;

    //Contains Stream-objects with extracted data from API-request.
    QList<my_program::Stream> followed_stream_data_;
};

#endif // MAINWINDOW_HH
