#ifndef RESET_PASSWORD_H
#define RESET_PASSWORD_H

#include <QDialog>

namespace Ui {
class reset_Password;
}

class reset_Password : public QDialog
{
    Q_OBJECT

public:
    explicit reset_Password(QWidget *parent = nullptr);
    ~reset_Password();

private slots:
    void on_submitButton_clicked();

private:
    Ui::reset_Password *ui;
};

#endif // RESET_PASSWORD_H
