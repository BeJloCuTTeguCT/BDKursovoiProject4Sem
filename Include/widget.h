#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>
#include "authorization.h"
#include "search.h"
#include "setconnectdb.h"
#include "editrow.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QMenuBar *_menubar;
    QMenu *_menu;
    QAction *_authMN;
    Authorization *auth;
    QStringList configure_DB;
    bool _succes_auth;
    Search *simple_search;
    Search *search;
    QSqlDatabase db;
    QSqlTableModel *_model;
    QMessageBox *_set_auth_msg;
    SetConnectDB *_setConDb;
    EditRow *_editRow;
    void authorization();
    void load_table(const QStringList &configure_DB);



public slots:
    void succes_auth();
    void set_connect();
    void set_connect_apply();
    void set_connect_cancel();
    void set_auth_message();
    void set_auth(QAbstractButton *btn);
    void querry_simple_search(Accuracy accuracy, QString name);
    void querry_search();
    void addRow();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);
};
#endif // WIDGET_H
