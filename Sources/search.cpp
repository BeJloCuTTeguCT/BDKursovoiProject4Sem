#include "search.h"
#include "ui_search.h"
#include "ui_simple_search.h"
#include "stylesheet.h"

Search::Search(SearchType type_search) :
    search(new Ui::Search), simple_search(new Ui::Simple_Search)
{
    if(type_search == SearchType::Advanced) {
        search->setupUi(this);
        search->search_btn->setStyleSheet(StyleSheet::getStyleForSearchBtn());
        search->author_cb->setStyleSheet(StyleSheet::getStyleForComboBox());
        search->name_cb->setStyleSheet(StyleSheet::getStyleForComboBox());
        search->genre_cb->setStyleSheet(StyleSheet::getStyleForComboBox());
    }
    if(type_search == SearchType::Simple) {
        simple_search->setupUi(this);
        simple_search->simple_search_btn->setStyleSheet(StyleSheet::getStyleForSearchBtn());
        simple_search->name_cb->setStyleSheet(StyleSheet::getStyleForComboBox());
    }
    this->setStyleSheet(StyleSheet::getStyleForSearchLine()); //поля ввода серым цветом как кнопка отмены
}

QList<Accuracy> Search::get_Accuracy()
{
    return this->_list_accuracy;
}

QList<QString> Search::get_DataSearch()
{
    return this->_data_search;
}

void Search::set_genre_list(QSqlTableModel *model)
{
    search->genre_cb->setModel(model);
    search->genre_cb->setModelColumn(1);

}

Search::~Search()
{
    delete search;
    delete simple_search;
}

void Search::on_search_btn_clicked()
{
    _data_search.clear();
    _list_accuracy.clear();
    QString temp = search->name_ln->text();
    this->_data_search.push_back(temp);
    if(search->name_cb->currentIndex() == 0)
        this->_list_accuracy.push_back(Accuracy::Same);
    if(search->name_cb->currentIndex() == 1)
        this->_list_accuracy.push_back(Accuracy::Contains);
    temp = search->author_ln->text();
    this->_data_search.push_back(temp);
    if(search->author_cb->currentIndex() == 0)
        this->_list_accuracy.push_back(Accuracy::Same);
    if(search->author_cb->currentIndex() == 1)
        this->_list_accuracy.push_back(Accuracy::Contains);
    temp = search->genre_cb->currentText();
    this->_data_search.push_back(temp);
    temp = search->publish_ln->text();
    this->_data_search.push_back(temp);
    temp = search->pub_date_ln->text();
    this->_data_search.push_back(temp);
    emit this->init_search();
}


void Search::on_simple_search_btn_clicked()
{
    int type_name = simple_search->name_cb->currentIndex();
    QString name = simple_search->name_ln->text();
    if(type_name == 0)
        emit this->init_simple_search(Accuracy::Same, name);
    if(type_name == 1)
        emit this->init_simple_search(Accuracy::Contains, name);
}

