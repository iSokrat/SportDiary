#include "infoaboutuserdelegate.h"

#include <userinfowidget.h>
#include <QDebug>

#include <QtSql>
Q_DECLARE_METATYPE(QSqlRecord)
InfoAboutUserDelegate::InfoAboutUserDelegate(QObject *parent):
    QStyledItemDelegate(parent){

}

QWidget *InfoAboutUserDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    UserInfoWidget* editorWidget = new UserInfoWidget(parent);
    const auto reducingCoeff = 3;

    editorWidget->setReducingCoeffForImageWidth(reducingCoeff);
    return editorWidget;
}

void InfoAboutUserDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.data().canConvert<QSqlRecord>()) {
            //QSqlRecord infoAboutUser = reinterpret_cast<QSqlRecord>(index.data());
            //UserInfoWidget *userInfoWidget = qobject_cast<UserInfoWidget *>(editor);

            qDebug()<<(index.data());
            //userInfoWidget->setName(infoAboutUser.value("name").toString());
            //userInfoWidget->setSurname(infoAboutUser.value("surname").toString());
    } else {
            QStyledItemDelegate::setEditorData(editor, index);
    }
}

void InfoAboutUserDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //if (index.data().canConvert<QSqlRecord>()) {
    //    UserInfoWidget *userInfoWidget = qobject_cast<UserInfoWidget *>(editor);

    //    model->setData(index, QVariant::fromValue(starEditor->starRating()));
    //} else {
    //    QStyledItemDelegate::setModelData(editor, model, index);
    //}
}

void InfoAboutUserDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(0,0,100,50);
}

#include <QPainter>
void InfoAboutUserDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->drawRect(option.rect);

}

void InfoAboutUserDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    option->rect.setSize(QSize(100,200));
}


