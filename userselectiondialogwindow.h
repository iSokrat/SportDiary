#ifndef USERSELECTIONDIALOGWINDOW_H
#define USERSELECTIONDIALOGWINDOW_H

#include "user.h"
//-----------------
#include <QDialog>
#include <QSharedPointer>
#include <QtSql>

namespace Ui {
class UserSelectionDialogWindow;
}

class UserSelectionDialogWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UserSelectionDialogWindow(QWidget *parent = 0);
    ~UserSelectionDialogWindow();
    static UserSelectionDialogWindow *createUserSelectionDialogWindow(QWidget *parent = 0);
signals:

    // Высылка сигнала об изменении пользователя
    void userChanged(const User& userInfo);

private slots:
    void userIsSelected(QModelIndex index);
    void setInfoForFoundUsers(const QString &searchInfo);
private:
     User convertJsonInfoToUserInfo(const QJsonObject& info) const;
private:
    Ui::UserSelectionDialogWindow *ui;

};

#endif // USERSELECTIONDIALOGWINDOW_H
