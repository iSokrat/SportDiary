#include "imagemodel.h"

#include <QGraphicsPixmapItem>

ImageModel::ImageModel(QObject* parent):
    QGraphicsScene(parent){
    addText(std::move(tr("Загрузить изображение...")));
}

void ImageModel::mousePressEvent(QGraphicsSceneMouseEvent *event){
    // Открываем диалоговое окно открытия
    // изображения, при выборе файла сохраняем
    // его имя в pathToImage
    QString pathToImage = QFileDialog::getOpenFileName(nullptr,
                                 tr("Открыть изображение..."),
                                 tr("/home"),
                                 tr("Изображение (*.png *.jpg *.bmp)"));

    // Если изображение не было выбрано -
    // ничего не делаем
    if (pathToImage.isEmpty())
        return;

    // Устанавливаем новый путь к изображению
    this->pathToImage = pathToImage;

    // Очищаем сцену
    clear();

    // Добавляем на сцену изображение
    // на основе pathToImage
    addPixmap(std::move(QPixmap(pathToImage)));

    QGraphicsScene::mousePressEvent(event);
}

