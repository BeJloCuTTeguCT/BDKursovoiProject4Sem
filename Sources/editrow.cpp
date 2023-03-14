#include "editrow.h"
#include "ui_editrow.h"
#include "stylesheet.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

EditRow::EditRow(QWidget *parent) :
    QDialog(parent), ui(new Ui::EditRow)
{
    ui->setupUi(this);
    this->setStyleSheet(StyleSheet::getStyleForEditLine());
    ui->save_btn->setStyleSheet(StyleSheet::getStyleForSaveBtn());
    ui->cancel_btn->setStyleSheet(StyleSheet::getStyleForCancelBtn());
    this->setWindowTitle("Редактирование данных о книге");

    _mapper = new QDataWidgetMapper(this);
}

void EditRow::setModel(QAbstractItemModel *model)
{
    _mapper->setModel(model);
    _mapper->addMapping(ui->name_ln, ColumnName::BookName);
    _mapper->addMapping(ui->author_ln, ColumnName::BookAuthor);
    _mapper->addMapping(ui->genre_ln, ColumnName::GenreName);
    _mapper->addMapping(ui->pub_date_ln, ColumnName::BookPublishDate);
    _mapper->addMapping(ui->count_ln, ColumnName::BookCount);
}

void EditRow::setSave(const int bookID, const int genreID, QSqlDatabase *db)
{
    _bookID = bookID;
    _genreID = genreID;
    _db = db;
}

void EditRow::setCurentModelIndex(const QModelIndex &index)
{
    _mapper->setCurrentModelIndex(index);
}

EditRow::~EditRow()
{
    delete ui;
}

void EditRow::on_cancel_btn_clicked()
{
    this->close();
}


void EditRow::on_save_btn_clicked()
{
    _mapper->submit();
    _table = new TempTable(ui->name_ln->text(), ui->author_ln->text(), ui->genre_ln->text(),
                           ui->pub_date_ln->text().toInt(), ui->count_ln->text().toInt(),
                           _bookID, _genreID);
    QString query = "call set_full_info_of_book(%1, %2, '%3', '%4', '%5', %6, %7)";
    query = query.arg(_table->_BookID).arg(_table->_GenreID).arg(_table->_BookName).arg(_table->_BookAuthor).arg(_table->_GenreName).arg(_table->_BookPublishDate).arg(_table->_BookCount);
    _db->transaction();
    if(_db->exec(query).lastError().isValid()) {
        _db->rollback();
        QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Warning, "Ошибка сохранения",
                "При отправке запроса для обновления\nданных произошла ошибка.",
                QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
        msg->show();
        _mapper->revert();
        return;
    }
    else
        _db->commit();
    this->close();
}

