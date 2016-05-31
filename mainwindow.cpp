#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "userselectiondialogwindow.h"
#include "userinfowidget.h"
#include "treemodel.h"
#include "tablerecordeditwidget.h"
#include "addnewuserwidget.h"
#include "mysqlexecutor.h"
#include "filemanager.h"
#include "exercisecatalogwindow.h"
/*-------------------------------------------------------*/
#include <QtSql>
#include <QSharedPointer>
#include <QVBoxLayout>
#include <QPoint>
#include <QSplitter>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){

    ui->setupUi(this);
    setWindowTitle(tr("Дневник спортивных тренировок"));

    // Инициализируем авторизированного
    // пользователя по id из Db
    const auto idAutorizedUser = 1;
    MySqlExecutor::initUser(idAutorizedUser,autorizedUser);

    updateUserInfoPanel(autorizedUser);

    setupUi();

    // Загружаем информацию из БД в таблицу запланированных результатов
    // на определенную дату, ключевое поле - date
    setInfoFromDBOnDateForNutrTableOfPlannedResults(QDate::currentDate(),
                                                    getKeyFieldForNutrTableOfPlannedResult());
    setInfoFromDBOnDateForTrainingTableOfPlannedResult(QDate::currentDate(),
                                                       getKeyFieldForTraningTableOfPlannedResult());

    // Настраиваем все вызовы connect
    setupConnection();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_menuAction_changeActualUser_triggered()
{

    QSharedPointer <UserSelectionDialogWindow> userSelectionWindow(UserSelectionDialogWindow::createUserSelectionDialogWindow(this));
    connect(userSelectionWindow.data(),
            SIGNAL(userChanged(User)),
            this,
            SLOT(updateAutorizedUser(User)));
    userSelectionWindow.data()->exec();
}

/* Метод обновляет информацию об авторизованном пользователе
 * на основе информации, которая передана с помощью
 * infoAboutUser
*/
void MainWindow::updateAutorizedUser(const User& user){
    autorizedUser = user;
    updateWindow();
}

void MainWindow::setInfoFromDBOnDateForNutrTableOfPlannedResults(const QDate &date,
                                                                 const QString &keyField){

    // Составляем текст для загловка таблицы
    QStringList headerLabels = std::move(getHeadersForNutrTableOfPlannedResult());

    // Проверяем, если модель занята старой информацией, то
    // удаляем ее
    if (sqlQueryModelForNutrTableOfPlannedResult!=nullptr)
        delete sqlQueryModelForNutrTableOfPlannedResult;

    // Создаем модель
    sqlQueryModelForNutrTableOfPlannedResult = new TreeModel(headerLabels,
                                                             std::move(MySqlExecutor::getTextQueryForNutrTableOfPlannedResult(autorizedUser,date)),
                                                             QStringList{keyField},
                                                             ui->TabsOfSportFields);

    // Установить созданную древовидную модель
    ui->nutrition_TableOfPlannedResults->setModel(sqlQueryModelForNutrTableOfPlannedResult);

    // Скрытие заголовка - отключить
    ui->nutrition_TableOfPlannedResults->setHeaderHidden(false);

    // Выровнять столбцы по контенту
    for (auto column = 0; column < sqlQueryModelForNutrTableOfPlannedResult->columnCount(); ++column)
        ui->nutrition_TableOfPlannedResults->resizeColumnToContents(column);

}

void MainWindow::setInfoFromDBOnDateForTrainingTableOfPlannedResult(const QDate &date, const QStringList &keyFields){
    // Составляем текст для загловка таблицы
    QStringList headerLabels = std::move(getHeadersForTrainingTableOfTableOfPlannedResult());

    // Проверяем, если модель занята старой информацией, то
    // удаляем ее
    if (sqlQueryModelForTrainingTableOfPlannedResult!=nullptr)
        delete sqlQueryModelForTrainingTableOfPlannedResult;

    // Создаем модель
    sqlQueryModelForTrainingTableOfPlannedResult = new TreeModel(headerLabels,
                                                                 std::move(MySqlExecutor::getTextQueryForTrainingTableOfPlannedResult(autorizedUser,date)),
                                                                 keyFields,
                                                                 ui->TabsOfSportFields);

    // Установить созданную древовидную модель
    ui->training_TableOfPlannedResults->setModel(sqlQueryModelForTrainingTableOfPlannedResult);

    // Скрытие заголовка - отключить
    ui->training_TableOfPlannedResults->setHeaderHidden(false);

    // Выровнять столбцы по контенту
    for (auto column = 0; column < sqlQueryModelForTrainingTableOfPlannedResult->columnCount(); ++column)
        ui->training_TableOfPlannedResults->resizeColumnToContents(column);
}

void MainWindow::changeInfoForTables(const QDate &date){
    setInfoFromDBOnDateForNutrTableOfPlannedResults(date,getKeyFieldForNutrTableOfPlannedResult());
    setInfoFromDBOnDateForTrainingTableOfPlannedResult(date,getKeyFieldForTraningTableOfPlannedResult());
}

/*
 * Отображение окна редактирования записи
*/
void MainWindow::showWindowForEditTheRecord(const QModelIndex &index){

    TableRecordEditWidget* window = new TableRecordEditWidget(index,this);
    connect(window,
            SIGNAL(acceptData(QModelIndex,QList<QVariant>)),
            this,
            SLOT(updateRowForPlannedResultModel(QModelIndex,QList<QVariant>)));
    // Окно становиться главным,
    // пока его не закроют
    window->exec();

}

void MainWindow::showExerciseCatalogWindow(){
    ExerciseCatalogWindow* window = new ExerciseCatalogWindow(this);
    window->show();
}

/* Обновление информации на основании data. Index
 * должен хранить строку модели, которую необходимо
 * обновить
*/
void MainWindow::updateRowForPlannedResultModel(const QModelIndex & index,
                                                const QList<QVariant> & data) noexcept{

    if (!index.isValid()){
        qDebug() <<"Ошибка в updateRowForPlannedResultModel:"<<index;
        return;
    }

    // Устанавливаем информацию для каждого столбца из data в
    // sqlQueryModelForNutrTableOfPlannedResult на основе index.row()
    // и index.parent()
    for (auto column = 0;column< sqlQueryModelForNutrTableOfPlannedResult->columnCount();column++){
        QModelIndex dataCell = std::move( sqlQueryModelForNutrTableOfPlannedResult->index(index.row(),
                                                                                          column,
                                                                                          index.parent()) );
        sqlQueryModelForNutrTableOfPlannedResult->setData(dataCell,data[column],Qt::EditRole);
    }

}

/*  Полностью обновляет окно, используя свои поля
*/
void MainWindow::updateWindow(){
    updateUserInfoPanel(autorizedUser);
    setInfoFromDBOnDateForNutrTableOfPlannedResults(QDate::currentDate(),
                                                    getKeyFieldForNutrTableOfPlannedResult());
    setInfoFromDBOnDateForTrainingTableOfPlannedResult(QDate::currentDate(),
                                                       getKeyFieldForTraningTableOfPlannedResult());
    ui->Calendar->setSelectedDate(QDate::currentDate());
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

/* Настройка connect связей
*/
void MainWindow::setupConnection(){
    connect(ui->exerciseCatalog,
            SIGNAL(triggered()),
            this,
            SLOT(showExerciseCatalogWindow()));
    connect(ui->Calendar,
            SIGNAL(activated(QDate)),
            this,
            SLOT(changeInfoForTables(QDate)));

    // При клике по строке таблицы открыть окно редактирования
    connect(ui->nutrition_TableOfPlannedResults,
            SIGNAL(doubleClicked(QModelIndex)),
            this,
            SLOT(showWindowForEditTheRecord(QModelIndex)));
}

// Настройка графического интерфейса окна
void MainWindow::setupUi(){

    QHBoxLayout* userPanelAndCalendarLayout = new QHBoxLayout;
    userPanelAndCalendarLayout->addWidget(&userInfoPanel);
    userPanelAndCalendarLayout->addWidget(ui->Calendar);

    // Создаем компоновщик для userInfoWidget и ui->TabsOfSportFields
    QVBoxLayout* centralLayout = new QVBoxLayout;
    centralLayout->addLayout(userPanelAndCalendarLayout);
    centralLayout->addWidget(ui->TabsOfSportFields);

    // Устанавливаем компоновщик для центрального виджета нашего окна
    ui->centralWidget->setLayout(centralLayout);
}

/* Открыть окно "Добавить нового пользователя"
*/
void MainWindow::on_menuAction_addNewUser_triggered(){
    AddNewUserWidget* window = new AddNewUserWidget;

    window->exec();


    // После exec получаем введенную информацию
    User newUser = window->getNewUser() ;

    // Скачиваем аватар в рабочую папку для пользователя
    QString pathToUpladFile = FileManager::uploadAvatarForUser(newUser);

    // Если файл был не скачен
    if (pathToUpladFile.isEmpty()){
        qDebug() <<" В on_menuAction_addNewUser_triggered():"
                   " файл не был скачен - FileManager::uploadAvatarForUser()"
                   " ничего не вернул (пустой путь).";
        return;
    }

    // Устанавливаем новый путь к аватарке
    newUser.setPathToAvatar(pathToUpladFile);

    // Добавляем
    bool isAdded = MySqlExecutor::addNewUserIntoDB(newUser);



    if (isAdded)
        QMessageBox::information(this,
                                 tr("Операция выполнена:)"),
                                 tr("Новый пользователь добавлен!"),
                                 QMessageBox::Ok);
    else
        QMessageBox::critical(this,
                              tr("Операция не выполнена:("),
                              tr("Пользователь не может быть добавлен, "
                                 "обратитесь к разработчику, чтобы "
                                 "он устранил ошибку. "),
                              QMessageBox::Ok);
    delete window;

}
