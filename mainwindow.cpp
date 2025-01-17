//#include "mainwindow.h"
//#include "ui_mainwindow.h"
//#include <QtSql/QSqlDatabase>
//#include <QDebug>
//#include <QSqlRecord>
//#include <QSqlQuery>
//#include <QMessageBox>
//#include <QSqlError>
//#include "reset_password.h"
//#include "dashboard.h"
//#include "newuser.h"
//MainWindow::MainWindow(QWidget *parent)
//    : QMainWindow(parent)
//    , ui(new Ui::MainWindow)
//{
//    ui->setupUi(this);
//    ui->newUserButton->hide();


//    // Set the stylesheet for the newUserSubmitButton to make it 3D and attractive
//    QString buttonStylesheet =
//            "QPushButton {"
//            "    background-color: #28A745;"  // Button color
//            "    color: white;"               // Text color
//            "    border-radius: 7px;"        // Rounded corners
//            "    box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2);"  // Shadow for 3D effect
//            "    border: 1px solid rgb(0, 0, 0); "/* Dark blue border */

//            "}"
//            "QPushButton:hover {"
//            "    background-color: #FD7E14;"    // Background color on hover
//            "    color: black;"               // Text color on hover
//            "    box-shadow: 0 12px 24px 0 rgba(0,0,0,0.2);"  // Larger shadow on hover
//            "}"
//            "QPushButton:pressed {"
//            "    background-color: #87CEFA;"  // Background color when pressed
//            "    box-shadow: 0 5px 10px 0 rgba(0,0,0,0.2);"   // Smaller shadow when pressed
//            "    transform: translateY(4px);" // Move the button down slightly when pressed
//            "}";
//    //    ui->Loginbutton->setStyleSheet(buttonStylesheet);
////    ui->newUserButton->setStyleSheet(buttonStylesheet);
//    //    ui->forgotPasswordButton->setStyleSheet(buttonStylesheet);

//    //Database Connection with QSQLITE.  Dattabase name -> loginDetails
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("loginDetails.db");

//    if(db.open())
//    {
//        qDebug() << "Database Connected Successfully... !!!";
//    }
//    else
//    {
//        qDebug() << "Database Not Connected";
//        qDebug() << db.lastError().text();
//    }
//}

//MainWindow::~MainWindow()
//{
//    delete ui;
//}
//void MainWindow::on_Loginbutton_clicked()
//{
//    // Set the Stylesheet for QMessageBox
//    QMessageBox msgBox;
//    // Apply stylesheet to change background color and text color
//    msgBox.setStyleSheet("QMessageBox { background-color:#cfe2f3; }"  // Light blue background
//                         "QLabel { color: #1a237e; }"  // Dark blue text
//                         "QPushButton { color: #1a237e; background-color: #ffffff; }"  // White buttons with dark blue text
//                         "QPushButton:hover { background-color: #e0e0e0; }");  // Light gray hover effect

//    QString line_edit_user_id = ui->userIdLineEdit->text();
//    QString line_edit_password = ui->passwordLineEdit->text();

//    //It is used to match user id and password of User and Admin. If user id and password is not SuperAdmin.
//    QSqlQuery query;
//    QString userId;
//    QString password;

//    //Initially ValidUser(SuperAdmin) will be false
//    bool isValidUser = false;
//    query.exec("SELECT * FROM users");
//    while(query.next())
//    {
//        userId = query.value(0).toString();
//        password = query.value(5).toString();

//        if (line_edit_user_id == userId && line_edit_password == password)
//        {
//            //If both User Id and Password will be SuperAdmin then ValidUser will be true
//            isValidUser = true;
//            break;
//        }
//        qDebug() << "User Id is :" <<userId;
//        qDebug() << "Password is :" <<password;
//    }
//    //If User Id and Password is valid and user click on Login Button
//    if((line_edit_user_id == "SuperAdmin" && line_edit_password == "SuperAdmin") || (line_edit_user_id == userId && line_edit_password == password) || /*Pappu*/(isValidUser))
//    {
//        //If both UserId and password will be SuperAdmin then Login will be Successfully
//        bool isSuperAdmin = (line_edit_user_id == "SuperAdmin" && line_edit_password == "SuperAdmin");
//        msgBox.setIcon(QMessageBox::Information);
//        msgBox.setText("Logged in Successfully !!!");
//        msgBox.setWindowTitle("Success");
//        msgBox.exec();

//        ui->userIdLineEdit->clear();
//        ui->passwordLineEdit->clear();

//        //It will close the Previous window (login window)
//        hide();

////        mainWindowDialog mWindowDialog;
////        mWindowDialog.setEnabled(true);
////        mWindowDialog.exec();


//        //It will open the Dashboard window
//                Dashboard mDashboard(nullptr, isSuperAdmin);

//                mDashboard.setEnabled(true);
//                mDashboard.exec();
//    }
//    //If user didn't enter Valid User Id and Password and click on Login Button
//    else if(line_edit_user_id == "" && line_edit_password == "")
//    {
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.setText("Please Enter User Id and Password");
//        msgBox.setWindowTitle("Warning");
//        msgBox.exec();
//    }
//    //If user entered Invalid User Id and Password and click on Login Button
//    else if(line_edit_user_id != "SuperAdmin"  && line_edit_password != "SuperAdmin")
//    {
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.setText("Please Enter Valid User Id and Password");
//        msgBox.setWindowTitle("Warning");
//        msgBox.exec();
//        ui->userIdLineEdit->clear();
//        ui->passwordLineEdit->clear();
//    }
//    //If user entered only Password and click on Login Button
//    else if(line_edit_user_id ==""  && line_edit_password == "SuperAdmin")
//    {
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.setText("Please Enter User Id");
//        msgBox.setWindowTitle("Warning");
//        msgBox.exec();
//    }
//    //If user entered only User Id and click on Login Button
//    else if(line_edit_user_id =="SuperAdmin"  && line_edit_password == "")
//    {
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.setText("Please Enter Password");
//        msgBox.setWindowTitle("Warning");
//        msgBox.exec();
//    }
//    //If user entered Valid User Id and Invalid Password click on Login Button
//    else if(line_edit_user_id == "SuperAdmin"  && line_edit_password != "SuperAdmin")
//    {
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.setText("Please Enter Valid Password");
//        msgBox.setWindowTitle("Warning");
//        msgBox.exec();
//        ui->passwordLineEdit->clear();
//    }
//    //If user entered Invalid User Id and Valid Password click on Login Button
//    else if(line_edit_user_id != "SuperAdmin"  && line_edit_password == "SuperAdmin")
//    {
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.setText("Please Enter Valid User Id");
//        msgBox.setWindowTitle("Warning");
//        msgBox.exec();
//        ui->userIdLineEdit->clear();
//    }
//    qDebug() << "User Id:" << "SuperAdmin";
//    qDebug() << "Password :" << "SuperAdmin";

//}
//void MainWindow::on_resetButton_clicked()
//{
//    ui->userIdLineEdit->clear();
//    ui->passwordLineEdit->clear();
//}
//void MainWindow::on_forgotPasswordButton_clicked()
//{
//    //    reset_Password mReset_Password;
//    //    mReset_Password.setEnabled(true);
//    //    mReset_Password.exec();
//}
//void MainWindow::on_newUserButton_clicked()
//{
//    NewUser mNewUser;
//    mNewUser.setEnabled(true);
//    mNewUser.exec();
//}
