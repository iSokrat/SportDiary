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
    // Формируем путь для сохранения файла
    auto newLocation = PathsOfFiles::uploadsDirForAvatar;


    if (!QFile::copy( user.getPathToAvatar(),newLocation.currentPath() )){
        qDebug() <<" В uploadAvatarForUser: файл не скачен!";
        return QString();
    }
    else
        return newLocation.currentPath();
}

