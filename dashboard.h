#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QDialog>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>

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
    void updateUserTable();
    void updateAdminTable();
    void on_adminUserButton_clicked();
    void onDeleteButtonClicked(int row);
    void showContextMenu(const QPoint &pos);
    void deleteRow();
    void updateRow();
    void on_searchButton_clicked();

    void on_saveToPdfButton_clicked();

private:
    Ui::Dashboard *ui;
    bool isSuperAdmin;
    int contextMenuRow; // Store the row where the context menu is triggered
    QString fetchSecurityQuestion(const QString &questionId);
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

};

#endif // DASHBOARD_H
