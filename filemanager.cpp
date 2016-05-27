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

    // Если файл не скопировался
    if (!QFile::copy( user.getPathToAvatar(),newLocation)){
        qDebug() <<" В uploadAvatarForUser: файл не скачен!";
        return QString();
    }
    else // Если файл скопирован успешно
        return newLocation;
}

/*
 *  Формирование уникального имени для пользователя
 *  user
*/
QString FileManager::getHashFileName(const User &user){
    QString hashName =  user.getName()+
                        '_'+
                        QDateTime::currentDateTime().toString(Qt::ISODate);
    return std::move(hashName);
}

