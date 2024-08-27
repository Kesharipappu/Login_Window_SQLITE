#ifndef ADD_NEW_ADMIN_H
#define ADD_NEW_ADMIN_H
#include "dashboard.h"
#include <QTableWidget>
#include <QDialog>
#include <QComboBox>

namespace Ui {
class Add_New_Admin;
}

class Add_New_Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Add_New_Admin(QWidget *parent = nullptr);
    ~Add_New_Admin();
    void hideUpdateButton();
    void setAdminFields(const QString &userId, const QString &firstName, const QString &lastName, const QString &password, const QString &securityQuestion, const QString &answer);
    void hideSubmitAndCancelButtons();
    void disableAdminIdEdit();
    QPushButton* getUpdateAdminButton() const;

private slots:
    void on_submitButton_clicked();
    void on_cancelButton_clicked();

public slots:
    void on_updateAdminButton_clicked();

private:
    Ui::Add_New_Admin *ui;
    void populateSecurityQuestionComboBox(QComboBox *comboBox);

signals:
    void adminAdded();
    void adminUpdated();
    void updateAdminButtonClicked();
};

#endif // ADD_NEW_ADMIN_H
