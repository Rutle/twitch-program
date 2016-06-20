#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH
#include "networkmanager.hh"
#include "settings.hh"
#include <QJsonObject>
#include <QMainWindow>
#include <memory>


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
    void on_follows_make_request_clicked();

    void on_save_settings_button_clicked();

private:
    void check_channel_online_status();
    void update_settings();

    Ui::MainWindow *ui;
    Networkmanager data_retriever_;
    QJsonObject json_data_follows_;
    QJsonObject json_data_on_followed_channels_;
    QMap<QString, bool> followed_online_status_;
    std::unique_ptr<Settings::Settings> settings_;
};

#endif // MAINWINDOW_HH
