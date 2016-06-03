#include "exerciseinfo.h"

ExerciseInfo::ExerciseInfo(const QString &name,
                           const QString &describtion,
                           const QString &pathToImage,
                           const QString &typeOfLoad):
    name(name),
    describtion(describtion),
    pathToImage(pathToImage),
    typeOfLoad(typeOfLoad){}
