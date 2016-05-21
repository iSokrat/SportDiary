#include "treemodel.h"
#include "treeitem.h"

#include <QDebug>
TreeModel::TreeModel(const QStringList &headers,
                     const QString &textForQuery,
                     const QString& nameOfKeyField,
                     QObject *parent)
    :QAbstractItemModel(parent){

    QVector<QVariant> rootData;
    foreach (const QString& header, headers)
        rootData << header;

    rootItem = new TreeItem(rootData);
    setKeyField(nameOfKeyField);
    setupModelData(textForQuery,rootItem);
}

TreeModel::~TreeModel(){
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    // Получаем информацию из определенной ячейки
    QVariant data = std::move( item->data( index.column() ) );
    /* Возвращаем конвертированную версию данных
     * К примеру, информацию типа даты "1111-11-11 11:11:11"
     * конвертируем в заданный стиль для даты (к примеру,
     * выведем только время с текстом "ВРЕМЯ: 11:11:11")
    */

    QVariant decoratedData = std::move(getConvertData(data));
    return decoratedData;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success = false;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = false;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success = false;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

void TreeModel::setDataFromSqlQuery(const QString &textForQuery){

    // Создаем контейнер, где сохраним значение header для таблицы
    QVector<QVariant> rootData;
    rootData.reserve(rootItem->columnCount());
    // Сохраняем значения заголовка rootItem
    for (auto column = 0 ;column<rootItem->columnCount();column++)
        rootData.push_back(rootItem->data(column));

    // если корень не пуст, то очищаем
    // полностью наше дерево с информацией
    if (rootItem!= nullptr)
        delete rootItem;

    rootItem = new TreeItem(rootData);
    setupModelData(textForQuery, rootItem);

}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void TreeModel::setupModelData(const QString &textForQuery, TreeItem *parent)
{
    QSqlQuery query(textForQuery);
    // Если запрос при выполнении встретил ошибку
    if (!query.exec()){
        qDebug()<<query.lastError();
        return;
    }

    auto keyField=getKeyField();

    // Если ключевое слово не задано, то берем первое поле
    if (keyField.isEmpty())
        keyField = query.record().fieldName(0);

    // Пока есть записи, полученные при запросе
    while (query.next()){

        // Вставляем один новый узел на последнюю позицию с количеством
        // столбцов, равным количеству заголовочных столбцов у parent
        bool statusOfInsertion = parent->insertChildren(parent->childCount(),1,parent->columnCount());
        // Если вставка прошла с ошибкой, то переходим
        // к следующей записи
        if (!statusOfInsertion)
            continue;

        // Получаем вставленный узел (ребенка parent)
        auto* dropDownNode = parent->child(parent->childCount()-1);

        // Полученный узел является раскрывающимя списком,
        // устанавливаем для него текст в первом поле
        QVariant data = query.value(keyField);
        dropDownNode->setData(0,data);

        // Создаем внутреннее наполнение списка
        do{
            // Если значение по ключевовому полю не совпадает с предыдущими значениями
            if (query.value(keyField)!=data){
                // Делаем шаг назад для того, чтобы не пропустить запись случайно
                // из-за этого цикла
                query.previous();
                break;
            }

            // Вставляем элемент в раскрывающийся список
            dropDownNode->insertChildren(dropDownNode->childCount(),1,parent->columnCount());
            // Получаем его
            auto* childOfDropDownNode = dropDownNode->child(dropDownNode->childCount()-1);

            // Вставляем значения в элемент, исключая поле, имя которого
            // совпадает с ключевым полем ( его значение и так уже выведено
            // в заголовке раскрывающегося списка )
            auto dataIndex=0;
            for (auto fieldPos = 0; fieldPos< query.record().count();fieldPos++)
                if (query.record().fieldName(fieldPos)!=keyField)
                    childOfDropDownNode->setData(dataIndex++,query.value(fieldPos));

        }while(query.next());
    }
}

/*
 * Конвертирует информацию data в соответсвующее
 * декорированное значение (сама data
 * не изменяется, мы получаем модифициорванную копию).
 * Внутри метода происходит определение типа,
 * на основе него дается декорированное значение.
*/
QVariant TreeModel::getConvertData(const QVariant& data) const{

    if (data.type() == QMetaType::QDateTime)
        return QVariant("Время: "+data.toDateTime().toString("hh:mm"));
    return data;
}
