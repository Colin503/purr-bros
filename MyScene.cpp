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
    //chargement des textures (images) pour éviter de trop faire d'accès disque
    textures.player.load("../assets/player/player.png");
    textures.playerJump.load("../assets/player/player_jump.png");
    textures.playerFinal.load("../assets/player/player_final.png");
    textures.background.load("../assets/environment/background.png");
    textures.platformC.load("../assets/environment/platformBig.png");
    textures.floor.load("../assets/environment/floor1.png");
    textures.goal.load("../assets/environment/goal.png");
    textures.coin.load("../assets/environment/coin1.png");
    setSceneRect(0, 0, textures.background.width(), textures.background.height());
}

void MyScene::createGameItems() {
    // Background
    QGraphicsPixmapItem* bgItem = new QGraphicsPixmapItem(textures.background); //init de l'arriere plan
    bgItem->setZValue(-1000);
    addItem(bgItem);

    // Joueur
    playerItem = new QGraphicsPixmapItem(textures.player); //initialisation du joueur
    playerItem->setPos(100, -1000);
    playerItem->setZValue(100);
    addItem(playerItem);

    platforms.clear(); //on vide tous les vecteurs
    floors.clear();
    coins.clear();

    floors.append(createFloor(0, 1200, 1500, 400));
    floors.append(createFloor(1600, 1200, 1500,400));
    floors.append(createFloor(3500, 1200, 1500, 400));


    platforms.append(createPlatform(700, 700, 681, 256));
    platforms.append(createPlatform(2200, 600, 681, 256));
    platforms.append(createPlatform(2700, 750, 681, 256));
    movingPlatform=createPlatform(3700, 700, 681, 256);
    platforms.append(movingPlatform);
    platforms.append(createPlatform(3200, 50, 681, 256));
    platforms.append(createPlatform(2900, 50, 681, 256));



    coins.append(createCoin(850,650));


    goalItem = new QGraphicsPixmapItem(textures.goal);
    goalItem->setPos(2950, -90);
    goalItem->setZValue(50);
    addItem(goalItem);

    timeText = new QGraphicsTextItem("Temps: 00:00:00");
    timeText->setDefaultTextColor(Qt::white);
    timeText->setPos(10, 10); //à changer dans le update()
    addItem(timeText);

    bestTimeText = new QGraphicsTextItem("Meilleur temps: --:--:--");
    bestTimeText->setDefaultTextColor(Qt::white);
    bestTimeText->setPos(10, 40); //à changer dans le update()
    addItem(bestTimeText);



}

void MyScene::initTimers() {
    // Timer de jeu
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyScene::update);
    timer->start(10);

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


    for (QGraphicsPixmapItem* floors : floors) {
        if (playerItem->collidesWithItem(floors)) {
            QRectF playerRect = playerItem->boundingRect().translated(playerItem->pos());
            QRectF floorRect = floors->boundingRect().translated(floors->pos());

            if (playerRect.bottom() > floorRect.top() && velocityY > 0) {
                playerItem->setPos(pos.x(), floorRect.top()-150);
                velocityY = 0;
                onGround = true;
                playerItem->setPixmap(textures.player); // Retour à l'image normale

            }
        }
    }
    if (movingPlatform) {
        QPointF platformPos = movingPlatform->pos();
        platformPos.setY(platformPos.y() + (3 * platformDirection));
        if (platformPos.y() >= 800) { //changement du sens
            platformPos.setY(800);
            platformDirection = -1;
        }
        else if (platformPos.y() <= 50) {
            platformPos.setY(50);
            platformDirection = 1;
        }
        movingPlatform->setPos(platformPos);
        if (playerItem->collidesWithItem(movingPlatform)) {
            QRectF playerRect = playerItem->boundingRect().translated(playerItem->pos());
            QRectF platformRect = movingPlatform->boundingRect().translated(movingPlatform->pos());
            if (playerRect.bottom() > platformRect.top() && velocityY > 0) {
                playerItem->setPos(playerItem->pos().x(), platformRect.top() - playerRect.height());
                playerItem->moveBy(0, 3 * platformDirection);
                velocityY = 0;
                onGround = true;
            }
        }
    }

    for (QGraphicsPixmapItem* coins : coins) {
        if (playerItem->collidesWithItem(coins)) {
            //qDebug()<<"Pieces touché";
            removeItem(coins);
            this->score_piece+=1;
        }

    }

    // Collision avec le coffre
    if (playerItem->collidesWithItem(goalItem)) {
        handleWinCondition();
        playerItem->setPixmap(textures.playerFinal);
        //ajouter un texte pour rejouer
    }
}


void MyScene::updateTime() {
    if (isGameRunning) {
        gameTime = gameTime.addSecs(1);
        QString timeString = "Time: " + gameTime.toString("mm:ss");
        timeText->setPlainText(timeString);
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
            movePlayer(pos.x() + 15, pos.y());
            break;

        case Qt::Key_Left:
        case Qt::Key_Q:
            movePlayer(pos.x() - 15, pos.y());
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
        playerItem->setPixmap(textures.playerJump);
    }
}

void MyScene::movePlayer(qreal x, qreal y) {
    playerItem->setPos(x, y);
}

void MyScene::resetGame() {
    //  joueur
    playerItem->setPos(0, -100);
    playerItem->setPixmap(textures.player);
    velocityY = 0;
    onGround = false;

    //  temps
    gameTime = QTime(0, 0, 0);
    timeText->setPlainText("Temps: 00:00:00");

    //  jeu
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
        SaveStats();
    }
}

QGraphicsPixmapItem* MyScene::createPlatform(int x, int y, int width, int height) {
    QGraphicsPixmapItem* platform = new QGraphicsPixmapItem(textures.platformC);
    platform->setPixmap(textures.platformC.scaled(width, height));
    platform->setPos(x, y);
    addItem(platform);
    return platform;
}
QGraphicsPixmapItem* MyScene::createFloor(int x, int y, int width, int height) {
    QGraphicsPixmapItem* floors = new QGraphicsPixmapItem(textures.floor);
    floors->setPixmap(textures.floor.scaled(width, height));
    floors->setPos(x, y);
    addItem(floors);
    return floors;
}

QGraphicsPixmapItem* MyScene::createCoin(int x, int y) {
    QGraphicsPixmapItem* coin = new QGraphicsPixmapItem(textures.coin);
    coin->setPixmap(textures.coin);
    coin->setPos(x, y);
    addItem(coin);
    return coin;
}

void MyScene::SaveStats() {
    QFile file("stats.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Impossible d'ouvrir le fichier en écriture";
        return;
    }
    QTextStream out(&file);
    out << "Meilleur temps: " << bestTime.toString("hh:mm:ss") << "\n";
    out << "Pièces: " << score_piece << "\n";
    file.close();

}