#ifndef DB_WORKER_H
#define DB_WORKER_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>


///============================================= DB WORKER ================================================================================
//class wicket_reader;
class db_worker: public QObject {
    Q_OBJECT

public:
    db_worker(QString PlayerName="", QString conn_str="") : PlayerName(PlayerName), conn_str(conn_str)
    {
        //reader_ptr_to_string_for_db_connection_id = QString("0x%1").arg((quintptr)reader, QT_POINTER_SIZE * 2, 16, QChar('0'));
    }
    ~db_worker(){}

private:
    QString PlayerName = "hh";
    QString conn_str;
    bool db_open()
    {
        QSqlDatabase db = QSqlDatabase::database(PlayerName, true);
        if ( !db.isValid() )
        {
            qDebug() << "db worker: QSqlDatabase::database невалидно : PlayerName - " << PlayerName;
            return false;
        }
        if ( !db.isOpen() )
        {
            qDebug() << "  db worker: Соединение с БД закрыто,  пробуем открыть :";
            if ( !db.open() )
            {
                //   qDebug() << db.lastError();
                qDebug() << "  db worker: Не удалось соединиться с базой данных ! :";
                return false;
            }
            //qDebug() << db.lastError();
        }
        else
            qDebug() << "  db worker: Соединение с БД ОТКРЫТО !! :";
        return true;
    }


public slots:
    void create_db_connect()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE", PlayerName );
        db.setConnectOptions("QSQLITE_OPEN_URI");
        qDebug() << " create_db_connect - conn_str" << conn_str;
        db.setDatabaseName( conn_str );
        db_open();
    }
    void save_inventory( QByteArray inv)
    {
        if ( !db_open() )                   //пробуем открыть
            return;
        QSqlQuery save_inventory_query( QSqlDatabase::database(PlayerName) );
        //save_inventory_query.prepare( "UPDATE inventory SET invn = :inv WHERE player = :p_name" );
        save_inventory_query.prepare( "INSERT OR REPLACE INTO inventory VALUES(:name, :inv)"
 );
        save_inventory_query.bindValue(":name", PlayerName, QSql::In );
        save_inventory_query.bindValue(":inv", inv, QSql::In | QSql::Binary );

        if (!save_inventory_query.exec())
            qDebug() << " что то пошло не так.." <<save_inventory_query.lastError().text();

        qDebug() << "to db " << inv;

        save_inventory_query.finish();
    }
    void req_get_inventory()
    {
        if ( !db_open() )                   //пробуем открыть
            return;
        QByteArray inv(18, '\00');
        //inv.reserve(18);
        QSqlQuery get_inventory_query( QSqlDatabase::database(PlayerName) );
        get_inventory_query.prepare( "select inv FROM inventory WHERE name = :name" );
        //get_inventory_query.bindValue(":inv", inv, QSql::Out | QSql::Binary );
        get_inventory_query.bindValue(":name", PlayerName, QSql::In );
        if (!get_inventory_query.exec())
            qDebug() << " load - что то пошло не так.." <<get_inventory_query.lastError().text();
        while ( get_inventory_query.next() )
        {
            inv = get_inventory_query.value(0).toByteArray();
        }
        emit get_inventory_sig(inv);
                qDebug() << "from db " << inv;
        get_inventory_query.finish();
    }

private slots:
    void slot_timeout()
    {

    }

signals:
    void get_inventory_sig( QByteArray );

};

#endif // DB_WORKER_H
