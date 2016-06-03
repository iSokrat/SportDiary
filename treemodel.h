#ifndef TREEMODEL_H
#define TREEMODEL_H


#include <QAbstractItemModel>
#include <QtSql>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

class TreeModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeModel(const QStringList &headers, const QString &textForQuery, const QStringList &namesOfKeyFields, QObject *parent = nullptr);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    QStringList getKeyFields();

    void setDataFromSqlQuery(const QString& textForQuery);
private:
    void setupModelData(const QString &textForQuery, TreeItem *parent);
    QVariant getConvertData(const QVariant &data) const;
    TreeItem *getItem(const QModelIndex &index) const;
    void setKeyFields(const QStringList &names);
    void setKeyFields(QStringList&& names);
    void fillDropDown(TreeItem* dropDown,
                      QSqlQuery& query,
                      const QString& keyField,
                      qint32 columnCount);
    // Корневой Item,в нем хранится информация для заголовка
    TreeItem *rootItem;

    // Хранит имя столбца таблицы,
    // значение которого должно служить в качестве раскрывающегося списка
    QStringList namesOfKeyFields{};
};

// inline методы

/* Вставляет ключевые поля по имени names, по которому будет
 * отображаться строка в дереве
 *
 * Пример:
 *  Таблица с доменом:
 *  id_user time_of_day date
 *    1       10.00     10.10.2020
 *    1       20.00     10.10.2020
 * Если key field = {time_of_day}, то отображаться будет:
 *
 *  user      |    date
 *  >10.00    |
 *      1     |    10.10.2020
 *  >20.00    |
 *      1     |    10.10.2020
*/
inline void TreeModel::setKeyFields(const QStringList& names){
    namesOfKeyFields = names;
}

inline void TreeModel::setKeyFields(QStringList&& names){
    namesOfKeyFields = std::move(names);
}

inline QStringList TreeModel::getKeyFields(){
    return namesOfKeyFields;
}

#endif // TREEMODEL_H
