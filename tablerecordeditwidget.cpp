#include "tablerecordeditwidget.h"

#include <QDateEdit>
#include <QDateTimeEdit>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QTimeEdit>

#include <QDebug>

TableRecordEditWidget::TableRecordEditWidget(const QModelIndex& index, QWidget *parent, Qt::WindowFlags f):
QDialog(parent,f),
editRowIndex(index),
widgetsData(new QMap<QObject*,QVariant>){

    setWindowTitle("Изменение записи");

    // Получаем модель из index
    const QAbstractItemModel* model = index.model();

    // Создаем компоновщик для виджетов,
    // где будет храниться информация
    QFormLayout* contentLayout = new QFormLayout;

    // Получаем всю информацию из строки, в которой находидся index
    for (auto col = 0 ; col<model->columnCount();col++){
        // Получаем индекс текущей ячейки
        QModelIndex currentIndex = model->index(index.row(),col,index.parent());

        // Создаем виджет для представления
        // извлеченной информации (data) на основе типа,
        // которым она обладает
        QWidget* editWidget = createWidgetForCertainTypeWithValue(currentIndex.data(),this);

        // Сохраняем указатель на созданный widget в this->widgetsData
        widgetsData->insert(editWidget,currentIndex.data());

        if (!editWidget){
            qDebug() <<" EditWidget на итерации "<<col
                     <<" не создан: currentIndex.data()"
                       " имеет неизвестный тип.";
            continue;
        }

        // Получаем название столбца
        QString colName = std::move(model->headerData(col,Qt::Horizontal,Qt::DisplayRole).toString()) ;

        // Устанавливаем имя для метки (подписи для поля)
        QLabel* fieldName = new QLabel(colName,this);

        // Добавляем созданные метку и виджет в
        // компоновщик
        contentLayout->addRow(fieldName,editWidget);
    }

    // Создаем компоновщик для контента
    // и кнопки принять/отменить
    QVBoxLayout* centralLayout = new QVBoxLayout(this);

    // Создаем кнопку принять/отменить
    QDialogButtonBox* dialogButton = new QDialogButtonBox(this);
    dialogButton->addButton("Применить",QDialogButtonBox::AcceptRole);
    dialogButton->addButton("Отменить",QDialogButtonBox::RejectRole);

    centralLayout->addLayout(contentLayout);
    centralLayout->addWidget(dialogButton,0,Qt::AlignRight);

    // Устанавливаем заполненный компоновщик
    setLayout(centralLayout);

    connect(dialogButton,SIGNAL(accepted()),this,SLOT(accept()));
}

TableRecordEditWidget::~TableRecordEditWidget(){
    delete widgetsData;
}

void TableRecordEditWidget::accept(){
    // Извлекаем информацию из внутреннего хранилища
    QList <QVariant> data = std::move(extractDataFromWidgets(widgetsData));

    // Вызываем сигнал с индексом строки, в которой редактировалась информация
    // и самой измененной информацией
    emit acceptData(editRowIndex,data);

    // Вызываем базовую версию accept()
    QDialog::accept();
}

/*
 * Изменяет поле информации this->widgetsData
 *
*/
void TableRecordEditWidget::changeData(){
    // Получаем измененный виджет
    QObject* changedWidget = sender();
    // Получаем имя измененного виджета
    QString widgetName = changedWidget->metaObject()->className();

    // Если такого виджета не найдено в this->widgetsData
    // (на всякий случай делаем эту проверку
    // виджет все равно будет найден)
    if (!widgetsData->contains(changedWidget)){
        qDebug() << "В TableRecordEditWidget::widgetsData(QMap) не найден объект "
                 << changedWidget;
        return;
    }

    if (widgetName == tr("QSpinBox")){
        QSpinBox* sB = static_cast<QSpinBox*>(changedWidget);
        widgetsData->insert( sB,QVariant(sB->value()) );
    }else
    if (widgetName == tr("QDoubleSpinBox")){
        QDoubleSpinBox* sB = static_cast<QDoubleSpinBox*>(changedWidget);
        widgetsData->insert( sB, QVariant(sB->value()) );
    }else
    if (widgetName == tr("QLineEdit")){
        QLineEdit* lE = static_cast<QLineEdit*>(changedWidget);
        widgetsData->insert( lE,QVariant(lE->text()) );
    }else
    if (widgetName == tr("QDateEdit")){
        QDateEdit* dE = static_cast<QDateEdit*>(changedWidget);
        widgetsData->insert( dE,QVariant(dE->date()) );
    }else
    if (widgetName == tr("QDataTimeEdit")){
        QDateTimeEdit* dTE = static_cast<QDateTimeEdit*>(changedWidget);
        widgetsData->insert( dTE,QVariant(dTE->dateTime()) );
    }else
    if (widgetName == tr("QTimeEdit")){
        QTimeEdit* tE = static_cast<QTimeEdit*>(changedWidget);
        widgetsData->insert( tE,QVariant(tE->time()) );
    }else
        qDebug() <<"Тип виджета"<<changedWidget<<" не обработан "
                 <<"в слоте changeData()";
}

/* Создает виджет определенного типа
 * на основе типа значения value.
 * Значение value устанавливается внутри
 * созданного виджета
*/
QWidget *TableRecordEditWidget::createWidgetForCertainTypeWithValue(const QVariant &value, QWidget *parent) const noexcept{
    switch (value.type()){
        case(QMetaType::Int):{
            QSpinBox* spinBox =  new QSpinBox(parent);
            spinBox->setMaximum(10000);
            spinBox->setValue(value.toInt());

            return spinBox;
            break;
        }
        case(QMetaType::UInt):{

            QSpinBox* spinBox =  new QSpinBox(parent);
            spinBox->setMaximum(10000);
            spinBox->setValue(value.toUInt());
            connect(spinBox,SIGNAL(valueChanged(int)),this,SLOT(changeData()));

            return spinBox;
            break;
        }
        case QMetaType::Double:{

            QDoubleSpinBox* spinBox =  new QDoubleSpinBox(parent);
            spinBox->setValue(value.toDouble());
            connect(spinBox,SIGNAL(valueChanged(double)),this,SLOT(changeData()));

            return spinBox;
            break;
        }
        case QMetaType::QString:{

            QLineEdit* edit = new QLineEdit(value.toString(),parent);
            connect(edit,SIGNAL(textChanged(QString)),this,SLOT(changeData()));

            return edit;
            break;
        }
        case QMetaType::QDate:{

            QDateEdit *dateEdit = new QDateEdit(value.toDate(),parent);
            connect(dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(changeData()));

            return dateEdit;
            break;
        }
        case QMetaType::QDateTime:{
            QDateTimeEdit* dateTimeEdit = new QDateTimeEdit(value.toDateTime(),parent);
            connect(dateTimeEdit,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(changeData()));

            return dateTimeEdit;
            break;
        }
        case QMetaType::QTime:{
            QTimeEdit* timeEdit =  new QTimeEdit(value.toTime(),parent);
            connect(timeEdit,SIGNAL(timeChanged(QTime)),this,SLOT(changeData()));

            return timeEdit;
            break;
        }
        default:
            qDebug() <<"Необработанный тип:"<<value.type();
            return nullptr;
   }
    return nullptr;
}
