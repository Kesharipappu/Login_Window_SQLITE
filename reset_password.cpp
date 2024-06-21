#include "reset_password.h"
#include "ui_reset_password.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

reset_Password::reset_Password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reset_Password)
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
    ui->submitButton->setStyleSheet(buttonStylesheet);
}
reset_Password::~reset_Password()
{
    delete ui;
}
void reset_Password::on_submitButton_clicked()
{
    // Retrieve the new User Id and new password and confirm password from the input fields
    QString newUserId = ui->newUserIdlineEdit->text();
    QString resetPasswordSecurityQuestion = ui->resetPasswordSecurityQuestioncomboBox->currentText();
    QString resetPasswordAnswer = ui->resetPasswordSecurityQuestionlineEdit->text();
    QString newPassword = ui->newPasswordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    //Fetch User Id to match with newUserId. If both will be same then user will be able to change password
    QSqlQuery query_to_match;
    query_to_match.exec("SELECT * FROM new_user_details");
    QString usersId;
    QString securityQuestion;
    QString answer;
    while(query_to_match.next())
    {
        usersId = query_to_match.value(0).toString();
        securityQuestion = query_to_match.value(3).toString();
        answer = query_to_match.value(4).toString();
    }
    qDebug() << "User Id is for reset Password is :" <<usersId;
    qDebug() << "Security question for reset Password is :"<<securityQuestion;
    qDebug() <<"Question's Answer for reset Password is : " <<answer;

    // Check if the passwords match
    if (newPassword != confirmPassword)
    {
        QMessageBox::warning(this, "Error", "New password and Confirm password should match.");
        ui->newPasswordLineEdit->clear();
        ui->confirmPasswordLineEdit->clear();
        return;
    }
    if(newUserId == "" && newPassword == "" && confirmPassword =="")
    {
        QMessageBox::warning(this, "Warning", "Please enter User Id and Password");
    }
    else if(newUserId == "" && newPassword != "" && confirmPassword !="")
    {
        QMessageBox::warning(this, "Error", "Please enter User Id");
    }
    else if(newUserId != "" && newPassword == "" && confirmPassword == "")
    {
        QMessageBox::warning(this,"Reset User Id and Password","Please enter New password & Confirm password");
    }
    else
    {
        // It will update password if User Id is correct
        QSqlQuery query;
        if(usersId == newUserId && resetPasswordSecurityQuestion == securityQuestion && resetPasswordAnswer == answer)
        {
            QString updatePasswordQuery = QString("UPDATE new_user_details "
                                                  "SET password = '%1', confirm_password = '%2'"
                                                  "WHERE user_id = '%3'").arg(newPassword).arg(confirmPassword).arg(usersId);

            if (!query.exec(updatePasswordQuery))
            {
                qDebug() << "Failed to insert new password:" << query.lastError().text();
                QMessageBox::critical(this, "Error", "Failed to insert new password: " + query.lastError().text());
                return;
            }
            QMessageBox::information(this, "Success", "User ID and password updated successfully.");
            qDebug() << "New user ID and password inserted successfully.";
            accept(); //It will close the current opened window
        }
        else
        {
            QMessageBox::warning(this, "Warning", "You are not an authentic user");
            ui->newUserIdlineEdit->clear();
            ui->resetPasswordSecurityQuestioncomboBox->setCurrentIndex(0);
            ui->resetPasswordSecurityQuestionlineEdit->clear();
            ui->newPasswordLineEdit->clear();
            ui->confirmPasswordLineEdit->clear();
        }
    }
}
