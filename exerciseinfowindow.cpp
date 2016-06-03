#include "exerciseinfowindow.h"
#include "ui_exerciseinfowindow.h"

ExerciseInfoWindow::ExerciseInfoWindow(const ExerciseInfo &info,
                                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExerciseInfoWindow),
    info(QSharedPointer<ExerciseInfo>::create()){

    ui->setupUi(this);
    updateUiContent(info);
    setExerciseInfo(info);

    // При изменении информации обновляем контент окна this
    connect(this,
            SIGNAL(changeInfo(ExerciseInfo)),
            SLOT(updateUiContent(ExerciseInfo)));
}

ExerciseInfoWindow::~ExerciseInfoWindow(){
    delete ui;
}

void ExerciseInfoWindow::setExerciseInfo(const ExerciseInfo &info){
    auto internalInfo = this->info.data();
    *internalInfo = info;
    emit changeInfo(info);
}

/* Обновляет графический интерфейс новой информацией об упражнении
 * info.
*/
void ExerciseInfoWindow::updateUiContent(const ExerciseInfo& info){
    setWindowTitle(info.getName());
    auto&& image =QPixmap::fromImage(QImage(info.getPathToImage()));
    ui->imageExercise->setPixmap(std::move(image));
    ui->describtion->setText(info.getDescribtion());
}
