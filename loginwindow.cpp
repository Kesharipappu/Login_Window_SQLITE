#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "ui_dashboard.h"
#include <QtSql/QSqlDatabase>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include "dashboard.h"
#include "utils.h"
#include "forgetpassword.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    //It will set Window Title Name
    setWindowTitle("Login Window");

    // It will Remove icon from window title
    QPixmap transparentPixmap(1, 1);
    transparentPixmap.fill(Qt::transparent);
    setWindowIcon(QIcon(transparentPixmap));

    //It will set the cursor in userIdLineEdit when project will run
    ui->userIdLineEdit->setFocus();

    //Database Connection with QSQLITE.  Dattabase name -> loginDetails
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("loginDetails.db");

    if(db.open())
    {
        qDebug() << "Database Connected Successfully... !!!";
    }
    else
    {
        qDebug() << "Database Not Connected";
        qDebug() << db.lastError().text();
    }
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_Loginbutton_clicked()
{
    QMessageBox msgBox;

    QString line_edit_user_id = ui->userIdLineEdit->text();
    QString line_edit_password = ui->passwordLineEdit->text();

    //It is used to match user id and password of User and Admin. If user id and password is not SuperAdmin.
    QSqlQuery query;
    QString userId;
    QString password;

    //Initially ValidUser(SuperAdmin) will be false
    bool isValidUser = false;
    query.exec("SELECT * FROM users");
    while(query.next())
    {
        userId = query.value(0).toString();
        password = query.value(5).toString();

        if (line_edit_user_id == userId && line_edit_password == password)
        {
            //If both User Id and Password will be SuperAdmin then ValidUser will be true
            isValidUser = true;
            break;
        }
        qDebug() << "User Id is :" <<userId;
        qDebug() << "Password is :" <<password;
    }
    //If User Id and Password is valid and user click on Login Button
    if((line_edit_user_id == "SuperAdmin" && line_edit_password == "SuperAdmin") || (line_edit_user_id == userId && line_edit_password == password) || /*Pappu*/(isValidUser))
    {
        //If both UserId and password will be SuperAdmin then Login will be Successfully
        bool isSuperAdmin = (line_edit_user_id == "SuperAdmin" && line_edit_password == "SuperAdmin");

        // Insert login details into loginDateTime table
        QString tableName = "loginDateTime";
        bool tableExists = false;

        // Check if the table exists
        query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='" + tableName + "'");
        if (query.next()) {
            tableExists = true;
        }

        if (!tableExists)
        {
            query.exec("CREATE TABLE " + tableName + " (userId TEXT, password TEXT, dateTime DATETIME)");
        }

        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString dateTimeString = currentDateTime.toString("dddd, MMMM d, yyyy hh:mm:ss AP");
        QString insertQuery = QString("INSERT INTO %1 (userId, password, dateTime) VALUES ('%2', '%3', '%4')")
                .arg(tableName).arg(line_edit_user_id).arg(line_edit_password).arg(dateTimeString);

        if (!query.exec(insertQuery))
        {
            qDebug() << "Error inserting login details:" << query.lastError().text();
        }
        else
        {
            qDebug() << "Login details inserted successfully.";
        }

        //It will display on console When User has logged In
        qDebug() << "Logged In Current Date and Time:" << dateTimeString;

        applyMessageBoxStyle(msgBox);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Logged in Successfully !!!");
        msgBox.setWindowTitle("Success");
        msgBox.exec();

        ui->userIdLineEdit->clear();
        ui->passwordLineEdit->clear();

        //It will close the Previous window (login window)
        hide();

        //It will open the Dashboard window
        Dashboard mDashboard(nullptr, isSuperAdmin);
        mDashboard.setEnabled(true);
        mDashboard.exec();
    }
    else
    {
        applyMessageBoxStyle(msgBox);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Unable to Login");
        msgBox.setWindowTitle("Warning");
        msgBox.exec();
        ui->userIdLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
    qDebug() << "User Id:" << "SuperAdmin";
    qDebug() << "Password :" << "SuperAdmin";
}

void LoginWindow::on_forgotPasswordButton_clicked()
{
    QString userId = ui->userIdLineEdit->text();

    if(userId == "")
    {
        QMessageBox msgBox;
        applyMessageBoxStyle(msgBox);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please Enter User Id");
        msgBox.setWindowTitle("Warning");
        msgBox.exec();
    }
    else
    {
        // Ensure the database connection is open
        if (!QSqlDatabase::database().isOpen()) {
            qDebug() << "Database is not open.";
            QMessageBox::critical(this, "Database Error", "Database is not open.");
            return;
        }

        // First query to fetch user_id, answer, and Question_id from users table
        QSqlQuery query;
        query.prepare("SELECT user_id, answer, Question_id FROM users WHERE user_id = :user_id");

        // Debugging statement
        qDebug() << "Binding user_id:" << userId;

        query.bindValue(":user_id", userId);

        // Debugging statement
        qDebug() << "Executing query: SELECT user_id, answer, Question_id FROM users WHERE user_id =" << userId;

        if (!query.exec()) {
            qDebug() << "Failed to execute query:" << query.lastError();
            QMessageBox::critical(this, "Database Error", "Failed to execute query.");
            return;
        }

        if (!query.next()) {
            QMessageBox msgBox;
            applyMessageBoxStyle(msgBox);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("User ID not found.");
            msgBox.setWindowTitle("Invalid User.");
            msgBox.exec();
            ui->userIdLineEdit->clear();
            return;
        }

        QString fetchedUserId = query.value("user_id").toString();
        QString fetchedAnswer = query.value("answer").toString();
        int fetchedQuestionId = query.value("Question_id").toInt();
        qDebug() << "User ID:" << fetchedUserId;
        qDebug() << "Answer:" << fetchedAnswer;
        qDebug() << "Question ID:" << fetchedQuestionId;

        // Second query to fetch security_question from users_questions table
        QSqlQuery questionQuery;
        questionQuery.prepare("SELECT security_question FROM users_questions WHERE running_id = :Question_id");

        // Debugging statement
        qDebug() << "Binding Question_id:" << fetchedQuestionId;

        questionQuery.bindValue(":Question_id", fetchedQuestionId);

        // Debugging statement
        qDebug() << "Executing query: SELECT security_question FROM users_questions WHERE running_id =" << fetchedQuestionId;

        if (!questionQuery.exec()) {
            qDebug() << "Failed to execute question query:" << questionQuery.lastError();
            QMessageBox::critical(this, "Database Error", "Failed to execute question query.");
            return;
        }

        if (!questionQuery.next()) {
            qDebug() << "Question ID not found.";
            QMessageBox::warning(this, "Not Found", "Question ID not found.");
            return;
        }

        QString securityQuestion = questionQuery.value("security_question").toString();
        qDebug() << "Security Question Pappu:" << securityQuestion;

        ForgetPassword mForgetPassword;
        mForgetPassword.setUserData(fetchedUserId, fetchedAnswer, securityQuestion);
        mForgetPassword.setEnabled(true);
        mForgetPassword.exec();
    }
}
