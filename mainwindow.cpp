#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QGraphicsTextItem>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "structures.h"
#include "mainaction.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file("test/data.dat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    int num;
    in >> num;
    std::vector<Point*> points;
    for (int i = 0; i < num && !in.atEnd(); ++i) {
        float p, q, t;
        in >> p >> q >> t;
        points.push_back(new Point(p, q, t));
    }
    file.close();

    mainAction = new MainAction(points);
    scene = new QGraphicsScene;

    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow() {
    delete scene;
    delete mainAction;
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
    mainAction->run();
    std::vector<Triangle*> triangles = mainAction->getTriangles();
    std::vector<Point*> points = mainAction->getPoints();
    std::vector<Edge*> isolineEdges = mainAction->getIsolineEdges();

    QPen pointsPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QBrush pointsBrush(Qt::red, Qt::SolidPattern);
    QFont simpleFont("Monospace", 12);
    for (int i = 0; i < points.size(); ++i) {
        scene->addEllipse(points[i]->getX() - 2, points[i]->getY() - 2,
                          4, 4, pointsPen, pointsBrush);
        QGraphicsTextItem *item =
                scene->addText(QString::number(points[i]->getT()), simpleFont);
        item->setPos(points[i]->getX() + 3, points[i]->getY() - 3);
    }

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

    QPen isolinePen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    for (int i = 0; i < isolineEdges.size(); ++i) {
        Point *point1 = isolineEdges[i]->getPoint1(),
              *point2 = isolineEdges[i]->getPoint2();
        scene->addLine(point1->getX(), point1->getY(),
                       point2->getX(), point2->getY(), isolinePen);
    }
}
