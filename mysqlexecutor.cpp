#include "mysqlexecutor.h"
#include <QSqlQuery>

MySqlExecutor::MySqlExecutor(QObject *parent){}

bool MySqlExecutor::addNewUserIntoDB(const User& user){
    QStringList insertValues{
                                std::move(wrap('\'',user.getName())),
                                std::move(wrap('\'',user.getSurname())),
                                std::move(wrap('\'',user.getBirthDate().toString(Qt::ISODate))),
                                std::move(wrap('\'',user.getPathToAvatar()))
                             };
    qDebug() << insertValues.join(',');
    QSqlQuery addNewUser(" INSERT INTO user (name,surname,birthdate,path_to_avatar)"
                         " VALUES ("+insertValues.join(',')+")");
    if (addNewUser.exec())
        return true;
    qDebug() <<addNewUser.lastError();
    return false;
}

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


