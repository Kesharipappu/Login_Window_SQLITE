#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit CustomLineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {}

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {
            emit returnPressed();
            event->accept();
        }
        else
        {
            QLineEdit::keyPressEvent(event);
        }
    }
};

#endif // CUSTOMLINEEDIT_H
