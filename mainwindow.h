#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>
#include <QDate>
// ------------------------
#include "user.h"
#include "userinfowidget.h"
#include "states.h"

class TreeModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_menuAction_changeActualUser_triggered();
    void updateAutorizedUser(const User &user);

    void setInfoFromDBOnDateForNutrTableOfPlannedResults(const QDate &date, const QString &keyField);
    void setInfoFromDBOnDateForTrainingTableOfPlannedResult(const QDate &date, const QStringList &keyFields);
    void changeInfoForTables(const QDate& date);

    void showWindowForEditTheRecord(const QModelIndex& index);
    void updateRowForPlannedResultModel(const QModelIndex&index,
                           const QList<QVariant>&data) noexcept;
    void on_menuAction_addNewUser_triggered();
private:
    void setupConnection();

    void updateUserInfoPanel(User & user);
    void updateWindow();

    QStringList getFieldForTextQueryForPlannedResult() const;

private:
    Ui::MainWindow *ui;
    User autorizedUser{};
    UserInfoWidget userInfoPanel{};
    TreeModel* sqlQueryModelForNutrTableOfPlannedResult = nullptr,
             * sqlQueryModelForTrainingTableOfPlannedResult = nullptr;

    //QQueue <QString> deferredSqlExpression;
};

// Возвращает запрос для таблицы запланированных результатов
inline QStringList MainWindow::getFieldForTextQueryForPlannedResult() const{
    return QStringList{
        "eating.date",
        "products.name",
        "products.protein",
        "products.fat",
        "products.carbohydrates"
        "eating.weight_food ",
        "eating.id_eating ",
    };
}

inline QStringList getHeadersForNutrTableOfPlannedResult(){
    QStringList headerLabels;
    headerLabels << "Название продукта"
                 << "Белки, граммы"
                 << "Жиры, граммы"
                 << "Углеводы, граммы"
                 << "Вес, граммы"
                 << "id_eating";
    return headerLabels;
}
inline QStringList getHeadersForTrainingTableOfTableOfPlannedResult(){
    QStringList headerLabels;
    headerLabels << "Количество повторений"
                 << "Используемый вес"
                 << "Время выполнения упражнения";
    return headerLabels;
}

inline QString getKeyFieldForNutrTableOfPlannedResult(){
    return "date";
}

inline QStringList getKeyFieldForTraningTableOfPlannedResult(){
    return QStringList{
        "date_of_start_training",
        "id_exercise"
    };
}

#endif // MAINWINDOW_H
