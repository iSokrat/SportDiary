#include "sqlselectcreator.h"

SqlSELECTCreator::SqlSELECTCreator():
    tableField(new QMap <QString,QString>){

}


void SqlSELECTCreator::addSelectField(const QString &tableName, const QString &fieldName){
    tableField->insert(tableName,fieldName);
}

SqlSELECTCreator::~SqlSELECTCreator(){
    delete tableField;
}
