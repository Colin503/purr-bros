#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    this->mainScene = new MyScene;

    this->mainView = new QGraphicsView;
    this->mainView->setScene(mainScene);
    mainView->setSceneRect(mainScene->sceneRect());
//mainView->fitInView(mainScene->sceneRect(), Qt::KeepAspectRatio);

    this->setCentralWidget(mainView);
    this->setWindowTitle("My main window");
    this->resize(1600, 1080);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction* actionHelp = new QAction(tr("&About"), this);
    connect(actionHelp, SIGNAL(triggered()), this, SLOT(slot_aboutMenu()));
    helpMenu->addAction(actionHelp);

    QAction* restartAction = new QAction(tr("&Rejouer"), this);
    connect(restartAction, SIGNAL(triggered()), mainScene, SLOT(resetGame()));
    helpMenu->addAction(restartAction);

    //optimisation du redimensionnement
    mainView->setRenderHint(QPainter::SmoothPixmapTransform);
    mainView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    mainView->setSceneRect(mainScene->sceneRect());

    mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->setRenderHint(QPainter::Antialiasing);
    mainView->setCacheMode(QGraphicsView::CacheBackground);
    mainView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Timer pour le suivi
    followTimer = new QTimer(this);
    connect(followTimer, &QTimer::timeout, this, &MainWindow::followPlayer);
    followTimer->start(16);  // ~60 FPS

}

MainWindow::~MainWindow(){
    delete followTimer;
}

void MainWindow::slot_aboutMenu(){
    QMessageBox msgBox;
    msgBox.setText("purr-bros is a videogame platformer with Qt\n\ndev:@Colin503");
    msgBox.setModal(true);
    msgBox.exec();
}

void MainWindow::followPlayer() { //j'ai refais une fonction hors du update pour que le raffraichissment soit encore plus rapide (trop saccadé sinon)
    if (!mainScene || !mainScene->playerItem) return;

    // 1. Centre la vue sur le joueur
    QPointF playerPos = mainScene->playerItem->pos();
    mainView->centerOn(playerPos);

    // 2. Calcule la position relative du timer par rapport à la vue (pas au monde)
    QPointF viewTopLeft = mainView->mapToScene(QPoint(0, 0)); // Coin haut-gauche de la vue
    qreal timerX = viewTopLeft.x() + 20; // 20px du bord gauche
    qreal timerY = viewTopLeft.y() + 20; // 20px du bord haut

    // 3. Positionne les éléments UI en coordonnées absolues de la vue
    mainScene->timeText->setPos(timerX, timerY);
    mainScene->bestTimeText->setPos(timerX, timerY + 30); // 30px sous le premier texte


    QRectF textRect = mainScene->timeText->boundingRect();
}