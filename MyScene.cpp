#include "MyScene.h"


MyScene::MyScene(QObject* parent) : QGraphicsScene(parent),
                                    velocityY(0), gravity(0.5f), jumpForce(-20.0f), onGround(false), isGameRunning(true) {

    // Initialisation des composants
    loadTextures();
    createGameItems();
    initTimers();
    loadBestTime();
}

void MyScene::loadTextures() {
    // Joueur
    if (!textures.player.load("../assets/player/player.png")) {
        qDebug() << "Failed to load player texture";
        textures.player = QPixmap(50, 50);
        textures.player.fill(Qt::blue);
    }

    if (!textures.playerJump.load("../assets/player/player_jump.png")) {
        textures.playerJump = textures.player;
    }

    textures.background.load("../assets/environment/background.png");

    // Plateformes
    if (!textures.platformC.load("../assets/environment/platformBig.png")) {
        qDebug() << "ERREUR: Impossible de charger platform_center.png";
        qDebug() << "Chemin essayé:" << QFileInfo("../assets/environment/platformBig.png").absoluteFilePath();
        qDebug() << "Le fichier existe?" << QFile::exists("../assets/environment/platformBig.png");

        // Crée une plateforme de repli (visible en rouge)
        textures.platformC = QPixmap(300, 30);
        textures.platformC.fill(Qt::red);
    }

    // Goal
    textures.goal.load("../assets/environment/goal.png");
    textures.coin.load("../assets/environment/coin.png");

    setSceneRect(0, 0, textures.background.width(), textures.background.height());
}

void MyScene::createGameItems() {
    // Background
    QGraphicsPixmapItem* bgItem = new QGraphicsPixmapItem(textures.background);
    bgItem->setZValue(-1000);
    addItem(bgItem);

    // Joueur
    playerItem = new QGraphicsPixmapItem(textures.player);
    playerItem->setPos(100, -1000);
    playerItem->setZValue(100);
    addItem(playerItem);

    // Plateformes
    platforms.clear();
    // Plateforme de départ (large)
    platforms.append(createPlatform(0, 1200, 681, 256));

// Plateformes intermédiaires (en escalier)
    platforms.append(createPlatform(900, 1100, 681, 256));
    platforms.append(createPlatform(1300, 900, 681, 256));
    platforms.append(createPlatform(1700, 700, 681, 256));

// Grand plateau central
    platforms.append(createPlatform(2200, 800, 681, 256));

// Plateforme finale (avant le goal)
    platforms.append(createPlatform(3500, 600, 681, 256));

    // Goal
    goalItem = new QGraphicsPixmapItem(textures.goal);
    goalItem->setPos(1500, -150);
    goalItem->setZValue(50);
    addItem(goalItem);

    //Coins
    coins.clear();
    coins.append(createCoin(200,1000));

    // UI
    timeText = new QGraphicsTextItem("Temps: 00:00:00");
    timeText->setDefaultTextColor(Qt::white);
    timeText->setPos(10, 10);
    addItem(timeText);

    bestTimeText = new QGraphicsTextItem("Meilleur temps: --:--:--");
    bestTimeText->setDefaultTextColor(Qt::white);
    bestTimeText->setPos(10, 40);
    addItem(bestTimeText);
}

void MyScene::initTimers() {
    // Timer de jeu
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyScene::update);
    timer->start(30);

    // Timer du chrono
    QTimer* timeTimer = new QTimer(this);
    connect(timeTimer, &QTimer::timeout, this, &MyScene::updateTime);
    timeTimer->start(1000);
}

void MyScene::loadBestTime() {
    QSettings settings;
    bestTime = settings.value("bestTime").toTime();

    if (!bestTime.isNull()) {
        bestTimeText->setPlainText("Meilleur temps: " + bestTime.toString("hh:mm:ss"));
    }

    gameTime = QTime(0, 0, 0);
}

void MyScene::update() {
    if (!isGameRunning) return;

    // Physique du joueur
    QPointF pos = playerItem->pos();
    velocityY += gravity;
    playerItem->setPos(pos.x(), pos.y() + velocityY);

    // Collisions avec les plateformes
    onGround = false;
    for (QGraphicsPixmapItem* platform : platforms) {
        if (playerItem->collidesWithItem(platform)) {
            QRectF playerRect = playerItem->boundingRect().translated(playerItem->pos());
            QRectF platformRect = platform->boundingRect().translated(platform->pos());

            if (playerRect.bottom() > platformRect.top() && velocityY > 0) {
                playerItem->setPos(pos.x(), platformRect.top()-55);
                velocityY = 0;
                onGround = true;
                playerItem->setPixmap(textures.player); // Retour à l'image normale

            }
        }
    }

    // Collision avec le goal
    if (playerItem->collidesWithItem(goalItem)) {
        handleWinCondition();
    }
}


void MyScene::updateTime() {
    if (isGameRunning) {
        gameTime = gameTime.addSecs(1);
        timeText->setPlainText("Temps: " + gameTime.toString("hh:mm:ss"));
    }
}

void MyScene::keyPressEvent(QKeyEvent* event) {
    QPointF pos = playerItem->pos();

    switch (event->key()) {
        case Qt::Key_P:
            togglePause();
            break;

        case Qt::Key_Up:
        case Qt::Key_Z:
            jump();
            break;

        case Qt::Key_Right:
        case Qt::Key_D:
            movePlayer(pos.x() + 10, pos.y());
            break;

        case Qt::Key_Left:
        case Qt::Key_Q:
            movePlayer(pos.x() - 10, pos.y());
            break;

        case Qt::Key_R:
            resetGame();
            break;
    }
}

void MyScene::togglePause() {
    isGameRunning = !isGameRunning;
    if (timer->isActive()) {
        timer->stop();
    } else {
        timer->start();
    }
}

void MyScene::jump() {
    if (onGround) {
        velocityY = jumpForce;
        onGround = false;
        playerItem->setPixmap(textures.playerJump); // Image de saut
    }
}

void MyScene::movePlayer(qreal x, qreal y) {
    playerItem->setPos(x, y);
}

void MyScene::resetGame() {
    // Réinitialisation du joueur
    playerItem->setPos(0, -100);
    playerItem->setPixmap(textures.player);
    velocityY = 0;
    onGround = false;

    // Réinitialisation du temps
    gameTime = QTime(0, 0, 0);
    timeText->setPlainText("Temps: 00:00:00");

    // Redémarrage du jeu
    isGameRunning = true;
    timer->start();
}

void MyScene::drawBackground(QPainter* painter, const QRectF& rect) {
    Q_UNUSED(rect);
    painter->drawPixmap(sceneRect(), textures.background, textures.background.rect());
}

MyScene::~MyScene() {
    delete timer;
}

void MyScene::handleWinCondition() {
    isGameRunning = false;
    timer->stop();

    if (bestTime.isNull() || gameTime < bestTime) {
        bestTime = gameTime;
        bestTimeText->setPlainText("Meilleur temps: " + bestTime.toString("hh:mm:ss"));

        QSettings settings;
        settings.setValue("bestTime", bestTime);
    }
}

QGraphicsPixmapItem* MyScene::createPlatform(int x, int y, int width, int height) {
    QGraphicsPixmapItem* platform = new QGraphicsPixmapItem(textures.platformC);
    platform->setPixmap(textures.platformC.scaled(width, height));
    platform->setPos(x, y);
    addItem(platform);
    return platform;
}

QGraphicsPixmapItem* MyScene::createCoin(int x, int y) {
    QGraphicsPixmapItem* coin = new QGraphicsPixmapItem(textures.coin);
    coin->setPixmap(textures.coin);
    coin->setPos(x, y);
    addItem(coin);
    return coin;


}
