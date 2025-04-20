#include "MyScene.h"

MyScene::MyScene(QObject* parent) : QGraphicsScene(parent) {
    this->qgri = new QGraphicsRectItem(0, 0, 300, 200);
    qgri->setPos(0, 100);
    this->addItem(qgri);

    this->qgri1 = new QGraphicsRectItem(0, 0, 200, 100);
    qgri1->setPos(400, -100);
    this->addItem(qgri1);

    this->qgri2 = new QGraphicsRectItem(0, 0, 1000, 200);
    qgri2->setPos(800, 100);
    this->addItem(qgri2);

    goalItem = new QGraphicsPixmapItem(QPixmap("../img/file_99668.png"));
    goalItem->setPos(1500, -150); 
    this->addItem(goalItem);

    this->qgti = new QGraphicsTextItem("PURR BROS");
    this->addItem(qgti);

    playerPixmap = QPixmap("../img/file_99668.png");
    if (playerPixmap.isNull()) {
        qDebug() << "Erreur : impossible de charger l'image du joueur !";
    }

    playerPixmapJump = QPixmap("../img/file_99666.png");
    if (playerPixmapJump.isNull()) {
        qDebug() << "Erreur : impossible de charger l'image du joueur !";
    }
    this->qgpi = new QGraphicsPixmapItem(playerPixmap);
    qgpi->setPos(0, -100);
    this->addItem(qgpi);

    this->tab_rect.push_back(qgri);
    this->tab_rect.push_back(qgri1);
    this->tab_rect.push_back(qgri2);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(30); // Toutes les 30 millisecondes (~33 FPS)


    //Le saut a été fait grace à VoidRealms sur Youtube
    // Initialisation des variables de physique
    velocityY = 0;
    gravity = 0.5;    // Ajuste selon tes besoins
    jumpForce = -12;  // Négatif car l’axe Y va vers le haut
    onGround = false;

    // Initialisation du chronomètre
    timeTextItem = new QGraphicsTextItem("Temps: 00:00:00");
    timeTextItem->setPos(10, 10);
    this->addItem(timeTextItem);

    bestTimeTextItem = new QGraphicsTextItem("Meilleur temps: --:--:--");
    bestTimeTextItem->setPos(10, 40);
    this->addItem(bestTimeTextItem);

    gameTime = QTime(0, 0, 0);
    isGameRunning = true;

    // Charge le meilleur temps depuis QSettings
    QSettings settings;
    bestTime = settings.value("bestTime", QTime(0, 0, 0)).toTime();
    if (bestTime != QTime(0, 0, 0)) {
        bestTimeTextItem->setPlainText("Meilleur temps: " + bestTime.toString("hh:mm:ss"));
    }

    // Nouveau timer pour le chronomètre (déclenché toutes les secondes)
    QTimer* timeTimer = new QTimer(this);
    connect(timeTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timeTimer->start(1000);  // 1 seconde
}

MyScene::~MyScene() {
}

// Nouvelle méthode pour mettre à jour le chronomètre
void MyScene::updateTime() {
    if (isGameRunning) {
        gameTime = gameTime.addSecs(1);
        timeTextItem->setPlainText("Temps: " + gameTime.toString("hh:mm:ss"));
    }
}

void MyScene::update() {
    QPointF point = qgpi->pos();

    // Appliquer la gravité
    velocityY += gravity;
    qgpi->setPos(point.x(), point.y() + velocityY);

    // Vérifier les collisions avec les plateformes
    onGround = false; // Réinitialiser à chaque frame
    for (auto* platform : tab_rect) {
        if (qgpi->collidesWithItem(platform)) {
            QRectF playerRect = qgpi->boundingRect().translated(qgpi->pos());
            QRectF platformRect = platform->boundingRect().translated(platform->pos());

            // Si le joueur vient d’au-dessus (atterrissage)
            if (playerRect.bottom() >= platformRect.top() && velocityY > 0) {
                qgpi->setPos(point.x(), platformRect.top() - playerRect.height());
                velocityY = 0;
                onGround = true;
                //qDebug() << "Atterrissage sur plateforme !";
            }
        }
    }

    // Si aucune collision, le joueur est en l’air
    if (!onGround && velocityY != 0) {
        //qDebug() << "En l’air !";
    }
    // Exemple : si le joueur atteint un objectif (à adapter)
    if (qgpi->collidesWithItem(goalItem)) {  // Supposons que goalItem est un objet de fin
        if (bestTime.isNull() || gameTime < bestTime) {
            bestTime = gameTime;
            bestTimeTextItem->setPlainText("Meilleur temps: " + bestTime.toString("hh:mm:ss"));

            // Sauvegarde dans QSettings
            QSettings settings;
            settings.setValue("bestTime", bestTime);
        }
        isGameRunning = false;
        timer->stop();
    }
}

void MyScene::keyPressEvent(QKeyEvent* event) {
    QPointF point = qgpi->pos();

    if (event->key() == Qt::Key_P) { // Pause
        //qDebug() << "Touche P appuyée";
        if (timer->isActive()) timer->stop();
        else timer->start();
    }
    if ((event->key() == Qt::Key_Up || event->key() == Qt::Key_Z) && onGround) { // Saut
        velocityY = jumpForce; // Appliquer la force de saut
        onGround = false;
        //qDebug() << "Saut !";
    }
    if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) { // Droite
        qgpi->setPos(point.x() + 10, point.y());
    }
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Q) { // Gauche
        qgpi->setPos(point.x() - 10, point.y());
    }
    // TEST -- À SUPPRIMER APRÈS
    if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S) { // Bas
        qgpi->setPos(point.x(), point.y() + 10);
    }
    if (event->key() == Qt::Key_P) {
        isGameRunning = !isGameRunning;  // Inverse l'état du jeu
        if (timer->isActive()) timer->stop();
        else timer->start();
    }
}

void MyScene::resetGame() {
    // Réinitialise la position du joueur
    qgpi->setPos(0, -100);

    // Réinitialise le chronomètre
    gameTime = QTime(0, 0, 0);
    timeTextItem->setPlainText("Temps: 00:00:00");

    // Relance le jeu
    isGameRunning = true;
    timer->start(30);
    velocityY = 0;
    onGround = false;
}