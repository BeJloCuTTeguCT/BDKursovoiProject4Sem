#include "authorization.h"
#include "ui_authorization.h"
#include "stylesheet.h"
#include <QMessageBox>

Authorization::Authorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);
    this->setStyleSheet(StyleSheet::getStyleForEditLine() + StyleSheet::getStyleForSaveBtn());
    this->setWindowTitle("Авторизация");
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::authorization(const QString &login, const QString &password)
{
    this->_login = login;
    this->_password = password;
}

void Authorization::on_login_pb_clicked()
{
    if(this->ui->login_ln->text() == _login  && this->ui->password->text() == _password)
        emit succes_authoriz();
    else
    {
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Warning, "Ошибка авторизации",
                "Неправильный логин или пароль!",
                QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
        msg->show();
    }
}

