#include "newuser.h"
#include "ui_newuser.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

NewUser::NewUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewUser)
{
    ui->setupUi(this);
    // Set the stylesheet for the newUserSubmitButton to make it 3D and attractive
    QString buttonStylesheet =
            "QPushButton {"
            "    background-color: #28A745;"  // Button color
            "    color: white;"               // Text color
            "    border-radius: 7px;"        // Rounded corners
            "    box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2);"  // Shadow for 3D effect
            "}"
            "QPushButton:hover {"
            "    background-color: #FD7E14;"    // Background color on hover
            "    color: black;"               // Text color on hover
            "    box-shadow: 0 12px 24px 0 rgba(0,0,0,0.2);"  // Larger shadow on hover
            "}"
            "QPushButton:pressed {"
            "    background-color: #87CEFA;"  // Background color when pressed
            "    box-shadow: 0 5px 10px 0 rgba(0,0,0,0.2);"   // Smaller shadow when pressed
            "    transform: translateY(4px);" // Move the button down slightly when pressed
            "}";
    ui->newUserSubmitButton->setStyleSheet(buttonStylesheet);

    qDebug() << "Database Connected Successfully... !!!!";

    //It will create a table name will be new_user_details inside loginDetails Database
    QSqlQuery query;
    QString createTable = "CREATE TABLE IF NOT EXISTS new_user_details ("
                          "user_id TEXT, "
                          "password TEXT, "
                          "confirm_password TEXT, "
                          "security_question TEXT, "
                          "answer TEXT)";

    if (query.exec(createTable)) {
        qDebug() << "Table created or already exists ..";
    }
    else {
        qDebug() << "Failed to create table:" << query.lastError().text();
    }
}

NewUser::~NewUser()
{
    delete ui;
}
void NewUser::on_newUserSubmitButton_clicked()
{
    QString userId = ui->newuserUserIdlineEdit->text();
    QString password = ui->newuserPasswordlineEdit->text();
    QString confirmPassword = ui->newuserConfirmPasswordlineEdit->text();
    QString securityQuestion = ui->securityQuestioncomboBox->currentText();
    QString answer = ui->securityQuestionlineEdit->text();

    if(userId == "" && password == "" && confirmPassword == "" && answer == "" )
    {
        QMessageBox::warning(this,"Warning","Please Fill all details");
    }
    else if(userId != "" && password == "" && confirmPassword == "" && answer == "")
    {
        QMessageBox::warning(this,"Warning","Please Fill all details");
    }
    else if(userId != "" && password != "" && confirmPassword == "" && answer =="")
    {
        QMessageBox::warning(this,"Warning","Please Fill all details");
    }
    else if(userId != "" && password != "" && confirmPassword != "" && answer =="")
    {
        QMessageBox::warning(this,"Warning","Please Fill all details");
    }
    else if(userId != "" && password != "" && confirmPassword != "" && answer != "" && password != confirmPassword)
    {
        QMessageBox::warning(this,"Warning","Password and confirm password should same");
        ui->newuserPasswordlineEdit->clear();
        ui->newuserConfirmPasswordlineEdit->clear();
    }
    else if(userId != "" && password == "" && confirmPassword != "" && answer != "" && password != confirmPassword)
    {
        QMessageBox::warning(this,"Warning","Password and confirm password should same");
        ui->newuserPasswordlineEdit->clear();
        ui->newuserConfirmPasswordlineEdit->clear();
    }
    else if(userId != "" && password != "" && confirmPassword == "" && answer != "" && password != confirmPassword)
    {
        QMessageBox::warning(this,"Warning","Password and confirm password should same");
        ui->newuserPasswordlineEdit->clear();
        ui->newuserConfirmPasswordlineEdit->clear();
    }
    else
    {
        //Before inserting new details in new_user_details table. It will delete previous records
        QSqlQuery query;
        QString deleteQuery = "DELETE FROM new_user_details";
        if(query.exec(deleteQuery))
        {
            qDebug() << "All user records deleted successfully.";
        }
        else
        {
            qDebug() << "Failed to delete user records:" << query.lastError().text();
            QMessageBox::critical(this, "Error", "Failed to delete user records: " + query.lastError().text());
            return;
        }

        //QSqlQuery query;
        query.prepare("INSERT INTO new_user_details (user_id, password, confirm_password, security_question, answer) "
                      "VALUES (:user_id, :password, :confirm_password, :security_question, :answer)");

        query.bindValue(":user_id", userId);
        query.bindValue(":password", password);
        query.bindValue(":confirm_password", confirmPassword);
        query.bindValue(":security_question", securityQuestion);
        query.bindValue(":answer", answer);

        if (!query.exec())
        {
            qDebug() << "Error: Could not insert data.";
            qDebug() << query.lastError();
        }
        else
        {
            qDebug() << "User details inserted successfully.";
            QMessageBox::information(this,"Success","User details inserted successfully");
            ui->newuserUserIdlineEdit->clear();
            ui->newuserPasswordlineEdit->clear();
            ui->newuserConfirmPasswordlineEdit->clear();
            ui->securityQuestioncomboBox->setCurrentIndex(0);
            ui->securityQuestionlineEdit->clear();

            //It will close the newuser window after successful insertion of data
            accept();
        }

        //It will fetch details from Database and display on console only..
        query.exec("SELECT * FROM new_user_details");
        while(query.next())
        {
            QString userId = query.value(0).toString();
            QString password = query.value(1).toString();
            QString confirmPassword = query.value(2).toString();
            QString securityQuestion = query.value(3).toString();
            QString answer = query.value(4).toString();
        }
        qDebug() << "User Id is :" <<userId;
        qDebug() << "Password is :" <<password;
        qDebug() << "Confirm Password is :" <<confirmPassword;
        qDebug() << "Security Question is :"<<securityQuestion;
        qDebug() << "Security Question's Answer is :" <<answer;

    }
}
