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

    void setInfoFromDBOnDateForTableOfPlannedResults(const QDate &date, const QString &keyField);
    void changeInfoForTableOfPlannedResults(const QDate& date);

    void showWindowForEditTheRecord(const QModelIndex& index);
    void updateRowForPlannedResultModel(const QModelIndex&index,
                           const QList<QVariant>&data) noexcept;
    void on_menuAction_addNewUser_triggered();

private:
    void setupConnection();

    void updateUserInfoPanel(User & user);
    void updateWindow();

    QStringList getFieldForTextQueryForPlannedResult() const;
    QStringList getHeadersForPlannedResult();

private:
    Ui::MainWindow *ui;
    User autorizedUser{};
    UserInfoWidget userInfoPanel{};
    TreeModel* sqlQueryModelForPlannedResult = nullptr;

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


inline QStringList MainWindow::getHeadersForPlannedResult(){
    QStringList headerLabels;
    headerLabels << "Название продукта"
                 << "Белки, граммы"
                 << "Жиры, граммы"
                 << "Углеводы, граммы"
                 << "Вес, граммы"
                 << "id_eating";
    return headerLabels;
}

inline QString getKeyFieldForPlannedResult(){
    return "date";
}

#endif // MAINWINDOW_H
