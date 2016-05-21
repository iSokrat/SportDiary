#include "userselectiondialogwindow.h"
#include "ui_userselectiondialogwindow.h"

#include <userinfowidget.h>
#include <infoaboutuserdelegate.h>
/*-----------------------*/
#include <QDebug>
#include <QJsonArray>
UserSelectionDialogWindow::UserSelectionDialogWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserSelectionDialogWindow)
{
    ui->setupUi(this);

    // Создаем переменные для хранения названия полей БД
    QString fieldId = "id_user";
    QString fieldName = "name";
    QString fieldSurname = "surname";
    QString fieldPathToAvatar = "path_to_avatar";

    // Создаем запрос для выборки всех пользователей
    //Сам текст запроса
    QString textQuery(" SELECT "+fieldId+","+fieldName+","+fieldSurname+","+fieldPathToAvatar+
                      " FROM user"
                      " ORDER BY "+fieldSurname);
    QSqlQuery usersQuery(textQuery);
    usersQuery.exec();

    // Заполняем спиок пользователей информацией из базы данных
    while (usersQuery.next()){
        // Создаем виджет для отображения инф-ии для отдельного пользователя
        UserInfoWidget *userInfoWidget = new UserInfoWidget(ui->listOfUsersView);

        // Получаем информацию из БД
        auto userId = usersQuery.record().value(fieldId).toInt();
        auto userName = usersQuery.record().value(fieldName).toString(),
             userSurname = usersQuery.record().value(fieldSurname).toString(),
             userPathToAvatar = usersQuery.record().value(fieldPathToAvatar).toString();

        // Записываем эту информацию в объект JSON
        QJsonObject userInfo;
        userInfo.insert(fieldId,QJsonValue(userId));
        userInfo.insert(fieldName,QJsonValue(userName));
        userInfo.insert(fieldSurname,QJsonValue(userSurname));
        userInfo.insert(fieldPathToAvatar,QJsonValue(userPathToAvatar));


        // Заполняем поля виджета информацией из БД
        userInfoWidget->setName(userName);
        userInfoWidget->setSurname(userSurname);
        userInfoWidget->setImage(userPathToAvatar);

        // Создаем item для списка пользователей (listOfUsersView)
        QListWidgetItem *item = new QListWidgetItem(ui->listOfUsersView);

        // Устанавливаем данные о пользователе
        item->setData(Qt::DisplayRole,userInfo);

        // Ставим размеры для item на основе виджета user
        item->setSizeHint(userInfoWidget->sizeHint());
        ui->listOfUsersView->setItemWidget(item,userInfoWidget);
    }

    // При 2-ом щелчке по определенному пользователю мы высылаем сигнал о
    // смене пользователя
    connect(ui->listOfUsersView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(userIsSelected(QModelIndex)));

    // При вводе информации в поиск производим поиск в БД
    connect(ui->searchWidget,SIGNAL(textEdited(QString)),this,SLOT(setInfoForFoundUsers(QString)));
}

UserSelectionDialogWindow::~UserSelectionDialogWindow()
{
    delete ui;
}

UserSelectionDialogWindow *UserSelectionDialogWindow::createUserSelectionDialogWindow(QWidget *parent)
{
    return new UserSelectionDialogWindow(parent);
}

void UserSelectionDialogWindow::userIsSelected(QModelIndex index)
{

    emit userChanged(index.data());
    close();
}

/* Ищет информацию о пользователях на основе
 * searchInfo и устанавливает ее в listView
*/
void UserSelectionDialogWindow::setInfoForFoundUsers(const QString& searchInfo)
{
    // Создаем переменные для хранения названия полей БД
    QString fieldId = "id_user";
    QString fieldName = "name";
    QString fieldSurname = "surname";
    QString fieldPathToAvatar = "path_to_avatar";

    // Создаем текст запроса
    QString textQuery{};

    // Если информации для поиска нет
    if (searchInfo.isEmpty()){
        // Ищем всех пользователей
        textQuery =  " SELECT "+fieldId+","+fieldName+","+fieldSurname+","+fieldPathToAvatar+
                     " FROM user"
                     " ORDER BY "+fieldSurname;
    }else{
        // Пока что поиск производится только по фамилии
        QString surname = searchInfo;

        textQuery =  " SELECT "+fieldId+","+fieldName+","+fieldSurname+","+fieldPathToAvatar+
                     " FROM user "
                     " WHERE "+fieldSurname+"="+surname+
                     " ORDER BY "+fieldSurname;
    }

    ui->listOfUsersView->clear();

    // Создаем запрос и выполняем его
    QSqlQuery usersQuery(textQuery);
    usersQuery.exec();
    qDebug() <<usersQuery.lastError();
    // Проверяем результат запроса на пустоту
    // Переходим на первую запись
    usersQuery.next();
    if (!usersQuery.isValid())
        return;

    // Заполняем спиок пользователей информацией из базы данных
    while (usersQuery.next()){
        // Создаем виджет для отображения инф-ии для отдельного пользователя
        UserInfoWidget *userInfoWidget = new UserInfoWidget(ui->listOfUsersView);

        // Получаем информацию из БД
        auto userId = usersQuery.record().value(fieldId).toInt();
        auto userName = usersQuery.record().value(fieldName).toString(),
             userSurname = usersQuery.record().value(fieldSurname).toString(),
             userPathToAvatar = usersQuery.record().value(fieldPathToAvatar).toString();

        // Записываем эту информацию в объект JSON
        QJsonObject userInfo;
        userInfo.insert(fieldId,QJsonValue(userId));
        userInfo.insert(fieldName,QJsonValue(userName));
        userInfo.insert(fieldSurname,QJsonValue(userSurname));
        userInfo.insert(fieldPathToAvatar,QJsonValue(userPathToAvatar));


        // Заполняем поля виджета информацией из БД
        userInfoWidget->setName(userName);
        userInfoWidget->setSurname(userSurname);
        userInfoWidget->setImage(userPathToAvatar);

        // Создаем item для списка пользователей (listOfUsersView)
        QListWidgetItem *item = new QListWidgetItem(ui->listOfUsersView);

        // Устанавливаем данные о пользователе
        item->setData(Qt::DisplayRole,userInfo);

        // Ставим размеры для item на основе виджета user
        item->setSizeHint(userInfoWidget->sizeHint());
        ui->listOfUsersView->setItemWidget(item,userInfoWidget);
    }
}
