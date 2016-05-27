#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>

class User;
class FileManager
{
public:
    FileManager();

    static QString uploadAvatarForUser(const User& user);
private:
    static QString getHashFileName(const User& user);
};

#endif // FILEMANAGER_H
