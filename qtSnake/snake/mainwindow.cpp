#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    for (int i = 0; i < ROOM_WIDTH; i++) {
        for (int j = 0; j < ROOM_HEIGHT; j++) {
            room[i][j] = 0;
            roomPoints[i][j] = QPoint(i, j);
        }
    }

    for (int i = 0; i < ROOM_WIDTH; i++) {
        room[0][i] = 1;
        room[ROOM_HEIGHT-1][i] = 1;
    }
    for (int i = 0; i < ROOM_HEIGHT; i++) {
        room[i][0] = 1;
        room[i][ROOM_WIDTH-1] = 1;
    }

    room[20][20] = 4;

    initSnake();

    connect(timer, &QTimer::timeout, this, &MainWindow::movePlayer);
    timer->start(speed);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    for (int i = 0; i < ROOM_WIDTH; i++) {
        for (int j = 0; j < ROOM_HEIGHT; j++) {
            QBrush brush;
            if (room[i][j] == 1) {// wall
                brush.setColor(QColor(0,255,255));
            }
            else if (room[i][j] == 2) {// snake head
                brush.setColor(QColor(255,0,0));
            }
            else if (room[i][j] == 3) {// snake
                brush.setColor(QColor(255,0,0));
            }
            else if (room[i][j] == 4) {// food
                brush.setColor(QColor(0,0,255));
            }
            else {// background
               brush.setColor(QColor(0,0,0));

            }

            painter.setPen(Qt::NoPen);
            brush.setStyle(Qt::SolidPattern);
            painter.setBrush(brush);
            painter.drawRect(25*roomPoints[i][j].x(), 25*roomPoints[i][j].y(), 25,25);
        }
    }

    QBrush brush;
    painter.setPen(Qt::NoPen);
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(snakeColor);
    painter.setBrush(brush);
    for(QPoint p : snake) {
        painter.drawRect(25*p.x(), 25*p.y(), 25,25);
    }
}

void MainWindow::movePlayer() {
    if (isPaused) return;
    QPoint newPos = snake[0];
    if (dir == 1) { // up
        newPos += QPoint(0,-1);
    } else if (dir == 2){ // left
        newPos += QPoint(-1,0);
    } else if (dir == 3){ // down
        newPos += QPoint(0,1);
    } else if (dir == 4){ // right
        newPos += QPoint(1,0);
    }

    for (QPoint p : snake) {
        if (p == newPos&&dir!=0) {
            endRun();
            return;
        }
    }

    if (room[newPos.x()][newPos.y()] == 1) {
        endRun();
    }
    else if (room[newPos.x()][newPos.y()] == 4) {
        snakeLen++;
        if (speed > 100) {
            speed -= 100;
            timer->stop();
            timer->start(speed);
        }
        snakeEat(newPos);
        room[newPos.x()][newPos.y()] = 0;
        room[QRandomGenerator::global()->bounded(28)+2][(QRandomGenerator::global()->bounded(28))+2] = 4;
    }
    else {
        snakeMove(newPos);
    }

    this->update();
    return;
}

void MainWindow::snakeMove(QPoint newPos) {
    QPoint tmp[ROOM_HEIGHT*ROOM_WIDTH];

    for (int i = 0; i < ROOM_HEIGHT*ROOM_WIDTH; i++) {
        tmp[i] = snake[i];
    }

    for (int i = 0; i < snakeLen-1; i++) {
        if (snake[i+1] == QPoint(-1,-1)) {
            break;
        }
        tmp[i+1] = snake[i];
    }
    tmp[0] = newPos;
    for (int i = 0; i < ROOM_WIDTH*ROOM_HEIGHT; i++) {
        snake[i] = tmp[i];
    }
}

void MainWindow::snakeEat(QPoint newPos) {
    QPoint tmp[ROOM_HEIGHT*ROOM_WIDTH];

    for (int i = 0; i < ROOM_HEIGHT*ROOM_WIDTH; i++) {
        tmp[i] = snake[i];
    }

    for (int i = 0; i < snakeLen-1; i++) {
        if (snake[i] == QPoint(-1,-1)) {
            break;
        }
        tmp[i+1] = snake[i];
    }
    tmp[0] = newPos;
    for (int i = 0; i < ROOM_WIDTH*ROOM_HEIGHT; i++) {
        snake[i] = tmp[i];
    }
    qDebug() << snakeLen;


}

void MainWindow::keyPressEvent(QKeyEvent *event) {


    if (event->key() == 87) { // w
        dir = 1;
    }
    else if (event->key() == 65) { // a
        dir = 2;
    }
    else if (event->key() == 83) { // s
        dir = 3;
    }
   else if (event->key() == 68) { // d
        dir = 4;
    }
    else if (event->key() == Qt::Key_Space) { // spacebar
        isPaused = !isPaused;
    }
    else if (event->key() == Qt::Key_R) { // r
        restartRun();
    }
    else {

    }
}

void MainWindow::setDifficulty(int diff) {
    if (diff !=0 ) {
        difficulty = diff;
    }
    speed = difficulty;
}

void MainWindow::endRun() {
    timer->stop();
    if (QMessageBox::question(this,"You died", "Your length was: "+QString::number(snakeLen)+"\nWould you like to restart?") == QMessageBox::Yes) {
        restartRun();
    } else {
        qApp->quit();
    }
}

void MainWindow::initSnake() {
    for (int i = 0; i < ROOM_HEIGHT*ROOM_WIDTH; i++) {
        snake[i] = QPoint(-1,-1);
    }

    snake[0] = QPoint(QRandomGenerator::global()->bounded(28)+2,QRandomGenerator::global()->bounded(28)+2);
    snake[1] = snake[0]+=QPoint(1,0);
    snakeLen = 2;
    dir = 0;
    speed = difficulty;
    timer->stop();
    timer->start(speed);
    this->update();
}

void MainWindow::restartRun() {
    initSnake();
}

void MainWindow::setSnakeColor(QColor color) {
    snakeColor = color;
}

void MainWindow::setSnakeColor(int red, int green, int blue) {
    snakeColor = QColor(red, green, blue);
}
