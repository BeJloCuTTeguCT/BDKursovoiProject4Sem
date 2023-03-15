#ifndef EDITROW_H
#define EDITROW_H

#include <QDataWidgetMapper>
#include <QSqlDatabase>
#include <QDialog>
#include "myenums.h"

namespace Ui {
class EditRow;
}
class DataWidgetMap;

class EditRow : public QDialog
{
    Q_OBJECT

public:
    explicit EditRow(TypeEditRow type = TypeEditRow::EditingRow, QWidget *parent = nullptr);
    void setModel(QAbstractItemModel *model);
    void setSave(const int bookID, const int genreID, QSqlDatabase *db);
    void setCurentModelIndex(const QModelIndex &index);
    void setGenreList(QAbstractItemModel *model);
    ~EditRow();


signals:

private slots:
    void on_cancel_btn_clicked();
    void on_save_btn_clicked();

private:
    int _bookID;
    int _genreID;
    QSqlDatabase *_db;
    QDataWidgetMapper *_mapper;
    TempTable *_table;
    TypeEditRow _type;
    Ui::EditRow *ui;
};

#endif // EDITROW_H
