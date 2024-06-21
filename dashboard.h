#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class Dashboard;
}

class Dashboard : public QDialog
{
    Q_OBJECT

public:

    explicit Dashboard(QWidget *parent = nullptr, bool isSuperAdmin = false);
    ~Dashboard();

private slots:
    void on_newUserButton_clicked();

    void on_newAdminButton_clicked();

    void updateUserTable();

    void updateAdminTable();

    void on_adminUserButton_clicked();

    void onDeleteButtonClicked(int row);


private:
    Ui::Dashboard *ui;
    bool isSuperAdmin;  // Add this member variable

};

#endif // DASHBOARD_H
