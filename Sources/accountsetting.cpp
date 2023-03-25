#include "accountsetting.h"
#include "ui_accountsetting.h"
#include "stylesheet.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QMessageBox>

QSqlRecord AccountSetting::sendQuery(const QString &query)
{
    QSqlQuery response = QSqlQuery(_db);
    response.exec(query);
    response.next();
    return response.record();
}

void AccountSetting::errorMes(const QString &mes)
{
    QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Warning, "Ошибка сохранения", mes,
            QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    msg->setAttribute(Qt::WA_DeleteOnClose);
    msg->show();
}

AccountSetting::AccountSetting(const QSqlDatabase &db, const QString &login, QWidget *parent) :
    QDialog(parent), ui(new Ui::AccountSetting), _db(db)
{
    ui->setupUi(this);
    ui->tabWgt->setStyleSheet(StyleSheet::getStyleForSearchLine() + StyleSheet::getStyleForSaveBtn());
    ui->cancel_btn->setStyleSheet(StyleSheet::getStyleForCancelBtn());
    QString query = QString("select \"FirstName\", \"LastName\", \"Patronymic\", \"BirthDate\" from public.reader_ticket where \"login\" = '%1'").arg(login);
    QSqlRecord response = sendQuery(query);
    ui->first_name_lb->setText(response.value(0).toString());
    ui->last_name_lb->setText(response.value(1).toString());
    ui->patronymic_lb->setText(response.value(2).toString());
    ui->bith_date_lb->setText(response.value(3).toDate().toString("dd MMMM yyyy"));
    ui->login_lb->setText(login);
    this->setWindowTitle("Настройки аккаунта");
}

AccountSetting::~AccountSetting()
{
    delete ui;
}

void AccountSetting::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit closed_window();
}

void AccountSetting::on_save_fio_btn_clicked()
{
    QString query = QString("update reader_ticket set \"FirstName\" = '%1',"
                            "\"LastName\" = '%2', \"Patronymic\" = '%3' where \"login\" = '%4'").arg(
                ui->first_name_le->text(), ui->last_name_le->text(), ui->patronymic_le->text(), ui->login_lb->text());
    this->sendQuery(query);
    ui->first_name_lb->setText(ui->first_name_le->text());
    ui->last_name_lb->setText(ui->last_name_le->text());
    ui->patronymic_lb->setText(ui->patronymic_le->text());
    ui->tabWgt->setCurrentIndex(0);
}


void AccountSetting::on_cancel_btn_clicked()
{
    this->close();
    emit closed_window();
}


void AccountSetting::on_save_login_btn_clicked()
{
    QString query = QString("select * from set_login('%1', '%2', '%3')").arg(
                ui->login_lb->text(), ui->crnt_passwd_le_login->text(), ui->new_login_le->text());
    QSqlRecord response = this->sendQuery(query);
    if(!response.value(0).toBool()) {
        errorMes("Неправильный пароль!");
        return;
    }
    ui->tabWgt->setCurrentIndex(0);
    ui->login_lb->setText(ui->new_login_le->text());
    emit userLoginChanged(ui->new_login_le->text());
}


void AccountSetting::on_save_passwd_btn_clicked()
{
    if (ui->new_passwd_le->text() != ui->new_passwd2_le->text()) {
        errorMes("Новый пароль введен неодинаково!");
        return;
    }

    QString query = QString("select * from set_password('%1', '%2', '%3')").arg(
                ui->login_lb->text(), ui->crnt_passwd_le_passwd->text(), ui->new_passwd_le->text());
    QSqlRecord response = this->sendQuery(query);
    if(!response.value(0).toBool()) {
        errorMes("Неправильный пароль!");
        return;
    }
    ui->tabWgt->setCurrentIndex(0);
}

