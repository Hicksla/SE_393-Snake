#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

    QTimer *timer = new QTimer(this);

    // functions to control the game
    void movePlayer();
    void snakeMove(QPoint newPos);
    void snakeEat(QPoint newPos);
    void setDifficulty(int diff);
    void endRun();
    void initSnake();
    void restartRun();
    void setSnakeColor(QColor color);
    void setSnakeColor(int red, int green, int blue);

    // game room co-ordinates

    const int ROOM_WIDTH = 32;
    const int ROOM_HEIGHT = 32;
    QPoint roomPoints[32][32] = {};

    int room[32][32] = {};
    QPoint snake[32*32] = {};
    int snakeLen = 2;
    QColor snakeColor = QColor(0,255,0);

    int dir = 0;
    int speed = 300;
    int difficulty = 300;
    bool isPaused = false;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
