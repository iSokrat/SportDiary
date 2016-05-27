#ifndef PATHSOFFILES_H
#define PATHSOFFILES_H

#include <QDir>

namespace{

struct PathsOfFiles{
public:
    static const QDir uploadsDirForAvatar;
};

const QDir PathsOfFiles::uploadsDirForAvatar = QDir("/uploads/images/avatars");

}
#endif // PATHSOFFILES_H
