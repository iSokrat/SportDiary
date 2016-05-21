#ifndef USERINFOWIDGET_H
#define USERINFOWIDGET_H

#include <QWidget>

#include <QWidget>
#include <QLabel>
#include <QSize>
#include <QPixmap>
#include <QVBoxLayout>
class UserInfoWidget : public QWidget
{
    Q_OBJECT

public:
    UserInfoWidget(QWidget *parent = 0);

    // New User's methods
    void setImage(const QPixmap& image) { this->image = image; update();}
    void setImage(const QString& pathToImage){this->image.load(pathToImage); update();}
    void setName(const QString& name)  { this->name = name; update();}
    void setSurname(const QString& surname){ this->surname = surname; update();}
    void setReducingCoeffForImageWidth(const qint16& coefficient){ this->reducingCoeffForImageWidth = coefficient; update();}
    void paint(QPainter* painter, const QRect &rect);
public slots:
    void update();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent * event) override;
private:
    QPixmap image{":/images/defaultAvatar.jpg"};
    QString name{"Not exist"};
    QString surname{"Not exist"};

    QLabel imageView{};
    QLabel textView{};

    QHBoxLayout horizLayout{};

    quint16 reducingCoeffForImageWidth = 1;

};

#endif // USERINFOWIDGET_H
