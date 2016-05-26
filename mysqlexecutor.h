#ifndef MYSQLEXECUTOR_H
#define MYSQLEXECUTOR_H

#include "user.h"
//---------------------
#include <QDebug>
#include <QtSql>

class MySqlExecutor{
private:
    static QString wrap(QChar wrapSymbol,const QString& string);
public slots:
    MySqlExecutor(QObject* parent = nullptr);
    static bool addNewUserIntoDB(const User& user);
    static bool initUser(quint32 idUser, User &forUser);
    static QString getTextQueryForPlannedResult(const User& autorizedUser,const QVariant& date);
};

inline
QString MySqlExecutor::wrap(QChar wrapSymbol, const QString &string){
    return wrapSymbol+string+wrapSymbol;
}

inline
QString MySqlExecutor::getTextQueryForPlannedResult(const User& autorizedUser,const QVariant& date){

    // Если переданный тип не может быть конвертирован в QDate
    // То возвращаем пустой запрос
    if (!date.canConvert<QDate>())
        return QString{};

    //  Данный текст для запроса выбирает информацию
    // о питании для авторизированного пользователя
    // на определенную дату
    QString textQueryForPlannedResult = "SELECT "
                                        "       eating.date"
                                        "      ,products.name"
                                        "      ,products.protein"
                                        "      ,products.fat"
                                        "      ,products.carbohydrates"
                                        "      ,eating.weight_food "
                                        "      ,eating.id_eating "
                                        "FROM   nutrition_product as products,"
                                        "       eating "
                                        "WHERE  eating.id_user = "+QString::number(autorizedUser.getId())+
                                        "   AND eating.id_product = products.id_product "
                                        "   AND CONVERT(eating.date,DATE) = '"+date.toDate().toString("yyyy-MM-dd")+"'"
                                        " ORDER BY DATE";

    return textQueryForPlannedResult;
}

#endif // MYSQLEXECUTOR_H
