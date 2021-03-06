#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QDebug>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include <QUdpSocket>
#include <QHostAddress>
#include <QPushButton>
#include <QLabel>
#include <QSlider>

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
    void pauseMenu();
    void continueFromPause();
    void exitFromPause();
    void restartFromPause();
    void startMenu();
    void sliderLogic();

    QFrame *pauseScreen;
    QFrame *startScreen;
    QSlider *diff;
    QLabel *diffMessage;

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
    bool startup = true;

    // multiplayer setup

    QPoint altSnake[12][100*100] = {}; // MAX_CLIENTS
    QPoint foodPos[12] = {}; // MAX_CLIENTS
    bool foodMap[12] = {}; // MAX_CLIENTS
    const int MAX_CLIENTS = 12;

    QPoint noSnake = QPoint(-1,-1); // spot to put snake null
    QPoint noFood = QPoint(-1,-2); // spot to put food null

    QUdpSocket clientSocket;
//    QDataStream clientStream;

    QTimer *multTimer = new QTimer(this);

    void connectToServer(const QHostAddress &address, quint16 port);
    void readData();
    void setId();
    void sendData();
    void initRoom();
    void genNewFood();
    bool snakeEatFood(QPoint newPos);
    void onSocketError(QAbstractSocket::SocketError socketError);

    int socketId = -1;
    QHostAddress serverAddress = QHostAddress("3.139.31.84");
    int serverPort = 8005;
    QHostAddress clientAddress = QHostAddress("127.0.0.1");
    int clientPort = 8006+int(QRandomGenerator::global()->bounded(12));

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
