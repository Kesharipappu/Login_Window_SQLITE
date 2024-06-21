#include "add_new_user.h"
#include "ui_add_new_user.h"
#include "dashboard.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTableWidget>

Add_New_User::Add_New_User(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_New_User)
{
    ui->setupUi(this);
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
    ui->submitButton->setStyleSheet(buttonStylesheet);
    ui->cancelButton->setStyleSheet(buttonStylesheet);

    //It will crate users table.  Create table if it doesn't exist. Table name -> users
    QSqlQuery query;
    QString createTableQuery = ("CREATE TABLE IF NOT EXISTS users ("
                                "user_id TEXT PRIMARY KEY, "
                                "first_name TEXT NOT NULL, "
                                "last_name TEXT NOT NULL, "
                                "answer TEXT NOT NULL,"
                                "role TEXT NOT NULL,"
                                "password,"
                                "Question_id)");
    if(query.exec(createTableQuery))
    {
        qDebug() << "Table created or already exists.";
    }
    else
    {
        qDebug() << "Failed to create table:" << query.lastError().text();
    }

    // Create users_questions table if it doesn't exist
    QString createUsersQuestionsTableQuery = ("CREATE TABLE IF NOT EXISTS users_questions ("
                                              "running_id, "
                                              "security_question TEXT NOT NULL)");
    if (query.exec(createUsersQuestionsTableQuery)) {
        qDebug() << "Users_questions table created or already exists.";
    }
    else
    {
        qDebug() << "Failed to create users_questions table:" << query.lastError().text();
    }

    // It will fetch all security questions from users_questions table and put in Combo Box
    QString selectSecurityQuestionsQuery = "SELECT security_question FROM users_questions";
    if (query.exec(selectSecurityQuestionsQuery))
    {
        while (query.next())
        {
            QString securityQuestion = query.value(0).toString();
            ui->addNewUsersecurityQuestioncomboBox->addItem(securityQuestion);
        }
    }
    else
    {
        qDebug() << "Failed to fetch security questions:" << query.lastError().text();
    }

}
Add_New_User::~Add_New_User()
{
    delete ui;
}
void Add_New_User::on_submitButton_clicked()
{
    QString userId = ui->addNewUserIdLineEdit->text();
    QString firstName = ui->addNewUserFirstNamelineEdit->text();
    QString lastName = ui->addNewUserLastNamelineEdit->text();
    QString securityQuestion = ui->addNewUsersecurityQuestioncomboBox->currentText();
    QString answer = ui->addNewUserSecurityQuestionlineEdit->text();
    QString role = "User";
    QString password = ui->addNewUserPasswordlineEdit->text();
    int64_t index = ui->addNewUsersecurityQuestioncomboBox->currentIndex() + 1;
    qDebug() <<"Index is" <<index + 1;


    if (userId.isEmpty() || firstName.isEmpty() || lastName.isEmpty() || answer.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill all fields.");
        return;
    }

    //It will insert into users table
    QSqlQuery query;
    query.prepare("INSERT INTO users (user_id, first_name, last_name, answer, role, password, Question_id) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(firstName);
    query.addBindValue(lastName);
    query.addBindValue(answer);
    query.addBindValue(role);
    query.addBindValue(password);
    query.addBindValue(index);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
    }
    emit userAdded();
    accept(); //It will close the current opened window
}
void Add_New_User::on_cancelButton_clicked()
{
    accept();
}
