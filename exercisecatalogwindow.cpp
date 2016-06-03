#include "exercisecatalogwindow.h"
#include "ui_exercisecatalogwindow.h"
#include "mysqlexecutor.h"
#include "exerciseinfowindow.h"

//-----------------
#include "QListView"
#include <tuple>

ExerciseCatalogWindow::ExerciseCatalogWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExerciseCatalogWindow){

    ui->setupUi(this);
    setWindowTitle(tr("Каталог упражнений"));
    setupConnection();
    setupUI();
}

ExerciseCatalogWindow::~ExerciseCatalogWindow(){
    delete ui;
}

void ExerciseCatalogWindow::openInfoWindowForExercise(QListWidgetItem *item){

    // Сделать перемещающий конструктор
    ExerciseInfo exercise{ MySqlExecutor::getInfoAboutExercise(item->data(Qt::UserRole).toUInt()) };
    auto window = new ExerciseInfoWindow(this);

    window->show();

}

/*
 * Настройка графического интерфейса окна
*/
void ExerciseCatalogWindow::setupUI(){

    // Создаем view для списков упражнений для разных групп мышц
    for (int musculeEnumIdx = Legs; musculeEnumIdx != EndTabIndex ; musculeEnumIdx++){

        // Получаем вкладку с индексом musculeEnumIdx
        QWidget* tab = ui->TabsOfExercises->widget(musculeEnumIdx);

        // Создаем для вкладки view
        QListWidget* view = new QListWidget(tab);

        // Добавляем созданный view в внутреннее хранилище класса
        // для дальнейшего доступа к view по ключу
        viewsForExercises.insert(TabIndexType(musculeEnumIdx),view);

        //Настраиваем connect для view.
        //При клике по записи открыть окно с упражнениями
        connect(view,
                SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this,
                SLOT(openInfoWindowForExercise(QListWidgetItem*)));

        // Создаем компоновщик для view.
        // Компоновщик будет размещен на вкладке
        // с номером musculeEnumIdx
        QHBoxLayout* layout = new QHBoxLayout(ui->TabsOfExercises->widget(musculeEnumIdx));
        layout->addWidget(view);
    }

}

/*
 * Слот, устанавливающий информацию для вкладки
 * с номером tabIndex из базы данных
*/
void ExerciseCatalogWindow::setInfoForTabFromDB(int tabIndex){

    switch (tabIndex){
    case Legs:{
        auto records = MySqlExecutor::getInfoOfExercisesForCertainMuscules(getTagsForFindingLegsMuscules());
        setupInfoForTab(Legs,records);
        break;
    }
    case Back:{
        auto records = MySqlExecutor::getInfoOfExercisesForCertainMuscules(getTagsForFindingBackMuscules());
        setupInfoForTab(Back,records);
        break;
    }
    case Chest:{
        auto records = MySqlExecutor::getInfoOfExercisesForCertainMuscules(getTagsForFindingChestMuscules());
        setupInfoForTab(Chest,records);
        break;
    }
    case Biceps:{
        auto records = MySqlExecutor::getInfoOfExercisesForCertainMuscules(getTagsForFindingBicepsMuscules());
        setupInfoForTab(Biceps,records);
        break;
    }
    case Triceps:{
        auto records = MySqlExecutor::getInfoOfExercisesForCertainMuscules(getTagsForFindingTricepsMuscules());
        setupInfoForTab(Triceps,records);
        break;
    }
    case Abdominal:{
        auto records = MySqlExecutor::getInfoOfExercisesForCertainMuscules(getTagsForFindingAbdominalMuscules());
        setupInfoForTab(Abdominal,records);
        break;
    }
    case Forearms:{
        auto records = MySqlExecutor::getInfoOfExercisesForCertainMuscules(getTagsForFindingForearmsMuscules());
        setupInfoForTab(Forearms,records);
        break;
    }
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
 * Создает элемент списка заданной высоты height и заполняет информацией
 * record
*/
template <typename RecordType>
QListWidgetItem *ExerciseCatalogWindow::createExerciceListItem(qint32 height,
                                                               const RecordType& record){
    // Создаем переменные имени, пути к аватарке
    // и id упражнения
    QString name{},
            pathToAvatar{};
    auto id_exercise = 0;

    // Записываем в переменные данные из очередной записи
    std::tie(name,pathToAvatar,id_exercise) = record;

    // Создаем элемент списка
    QListWidgetItem* item = new QListWidgetItem(QIcon(pathToAvatar),name);

    // Для UserRole назначаем хранение id упражнения
    item->setData(Qt::UserRole,id_exercise);

    // Устанавливаем высоту height для элемента списка,
    // сохраняя его ширину, заданную по-умолчанию
    item->setSizeHint( QSize( item->sizeHint().width(), height ) );

    return std::move(item);
}

/*
 * Метод, устанавливающий информацию
 * records для закладки
 * с номером tabIndex. Если records - путое значение -
 * метод не изменяет вкладку с номером tabIndex.
 * Если ранее была установлена информация в вкладке с указанным
 * tabIndex, то старая информация зачищается.
*/
template <typename InfoType>
void ExerciseCatalogWindow::setupInfoForTab(int tabIndex,
                                            const QVector<InfoType> &records){

    auto indexIsOutOfRange = (tabIndex >= ui->TabsOfExercises->count());

    if ( indexIsOutOfRange ||
          records.isEmpty()){
        qDebug() << "Метод setupTab не установил для вкладки "<<tabIndex
                 << " записи records.";
        return;
    }

    // Получаем view, в котором мы разместим
    // информацию о упражнениях
    auto view = viewsForExercises[TabIndexType(tabIndex)];

    // Очищаем view, чтобы избежать ситуации,
    // когда view уже отображало информацию
    view->clear();

    // Размещаем информацию
    for (const auto& record:records){
        view->addItem(createExerciceListItem(50,record));
    }

}
