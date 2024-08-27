#include "add_new_user.h"
#include "ui_add_new_user.h"
#include "dashboard.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTableWidget>
#include "utils.h"

Add_New_User::Add_New_User(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_New_User)
{
    ui->setupUi(this);

    //It will set Window Title Name
    setWindowTitle("Add New User Window");

    // It will Remove icon from window title
    QPixmap transparentPixmap(1, 1);
    transparentPixmap.fill(Qt::transparent);
    setWindowIcon(QIcon(transparentPixmap));

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

QPushButton* Add_New_User::getUpdateUserButton() const {
    return ui->updateUserButton;
}

void Add_New_User::on_updateUserButton_clicked() {
    QString userId = ui->addNewUserIdLineEdit->text();
    QString firstName = ui->addNewUserFirstNamelineEdit->text();
    QString lastName = ui->addNewUserLastNamelineEdit->text();
    QString password = ui->addNewUserPasswordlineEdit->text();
    QString securityQuestion = ui->addNewUserSecurityQuestionlineEdit->text();
    int64_t questionId = ui->addNewUsersecurityQuestioncomboBox->currentIndex() + 1; // Assuming it is a combo box

    QSqlQuery query;
    query.prepare("UPDATE users SET first_name = :first_name, last_name = :last_name, password = :password, answer = :answer, Question_id = :question_id WHERE user_id = :user_id");
    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":password", password);
    query.bindValue(":answer", securityQuestion);
    query.bindValue(":question_id", questionId);
    query.bindValue(":user_id", userId);

    if (!query.exec())
    {
        qDebug() << "Failed to update user: " << query.lastError();
    }
    else
    {
        emit userUpdated(); // Emit a signal if you want to update the table in Dashboard
        QMessageBox msgBox;
        applyMessageBoxStyle(msgBox);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Updated Successfully !!!");
        msgBox.setWindowTitle("Success");
        msgBox.exec();
        this->accept();
    }
}

void Add_New_User::hideSubmitAndCancelButtons()
{
    ui->submitButton->hide();
    ui->cancelButton->hide();
}

void Add_New_User::populateSecurityQuestionComboBox(QComboBox *comboBox)
{
    QSqlQuery query("SELECT security_question FROM users_questions");
    comboBox->clear(); // Clear existing items
    while (query.next()) {
        QString securityQuestion = query.value("security_question").toString();
        comboBox->addItem(securityQuestion);
    }
}

void Add_New_User::setUserFields(const QString &userId, const QString &firstName, const QString &lastName, const QString &password, const QString &securityQuestion, const QString &answer)
{
    ui->addNewUserIdLineEdit->setText(userId);
    ui->addNewUserFirstNamelineEdit->setText(firstName);
    ui->addNewUserLastNamelineEdit->setText(lastName);
    ui->addNewUserPasswordlineEdit->setText(password);

    populateSecurityQuestionComboBox(ui->addNewUsersecurityQuestioncomboBox);
    int index = ui->addNewUsersecurityQuestioncomboBox->findText(securityQuestion);
    qDebug() << "Setting security question index to: " << index; // Add this line for debugging
    if (index != -1) {
        ui->addNewUsersecurityQuestioncomboBox->setCurrentIndex(index);
    } else {
        qDebug() << "Security question not found in combo box.";
    }

    ui->addNewUserSecurityQuestionlineEdit->setText(answer);
}

void Add_New_User::on_submitButton_clicked()
{
    // Set the Stylesheet for QMessageBox
    QMessageBox msgBox;

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

        applyMessageBoxStyle(msgBox);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please fill all fields");
        msgBox.setWindowTitle("Warning");
        msgBox.exec();
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

    applyMessageBoxStyle(msgBox);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("Record added Successfully !!!");
    msgBox.setWindowTitle("Success");
    msgBox.exec();

    if (!query.exec())
    {
        QString errorText = query.lastError().text();
        applyMessageBoxStyle(msgBox);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(errorText);
        msgBox.setWindowTitle("Database Error");
        msgBox.exec();
    }
    emit userAdded();
    accept(); //It will close the current opened window
}
void Add_New_User::on_cancelButton_clicked()
{
    accept();
}

void Add_New_User::hideUpdateButton()
{
    ui->updateUserButton->hide(); // Hide the updateUserButton
}
void Add_New_User::disableUserIdEdit()
{
    ui->addNewUserIdLineEdit->setReadOnly(true);  // Set the line edit to read-only
}

