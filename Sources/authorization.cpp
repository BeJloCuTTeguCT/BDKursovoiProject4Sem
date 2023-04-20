#include "ui_authorization.h"
#include "ui_registration.h"
#include "authorization.h"
#include "qsqlrecord.h"
#include "stylesheet.h"
#include <qmessagebox.h>
#include <qsqlquery.h>
#include <qsqlerror.h>

void Authorization::errorMes()
{
    QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Warning, "Ошибка авторизации",
            "Неправильный логин или пароль!",
            QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    msg->setAttribute(Qt::WA_DeleteOnClose);
    msg->show();
}

Authorization::Authorization(QWidget *parent) :
    QDialog(parent), ui(new Ui::Authorization), _registr(new Registration(this))
{
    ui->setupUi(this);
    this->setStyleSheet(StyleSheet::getStyleForEditLine());
    ui->login_pb->setStyleSheet(StyleSheet::getStyleForSaveBtn());
    ui->registration_btn->setStyleSheet(StyleSheet::getStyleForCancelBtn());
    this->setWindowTitle("Авторизация");
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::authorization(const QString &login, const QString &password)
{
    _db.setUserName(login);
    _db.setPassword(password);
    if (!_db.open()) {
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Critical, "Ошибка подключения", _db.lastError().databaseText(),
                                        QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->show();
        return;
    }
}

void Authorization::setConfigDB(const QStringList &config)
{
    _db = QSqlDatabase::addDatabase("QPSQL");
    _db.setHostName(config.value(ConfigurateJson::Host));
    _db.setPort(config.value(ConfigurateJson::Port).toInt());
    _db.setDatabaseName(config.value(ConfigurateJson::NameDB));
}

void Authorization::getLastUserRole(const QStringList authPair)
{
    ui->login_ln->setText(authPair.value(AuthPair::Login));
    ui->password->setText(authPair.value(AuthPair::Password));
    emit on_login_pb_clicked();
}

void Authorization::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit closed();
}

void Authorization::on_login_pb_clicked()
{
    this->setCursor(Qt::BusyCursor);
    UserRole role = Guest;
    QString query;
    query = QString("select * from authorizate('%1', '%2')").arg(ui->login_ln->text(), ui->password->text());
    QSqlQuery response = QSqlQuery(_db);
    response.exec(query);
    response.next();
    if(response.isValid()) {
        if(!response.value(0).toBool()) {
            errorMes();
            return;
        }
        if(response.value(1).toString() == "admin")
            role = Admin;
        if(response.value(1).toString() == "user")
            role = User;
        if(response.value(1).toString() == "guest")
            role = Guest;
        this->setCursor(Qt::ArrowCursor);
        emit succes_authoriz(role, QStringList() << ui->login_ln->text() << ui->password->text());
        return;
    }
    else
        errorMes();
    this->setCursor(Qt::ArrowCursor);
}

void Authorization::on_registration_btn_clicked()
{
    _registr->setDatabase(_db);
    _registr->open();
    connect(_registr, &Registration::SuccesRegistration, this, &Authorization::pullFields);
}

void Authorization::pullFields(QString login, QString passwd)
{
    _registr->close();
    ui->login_ln->setText(login);
    ui->password->setText(passwd);
}





//              CLASS REGISTRATION              //






void Registration::errorMes(const QString &mes)
{
    QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Warning, "Ошибка при регистрации", mes,
            QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    msg->setAttribute(Qt::WA_DeleteOnClose);
    msg->show();
}

Registration::Registration(QWidget *parent) :
    QDialog(parent), ui(new Ui::Registration)
{
    ui->setupUi(this);
    this->setStyleSheet(StyleSheet::getStyleForEditLine());
    ui->save_btn->setStyleSheet(StyleSheet::getStyleForSaveBtn());
    ui->cancel_btn->setStyleSheet(StyleSheet::getStyleForCancelBtn());
    this->setWindowTitle("Регистрация");
}

void Registration::setDatabase(const QSqlDatabase &db)
{
     _db = db;
}

Registration::~Registration()
{
    delete ui;
}

void Registration::on_save_btn_clicked()
{
    this->setCursor(Qt::BusyCursor);
    bool error = false;
    if (ui->first_name->text().isEmpty()) {
        error = true;
        ui->first_name->setStyleSheet(StyleSheet::getStyleForErrorLineEdit());
    }
    if (ui->last_name->text().isEmpty()) {
        error = true;
        ui->last_name->setStyleSheet(StyleSheet::getStyleForErrorLineEdit());
    }
    if (ui->birhday_ln->text().isSimpleText() && (ui->birhday_ln->text().size() != 10)){
        error = true;
        ui->birhday_ln->setStyleSheet(StyleSheet::getStyleForErrorLineEdit());
    }
    if (ui->login_ln ->text().isEmpty()) {
        error = true;
        ui->login_ln->setStyleSheet(StyleSheet::getStyleForErrorLineEdit());
    }
    if (ui->passwd_ln->text().isEmpty() || ui->passwd_ln->text().size() < 6) {
        error = true;
        ui->passwd_ln->setStyleSheet(StyleSheet::getStyleForErrorLineEdit());
    }
    if (error) {
        errorMes("Ошибка в заполнении данных.\n* - обязательные поля.\nПароль не меньше 6 символов");
        this->setCursor(Qt::ArrowCursor);
        return;
    }
    QString sql;
    if (ui->patromymic->text().isEmpty())
        sql = QString("select * from registration('%1', '%2', NULL, '%3', '%4', '%5')").arg(ui->first_name->text(), ui->last_name->text(),
                                                                        ui->birhday_ln->text(), ui->login_ln->text(), ui->passwd_ln->text());
    else
        sql = QString("select * from registration('%1', '%2', '%3', '%4', '%5', '%6')").arg(ui->first_name->text(), ui->last_name->text(),
                                                ui->patromymic->text(), ui->birhday_ln->text(), ui->login_ln->text(), ui->passwd_ln->text());
    QSqlQuery query = QSqlQuery(_db);
    query.exec(sql);
    query.next();
    if (!query.record().value(0).toBool()) {
        errorMes(QString("Пользователь с таким логином\nуже зарегистрирован!\n%1").arg(query.lastError().databaseText()));
        this->setCursor(Qt::ArrowCursor);
        return;
    }
    this->setCursor(Qt::ArrowCursor);
    emit SuccesRegistration(ui->login_ln->text(), ui->passwd_ln->text());
}


void Registration::on_cancel_btn_clicked()
{
    this->close();
}

