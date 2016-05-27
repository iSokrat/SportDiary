#include "filemanager.h"

#include "user.h"
#include "pathsoffiles.h"
//---------------------
#include <QFile>
#include <QDir>
#include <QDebug>

FileManager::FileManager(){

}

/*
 * Скачивает файл в рабочую папку с проектом
 * возвращает путь к скачанному файлу
*/
QString FileManager::uploadAvatarForUser(const User &user){
    // Получаем имя для нового пользователя
    QString newFileName = std::move(getHashFileName(user));

    // Определяем информацию о файле
    QFileInfo clientPathToAvatarInfo(user.getPathToAvatar());

    // Формируем путь для сохранения файла
    auto newLocation =  QDir::currentPath()+
                        PathsOfFiles::uploadsDirForAvatar+
                        QDir::separator()+
                        newFileName+'.'+
                        clientPathToAvatarInfo.suffix();

    qDebug() <<newLocation;
    if (!QFile::copy( user.getPathToAvatar(),newLocation)){
        qDebug() <<" В uploadAvatarForUser: файл не скачен!";
        return QString();
    }
    else
        return newLocation;
}

/*
 * Формирование уникального имени для пользователя
 *  user
*/
QString FileManager::getHashFileName(const User &user){
    QString hashName = QString::number(user.getName())+
                        '_'+
                        QDateTime::currentDateTime().toString();
    return std::move(hashName);
}

