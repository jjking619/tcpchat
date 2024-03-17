#ifndef TCPLOGIN_H
#define TCPLOGIN_H

#include <QWidget>

namespace Ui {
class tcplogin;
}

class tcplogin : public QWidget
{
    Q_OBJECT

public:
    explicit tcplogin(QWidget *parent = nullptr);
    ~tcplogin();

private slots:
    void on_loginbtn_clicked();

private:
    Ui::tcplogin *ui;
};

#endif // TCPLOGIN_H
