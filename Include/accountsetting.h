#ifndef ACCOUNTSETTING_H
#define ACCOUNTSETTING_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlRecord>

namespace Ui {
class AccountSetting;
}

class AccountSetting : public QDialog
{
    Q_OBJECT
    Ui::AccountSetting *ui;
    QSqlDatabase _db;
    QSqlRecord sendQuery(const QString &query);
    void errorMes(const QString &mes);

public:
    explicit AccountSetting(const QSqlDatabase &db, const QString &login, QWidget *parent = nullptr);
    ~AccountSetting();

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void userLoginChanged(QString new_login);
    void closed_window();

private slots:
    void on_save_fio_btn_clicked();
    void on_cancel_btn_clicked();
    void on_save_login_btn_clicked();
    void on_save_passwd_btn_clicked();
};

#endif // ACCOUNTSETTING_H
