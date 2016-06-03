#ifndef EXERCISEINFOWINDOW_H
#define EXERCISEINFOWINDOW_H

#include <QDialog>

namespace Ui {
class ExerciseInfoWindow;
}

class ExerciseInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ExerciseInfoWindow(QWidget *parent = 0);
    ~ExerciseInfoWindow();

private:
    Ui::ExerciseInfoWindow *ui;
};

#endif // EXERCISEINFOWINDOW_H
