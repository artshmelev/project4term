#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPainter>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "structures.h"
#include "triangleaction.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isTriangleEnded = 0;

    QFile file("data.dat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    int num;
    in >> num;
    for (int i = 0; i < num && !in.atEnd(); ++i) {
        int p, q;
        in >> p >> q;
        points.push_back(new Point(p, q));
        //qDebug() << p;
    }
    file.close();

    TriangleAction triangulation(points);
    triangulation.run();
    triangles = triangulation.getTriangles();
    //qDebug() << triangles[0].point1.getX();

    isTriangleEnded = 1;
}

MainWindow::~MainWindow() {
    for (int i = 0; i < points.size(); ++i)
        delete points[i];
    for (int i = 0; i < triangles.size(); ++i)
        delete triangles[i];
    delete ui;
}

void MainWindow::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 5));
    for (int i = 0; i < points.size(); ++i) {
        painter.drawPoint(points[i]->getX(), points[i]->getY());
    }
    painter.setPen(QPen(Qt::black, 1));
    if (isTriangleEnded)
        for (int i = 0; i < triangles.size(); ++i) {
            //qDebug() << triangles[i].point1.getX();
            std::vector<Point*> pnts = triangles[i]->getPoints();
            painter.drawLine(pnts[0]->getX(),
                             pnts[0]->getY(),
                             pnts[1]->getX(),
                             pnts[1]->getY());
            painter.drawLine(pnts[1]->getX(),
                             pnts[1]->getY(),
                             pnts[2]->getX(),
                             pnts[2]->getY());
            painter.drawLine(pnts[0]->getX(),
                             pnts[0]->getY(),
                             pnts[2]->getX(),
                             pnts[2]->getY());
        }
}
