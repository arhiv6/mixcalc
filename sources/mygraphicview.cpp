#include "mygraphicview.h"
#include "myrect.h"
#include <QDebug>
#include <QtMath>

MyGraphicView::MyGraphicView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(parent)
{
    _min=0;
    _max=0;

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтале
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по ввертикале
    this->setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    this->setMinimumHeight(100);
    this->setMinimumWidth(100);
    this->setScene(scene);          // Устанавливаем сцену в виджет
    this->setMouseTracking(true);

//    timer = new QTimer();
//    timer->setInterval(100);
//    timer->setSingleShot(true);
//    connect(timer, SIGNAL(timeout()), this, SLOT(resezeSceneToView()));
    //  connect(scene, SIGNAL(sceneRectChanged(QRectF)), timer, SLOT(start()));
}

MyGraphicView::~MyGraphicView()
{

}

void MyGraphicView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);  // Запускаем событие родителького класса
  //  timer->start();
  //  QTimer::singleShot(100,this,SLOT(resezeSceneToView()));

resezeSceneToView();
}

void MyGraphicView::resezeSceneToView()
{

//    for(int j=0;j<2;j++)
//{
   // qDebug() << "R";

    // 1. cut free space on scene
    //qreal top = 0, bottom = 0, left = 0, right = 0;

    qreal top = 0,bottom = 0;
    for (int i = 0; i < scene()->items().size(); i++)
    {
        if (scene()->items().at(i)->boundingRect().top() < top)
        {
            top = scene()->items().at(i)->boundingRect().top();
        }
        if (scene()->items().at(i)->boundingRect().bottom() > bottom)
        {
            bottom = scene()->items().at(i)->boundingRect().bottom();
        }
//        if (scene()->items().at(i)->boundingRect().left() < left)
//        {
//            left = scene()->items().at(i)->boundingRect().left();
//        }
//        if (scene()->items().at(i)->boundingRect().right() > right)
//        {
//            right = scene()->items().at(i)->boundingRect().right();
//        }
    }
    //QSizeF realSize(right - left, bottom - top);

        scene()->setSceneRect(_min, 0, _max, bottom - top-1);

    //    qDebug() << "t" <<top<< "b" <<bottom;

    //scene()->setSceneRect(left, top, realSize.width(), realSize.height());
//    scene()->setSceneRect(_min, 0, _max, realSize.height()-1);
    //scene()->setSceneRect(0, 0, realSize.width(), realSize.height());

    // scale scene
    this->fitInView(scene()->sceneRect(), Qt::IgnoreAspectRatio);
    //}
}

void MyGraphicView::run(float min, float max)
{
    _min=min;
    _max=max;
    resezeSceneToView();
  //  timer->start();
}
