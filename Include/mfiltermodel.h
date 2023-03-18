 #ifndef MFILTERMODEL_H
#define MFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "myenums.h"



struct SearchColumnProp {
    QString _value;
    ColumnName _column;
    Accuracy _accuracy = Accuracy::Contains;
    SearchColumnProp(const QString &value,
                ColumnName column,
                     Accuracy accuracy = Accuracy::Contains);
};


class MFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit MFilterModel(QObject *parent);
    MFilterModel();
    void setFilter(const QString &value, ColumnName columnName, Accuracy accuracy = Accuracy::Same);
    void setFilter(QList<SearchColumnProp> &columns);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool filterOneColumn(int source_row, const QModelIndex &source_parent, const QString &value, ColumnName column, Accuracy accurancy) const;
    bool filterMultiColumns(int source_row, const QModelIndex &source_parent) const;

private:
    enum TypeFilter {
        OneColumn,
        MultiColumns
    };
    TypeFilter typefilter;
    QString _filter;
    Accuracy _accuracy;
    ColumnName _column;
    QList<SearchColumnProp> _columnS;
};

#endif // MFILTERMODEL_H
