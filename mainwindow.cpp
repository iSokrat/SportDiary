#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "userselectiondialogwindow.h"
#include "userinfowidget.h"
#include "treemodel.h"
#include "tablerecordeditwidget.h"
#include "addnewuserwidget.h"
#include "mysqlexecutor.h"
#include "filemanager.h"
/*-------------------------------------------------------*/
#include <QtSql>
#include <QSharedPointer>
#include <QVBoxLayout>
#include <QSplitter>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){

    ui->setupUi(this);

    // Инициализируем авторизированного
    // пользователя по id из Db
    const auto idAutorizedUser = 1;
    MySqlExecutor::initUser(idAutorizedUser,autorizedUser);

    updateUserInfoPanel(autorizedUser);

    // Создаем компоновщик для userInfoWidget и ui->TabsOfSportFields
    QVBoxLayout* centralLayout = new QVBoxLayout;
    centralLayout->addWidget(&userInfoPanel);
    centralLayout->addWidget(ui->TabsOfSportFields);

    // Устанавливаем компоновщик для центрального виджета нашего окна
    ui->centralWidget->setLayout(centralLayout);

    // Загружаем информацию из БД в таблицу запланированных результатов
    // на определенную дату, ключевое поле - date
    setInfoFromDBOnDateForTableOfPlannedResults(QDate::currentDate(),
                                                getKeyFieldForPlannedResult());
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

void MainWindow::setInfoFromDBOnDateForTableOfPlannedResults(const QDate &date,
                                                             const QString &keyField){

    // Составляем текст для загловка таблицы
    QStringList headerLabels = std::move(getHeadersForPlannedResult());

    // Проверяем, если модель занята старой информацией, то
    // удаляем ее
    if (sqlQueryModelForPlannedResult!=nullptr)
        delete sqlQueryModelForPlannedResult;

    // Создаем модель
    sqlQueryModelForPlannedResult = new TreeModel(headerLabels,
                                                  std::move(MySqlExecutor::getTextQueryForPlannedResult(autorizedUser,date)),
                                                  keyField,
                                                  ui->nutrition_TableOfPlannedResults);

    // Установить созданную древовидную модель
    ui->nutrition_TableOfPlannedResults->setModel(sqlQueryModelForPlannedResult);

    // Скрытие заголовка - отключить
    ui->nutrition_TableOfPlannedResults->setHeaderHidden(false);

    // Выровнять столбцы по контенту
    for (auto column = 0; column < sqlQueryModelForPlannedResult->columnCount(); ++column)
        ui->nutrition_TableOfPlannedResults->resizeColumnToContents(column);

}

void MainWindow::changeInfoForTableOfPlannedResults(const QDate &date){
    setInfoFromDBOnDateForTableOfPlannedResults(date,getKeyFieldForPlannedResult());
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
    // sqlQueryModelForPlannedResult на основе index.row()
    // и index.parent()
    for (auto column = 0;column< sqlQueryModelForPlannedResult->columnCount();column++){
        QModelIndex dataCell = std::move( sqlQueryModelForPlannedResult->index(index.row(),
                                                                               column,index.parent()) );
        sqlQueryModelForPlannedResult->setData(dataCell,data[column],Qt::EditRole);
    }

}

/*  Полностью обновляет окно, используя свои поля
*/
void MainWindow::updateWindow(){
    updateUserInfoPanel(autorizedUser);
    setInfoFromDBOnDateForTableOfPlannedResults(QDate::currentDate(),
                                                getKeyFieldForPlannedResult());
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
    connect(ui->Calendar,
            SIGNAL(activated(QDate)),
            this,
            SLOT(changeInfoForTableOfPlannedResults(QDate)));

    // При клике по строке таблицы открыть окно редактирования
    connect(ui->nutrition_TableOfPlannedResults,
            SIGNAL(doubleClicked(QModelIndex)),
            this,
            SLOT(showWindowForEditTheRecord(QModelIndex)));
}

/* Открыть окно "Добавить нового пользователя"
*/
void MainWindow::on_menuAction_addNewUser_triggered(){
    AddNewUserWidget* window = new AddNewUserWidget;

    window->exec();


    // После exec добавляем нового пользователя в БД пользователя
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
