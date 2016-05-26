#ifndef TABLERECORDEDITWIDGET_H
#define TABLERECORDEDITWIDGET_H

#include <QDialog>
#include <QVariant>
#include <QModelIndex>
#include <QMap>

class QModelIndex;

class TableRecordEditWidget : public QDialog
{
    Q_OBJECT

public:
    TableRecordEditWidget(const QModelIndex& index, QWidget* parent = nullptr, Qt::WindowFlags f = 0);

    ~TableRecordEditWidget();

protected:
    void accept() override;

signals:
    void acceptData(const QModelIndex&,
                    const QList<QVariant>& );

private slots:
    void changeData();

// Private Methods
private:
    QWidget *createWidgetForCertainTypeWithValue(const QVariant& value, QWidget *parent = nullptr) const noexcept;
    QList<QVariant> extractDataFromWidgets(const QMap<QObject *, QVariant> *widgets) const noexcept;

// Private Fields
private:
    QModelIndex editRowIndex;
    QMap <QObject*,QVariant> *widgetsData = nullptr;
};

#endif // TABLERECORDEDITWIDGET_H

inline QList<QVariant> TableRecordEditWidget::extractDataFromWidgets(const QMap<QObject*,QVariant> *widgetsData) const noexcept{
    return widgetsData->values();
}
