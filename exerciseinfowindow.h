#ifndef EXERCISEINFOWINDOW_H
#define EXERCISEINFOWINDOW_H

#include <QDialog>
#include  "exerciseinfo.h"

namespace Ui {
class ExerciseInfoWindow;
}

class ExerciseInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ExerciseInfoWindow(const ExerciseInfo& info,
                                QWidget *parent = 0);
    virtual ~ExerciseInfoWindow();

    void setExerciseInfo(const ExerciseInfo& info);
signals:
    void changeInfo(const ExerciseInfo &info);
protected slots:
    virtual void updateUiContent(const ExerciseInfo &info);
private:
    Ui::ExerciseInfoWindow *ui;

    QSharedPointer<ExerciseInfo> info;
};

#endif // EXERCISEINFOWINDOW_H
