#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QSqlTableModel>
#include "myenums.h"

namespace Ui {
class Search;
class Simple_Search;
}



class Search : public QWidget
{
    Q_OBJECT

public:
    QList<Accuracy> get_Accuracy();
    QList<QString> get_DataSearch();
    explicit Search(SearchType type_search = SearchType::Simple);
    void set_genre_list(QSqlTableModel *model);
    ~Search();

signals:
    void init_search();
    void init_simple_search(Accuracy accuracy, QString name);

private slots:
    void on_search_btn_clicked();
    void on_simple_search_btn_clicked();

private:
    QList<Accuracy> _list_accuracy;
    QList<QString> _data_search;
    Ui::Search *search;
    Ui::Simple_Search *simple_search;
};

#endif // SEARCH_H
