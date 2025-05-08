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

}

MainWindow::~MainWindow(){

}

void MainWindow::slot_aboutMenu(){
    QMessageBox msgBox;
    msgBox.setText("A small QT/C++ projet...");
    msgBox.setModal(true); // on souhaite que la fenetre soit modale i.e qu'on ne puisse plus cliquer ailleurs
    msgBox.exec();
}