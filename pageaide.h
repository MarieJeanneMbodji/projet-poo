#ifndef PAGEAIDE_H
#define PAGEAIDE_H

#include <QWidget>

namespace Ui {
class pageaide;
}

class pageaide : public QWidget
{
    Q_OBJECT
    
public:
    explicit pageaide(QWidget *parent = 0);
    ~pageaide();
    
private slots:
    void on_pageaide_destroyed();

private:
    Ui::pageaide *ui;
};

#endif // PAGEAIDE_H
