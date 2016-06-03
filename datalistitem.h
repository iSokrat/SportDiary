#ifndef DATALISTITEM_H
#define DATALISTITEM_H

#include <QListWidgetItem>
#include <QHash>

class DataListItem:public QListWidgetItem
{
public:
    //DataListItem(const QIcon &icon, const QString &text, QListWidget *view, int type) = default;

    void setAdditionalData(const QVariant& key,
                           const QVariant& value);
private:
    QHash <QVariant,QVariant> additionalData{};
};

#endif // DATALISTITEM_H
