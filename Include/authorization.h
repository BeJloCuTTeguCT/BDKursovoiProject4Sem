#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QDialog>

namespace Ui {
class Authorization;
}

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

private:
    QString _login;
    QString _password;
    Ui::Authorization *ui;
};

#endif // AUTHORIZATION_H
