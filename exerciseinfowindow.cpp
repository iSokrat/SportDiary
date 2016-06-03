#include "exerciseinfowindow.h"
#include "ui_exerciseinfowindow.h"

ExerciseInfoWindow::ExerciseInfoWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExerciseInfoWindow)
{
    ui->setupUi(this);
}

ExerciseInfoWindow::~ExerciseInfoWindow()
{
    delete ui;
}
