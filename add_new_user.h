#ifndef ADD_NEW_USER_H
#define ADD_NEW_USER_H
#include <QTableWidget>
#include "dashboard.h"
#include <QDialog>

namespace Ui {
class Add_New_User;
}

class Add_New_User : public QDialog
{
    Q_OBJECT

public:
    explicit Add_New_User(QWidget *parent = nullptr);
    ~Add_New_User();

private slots:
    void on_submitButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::Add_New_User *ui;

signals:
    void userAdded();
};

#endif // ADD_NEW_USER_H
