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
    void updateAutorizedUser(QVariant infoAboutUser);

    void setInfoFromDBOnDateForTableOfPlannedResults(const QDate &date, const QString &keyField);
    void changeInfoForTableOfPlannedResults(const QDate& date);
    void on_applyChangesAction_triggered();

private:
    void initActualUserFromDB();

    void updateUserInfoPanel(User & user);
    void updateWindow();

    QString getTextQueryForPlannedResult(const User &autorizedUser,const QVariant& date) const;
    QStringList getHeadersForPlannedResult();
private:
    Ui::MainWindow *ui;
    User autorizedUser{};
    UserInfoWidget userInfoPanel{};
    TreeModel* sqlQueryModelForPlannedResult = nullptr;
    States programState{};
};

// Возвращает запрос для таблицы запланированных результатов
inline QString MainWindow::getTextQueryForPlannedResult(const User& autorizedUser,const QVariant& date) const{

    // Если переданный тип не может быть конвертирован в QDate
    // То возвращаем пустой запрос
    if (!date.canConvert<QDate>())
        return QString{};

    //  Данный текст для запроса выбирает информацию
    // о питании для авторизированного пользователя
    // на определенную дату
    QString textQueryForPlannedResult = "SELECT "
                                        "       eating.date"
                                        "      ,products.name"
                                        "      ,products.protein"
                                        "      ,products.fat"
                                        "      ,products.carbohydrates"
                                        "      ,eating.weight_food "
                                        "FROM   nutrition_product as products,"
                                        "       eating "
                                        "WHERE  eating.id_user = "+QString::number(autorizedUser.getId())+
                                        "   AND eating.id_product = products.id_product "
                                        "   AND CONVERT(eating.date,DATE) = '"+date.toDate().toString("yyyy-MM-dd")+"'"
                                        " ORDER BY DATE";

    return textQueryForPlannedResult;
}

inline QStringList MainWindow::getHeadersForPlannedResult(){
    QStringList headerLabels;
    headerLabels << "Название продукта"
                 << "Белки, граммы"
                 << "Жиры, граммы"
                 << "Углеводы, граммы"
                 << "Вес, граммы";
    return headerLabels;
}

inline QString getKeyFieldForPlannedResult(){
    return "date";
}

#endif // MAINWINDOW_H
