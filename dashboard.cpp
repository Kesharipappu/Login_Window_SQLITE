#include "dashboard.h"
#include "ui_dashboard.h"
#include "add_new_user.h"
#include "add_new_admin.h"
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QDebug>
#include <QSqlError>

Dashboard::Dashboard(QWidget *parent, bool isSuperAdmin) :
    QDialog(parent),
    ui(new Ui::Dashboard),
    isSuperAdmin(isSuperAdmin)  // Store the isSuperAdmin flag
{
    ui->setupUi(this);

    //It will Hide newUserButton and adminUserButton if userid and password != SuperAdmin
    if (!isSuperAdmin)
    {
        ui->newUserButton->hide();
        ui->adminUserButton->hide();
    }

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
    ui->newUserButton->setStyleSheet(buttonStylesheet);
    ui->adminUserButton->setStyleSheet(buttonStylesheet);

    updateUserTable();
    updateAdminTable();
}
void Dashboard::updateUserTable()
{
    //It will fetch all the data from users table and display on dashboardTableWidget. It will update table when ever new User is added
    ui->dashboardTableWidget->setRowCount(0);  // Clear any existing rows
    QSqlQuery query("SELECT * FROM users");
    int row = 0;
    while (query.next())
    {
        ui->dashboardTableWidget->insertRow(row);

        QTableWidgetItem *userIdItem = new QTableWidgetItem(query.value("user_id").toString());
        QTableWidgetItem *firstNameItem = new QTableWidgetItem(query.value("first_name").toString());
        QTableWidgetItem *lastNameItem = new QTableWidgetItem(query.value("last_name").toString());
        QTableWidgetItem *answerItem = new QTableWidgetItem(query.value("answer").toString());
        QTableWidgetItem *role = new QTableWidgetItem(query.value("role").toString());
        QTableWidgetItem *questionId = new QTableWidgetItem(query.value("Question_id").toString());

        ui->dashboardTableWidget->setItem(row, 0, userIdItem);
        ui->dashboardTableWidget->setItem(row, 1, firstNameItem);
        ui->dashboardTableWidget->setItem(row, 2, lastNameItem);
        ui->dashboardTableWidget->setItem(row, 3, questionId);
        ui->dashboardTableWidget->setItem(row, 4, answerItem);
        ui->dashboardTableWidget->setItem(row, 5, role);

        if (isSuperAdmin)
        {
            // Create the Delete button in table at each row
            QPushButton *deleteButton = new QPushButton("Delete");
            // Set the size of the Delete button
            deleteButton->setFixedSize(80, 25); // Adjust width and height as needed
            deleteButton->setStyleSheet("QPushButton { background-color: #FF5733; color: white; }"); // Adjust color as needed

            // It will add Delete Button at Action Column
            ui->dashboardTableWidget->setCellWidget(row, 6, deleteButton);

            // Connect the delete button's signal to a slot
            connect(deleteButton, &QPushButton::clicked, this, [this, row]
            {
                onDeleteButtonClicked(row);
            });
        }
        row++;
    }
    if (!isSuperAdmin)
    {
        ui->dashboardTableWidget->hideColumn(6);  // Hide the Action column if not SuperAdmin
    }
}

void Dashboard::updateAdminTable()
{
    //It will fetch all the data from users table and display on dashboardTableWidget. It will update table when ever new Admin is added
    ui->dashboardTableWidget->setRowCount(0);  // Clear any existing rows
    QSqlQuery query("SELECT * FROM users");
    int row = 0;
    while (query.next())
    {
        ui->dashboardTableWidget->insertRow(row);

        QTableWidgetItem *userIdItem = new QTableWidgetItem(query.value("user_id").toString());
        QTableWidgetItem *firstNameItem = new QTableWidgetItem(query.value("first_name").toString());
        QTableWidgetItem *lastNameItem = new QTableWidgetItem(query.value("last_name").toString());
        QTableWidgetItem *answerItem = new QTableWidgetItem(query.value("answer").toString());
        QTableWidgetItem *role = new QTableWidgetItem(query.value("role").toString());
        QTableWidgetItem *questionId = new QTableWidgetItem(query.value("Question_id").toString());


        ui->dashboardTableWidget->setItem(row, 0, userIdItem);
        ui->dashboardTableWidget->setItem(row, 1, firstNameItem);
        ui->dashboardTableWidget->setItem(row, 2, lastNameItem);
        ui->dashboardTableWidget->setItem(row, 3, questionId);
        ui->dashboardTableWidget->setItem(row, 4, answerItem);
        ui->dashboardTableWidget->setItem(row, 5, role);

        if (isSuperAdmin)
        {
            // Create the Delete button in table at each row
            QPushButton *deleteButton = new QPushButton("Delete");
            // Set the size of the Delete button
            deleteButton->setFixedSize(80, 25); // Adjust width and height as needed
            deleteButton->setStyleSheet("QPushButton { background-color: #FF5733; color: white; }"); // Adjust color as needed

            // It will add Delete Button at Action Column
            ui->dashboardTableWidget->setCellWidget(row, 6, deleteButton);

            // Connect the delete button's signal to a slot
            connect(deleteButton, &QPushButton::clicked, this, [this, row]
            {
                onDeleteButtonClicked(row);
            });
        }
        row++;
    }
    if (!isSuperAdmin)
    {
        ui->dashboardTableWidget->hideColumn(6);  // Hide the Action column if not SuperAdmin

        // Increase the width of each column by 42 units
        int numColumns = ui->dashboardTableWidget->columnCount();
        for (int i = 0; i < numColumns; ++i)
        {
            int currentWidth = ui->dashboardTableWidget->columnWidth(i);
            ui->dashboardTableWidget->setColumnWidth(i, currentWidth + 42);
        }
    }
}

//It will delete the selected Row's Data from table alongwith Database

void Dashboard ::onDeleteButtonClicked(int row)
{
    // Remove the row from the database if necessary
    QString userId = ui->dashboardTableWidget->item(row, 0)->text();
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);
    if (!query.exec())
    {
        // Handle query execution error if necessary
        qDebug() << "Failed to delete user from database: " << query.lastError();
    }

    // Remove the row from the table widget
    ui->dashboardTableWidget->removeRow(row);
}

Dashboard::~Dashboard()
{
    delete ui;
}
void Dashboard::on_newUserButton_clicked()
{
    Add_New_User mAdd_New_User;
    mAdd_New_User.setEnabled(true);
    connect(&mAdd_New_User, &Add_New_User::userAdded, this, &Dashboard::updateUserTable);
    mAdd_New_User.exec();
}

void Dashboard::on_adminUserButton_clicked()
{
    Add_New_Admin mAddNewAdmin;
    mAddNewAdmin.setEnabled(true);
    connect(&mAddNewAdmin, &Add_New_Admin::adminAdded, this, &Dashboard::updateAdminTable);
    mAddNewAdmin.exec();
}

void Dashboard:: on_newAdminButton_clicked()
{

}

