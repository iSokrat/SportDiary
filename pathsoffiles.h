#ifndef PATHSOFFILES_H
#define PATHSOFFILES_H

#include <QDir>

namespace{

struct PathsOfFiles{
public:
    static const QString uploadsDirForAvatar;
};

const QString PathsOfFiles::uploadsDirForAvatar = QString("/uploads/images/avatars");

}
#endif // PATHSOFFILES_H
