#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include <QTcpSocket>
#include <QHostAddress>

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

    const int ROOM_WIDTH = 100; // must be factor of 800
    const int ROOM_HEIGHT = 100; // must be the same as width
    QPoint roomPoints[100][100] = {};
    int tileScale;

    int room[100][100] = {};
    QPoint snake[100*100] = {};
    int snakeLen = 2;
    QColor snakeColors[12] = { // MAX_CLIENTS
        QColor(128,0,0), QColor(255,0,0), QColor(240,128,128), QColor(255,69,0),
        QColor(255,140,0), QColor(255,255,0), QColor(107,142,35), QColor(0,128,0),
        QColor(0,250,154), QColor(32,178,170), QColor(0,255,255), QColor(0,191,255)};/*,
        QColor(30,144,255), QColor(138,43,226), QColor(123,104,238), QColor(255,0,255) };*/

    int dir = 0;
    int speed = 300;
    int difficulty = 300;
    bool isPaused = false;

    // multiplayer setup

    QPoint altSnake[12][100*100] = {}; // MAX_CLIENTS
    QPoint foodPos[12] = {}; // MAX_CLIENTS
    const int MAX_CLIENTS = 12;

    QPoint noSnake = QPoint(-1,-1); // spot to put snake null
    QPoint noFood = QPoint(-1,-2); // spot to put food null

    QTcpSocket clientSocket;
    QDataStream clientStream;

    QTimer *multTimer = new QTimer(this);

    void connectToServer(const QHostAddress &address, quint16 port);
    void readData();
    void setId();
    void sendData();
    void initRoom();
    void genNewFood();
    bool snakeEatFood(QPoint newPos);

    int socketId = -1;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
