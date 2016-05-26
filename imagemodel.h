#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QGraphicsScene>
#include <QFileDialog>

class ImageModel : public QGraphicsScene
{
public:
    ImageModel(QObject *parent = nullptr);
    QString getPathToImage() const;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    QString pathToImage{":/images/defaultAvatar.jpg"};
};

inline QString ImageModel::getPathToImage() const{
    return pathToImage;
}

#endif // IMAGEMODEL_H
