#include "mainwindow.h"
#include <QApplication>

#include <QtSql>
#include <QDebug>

#include <userinfowidget.h>
#include <QCalendarWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db_sportDiary = QSqlDatabase::addDatabase("QMYSQL");
    db_sportDiary.setUserName("root");
    db_sportDiary.setHostName("localhost");
    db_sportDiary.setPort(3306);
    db_sportDiary.setDatabaseName("sport_diary");

    if (!db_sportDiary.open()){
        qDebug() <<db_sportDiary.lastError();
        return 1;
    }
    MainWindow w;
    w.showNormal();

    //UserInfoWidget u;
    //u.show();
    return a.exec();
}
