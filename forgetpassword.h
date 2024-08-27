#ifndef FORGETPASSWORD_H
#define FORGETPASSWORD_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class ForgetPassword;
}

class ForgetPassword : public QDialog
{
    Q_OBJECT

public:
    explicit ForgetPassword(QWidget *parent = nullptr);
    ~ForgetPassword();
    void setUserData(const QString &userId, const QString &answer, const QString &question);


private slots:
    void on_savePasswordButton_clicked();

private:
    Ui::ForgetPassword *ui;

    QString fetchedUserId;
    QString fetchedAnswer;
    QString securityQuestion;
};

#endif // FORGETPASSWORD_H
