#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QDialog>

namespace Ui {
class Authorization;
class Registration;
}
class Registration;

class Authorization : public QDialog
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = nullptr);
    ~Authorization();
    void authorization(const QString &login, const QString &password);

signals:
    void succes_authoriz();

private slots:
    void on_login_pb_clicked();
    void on_registration_btn_clicked();

private:
    QString _login;
    QString _password;
    Ui::Authorization *ui;
    Registration *_registr;
};

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();

signals:
    void RegistrationEnd();

private slots:
    void on_save_btn_clicked();

private:
    Ui::Registration *ui;
};

#endif // AUTHORIZATION_H
