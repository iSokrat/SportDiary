#include "addnewuserwidget.h"
#include "ui_addnewuserwidget.h"
//---------------------------------

#include <QFormLayout>
#include <QGraphicsScene>
#include <QLabel>
#include <QMessageBox>

AddNewUserWidget::AddNewUserWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewUserWidget),
    newUser(new User){

    ui->setupUi(this);

    setWindowTitle(tr("Добавить нового пользователя"));

    setupWindow();

    // Устанавливаем imageModel на View.
    // В imageModel храниться путь к аватарке
    avatarModel = new ImageModel(this);
    ui->Avatar->setScene(avatarModel);
}

AddNewUserWidget::~AddNewUserWidget(){
    delete newUser;
    delete ui;
}

void AddNewUserWidget::accept(){
    if (!validateForm()){
        QMessageBox::warning(this,
                             std::move(tr("Не заполнены обязательные поля")),
                             std::move(tr("Проверьте, заполнены ли обязательные "
                                          "поля (отмечены*)")),
                             QMessageBox::Ok);
        return;
    }

    extractInfoFromWidgetsToUser(newUser);
    emit accepted(*newUser);
    QDialog::accept();
}

void AddNewUserWidget::extractInfoFromWidgetsToUser(User *user){
    user->setName(ui->editName->text());
    user->setSurname(ui->editSurname->text());
    user->setPathToAvatar(avatarModel->getPathToImage());
}

void AddNewUserWidget::setupWindow(){

    QFormLayout* formLayout = new QFormLayout();
    formLayout->insertRow(0,ui->labelName,ui->editName);
    formLayout->insertRow(1,ui->labelSurname,ui->editSurname);
    formLayout->insertRow(2,ui->labelBirdthDate,ui->editBirdthDate);

    QVBoxLayout* centralLayout = new QVBoxLayout();
    centralLayout->addWidget(ui->Avatar,Qt::AlignCenter);
    centralLayout->addLayout(formLayout);
    centralLayout->addWidget(ui->buttonBox);

    setLayout(centralLayout);
}

bool AddNewUserWidget::validateForm(){
    if (ui->editName->text().isEmpty() ||
        ui->editSurname->text().isEmpty() )
        return false;
    return true;
}

