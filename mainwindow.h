#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "structures.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    Ui::MainWindow *ui;
    std::vector<Point*> points;
    std::vector<Triangle*> triangles;
    bool isTriangleEnded;
};

#endif // MAINWINDOW_H
