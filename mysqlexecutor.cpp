#include "mysqlexecutor.h"
//--------------------------
#include <QSqlQuery>
#include <tuple>

MySqlExecutor::MySqlExecutor(QObject *parent){}

bool MySqlExecutor::addNewUserIntoDB(const User& user){
    QStringList insertValues{
                                std::move(wrap("\'",user.getName())),
                                std::move(wrap("\'",user.getSurname())),
                                std::move(wrap("\'",user.getBirthDate().toString(Qt::ISODate))),
                                std::move(wrap("\'",user.getPathToAvatar()))
                             };
    qDebug() << insertValues.join(',');
    QSqlQuery addNewUser(" INSERT INTO user (name,surname,birthdate,path_to_avatar)"
                         " VALUES ("+insertValues.join(',')+")");
    if (addNewUser.exec())
        return true;
    qDebug() <<addNewUser.lastError();
    return false;
}

ExerciseInfo MySqlExecutor::getInfoAboutExercise(qint32 idExercise){
    QSqlQuery query(" SELECT * "
                    " FROM exercise"
                    " WHERE id_exercise = "+QString::number(idExercise));

    if (!query.exec()){
        qDebug() <<query.lastError();
        return ExerciseInfo{};
    }

    // Переходим на первую запись
    // Если ошибка
    if (!query.next()){
        qDebug() <<"Записи с id = "+QString::number(idExercise)+" "
                   "не найдена.";
        return ExerciseInfo{};
    }

    ExerciseInfo exercise{ query.value("name").toString(),
                           query.value("describtion").toString() };

    return std::move(exercise);
}

/*
 * Возвращает список описания упражнениц, в которых
 * включена работа muscules.
*/
QVector<MySqlExecutor::cortegeType>
MySqlExecutor::getInfoOfExercisesForCertainMuscules(const QStringList &muscules){

    // Если передали пустой список,
    // его же и возвращаем
    if (muscules.isEmpty())
        return QVector<cortegeType>{};

    // Формируем LIKE выражение
    // для каждого элемента muscules
    QString LikeExpr = "";
    for (const auto& muscule : muscules)
        LikeExpr.append(" for_muscules LIKE "+
                        wrap("'%",muscule)+
                        " OR ");

    // Убираем лишний AND в конце выражения (включая пробел)
    LikeExpr.chop(4);

    // Строим запрос
    QSqlQuery query{" SELECT name,path_to_general_image,id_exercise"
                        " FROM exercise "
                        " WHERE "+LikeExpr};

    if (!query.exec()){
        qDebug() <<query.lastError();
        return QVector<cortegeType>{};
    }

    QVector<cortegeType> result;
    while (query.next())
        result.push_back(std::make_tuple(query.value(0).toString(),
                                         query.value(1).toString(),
                                         query.value(2).toUInt()));

    return result;
}

/*
 * Инициализация forUser данными из БД
 * , где id пользователя = idUser
*/
bool MySqlExecutor::initUser(quint32 idUser,User& forUser){

    QString fieldId = "id_user",
            fieldName = "name",
            fieldSurname = "surname",
            fieldPathToAvatar = "path_to_avatar";

    QString queryText = "SELECT * "
                        "FROM user "
                        "WHERE "+fieldId+" ="+QString::number(idUser);

    // Совершаем запрос на основе текста запроса
    QSqlQuery queryToAutorizedUser{queryText};
    if (!queryToAutorizedUser.exec()){
        qDebug() <<queryToAutorizedUser.lastError();
        return false;
    }
    queryToAutorizedUser.next();

    // Если запись не действительна
    if (!queryToAutorizedUser.isValid())
        return false;

    // Инициализируем авторизированного пользователя
    forUser.setId(idUser);
    forUser.setName(queryToAutorizedUser.record().value(fieldName).toString());
    forUser.setSurname(queryToAutorizedUser.record().value(fieldSurname).toString());
    forUser.setPathToAvatar(queryToAutorizedUser.record().value(fieldPathToAvatar).toString());
    return true;
}


