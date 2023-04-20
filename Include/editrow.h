 #ifndef EDITROW_H
#define EDITROW_H

#include <qdialog.h>
#include <qdatawidgetmapper.h>
#include <qsqldatabase.h>
#include <qsqltablemodel.h>
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
    void showErrorMes(const QString &title, const QString &mes);
    bool saveQuery(const QString &query);

public:
    explicit EditRow(TypeEditRow type = TypeEditRow::EditingRow, QWidget *parent = nullptr);
    void setModel(QAbstractItemModel *model);
    void setSave(QSqlDatabase *db,QString bookID = "");
    void setCurentModelIndex(const QModelIndex &index);
    void setGenreList(QAbstractItemModel *model);
    ~EditRow();

protected:
    void closeEvent(QCloseEvent *event) override;

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

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void closed_window();

public slots:
    void on_cancel_btn_clicked();
    void on_save_btn_clicked();
};

#endif // EDITROW_H
