#ifndef EXERCISECATALOGWINDOW_H
#define EXERCISECATALOGWINDOW_H

#include <QDialog>
#include <QListWidgetItem>

class QStringList;

namespace Ui {
class ExerciseCatalogWindow;
}

class ExerciseCatalogWindow : public QDialog
{
    Q_OBJECT

public:
     explicit ExerciseCatalogWindow(QWidget *parent = 0);
    ~ExerciseCatalogWindow();

        enum TabIndexType:int   { Legs = 0, Back, Chest, Biceps,
                                  Triceps, Abdominal, Forearms, EndTabIndex };

public slots:
    void openInfoWindowForExercise(QListWidgetItem* item);
private slots:
    void setInfoForTabFromDB(int tabIndex);
private:
    void setupConnection();
    void setupUI();

    template <typename InfoType>
    void setupInfoForTab(int tabIndex,
                         const QVector<InfoType> &records);
// Inline methods
    QStringList getTagsForFindingLegsMuscules(){ return {"glutes","quads","hamstrings","calves"};}
    QStringList getTagsForFindingBackMuscules(){ return {"traps","lats","middleback","lower back"};}
    QStringList getTagsForFindingChestMuscules(){ return {"chest"};}
    QStringList getTagsForFindingBicepsMuscules(){ return {"biceps"};}
    QStringList getTagsForFindingTricepsMuscules(){ return {"triceps"}; }
    QStringList getTagsForFindingAbdominalMuscules() { return {"abdominal","abs"}; }
    QStringList getTagsForFindingForearmsMuscules() { return {"forearm"}; }
//~ Inline methods

    template <typename RecordType>
    QListWidgetItem *createExerciceListItem(qint32 height,
                                            const RecordType &record);

private:
    Ui::ExerciseCatalogWindow *ui;

    QHash <TabIndexType,QListWidget*> viewsForExercises{};
};

#endif // EXERCISECATALOGWINDOW_H
