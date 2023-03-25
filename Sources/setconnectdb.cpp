#include "setconnectdb.h"
#include "ui_setconnectdb.h"
#include "myenums.h"
#include "stylesheet.h"

SetConnectDB::SetConnectDB(QList<QString> present, QWidget *parent) :
     QDialog(parent), _port(present[ConfigurateJson::Port]), _host(present[ConfigurateJson::Host]),
  _DB_name(present[ConfigurateJson::NameDB]), _login(present[AuthPair::Login + 3]),
  _passwd(present[AuthPair::Password + 3]), ui(new Ui::SetConnectDB)
{
    ui->setupUi(this);
    ui->DBname_ln->setText(_DB_name);
    ui->host_ln->setText(_host);
    ui->login_ln->setText(_login);
    ui->passwd_ln->setText(_passwd);
    ui->port_ln->setText(_port);
    this->setStyleSheet(StyleSheet::getStyleForEditLine());
    ui->save_btn->setStyleSheet(StyleSheet::getStyleForSaveBtn());
    ui->cancel_btn->setStyleSheet(StyleSheet::getStyleForCancelBtn());
    this->setWindowTitle("Настройки подключения к БД");
}

SetConnectDB::~SetConnectDB()
{
    delete ui;
}

QString SetConnectDB::get_port()
{
    return this->_port;
}

QString SetConnectDB::get_host()
{
    return this->_host;
}

QString SetConnectDB::get_DB_name()
{
    return this->_DB_name;
}

QString SetConnectDB::get_login()
{
    return this->_login;
}

QString SetConnectDB::get_passwd()
{
    return this->_passwd;
}

void SetConnectDB::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit closed_window();
}

void SetConnectDB::on_save_btn_clicked()
{
    this->_host = this->ui->host_ln->text();
    this->_port = this->ui->port_ln->text();
    this->_DB_name = this->ui->DBname_ln->text();
    this->_login = this->ui->login_ln->text();
    this->_passwd = this->ui->passwd_ln->text();
    this->close();
    emit this->save_btn_clicked();
}


void SetConnectDB::on_cancel_btn_clicked()
{
    this->close();
    emit closed_window();
}

