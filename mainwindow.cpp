#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "userselectiondialogwindow.h"
#include "userinfowidget.h"
#include "treemodel.h"
/*-------------------------------------------------------*/
#include <QtSql>
#include <QSharedPointer>
#include <QVBoxLayout>
#include <QSplitter>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initActualUserFromDB();
    updateUserInfoPanel(autorizedUser);

    // Создаем компоновщик для userInfoWidget и ui->TabsOfSportFields
    QVBoxLayout* centralLayout = new QVBoxLayout(this);
    centralLayout->addWidget(&userInfoPanel);
    centralLayout->addWidget(ui->TabsOfSportFields);

    // Устанавливаем компоновщик для центрального виджета нашего окна
    ui->centralWidget->setLayout(centralLayout);

    // Загружаем информацию из БД в таблицу запланированных результатов
    // на определенную дату, ключевое поле - date
    setInfoFromDBOnDateForTableOfPlannedResults(QDate::currentDate(),getKeyFieldForPlannedResult());

    connect(ui->Calendar,
            SIGNAL(activated(QDate)),
            this,
            SLOT(changeInfoForTableOfPlannedResults(QDate)));
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_menuAction_changeActualUser_triggered()
{

   QSharedPointer <UserSelectionDialogWindow> userSelectionWindow(UserSelectionDialogWindow::createUserSelectionDialogWindow(this));
   connect(userSelectionWindow.data(),SIGNAL(userChanged(QVariant)),this,SLOT(updateAutorizedUser(QVariant)));
   userSelectionWindow.data()->exec();
}

/* Метод обновляет информацию об авторизованном пользователе
 * на основе информации, которая передана с помощью
 * infoAboutUser
*/
void MainWindow::updateAutorizedUser(QVariant infoAboutUser)
{
    // Парсим json формат
    QJsonObject info = infoAboutUser.toJsonObject();

    // Создаем переменные для хранения названия полей БД
    QString fieldId = "id_user";
    QString fieldName = "name";
    QString fieldSurname = "surname";
    QString fieldPathToAvatar = "path_to_avatar";


    autorizedUser = User(info[fieldId].toInt(),
                         info[fieldName].toString(),
                         info[fieldSurname].toString(),
                         info[fieldPathToAvatar].toString());
    updateWindow();
}

void MainWindow::setInfoFromDBOnDateForTableOfPlannedResults(const QDate &date,
                                                             const QString &keyField){
    // Составляем текст запроса
    QString textQueryForPlannedResult = std::move(getTextQueryForPlannedResult(autorizedUser,date));

    // Составляем текст для загловка таблицы
    QStringList headerLabels = std::move(getHeadersForPlannedResult());

    // Проверяем, если модель занята старой информацией, то
    // удаляем ее
    if (sqlQueryModelForPlannedResult!=nullptr)
        delete sqlQueryModelForPlannedResult;

    // Создаем модель
    sqlQueryModelForPlannedResult = new TreeModel(headerLabels,
                                                  textQueryForPlannedResult,
                                                  keyField,
                                                  ui->nutrition_TableOfPlannedResults);

    // Установить созданную древовидную модель
    ui->nutrition_TableOfPlannedResults->setModel(sqlQueryModelForPlannedResult);

    // Выровнять столбцы по контенту
    for (auto column = 0; column < sqlQueryModelForPlannedResult->columnCount(); ++column)
        ui->nutrition_TableOfPlannedResults->resizeColumnToContents(column);

}

void MainWindow::changeInfoForTableOfPlannedResults(const QDate &date){
    setInfoFromDBOnDateForTableOfPlannedResults(date,getKeyFieldForPlannedResult());
}

/*  Полностью обновляет окно, используя свои поля
*/
void MainWindow::updateWindow()
{
    updateUserInfoPanel(autorizedUser);
}

/*
 * Обновление панели пользовательской информации
 * на основе пользователя
*/
void MainWindow::updateUserInfoPanel(User &user)
{
    // Устанавливаем на панели информацию о пользователе user
    userInfoPanel.setName(user.getName());
    userInfoPanel.setSurname(user.getSurname());
    userInfoPanel.setImage(user.getPathToAvatar());
}

/*  Извлекает информацию из хранилища о пользователе
 * (из БД),
 * который последный раз
 * авторизировался в системе
*/
void MainWindow::initActualUserFromDB()
{
    // Делаем текст запроса к авторизованному пользователю
    const auto idUser = 1;

    QString fieldId = "id_user",
            fieldName = "name",
            fieldSurname = "surname",
            fieldPathToAvatar = "path_to_avatar";

    QString queryText = "SELECT * "
                        "FROM user "
                        "WHERE "+fieldId+" ="+QString::number(idUser);

    // Совершаем запрос на основе текста запроса
    QSqlQuery queryToAutorizedUser{queryText};
    queryToAutorizedUser.exec();
    queryToAutorizedUser.next();

    // Инициализируем авторизированного пользователя
    autorizedUser.setId(idUser);
    autorizedUser.setName(queryToAutorizedUser.record().value(fieldName).toString());
    autorizedUser.setSurname(queryToAutorizedUser.record().value(fieldSurname).toString());
    autorizedUser.setPathToAvatar(queryToAutorizedUser.record().value(fieldPathToAvatar).toString());
}

void MainWindow::on_applyChangesAction_triggered(){

}
