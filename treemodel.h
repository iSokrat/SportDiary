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
    TreeModel(const QStringList &headers, const QString &textForQuery, const QString &nameOfKeyField, QObject *parent = nullptr);
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

    QString getKeyField();

    void setDataFromSqlQuery(const QString& textForQuery);
private:
    void setupModelData(const QString &textForQuery, TreeItem *parent);
    QVariant getConvertData(const QVariant &data) const;
    TreeItem *getItem(const QModelIndex &index) const;
    void setKeyField(const QString& name);
    void setKeyField(QString&& name);

    // Корневой Item,в нем хранится информация для заголовка
    TreeItem *rootItem;

    // Хранит имя столбца таблицы,
    // значение которого должно служить в качестве раскрывающегося списка
    QString nameOfKeyField{};
};

// inline методы

/* Вставляет ключевое поле по имени name, по которому будет
 * отображаться строка в дереве
 *
 * Пример:
 *  Таблица с доменом:
 *  id_user time_of_day date
 *    1       10.00     10.10.2020
 *    1       20.00     10.10.2020
 * Если key field = time_of_day, то отображаться будет:
 *
 *  user      |    date
 *  >10.00    |
 *      1     |    10.10.2020
 *  >20.00    |
 *      1     |    10.10.2020
*/
inline void TreeModel::setKeyField(const QString& name){
    nameOfKeyField = name;
}

inline void TreeModel::setKeyField(QString &&name){
    nameOfKeyField = std::move(name);
}

inline QString TreeModel::getKeyField(){
    return nameOfKeyField;
}

#endif // TREEMODEL_H
