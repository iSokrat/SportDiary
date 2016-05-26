#include "user.h"

User::User(const qint32 id,
           const QString &name,
           const QString &surname,
           const QString &pathToAvatar, const QDateTime &birthDate):
    id(id),
    name(name),
    surname(surname),
    pathToAvatar(pathToAvatar),
    birthDate(birthDate){}


/*
 * Меняет пользователя на нового user
*/
User& User::operator=(const User &user){

    if (&user == this)
        return *this;
    id = user.id;
    name = user.name;
    surname = user.surname,
    pathToAvatar = user.pathToAvatar;
    birthDate = user.birthDate;
    return *this;
}

/*
 * Меняет пользователя на нового user (используется семантика перемещения)
*/
User& User::operator=(const User&& user){
    id = std::move(user.id);
    name = std::move(user.name);
    surname = std::move(user.surname);
    pathToAvatar = std::move(user.pathToAvatar);
    birthDate = user.birthDate;
    return *this;
}
