 #ifndef EDITROW_H
#define EDITROW_H

#include <QDataWidgetMapper>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QDialog>
#include "myenums.h"

namespace Ui {
class EditRow;
class RmBook;
class EditGenre;
}

class EditRow : public QDialog
{
    Q_OBJECT
    QString _bookID;
    QSqlDatabase *_db;
    QDataWidgetMapper *_mapper;
    TypeEditRow _type;
    Ui::EditRow *ui;
    Ui::RmBook *rm_ui;
    void saveQuery(const QString &query);

public:
    explicit EditRow(TypeEditRow type = TypeEditRow::EditingRow, QWidget *parent = nullptr);
    void setModel(QAbstractItemModel *model);
    void setSave(QSqlDatabase *db,QString bookID = "");
    void setCurentModelIndex(const QModelIndex &index);
    void setGenreList(QAbstractItemModel *model);
    ~EditRow();

signals:
    void closed_window();

private slots:
    void on_cancel_btn_clicked();
    void on_save_btn_clicked();
};


class EditGenre : public QDialog
{
    Q_OBJECT
    Ui::EditGenre *ui;
    TypeEditGenre _type;
    QSqlDatabase _db;
    QSqlTableModel *_tableModel;

public:
    explicit EditGenre(TypeEditGenre type, QWidget *parent = nullptr);
    ~EditGenre();
    void setGenreList(QSqlTableModel *model);

signals:
    void closed_window();

public slots:
    void on_cancel_btn_clicked();
    void on_save_btn_clicked();
};

#endif // EDITROW_H
