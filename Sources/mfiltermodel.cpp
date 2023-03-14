#include "mfiltermodel.h"
#include <iostream>

SearchColumnProp::SearchColumnProp (const QString &value, ColumnName column, Accuracy accuracy):
    _value(value), _column(column), _accuracy(accuracy) {}

MFilterModel::MFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent) {}

MFilterModel::MFilterModel() {}

void MFilterModel::setFilter(const QString &value, ColumnName columnName, Accuracy accuracy)
{
    this->_filter = value;
    this->_column = columnName;
    this->_accuracy = accuracy;
    typefilter = TypeFilter::OneColumn;
}

void MFilterModel::setFilter(QList<SearchColumnProp> &columns)
{
    this->_columnS = columns;
    typefilter = TypeFilter::MultiColumns;
}

bool MFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    switch (typefilter){
    case TypeFilter::MultiColumns:
        return this->filterMultiColumns(source_row, source_parent);
        break;
    default:
        return this->filterOneColumn(source_row, source_parent, this->_filter, this->_column, this->_accuracy);
        break;
    }
}

bool MFilterModel::filterOneColumn(int source_row, const QModelIndex &source_parent, const QString &value, ColumnName column, Accuracy accuracy = Same) const
{
    QModelIndex idx = sourceModel()->index( source_row, column, source_parent);
    QString pattern = sourceModel()->data( idx ).toString().toLower();
    switch (accuracy) {
    case Accuracy::Contains:
        if( pattern.contains(value.toLower()) )
            return true;
        else
            return false;
        break;
    default:
        if( pattern == value.toLower())
            return true;
        else
            return false;
        break;
    }
}

bool MFilterModel::filterMultiColumns(int source_row, const QModelIndex &source_parent) const
{
    int return_val = 0;
    foreach(SearchColumnProp column, this->_columnS)
        switch (column._column){
        case ColumnName::BookName:
            return_val += this->filterOneColumn(source_row, source_parent, column._value, column._column, column._accuracy);
            break;
        case ColumnName::BookAuthor:
            return_val += this->filterOneColumn(source_row, source_parent, column._value, column._column, column._accuracy);
            break;
        case ColumnName::GenreName:
            return_val += this->filterOneColumn(source_row, source_parent, column._value, column._column);
            break;
        case ColumnName::BookPublishDate:
            return_val += this->filterOneColumn(source_row, source_parent, column._value, column._column);
            break;
        case ColumnName::BookCount:
            return_val += this->filterOneColumn(source_row, source_parent, column._value, column._column);
            break;
        default:
            return_val += false;
            break;
        }
    if(return_val == _columnS.size())
        return true;
    else
        return false;
}
