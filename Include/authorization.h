#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include "myenums.h"
#include <qdialog.h>
#include <qsqldatabase.h>

namespace Ui {
class Authorization;
class Registration;
}
class Registration;

class Authorization : public QDialog
{
    Q_OBJECT
    QSqlDatabase _db;
    Ui::Authorization *ui;
    Registration *_registr;
    void errorMes();

public:
    explicit Authorization(QWidget *parent = nullptr);
    ~Authorization();
    void authorization(const QString &login, const QString &password);
    void setConfigDB(const QStringList &config);
    void getLastUserRole(const QStringList authPair);

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void succes_authoriz(UserRole role, QStringList authPair);
    void closed();

private slots:
    void on_login_pb_clicked();
    void on_registration_btn_clicked();
    void pullFields(QString login, QString passwd);

    // QWidget interface

};

class Registration : public QDialog
{
    Q_OBJECT
    QSqlDatabase _db;
    Ui::Registration *ui;
    void errorMes(const QString &mes);

public:
    explicit Registration(QWidget *parent = nullptr);
    void setDatabase(const QSqlDatabase &db);
    ~Registration();

signals:
    void SuccesRegistration(QString login, QString passwd);

private slots:
    void on_save_btn_clicked();

    void on_cancel_btn_clicked();
};

#endif // AUTHORIZATION_H
