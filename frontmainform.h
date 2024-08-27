#ifndef FRONTMAINFORM_H
#define FRONTMAINFORM_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class frontMainForm;
}

class frontMainForm : public QDialog
{
    Q_OBJECT

public:
    explicit frontMainForm(QWidget *parent = nullptr);
    ~frontMainForm();

private:
    Ui::frontMainForm *ui;
};

#endif // FRONTMAINFORM_H
