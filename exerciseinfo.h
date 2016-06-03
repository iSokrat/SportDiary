#ifndef EXERCISEINFO_H
#define EXERCISEINFO_H

#include <QString>

class ExerciseInfo
{
    static const char defaultPath[];
public:
    ExerciseInfo() = default;
    ExerciseInfo(const ExerciseInfo &) = default;
    ExerciseInfo(ExerciseInfo&&) = default;
    ExerciseInfo& operator =(const ExerciseInfo&) = default;
    ExerciseInfo& operator =(ExerciseInfo&&) = default;

    ExerciseInfo(const QString& name,
                 const QString& describtion,
                 const QString& pathToImage = ":/images/src/defaultImageForContent.jpeg",
                 const QString& typeOfLoad = "");

    QString getName() const{ return name;}
    QString getDescribtion() const{return describtion;}
    QString getPathToImage() const{return pathToImage;}
    QString getTypeOfLoad() const{return typeOfLoad;}
private:
    QString name{},
            describtion{},
            pathToImage{":/images/src/defaultImageForContent.jpeg"},
            typeOfLoad{};
};
#endif // EXERCISEINFO_H
