
#ifndef CPP_QT_TPMINICLAUDE_MYSCENE_H
#define CPP_QT_TPMINICLAUDE_MYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QTime>
#include <QSettings>
#include <QPixmap>
#include <QVector>
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QFileInfo>
#include <QGraphicsView>
#include <QFile>
#include <QTextStream>



class MyScene : public QGraphicsScene {
Q_OBJECT

private:
    QTimer* timer;
    float velocityY;
    float gravity;
    float jumpForce;
    bool onGround;
    bool isGameRunning;
    int score_piece=0;
    int platformDirection = 1;
    QGraphicsPixmapItem* movingPlatform = nullptr;



    // Textures
    struct Textures {
        QPixmap player;
        QPixmap playerJump;
        QPixmap playerFinal;
        QPixmap platformC;
        QPixmap background;
        QPixmap goal;
        QPixmap coin;
        QPixmap floor;
    } textures;


    void handleWinCondition();
    void togglePause();
    void jump();
    void movePlayer(qreal x, qreal y);
    void SaveStats();
    void SaveBestStats()

public:
    MyScene(QObject* parent = nullptr);
    ~MyScene() override;

    void loadTextures();
    void createGameItems();
    void initTimers();
    void loadBestTime();
    QGraphicsPixmapItem* createPlatform(int x, int y, int width, int height);
    QGraphicsPixmapItem* createFloor(int x, int y, int width, int height);
    QGraphicsPixmapItem* createCoin(int x, int y);

    QGraphicsPixmapItem* playerItem;
    QGraphicsPixmapItem* goalItem;
    QVector<QGraphicsPixmapItem*> platforms;
    QVector<QGraphicsPixmapItem*> coins;
    QVector<QGraphicsPixmapItem*> floors;
    QGraphicsTextItem* timeText;
    QGraphicsTextItem* bestTimeText;
    QTime gameTime;
    QTime bestTime;

public slots:
    void resetGame();
    void updateTime();
    void update();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;
};

#endif
