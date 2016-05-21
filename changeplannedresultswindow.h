#ifndef CHANGEPLANNEDRESULTSWINDOW_H
#define CHANGEPLANNEDRESULTSWINDOW_H

#include <QDialog>

namespace Ui {
class ChangePlannedResultsWindow;
}

class ChangePlannedResultsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePlannedResultsWindow(QWidget *parent = 0);
    ~ChangePlannedResultsWindow();

private:
    Ui::ChangePlannedResultsWindow *ui;
};

#endif // CHANGEPLANNEDRESULTSWINDOW_H
