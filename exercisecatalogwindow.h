#ifndef EXERCISECATALOGWINDOW_H
#define EXERCISECATALOGWINDOW_H

#include <QDialog>

namespace Ui {
class ExerciseCatalogWindow;
}

class ExerciseCatalogWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ExerciseCatalogWindow(QWidget *parent = 0);
    ~ExerciseCatalogWindow();

private:
    Ui::ExerciseCatalogWindow *ui;
};

#endif // EXERCISECATALOGWINDOW_H
