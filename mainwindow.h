#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QDebug>
#include <QThread>
#include <QMenu>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "inventoryitem.h"
#include "inventory_widget.h"

#include "db_worker.h"
#include "menu.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MainMenu *_main_menu;
public slots:
    void NewGame()
    {
        inventory_table.clear();
        source_table.setEnabled(true);
        inventory_table.setEnabled(true);
        _main_menu->accept();
    }
    void enableInterface(bool enable)
    {
        source_table.setEnabled(enable);
        inventory_table.setEnabled(enable);
    }

private:
    Ui::MainWindow *ui;

//inventory2 inventory;
    inventoryWidget inventory_table;
    QTableWidget source_table;
    QPushButton menu_button;

    QThread *db_worker_thread;
    db_worker *_db_worker;
    QString PlayerName{};

    QMediaPlayer player;
    //QMediaPlaylist playlist;


    QString conn_str = "./bases/inventory.db";

    void safe_delete_db_worker()
    {
        QSqlDatabase db = QSqlDatabase::database(PlayerName, false);
        db.close();
        QSqlDatabase::removeDatabase(PlayerName);
        db_worker_thread->exit(0);
    }

    void create_DB_worker(QString PlayerName)
    {
        qDebug() << "create_DB_worker - PlayerName as connection name: " << PlayerName;
        _db_worker = new db_worker(PlayerName, conn_str);
        db_worker_thread = new QThread();

        connect(this,  &MainWindow::req_get_inventory_sig, _db_worker, &db_worker::req_get_inventory);
        connect(_db_worker,  &db_worker::get_inventory_sig, this, &MainWindow::get_inventory_from_db);

        connect(this,  &MainWindow::save_inventory_sig, _db_worker, &db_worker::save_inventory);



        connect(db_worker_thread, &QThread::started, _db_worker, &db_worker::create_db_connect);
        connect(db_worker_thread, &QThread::finished, this, [=](){
            _db_worker->deleteLater();
            db_worker_thread->deleteLater();
        });
        _db_worker->moveToThread(db_worker_thread);
        db_worker_thread->start();
    }

private slots:
    void get_inventory_from_db(QByteArray inv)
    {
        inventory_table.deserializeFromByteArray(inv);
        enableInterface(true);

        _main_menu->accept();
    }
    void save_inventory()
    {
        emit save_inventory_sig(inventory_table.serializeToByteArray());
        enableInterface(true);
        _main_menu->accept();
    }
void itemDestroy(itemType it)
{
    QString ssh;
    switch (it) {
    case itemType::apple                 : ssh = "qrc:/apple.mp3"; break;
    case itemType::space_cucumber        : ssh = "qrc:/space_cucumber.mp3"; break;
    case itemType::double_space_cucumber : ssh = "qrc:/double_space_cucumber.mp3"; break;
    default                              : ssh = "qrc:/nothing.mp3";
    }
    //player.setMedia(QUrl(ssh));
    player.setVolume(50);
    player.play();
}

signals:
    void req_get_inventory_sig();
    void save_inventory_sig(QByteArray);
    void quit();
};
#endif // MAINWINDOW_H
