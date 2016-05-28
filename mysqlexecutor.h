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
    static QString getTextQueryForNutrTableOfPlannedResult(const User& autorizedUser,const QVariant& date);
    static QString getTextQueryForTrainingTableOfPlannedResult(const User& autorizedUser,const QVariant& date);
};

inline
QString MySqlExecutor::wrap(QChar wrapSymbol, const QString &string){
    return wrapSymbol+string+wrapSymbol;
}

inline
QString MySqlExecutor::getTextQueryForNutrTableOfPlannedResult(const User& autorizedUser,const QVariant& date){

    // Если переданный тип не может быть конвертирован в QDate
    // То возвращаем пустой запрос
    if (!date.canConvert<QDate>())
        return QString{};

    //  Данный текст для запроса выбирает информацию
    // о питании для авторизированного пользователя
    // на определенную дату
    QString textQueryForNutrTableOfPlannedResult = "SELECT "
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

    return textQueryForNutrTableOfPlannedResult;
}

inline QString MySqlExecutor::getTextQueryForTrainingTableOfPlannedResult(const User &autorizedUser, const QVariant &date){
    return " SELECT date_of_start_training,"
           "        id_exercise,"
           "        count_of_reps,"
           "        using_weight,"
           "        executing_time"
           " FROM result_strength_palanned"
           " WHERE id_user = "+QString::number(autorizedUser.getId())+
           "       AND CONVERT(date_of_start_training,DATE) = "+wrap('\'', date.toDate().toString("yyyy-MM-dd") )+
           " ORDER BY date_of_start_training";
}


#endif // MYSQLEXECUTOR_H
