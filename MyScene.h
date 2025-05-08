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



class MyScene : public QGraphicsScene {
Q_OBJECT

private:
    // Timer et physique
    QTimer* timer;
    float velocityY;
    float gravity;
    float jumpForce;
    bool onGround;
    bool isGameRunning;

    // Éléments de jeu
    QGraphicsPixmapItem* playerItem;
    QGraphicsPixmapItem* goalItem;
    QVector<QGraphicsPixmapItem*> platforms;
    QVector<QGraphicsPixmapItem*> coins;

    // Textures
    struct Textures {
        QPixmap player;
        QPixmap playerJump;
        QPixmap platformC;
        QPixmap background;
        QPixmap goal;
        QPixmap coin;
    } textures;

    // UI
    QGraphicsTextItem* timeText;
    QGraphicsTextItem* bestTimeText;
    QTime gameTime;
    QTime bestTime;

    void handleWinCondition();
    void togglePause();
    void jump();
    void movePlayer(qreal x, qreal y);

public:
    MyScene(QObject* parent = nullptr);
    ~MyScene() override;

    void loadTextures();
    void createGameItems();
    void initTimers();
    void loadBestTime();
    QGraphicsPixmapItem* createPlatform(int x, int y, int width, int height);
    QGraphicsPixmapItem* createCoin(int x, int y);

public slots:
    void resetGame();
    void updateTime();
    void update();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;
};

#endif