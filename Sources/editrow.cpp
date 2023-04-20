#include "ui_editrow.h"
#include "ui_rm_book.h"
#include "ui_edit_genre.h"
#include "editrow.h"
#include "stylesheet.h"
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qsqlindex.h>
#include <qmessagebox.h>

void EditRow::showErrorMes(const QString &title, const QString &mes)
{
    QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Warning, title, mes,
            QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    msg->setAttribute(Qt::WA_DeleteOnClose);
    msg->show();
}

bool EditRow::saveQuery(const QString &query)
{
    _db->transaction();
    if(_db->exec(query).lastError().isValid()) {
        _db->rollback();
        showErrorMes("Ошибка сохранения","При отправке запроса для обновления\nданных произошла ошибка.\n"
                                                                    "Или ошибка в заполнении данных\n");
        _mapper->revert();
        return false;
    }
    _db->commit();
    _mapper->submit();
    return true;
}

EditRow::EditRow(TypeEditRow type, QWidget *parent) :
    QDialog(parent), _type(type), ui(new Ui::EditRow), rm_ui(new Ui::RmBook)
{
    switch (type) {
    case TypeEditRow::AddingRow:
        ui->setupUi(this);
        ui->pub_date_ln->setValidator(new QIntValidator(this));
        ui->count_ln->setValidator(new QIntValidator(this));
        ui->genre_cb->setStyleSheet(StyleSheet::getStyleForComboBox());
        ui->save_btn->setStyleSheet(StyleSheet::getStyleForSaveBtn());
        ui->cancel_btn->setStyleSheet(StyleSheet::getStyleForCancelBtn());
        ui->save_btn->setText("Добавить");
        ui->title_lb->setText("Добаление книги");
        this->setWindowTitle("Добаление книги");
        break;
    case TypeEditRow::EditingRow:
        ui->setupUi(this);
        ui->pub_date_ln->setValidator(new QIntValidator(this));
        ui->count_ln->setValidator(new QIntValidator(this));
        ui->genre_cb->setStyleSheet(StyleSheet::getStyleForComboBox());
        ui->save_btn->setStyleSheet(StyleSheet::getStyleForSaveBtn());
        ui->cancel_btn->setStyleSheet(StyleSheet::getStyleForCancelBtn());
        this->setWindowTitle("Редактирование данных о книге");
        break;
    case TypeEditRow::RemovindRow:
        rm_ui->setupUi(this);
        rm_ui->names_cb->setStyleSheet(StyleSheet::getStyleForComboBox());
        rm_ui->save_btn->setStyleSheet(StyleSheet::getStyleForSaveBtn());
        rm_ui->cancel_btn->setStyleSheet(StyleSheet::getStyleForCancelBtn());
        this->setWindowTitle("Удаление книги и ее данных");
        break;
    }
    this->setStyleSheet(StyleSheet::getStyleForEditLine());
    _mapper = new QDataWidgetMapper(this);


}
void EditRow::setModel(QAbstractItemModel *model)
{
    _mapper->setModel(model);
    if (_type == RemovindRow) {
        _mapper->addMapping(rm_ui->names_cb, ColumnName::BookID);
        rm_ui->names_cb->setModel(model);
        rm_ui->names_cb->setModelColumn(ColumnName::BookName);
        return;
    }
    if (_type == AddingRow) {
        int row = model->rowCount();
        _mapper->model()->insertRow(row);
        _mapper->setCurrentModelIndex(model->index(row, 1));
    }

    _mapper->addMapping(ui->name_ln, ColumnName::BookName);
    _mapper->addMapping(ui->author_ln, ColumnName::BookAuthor);
    _mapper->addMapping(ui->genre_cb, ColumnName::GenreName);
    _mapper->addMapping(ui->pub_date_ln, ColumnName::BookPublishDate);
    _mapper->addMapping(ui->count_ln, ColumnName::BookCount);
}

void EditRow::setSave(QSqlDatabase *db, QString bookID)
{
    _bookID = bookID;
    _db = db;
}

void EditRow::setCurentModelIndex(const QModelIndex &index)
{
    if (_type == RemovindRow)
        rm_ui->names_cb->setCurrentIndex(index.row());
    else
        _mapper->setCurrentModelIndex(index);
}

void EditRow::setGenreList(QAbstractItemModel *model)
{
    ui->genre_cb->setModel(model);
    ui->genre_cb->setModelColumn(1);
    if(_type == EditingRow) {
        QString currentText = _mapper->model()->index(_mapper->currentIndex(), ColumnName::GenreName).data().toString();
        ui->genre_cb->setCurrentText(currentText);
    }
}

EditRow::~EditRow()
{
    delete ui;
}

void EditRow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit closed_window();
}

void EditRow::on_cancel_btn_clicked()
{
    this->close();
    emit closed_window();
}

void EditRow::on_save_btn_clicked()
{
    QStringList lines;
    QString mes_err = "Ошибка в заполении данных:";
    if (_type == EditingRow || _type == AddingRow) {
        bool int7 = false;
        if(ui->name_ln->text().isEmpty())
            mes_err += QString("\nНазвание книги должно быть заполнено!");
        else
            lines.push_back(ui->name_ln->text());
        if(ui->author_ln->text().isEmpty())
            lines.push_back("NULL");
        else
            lines.push_back(ui->author_ln->text());
        if(ui->pub_date_ln->text().isEmpty())
            lines.push_back("NULL");
        else {
            ui->pub_date_ln->text().toInt(&int7, 10);
            if(int7)
                lines.push_back(ui->pub_date_ln->text());
            else {
                mes_err += QString("\nГод издания должен содержать только цифры!");
            }
        }
        ui->count_ln->text().toInt(&int7, 10);
        if(int7)
            lines.push_back(ui->count_ln->text());
        else {
            mes_err += QString("\nКоличество книг должно быть указано и состоять из цифр!");
        }
        if (mes_err.size() > 26) {
            showErrorMes("Ошибка ввода данных" ,mes_err);
            return;
        }
    }

    QString query;
    switch (_type) {
    case AddingRow:
        query = QString("call add_book('%1', '%2', '%3', %4, %5)").arg(lines.value(0), lines.value(1),
                                    ui->genre_cb->currentText(), lines.value(2), lines.value(3));
        if (!this->saveQuery(query))
            return;
        break;
    case EditingRow:
        query = QString("call set_full_info_of_book(%1, '%2', '%3', '%4', %5, %6)").arg(_bookID, lines.value(0),
                                lines.value(1), ui->genre_cb->currentText(), lines.value(2), lines.value(3));
        if (!this->saveQuery(query))
            return;
        break;
    case RemovindRow:
        query = QString("delete from public.book where \"BookID\" = %1").arg(
                    rm_ui->names_cb->model()->data(rm_ui->names_cb->model()->index(rm_ui->names_cb->currentIndex(), ColumnName::BookID)).toString());
        if (!this->saveQuery(query))
            return;
        break;
    }
    this->close();
    emit closed_window();
}




//              EDIT_GENRE              //




EditGenre::EditGenre(TypeEditGenre type, QWidget *parent) :
    QDialog(parent), ui(new Ui::EditGenre), _type(type)
{
    ui->setupUi(this);
    switch (type) {
    case TypeEditGenre::CreateGenre:
        ui->genre_cb->setHidden(true);
        ui->save_btn->setText("Создать");
        this->setWindowTitle("Создание жанра");
        break;
    case TypeEditGenre::EditingGenre:
        ui->save_btn->setText("Изменить");
        this->setWindowTitle("Редактирование жанра");
        break;
    case TypeEditGenre::RemoveGenre:
        ui->genre_le->setHidden(true);
        ui->save_btn->setText("Удалить");
        this->setWindowTitle("Удаление жанра");
        break;
    }
    ui->genre_le->setStyleSheet(StyleSheet::getStyleForEditLine());
    ui->genre_cb->setStyleSheet(StyleSheet::getStyleForComboBox());
    ui->save_btn->setStyleSheet(StyleSheet::getStyleForSaveBtn());
    ui->cancel_btn->setStyleSheet(StyleSheet::getStyleForCancelBtn());
}

EditGenre::~EditGenre()
{
    delete ui;
}

void EditGenre::setGenreList(QSqlTableModel *model)
{
    _tableModel = model;
    ui->genre_cb->setModel(model);
    ui->genre_cb->setModelColumn(1);
}

void EditGenre::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit closed_window();
}

void EditGenre::on_cancel_btn_clicked()
{
    _tableModel->revertAll();
    this->close();
    emit closed_window();
}

void EditGenre::on_save_btn_clicked()
{
    switch (_type) {
    case CreateGenre:
        _tableModel->insertRow(_tableModel->rowCount());
        _tableModel->setData(_tableModel->index(_tableModel->rowCount()-1, 1), QVariant(ui->genre_le->text()));
        _tableModel->submitAll();
        break;
    case EditingGenre:
        _tableModel->setData(_tableModel->index(ui->genre_cb->currentIndex(), 1), QVariant(ui->genre_le->text()));
        _tableModel->submitAll();
        break;
    case RemoveGenre:
        int row = ui->genre_cb->currentIndex();
        _tableModel->removeRow(row);
        _tableModel->submitAll();
        if (_tableModel->lastError().nativeErrorCode() == "23503") {
            QMessageBox *msg = new QMessageBox(QMessageBox::Icon::Warning, "Ошибка удаления",
                    "Вы попытались удалить жанр,\nк которому привязана(-ы) книга(-и)!\n"
                    "Сначала удалите все книги,\nотносящиеся к этому жанру",
                    QMessageBox::StandardButton::Ok, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
            msg->setAttribute(Qt::WA_DeleteOnClose);
            msg->show();
            _tableModel->revertAll();
            return;
        }
        break;
    }
    this->close();
    emit closed_window();
}
