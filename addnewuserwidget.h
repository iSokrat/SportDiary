#ifndef ADDNEWUSERWIDGET_H
#define ADDNEWUSERWIDGET_H

#include <QDialog>
#include <user.h>
#include "imagemodel.h"

namespace Ui {
class AddNewUserWidget;
}

class AddNewUserWidget : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewUserWidget(QWidget *parent = 0);
    ~AddNewUserWidget();

    User getNewUser() const;
signals:
    void accepted(const User& newUser);

public slots:
    void accept() override;

private:
    void setupWindow();
    void extractInfoFromWidgetsToUser(User* user);
    bool validateForm();
private:
    Ui::AddNewUserWidget *ui;
    User* newUser = nullptr;
    ImageModel* avatarModel = nullptr;
};

inline User AddNewUserWidget::getNewUser() const{
    return *newUser;
}

#endif // ADDNEWUSERWIDGET_H
