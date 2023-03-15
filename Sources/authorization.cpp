#include "authorization.h"
#include "ui_authorization.h"
#include "ui_registration.h"
#include "stylesheet.h"
#include <QMessageBox>

Authorization::Authorization(QWidget *parent) :
    QDialog(parent), ui(new Ui::Authorization)
{
    ui->setupUi(this);
    this->setStyleSheet(StyleSheet::getStyleForEditLine() + StyleSheet::getStyleForSaveBtn());
    ui->registration_btn->setStyleSheet(StyleSheet::getStyleForCancelBtn());
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

void Authorization::on_registration_btn_clicked()
{
    _registr = new Registration(this);
    _registr->open();
}


Registration::Registration(QWidget *parent) :
    QDialog(parent), ui(new Ui::Registration)
{
    this->setStyleSheet(StyleSheet::getStyleForEditLine());
    ui->save_btn->setStyleSheet(StyleSheet::getStyleForSaveBtn());
    ui->cancel_btn->setStyleSheet(StyleSheet::getStyleForCancelBtn());
}

Registration::~Registration()
{
    delete ui;
}

void Registration::on_save_btn_clicked()
{
    return;
}

