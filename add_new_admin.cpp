#include "add_new_admin.h"
#include "ui_add_new_admin.h"
#include "dashboard.h"
#include "QMessageBox"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTableWidget>

Add_New_Admin::Add_New_Admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_New_Admin)
{
    ui->setupUi(this);

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
            ui->addNewAdminsecurityQuestioncomboBox->addItem(securityQuestion);
        }
    }
    else
    {
        qDebug() << "Failed to fetch security questions:" << query.lastError().text();
    }
}

Add_New_Admin::~Add_New_Admin()
{
    delete ui;
}

void Add_New_Admin::on_submitButton_clicked()
{
    QString userId = ui->addNewAdminIdLineEdit->text();
    QString firstName = ui->addNewAdminFirstNamelineEdit->text();
    QString lastName = ui->addNewAdminLastNamelineEdit->text();
    QString securityQuestion = ui->addNewAdminsecurityQuestioncomboBox->currentText();
    QString answer = ui->addNewAdminSecurityQuestionlineEdit->text();
    QString role = "Admin";
    QString password = ui->addNewAdminPasswordLineEdit->text();
    int64_t index = ui->addNewAdminsecurityQuestioncomboBox->currentIndex() + 1;
    qDebug() <<"Index is" <<index + 1;

    if(userId.isEmpty() || firstName.isEmpty() || lastName.isEmpty() || securityQuestion.isEmpty() || answer.isEmpty() || password.isEmpty())
    {
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
    emit adminAdded();
    accept(); //It will close the current opened window
}

void Add_New_Admin::on_cancelButton_clicked()
{
    accept();
}

