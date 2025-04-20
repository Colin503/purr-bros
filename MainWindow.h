#ifndef CPP_QT_TPMINICLAUDE_MAINWINDOW_H
#define CPP_QT_TPMINICLAUDE_MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
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

#include "MyScene.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private :
    MyScene* mainScene;
    QGraphicsView* mainView;
    QMenu* helpMenu;


public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

public slots:
    void slot_aboutMenu();
};


#endif //CPP_QT_TPMINIPROJET_MAINWINDOW_H
