#ifndef SQLSELECTCREATOR_H
#define SQLSELECTCREATOR_H

#include <QMap>
#include <QPair>
class SqlSELECTCreator
{
public:
    void addSelectField(const QString& tableName,const QString& fieldName);
    void addWhereExpression();
    ~SqlSELECTCreator();

private:
    QMap <QString,QString> *tableField;
};

#endif // SQLSELECTCREATOR_H
