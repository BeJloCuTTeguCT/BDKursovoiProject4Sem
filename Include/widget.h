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
#include <qsortfilterproxymodel.h>
#include <QSqlError>
#include <QMessageBox>
#include "authorization.h"
#include "search.h"
#include "setconnectdb.h"
#include "editrow.h"
#include "accountsetting.h"
#include "filerw.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    Ui::Widget *ui;

    FileRW *_FJson;
    UserRole _userRole;
    QString _userLogin;

    QMenuBar *_menubar;
    QMenu *_menuSet;
    QMenu *_menuAdmin;
    QMenu *_menuPersAcc;
    QAction * _authMN;
    QAction *_updTable;
    QAction *_editMod;
    QAction *_takenBook;
    QAction *_returnBook;

    Authorization *auth;
    bool _succes_auth;

    QSqlDatabase db;
    QSqlTableModel *_model;
    QSortFilterProxyModel *_model_sort;
    QSqlTableModel *_model_genre;

    Search *simple_search;
    Search *search;

    EditRow *_editRow;
    EditGenre *_editGenre;

    SetConnectDB *_setConDb;
    AccountSetting *_accSet;
    QMessageBox *_set_auth_msg;

    void load_table();
    void MesNoFind();

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void succes_auth(UserRole role, QStringList authPair);
    void set_connect_apply();
    void set_auth(QAbstractButton *btn);
    void querry_simple_search(Accuracy accuracy, QString name);
    void querry_search();

    //connectors slots
    void authorization(bool reauth = false);
    void update_table();
    void on_clicked_menu_takenBook();
    void on_clicked_menu_returnBook();
    void on_clicked_menu_personSetting();
    void on_clicked_menu_logout();
    void on_clicked_menu_createGenre();
    void on_clicked_menu_editGenre();
    void on_clicked_menu_removeRow();
    void removeGenre();
    void set_connect();
    void set_auth_message();
    void addRow();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);
};
#endif // WIDGET_H
