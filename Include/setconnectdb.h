#ifndef SETCONNECTDB_H
#define SETCONNECTDB_H

#include <QDialog>

namespace Ui {
class SetConnectDB;
}

class SetConnectDB : public QDialog
{
    Q_OBJECT

public:
    explicit SetConnectDB(QWidget *parent = nullptr);
    explicit SetConnectDB(QList<QString> present, QWidget *parent = nullptr);
    ~SetConnectDB();
    QString get_port();
    QString get_host();
    QString get_DB_name();
    QString get_login();
    QString get_passwd();

signals:
    void save_btn_clicked();
    void cancel_btn_clicked();

private slots:
    void on_save_btn_clicked();
    void on_cancel_btn_clicked();

private:
    QString _port;
    QString _host;
    QString _DB_name;
    QString _login;
    QString _passwd;
    Ui::SetConnectDB *ui;
};

#endif // SETCONNECTDB_H
