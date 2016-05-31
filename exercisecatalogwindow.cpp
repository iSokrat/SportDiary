#include "exercisecatalogwindow.h"
#include "ui_exercisecatalogwindow.h"
#include "mysqlexecutor.h"
//-----------------
#include "QListView"
#include <tuple>

void ExerciseCatalogWindow::setupTabs()
{
    setupTabs();
}

ExerciseCatalogWindow::ExerciseCatalogWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExerciseCatalogWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Каталог упражнений"));
    setupConnection();
    setupUI();
}

ExerciseCatalogWindow::~ExerciseCatalogWindow(){
    delete ui;
}

/*
 * Слот, устанавливающий информацию для вкладки
 * с номером tabIndex из базы данных
*/
void ExerciseCatalogWindow::setInfoForTabFromDB(int tabIndex){

    switch (tabIndex){
    case Legs:{
        auto records = MySqlExecutor::getInfoOfExercisesForCertainMuscules(getWordForFindingLegsMuscules());
        setupTab()
       break;
    }
    case Back:{break;}
        case Chest:{break;}
        case Biceps:{break;}
        case Triceps:{break;}
        case Abdominal:{break;}
        case Forearms:{break;}
    default:{
        qDebug() << "Вкладка с индексом "<<tabIndex
                 <<" не обработана.";
    }
    }

}

void ExerciseCatalogWindow::setupConnection(){
    connect(ui->TabsOfExercises,
            SIGNAL(currentChanged(int)),
            this,
            SLOT(setInfoForTabFromDB(int)));

}
/*
 * Создает элемент списка заданной ширины width
*/
QListWidgetItem *ExerciseCatalogWindow::createListItem(qint32 width){
    QListWidgetItem* item = new QListWidgetItem(QIcon(pathToAvatar),name);
    item->setSizeHint( QSize( width, item->sizeHint().height() ) );
    return item;
}

/*
 * Метод, устанавливающий информацию
 * records для закладки
 * с номером tabIndex
*/
template<class InfoType>
void ExerciseCatalogWindow::setupInfoForTab(int tabIndex,
                                     const QVector<ExerciseCatalogWindow::InfoType> &records){

    auto indexOutOfRange = (tabIndex >= ui->TabsOfExercises->count());

    if ( indexOutOfRange ||
          records.isEmpty()){
        qDebug() << "Метод setupTab не установил для вкладки "<<tabIndex
                 << " записи records.";
        return;
    }

    for (const auto& record:records){

        // Создаем переменные имени, пути к аватарке
        // и id упражнения
        QString name{},pathToAvatar{};
        auto id_exercise = 0;

        // Записываем в переменные данные из очередной записи
        std::tie(name,pathToAvatar,id_exercise) = record;

        view->addItem(createListItem(40));
    }

}
