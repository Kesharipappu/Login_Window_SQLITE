#include "forgetpassword.h"
#include "ui_forgetpassword.h"
#include <QDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "loginwindow.h"
#include "utils.h"
#include <QMessageBox>

ForgetPassword::ForgetPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForgetPassword)
{
    ui->setupUi(this);

    //It will set Window Title Name
    setWindowTitle("Forgot Password Window");

    // It will Remove icon from window title
    QPixmap transparentPixmap(1, 1);
    transparentPixmap.fill(Qt::transparent);
    setWindowIcon(QIcon(transparentPixmap));

    QSqlQuery query;
    // It will fetch all security questions from users_questions table and put in Combo Box
    QString selectSecurityQuestionsQuery = "SELECT security_question FROM users_questions";
    if (query.exec(selectSecurityQuestionsQuery))
    {
        while (query.next())
        {
            QString securityQuestion = query.value(0).toString();
            ui->forgotPasswordSecurityQuestioncomboBox->addItem(securityQuestion);
        }
    }
    else
    {
        qDebug() << "Failed to fetch security questions:" << query.lastError().text();
    }
}

ForgetPassword::~ForgetPassword()
{
    delete ui;
}

void ForgetPassword::setUserData(const QString &userId, const QString &answer, const QString &question)
{
    fetchedUserId = userId;
    fetchedAnswer = answer;
    securityQuestion = question;
    ui->forgotPasswordSecurityQuestioncomboBox->addItem(question);
}

void ForgetPassword::on_savePasswordButton_clicked()
{
    QMessageBox msgBox;
    QString selectedSecurityQuestion = ui->forgotPasswordSecurityQuestioncomboBox->currentText();
    QString enteredAnswer = ui->forgotPasswordSecurityQuestionlineEdit->text();
    QString newPassword = ui->newPasswordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordlineEdit->text();

    if (enteredAnswer.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty())
    {
        applyMessageBoxStyle(msgBox);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please fill all details");
        msgBox.setWindowTitle("Warning");
        msgBox.exec();
    }
    else
    {
        // Display the user input on the console
        qDebug() << "Selected Security Question:" << selectedSecurityQuestion;
        qDebug() << "Entered Answer:" << enteredAnswer;
        qDebug() << "New Password:" << newPassword;
        qDebug() << "Confirm Password:" << confirmPassword;

        // Check if newPassword and confirmPassword are the same
        if (newPassword == confirmPassword)
        {
            if (enteredAnswer.toLower() == fetchedAnswer.toLower() && selectedSecurityQuestion == securityQuestion)
            {
                // Update the password in the database
                QSqlQuery updateQuery;
                updateQuery.prepare("UPDATE users SET password = :newPassword WHERE user_id = :userId");
                updateQuery.bindValue(":newPassword", newPassword);
                updateQuery.bindValue(":userId", fetchedUserId);

                if (updateQuery.exec())
                {
                    qDebug() << "Password updated successfully.";
                    applyMessageBoxStyle(msgBox);
                    msgBox.setIcon(QMessageBox::Information);
                    msgBox.setText("Password updated Successfully !!!");
                    msgBox.setWindowTitle("Success");
                    msgBox.exec();
                    accept();
                }
                else
                {
                    qDebug() << "Failed to update password:" << updateQuery.lastError();
                    QMessageBox::critical(this, "Database Error", "Failed to update password.");
                }
            }
            else
            {
                applyMessageBoxStyle(msgBox);
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText("Invalid Operation.");
                msgBox.setWindowTitle("Invalid");
                msgBox.exec();
                ui->forgotPasswordSecurityQuestioncomboBox->setCurrentIndex(0);
                ui->forgotPasswordSecurityQuestionlineEdit->clear();
                ui->newPasswordLineEdit->clear();
                ui->confirmPasswordlineEdit->clear();
            }
        }
        else
        {
            applyMessageBoxStyle(msgBox);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("New Password and Confirm Password should be same.");
            msgBox.setWindowTitle("Password Mismatch.");
            msgBox.exec();

            ui->newPasswordLineEdit->clear();
            ui->confirmPasswordlineEdit->clear();
        }
    }
}

