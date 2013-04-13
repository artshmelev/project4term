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

    QFile file("data.dat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    int num;
    in >> num;
    std::vector<Point*> points;
    for (int i = 0; i < num && !in.atEnd(); ++i) {
        int p, q;
        in >> p >> q;
        points.push_back(new Point(p, q));
    }
    file.close();

    triangulation = new TriangleAction(points);
    scene = new QGraphicsScene;

    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow() {
    delete scene;
    delete triangulation;
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

void MainWindow::on_pushButton_clicked()
{
    triangulation->run();
    std::vector<Triangle*> triangles = triangulation->getTriangles();
    std::vector<Point*> points = triangulation->getPoints();

    QPen pointsPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QBrush pointsBrush(Qt::red, Qt::SolidPattern);
    for (int i = 0; i < points.size(); ++i)
        scene->addEllipse(points[i]->getX() - 2, points[i]->getY() - 2,
                          4, 4, pointsPen, pointsBrush);

    QPen edgesPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    for (int i = 0; i < triangles.size(); ++i) {
        std::vector<Point*> pnts = triangles[i]->getPoints();
        scene->addLine(pnts[0]->getX(), pnts[0]->getY(),
                       pnts[1]->getX(), pnts[1]->getY(), edgesPen);
        scene->addLine(pnts[1]->getX(), pnts[1]->getY(),
                       pnts[2]->getX(), pnts[2]->getY(), edgesPen);
        scene->addLine(pnts[0]->getX(), pnts[0]->getY(),
                       pnts[2]->getX(), pnts[2]->getY(), edgesPen);
    }
}
