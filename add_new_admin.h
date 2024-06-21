#ifndef ADD_NEW_ADMIN_H
#define ADD_NEW_ADMIN_H
#include "dashboard.h"
#include <QTableWidget>
#include <QDialog>

namespace Ui {
class Add_New_Admin;
}

class Add_New_Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Add_New_Admin(QWidget *parent = nullptr);
    ~Add_New_Admin();

private slots:

    void on_submitButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::Add_New_Admin *ui;

signals:
    void adminAdded();
};

#endif // ADD_NEW_ADMIN_H
