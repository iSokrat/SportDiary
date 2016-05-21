#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User() = default;

    User(const qint32 id,
         const QString& name,
         const QString& surname,
         const QString& pathToAvatar);

    void setId(quint32 id);
    void setName(const QString& name);
    void setSurname(const QString& surname);
    void setPathToAvatar(const QString& pathToAvatar);

    quint32 getId() const;
    QString getName() const;
    QString getSurname() const;
    QString getPathToAvatar() const;

    User &operator=(const User& user);
    User &operator=(const User&& user);
private:
    /* При добавлении новых полей не забыть изменить соответствующие операторы присваивания и
     * конструкторы
    */
    quint32 id = 0;
    QString name{"-"},
            surname{"-"},
            pathToAvatar{"-"};

};

inline void User::setId(quint32 id)
{
    this->id = id;
}

inline void User::setName(const QString &name)
{
    this->name = name;
}

inline void User::setSurname(const QString &surname)
{
    this->surname = surname;
}

inline void User::setPathToAvatar(const QString &pathToAvatar)
{
    this->pathToAvatar = pathToAvatar;
}

inline quint32 User::getId() const
{
    return id;
}

inline QString User::getName() const
{
    return name;
}

inline QString User::getSurname() const
{
    return surname;
}

inline QString User::getPathToAvatar() const
{
    return pathToAvatar;
}

#endif // USER_H
