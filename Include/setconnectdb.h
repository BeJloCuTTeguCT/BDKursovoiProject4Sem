#ifndef SETCONNECTDB_H
#define SETCONNECTDB_H

#include <qdialog.h>

namespace Ui {
class SetConnectDB;
}

class SetConnectDB : public QDialog
{
    Q_OBJECT
    QString _port;
    QString _host;
    QString _DB_name;
    QString _login;
    QString _passwd;
    Ui::SetConnectDB *ui;

public:
    explicit SetConnectDB(QList<QString> present, QWidget *parent = nullptr);
    ~SetConnectDB();
    QString get_port();
    QString get_host();
    QString get_DB_name();
    QString get_login();
    QString get_passwd();

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void save_btn_clicked();
    void closed_window();

private slots:
    void on_save_btn_clicked();
    void on_cancel_btn_clicked();
};

#endif // SETCONNECTDB_H
