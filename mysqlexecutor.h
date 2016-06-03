#ifndef MYSQLEXECUTOR_H
#define MYSQLEXECUTOR_H

#include "user.h"
//---------------------
#include <QDebug>
#include <QtSql>

class MySqlExecutor{
private:
    static QString wrap(const QString &wrapSymbols, const QString& string);
public slots:
    using cortegeType = std::tuple <QString,QString,quint32>;

    MySqlExecutor(QObject* parent = nullptr);
    static bool addNewUserIntoDB(const User& user);
    static ExerciseInfo getInfoAboutExercise(qint32 idExercise);

    static QVector<cortegeType> getInfoOfExercisesForCertainMuscules(const QStringList& muscules);
    static bool initUser(quint32 idUser, User &forUser);
    static QString getTextQueryForNutrTableOfPlannedResult(const User& autorizedUser,const QVariant& date);
    static QString getTextQueryForTrainingTableOfPlannedResult(const User& autorizedUser,const QVariant& date);

};

inline
QString MySqlExecutor::wrap(const QString& wrapSymbols, const QString &string){
    QString revertSymbols = wrapSymbols;
    auto end = revertSymbols.size()-1;

    for (auto i = 0;i<revertSymbols.size()/2;i++){
        const QChar tmp = revertSymbols[i];
        revertSymbols[i] = revertSymbols[end-i];
        revertSymbols[end-i] = tmp;
    }

    return wrapSymbols+string+revertSymbols;
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
    return " SELECT rsp.date_of_start_training,"
           "        exr.name,"
           "        rsp.count_of_reps,"
           "        rsp.using_weight,"
           "        rsp.executing_time"
           " FROM result_strength_palanned as rsp,exercise as exr"
           " WHERE id_user = "+QString::number(autorizedUser.getId())+
           "   AND CONVERT(date_of_start_training,DATE) = "+wrap("\'", date.toDate().toString("yyyy-MM-dd") )+
           "   AND rsp.id_exercise = exr.id_exercise"
           " ORDER BY date_of_start_training,exr.name";
}


#endif // MYSQLEXECUTOR_H
