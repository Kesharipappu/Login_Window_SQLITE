#include "dashboard.h"
#include "ui_dashboard.h"
#include "add_new_user.h"
#include "add_new_admin.h"
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QDebug>
#include <QSqlError>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMenu>
#include <QContextMenuEvent>
#include "utils.h"
#include "CustomLineEdit.h"
#include <QEvent>
#include <QKeyEvent>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>

Dashboard::Dashboard(QWidget *parent, bool isSuperAdmin) :
    QDialog(parent),
    ui(new Ui::Dashboard),
    isSuperAdmin(isSuperAdmin)  // Store the isSuperAdmin flag
{
    ui->setupUi(this);

    //It will set Window Title Name
    setWindowTitle("Dashboard Window");

    // It will Remove icon from window title
    QPixmap transparentPixmap(1, 1);
    transparentPixmap.fill(Qt::transparent);
    setWindowIcon(QIcon(transparentPixmap));

    ui->newUserButton->setDefault(false);
    ui->adminUserButton->setDefault(false);

    ui->searchLineEdit->setFocus();

    // Apply event filter to searchLineEdit
    ui->searchLineEdit->installEventFilter(this);

    // Connect the returnPressed signal of searchLineEdit to the on_searchButton_clicked slot
    connect(ui->searchLineEdit, &QLineEdit::returnPressed, this, &Dashboard::on_searchButton_clicked);


    // Enable sorting
    ui->dashboardTableWidget->setSortingEnabled(true);

    // Set context menu policy
    ui->dashboardTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->dashboardTableWidget, &QWidget::customContextMenuRequested, this, &Dashboard::showContextMenu);

    //It will Hide newUserButton and adminUserButton if userid and password != SuperAdmin
    if (!isSuperAdmin)
    {
        ui->newUserButton->hide();
        ui->adminUserButton->hide();
        ui->saveToPdfButton->hide();
    }

    //    connect(ui->saveToPdfButton, &QPushButton::clicked, this, &Dashboard::on_saveToPdfButton_clicked);

    updateUserTable();
    updateAdminTable();
}

bool Dashboard::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->searchLineEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        {
            // Only trigger the search action
            on_searchButton_clicked();
            return true;
        }
    }
    return QDialog::eventFilter(obj, event);
}

Dashboard::~Dashboard()
{
    delete ui;
}

// Context menu handler
void Dashboard::showContextMenu(const QPoint &pos)
{
    // Check if the user is not a super admin
    if (!isSuperAdmin) {
        return; // Do not show the context menu
    }
    QPoint globalPos = ui->dashboardTableWidget->mapToGlobal(pos);
    QMenu contextMenu;

    QAction *deleteAction = contextMenu.addAction("Delete");
    QAction *updateAction = contextMenu.addAction("Update");

    // Store the row where the context menu is triggered
    contextMenuRow = ui->dashboardTableWidget->indexAt(pos).row();

    connect(deleteAction, &QAction::triggered, this, &Dashboard::deleteRow);
    connect(updateAction, &QAction::triggered, this, &Dashboard::updateRow);

    contextMenu.exec(globalPos);
}

void Dashboard::deleteRow()
{
    onDeleteButtonClicked(contextMenuRow);
}

void Dashboard::updateRow()
{
    QString userId = ui->dashboardTableWidget->item(contextMenuRow, 0)->text();
    QString firstName = ui->dashboardTableWidget->item(contextMenuRow, 1)->text();
    QString lastName = ui->dashboardTableWidget->item(contextMenuRow, 2)->text();
    QString questionId = ui->dashboardTableWidget->item(contextMenuRow, 3)->text();
    QString answer = ui->dashboardTableWidget->item(contextMenuRow, 4)->text();
    QString role = ui->dashboardTableWidget->item(contextMenuRow, 5)->text();

    qDebug() << "Retrieved Question_id: " << questionId; // Verify Question_id

    QString securityQuestion = fetchSecurityQuestion(questionId); // Fetch security question based on Question_id

    QString password;
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (query.exec() && query.next()) {
        password = query.value("password").toString();
    } else {
        qDebug() << "Failed to fetch password from database: " << query.lastError();
    }

    if (role.toLower() == "admin")
    {
        Add_New_Admin mAddNewAdmin;
        mAddNewAdmin.setEnabled(true);
        mAddNewAdmin.setAdminFields(userId, firstName, lastName, password, securityQuestion, answer);
        mAddNewAdmin.hideSubmitAndCancelButtons(); // Hide buttons
        mAddNewAdmin.disableAdminIdEdit(); // Disable admin ID edit

        connect(&mAddNewAdmin, &Add_New_Admin::adminUpdated, this, &Dashboard::updateAdminTable);  // Connect the signal to update the table
        mAddNewAdmin.exec();
    }
    else if (role.toLower() == "user")
    {
        Add_New_User mAdd_New_User;
        mAdd_New_User.setEnabled(true);
        mAdd_New_User.setUserFields(userId, firstName, lastName, password, securityQuestion, answer);
        mAdd_New_User.hideSubmitAndCancelButtons(); // Hide buttons
        mAdd_New_User.disableUserIdEdit(); // Disable user ID edit

        connect(&mAdd_New_User, &Add_New_User::userUpdated, this, &Dashboard::updateUserTable);  // Connect the signal to update the table
        mAdd_New_User.exec();
    }
    else
    {
        qDebug() << "Unknown role or condition";
    }
}

QString Dashboard::fetchSecurityQuestion(const QString &questionId)
{
    QString securityQuestion;
    QSqlQuery query;

    // Query to fetch security_question based on Question_id matching running_id
    query.prepare("SELECT security_question FROM users_questions WHERE running_id = "
                  "(SELECT running_id FROM users WHERE Question_id = :question_id)");
    query.bindValue(":question_id", questionId);

    if (query.exec()) {
        if (query.next()) {
            securityQuestion = query.value("security_question").toString();
        } else {
            qDebug() << "No matching security question found for Question_id: " << questionId;
        }
    } else {
        qDebug() << "Failed to execute query: " << query.lastError().text();
    }

    qDebug() << "Fetched security question: " << securityQuestion;
    return securityQuestion;
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
    }
}

//It will delete the selected Row's Data from table alongwith Database
void Dashboard ::onDeleteButtonClicked(int row)
{
    // Create a QMessageBox instance
    QMessageBox msgBox;
    applyMessageBoxStyle(msgBox);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Warning");
    msgBox.setText("Do you want to delete the record?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    // Display the message box and get the user's response
    int reply = msgBox.exec();
    if(reply == QMessageBox::Yes)
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
        else
        {
            // Remove the row from the table widget
            ui->dashboardTableWidget->removeRow(row);
        }
    }
    else{}
}

void Dashboard::on_newUserButton_clicked()
{
    Add_New_User mAdd_New_User;
    mAdd_New_User.setEnabled(true);
    mAdd_New_User.hideUpdateButton(); // Call the method to hide the updateUserButton
    connect(&mAdd_New_User, &Add_New_User::userAdded, this, &Dashboard::updateUserTable);
    mAdd_New_User.exec();
}
void Dashboard::on_adminUserButton_clicked()
{
    Add_New_Admin mAddNewAdmin;
    mAddNewAdmin.setEnabled(true);
    mAddNewAdmin.hideUpdateButton(); // Call the method to hide the updateAdminButton
    connect(&mAddNewAdmin, &Add_New_Admin::adminAdded, this, &Dashboard::updateAdminTable);
    mAddNewAdmin.exec();
}

void Dashboard::on_searchButton_clicked()
{
    QString text = ui->searchLineEdit->text();
    QMessageBox msgBox;

    // Reset the background color of all items to the default color
    for (int row = 0; row < ui->dashboardTableWidget->rowCount(); ++row)
    {
        for (int column = 0; column < ui->dashboardTableWidget->columnCount(); ++column)
        {
            QTableWidgetItem *item = ui->dashboardTableWidget->item(row, column);
            if (item)
            {
                item->setBackground(QColor("#1A1A2A"));  // Reset background color to default
            }
        }
    }

    // Check if the search text is empty
    if (text.isEmpty())
    {
        applyMessageBoxStyle(msgBox);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please enter text to search.");
        msgBox.setWindowTitle("Warning");
        msgBox.exec();
        return; // Exit the function early
    }

    qDebug() << "Search text:" << text;

    bool found = false; // Flag to check if any match is found

    // Highlight matching items
    for (int row = 0; row < ui->dashboardTableWidget->rowCount(); ++row)
    {
        for (int column = 0; column < ui->dashboardTableWidget->columnCount(); ++column)
        {
            QTableWidgetItem *item = ui->dashboardTableWidget->item(row, column);
            if (item)
            {
                if (item->text().contains(text, Qt::CaseInsensitive))
                {
                    item->setBackground(QColor("#03DD73"));  // Highlight matching text
                    qDebug() << "Highlighted Row:" << row << "Column:" << column << "Text:" << item->text();
                    found = true; // Set the flag to true if a match is found
                }
            }
        }
    }

    if (!found) // If no matches were found, show a message box
    {
        applyMessageBoxStyle(msgBox);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(text + " is not available in the table.");
        msgBox.setWindowTitle("Warning");
        msgBox.exec();
    }
}


void Dashboard::on_saveToPdfButton_clicked()
{
    const int maxRowsPerPage = 45;

    QString fileName = QFileDialog::getSaveFileName(this, "Save as PDF", QDir::homePath(), "PDF files (*.pdf)");
    if (fileName.isEmpty())
        return;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPagedPaintDevice::A4);
    pdfWriter.setTitle("Dashboard Data");
    QPainter painter(&pdfWriter);

    // Set font for the PDF
    QFont font = painter.font();
    font.setPointSize(7);
    painter.setFont(font);

    // Calculate column width and row height to fit the page
    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();
    int margin = 20;
    int tableWidth = pageWidth - 2 * margin;
    int tableHeight = pageHeight - 2 * margin;

    int fixedIndexColWidth = 400; // Fixed width for the "Index" column
    int remainingWidth = tableWidth - fixedIndexColWidth;
    int columnCount = ui->dashboardTableWidget->columnCount();
    int rowCount = ui->dashboardTableWidget->rowCount();

    int colWidth = remainingWidth / columnCount;
    int rowHeight = tableHeight / (maxRowsPerPage + 1);  // +1 for the header row

    int x = margin;
    int y = margin;

    // Set font to bold for header
    QFont boldFont = painter.font();
    boldFont.setBold(true);
    painter.setFont(boldFont);

    // Set brush color for header background
    QBrush headerBrush(QColor(200, 200, 200)); // Light black/grey color
    painter.setBrush(headerBrush);

    // Function to print headers
    auto printHeaders = [&]() {
        x = margin;
        painter.drawRect(x, y, fixedIndexColWidth, rowHeight);
        painter.drawText(x + 5, y + 5, fixedIndexColWidth - 10, rowHeight - 10, Qt::AlignCenter, "Index");
        x += fixedIndexColWidth;

        for (int col = 0; col < columnCount; ++col) {
            QString headerText = ui->dashboardTableWidget->horizontalHeaderItem(col) ? ui->dashboardTableWidget->horizontalHeaderItem(col)->text() : "";
            painter.drawRect(x, y, colWidth, rowHeight);
            painter.drawText(x + 5, y + 5, colWidth - 10, rowHeight - 10, Qt::AlignCenter, headerText);
            x += colWidth;
        }
        y += rowHeight;
        x = margin;
    };

    // Print the headers for the first page
    printHeaders();

    // Reset font and brush for data
    painter.setFont(font);
    painter.setBrush(Qt::NoBrush);

    int currentRow = 0; // Track the current row count on the page

    // Print the data
    for (int row = 0; row < rowCount; ++row) {
        // Print the serial number
        painter.drawRect(x, y, fixedIndexColWidth, rowHeight);
        painter.drawText(x + 5, y + 5, fixedIndexColWidth - 10, rowHeight - 10, Qt::AlignCenter, QString::number(row + 1));
        x += fixedIndexColWidth;

        // Print the other data
        for (int col = 0; col < columnCount; ++col) {
            QTableWidgetItem *item = ui->dashboardTableWidget->item(row, col);
            QString cellText = item ? item->text() : "";
            painter.drawRect(x, y, colWidth, rowHeight);
            painter.drawText(x + 5, y + 5, colWidth - 10, rowHeight - 10, Qt::AlignCenter, cellText);
            x += colWidth;
        }
        y += rowHeight;
        x = margin;

        // Increment the current row count
        currentRow++;

        // Check if we need a new page
        if (currentRow >= maxRowsPerPage) {
            pdfWriter.newPage();
            y = margin;
            currentRow = 0;

            // Print the headers again on the new page
            painter.setFont(boldFont);
            painter.setBrush(headerBrush);
            printHeaders();

            // Reset font and brush for data
            painter.setFont(font);
            painter.setBrush(Qt::NoBrush);
        }
    }

    painter.end();
    QMessageBox::information(this, "PDF Saved", "The dashboard data has been saved to a PDF file.");
}
