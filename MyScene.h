#ifndef CPP_QT_TPMINICLAUDE_MYSCENE_H
#define CPP_QT_TPMINICLAUDE_MYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QSettings>
#include <QKeyEvent>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>
#include <cctype>
#include <cstring>
#include <map>
#include <utility>
#include <algorithm>
#include <numeric>
using namespace std;

class MyScene : public QGraphicsScene {
    Q_OBJECT

private:
    QTimer* timer;
    vector<QGraphicsRectItem*> tab_rect;
    QGraphicsTextItem* qgti;
    QGraphicsRectItem* qgri;
    QGraphicsRectItem* qgri2;
    QGraphicsRectItem* qgri1;
    QGraphicsPixmapItem* qgpi;
    QPixmap playerPixmap;  // Variable pour stocker l'image en mémoire
    QPixmap playerPixmapJump;  // Variable pour stocker l'image en mémoire
    QGraphicsTextItem* timeTextItem;
    QGraphicsTextItem* bestTimeTextItem;
    QTime gameTime;
    QTime bestTime;
    bool isGameRunning;
    QGraphicsPixmapItem* goalItem;

    int sensY;
    float velocityY;   // Vitesse verticale
    float gravity;     // Gravité
    float jumpForce;   // Force du saut
    bool onGround;     // État : au sol ou non
public:
    MyScene(QObject* parent = nullptr);
    virtual ~MyScene();

public slots:
    void resetGame();
    void updateTime();
    void update();

protected:
    void keyPressEvent(QKeyEvent* event);

};


#endif //CPP_QT_TPMINIPROJET_MYSCENE_H
