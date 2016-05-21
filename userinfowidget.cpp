#include "userinfowidget.h"

#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
UserInfoWidget::UserInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    resize(200,100);

    horizLayout.addWidget(&imageView,0,Qt::AlignLeft);
    horizLayout.addWidget(&textView,0,Qt::AlignLeft);

    setLayout(&horizLayout);
    // Create the new image with the new size
    // Set the image and the user's information on imageView and textView
    update();
}


void UserInfoWidget::paint(QPainter *painter, const QRect &rect)
{
}

void UserInfoWidget::update()
{
    // Set the image and the user's information on imageView and textView
    if (image.width()!=width()/reducingCoeffForImageWidth)
        image = image.scaled(width()/reducingCoeffForImageWidth,height(),Qt::KeepAspectRatio);

    imageView.setPixmap(image);
    textView.setText(name+"\n"+surname);
}

void UserInfoWidget::resizeEvent(QResizeEvent * event){}

void UserInfoWidget::paintEvent(QPaintEvent *event)
{
//    image = image.scaled(width()/reducingCoeffForImageWidth,height(),Qt::KeepAspectRaevent
    QPainter* painter = new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setBrush(QBrush(QColor(255,210,60)));
    painter->drawRoundedRect(rect(),5,5);
    painter->end();
}


