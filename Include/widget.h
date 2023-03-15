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
#include "filerw.h"

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

    FileRW *_FJson;
    UserRole _userRole;

    QMenuBar *_menubar;
    QMenu *_menuSet;
    QMenu *_menuAdmin;
    QMenu *_menuPersAcc;
    QAction *_authMN;
    QAction *_editMod;

    Authorization *auth;
    bool _succes_auth;

    QSqlTableModel *_model;
    QSqlDatabase db;
    QSqlTableModel *_model_genre;

    Search *simple_search;
    Search *search;
    EditRow *_editRow;
    SetConnectDB *_setConDb;
    QMessageBox *_set_auth_msg;

    void load_table();

public slots:
    void succes_auth();
    void set_connect_apply();
    void set_auth(QAbstractButton *btn);
    void querry_simple_search(Accuracy accuracy, QString name);
    void querry_search();

    //connectors slots
    void authorization();
    void update_table();
    void on_clicked_menu_takenBook();
    void on_clicked_menu_returnBook();
    void on_clicked_menu_personSetting();
    void on_clicked_menu_createGenre();
    void on_clicked_menu_removeRow();
    void removeGenre();
    void set_connect();
    void set_auth_message();
    void addRow();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);
};
#endif // WIDGET_H
