#ifndef ADD_NEW_USER_H
#define ADD_NEW_USER_H
#include <QTableWidget>
#include "dashboard.h"
#include <QDialog>
#include <QComboBox>

namespace Ui {
class Add_New_User;
}

class Add_New_User : public QDialog
{
    Q_OBJECT

public:
    explicit Add_New_User(QWidget *parent = nullptr);
    ~Add_New_User();
    void hideUpdateButton();
    void setUserFields(const QString &userId, const QString &firstName, const QString &lastName, const QString &password, const QString &securityQuestion, const QString &answer);
    void hideSubmitAndCancelButtons();
    void disableUserIdEdit();
    QPushButton* getUpdateUserButton() const;

private slots:
    void on_submitButton_clicked();
    void on_cancelButton_clicked();

public slots:
    void on_updateUserButton_clicked();

private:
    Ui::Add_New_User *ui;
    void populateSecurityQuestionComboBox(QComboBox *comboBox);

signals:
    void userAdded();
    void userUpdated();
};

#endif // ADD_NEW_USER_H
