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

    QFile file("test/data1.dat");
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

    mainAction->run();
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
    //mainAction->run();
    std::vector<Triangle*> triangles = mainAction->getTriangles();
    std::vector<Point*> points = mainAction->getPoints();
    std::vector<Edge*> isolineEdges = mainAction->getIsolineEdges();

    scene->clear();
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

    if (ui->radioButton->isChecked() || ui->radioButton_3->isChecked()) {
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

    if (ui->radioButton_2->isChecked() || ui->radioButton_3->isChecked()) {
        std::vector<QColor> colors{QColor(138, 43, 226), QColor(0, 0, 255),
                                   QColor(60, 179, 113), QColor(0, 255, 0),
                                   QColor(255, 215, 0)};
        for (int i = 0; i < isolineEdges.size(); ++i) {
            Point *point1 = isolineEdges[i]->getPoint1(),
                  *point2 = isolineEdges[i]->getPoint2();
            int numberColor = 0, temp = point1->getT();
            if (temp < 15)
                numberColor = 1;
            else if (temp == 15)
                numberColor = 3;
            else
                numberColor = 4;
            QPen isolinePen(colors[numberColor], 2, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin);
            scene->addLine(point1->getX(), point1->getY(),
                           point2->getX(), point2->getY(), isolinePen);
        }
    }
}
