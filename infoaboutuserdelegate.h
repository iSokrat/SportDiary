#ifndef INFOABOUTUSERDELEGATE_H
#define INFOABOUTUSERDELEGATE_H

#include <QStyledItemDelegate>

class InfoAboutUserDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    InfoAboutUserDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
};

#endif // INFOABOUTUSERDELEGATE_H
