#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    tileScale = 800/ROOM_WIDTH;
    // initialize room to 0
    for (int i = 0; i < ROOM_WIDTH; i++) {
        for (int j = 0; j < ROOM_HEIGHT; j++) {
            room[i][j] = 0;
            roomPoints[i][j] = QPoint(i, j);
        }
    }
    // create top and bottom walls
    for (int i = 0; i < ROOM_WIDTH; i++) {
        room[0][i] = 1;
        room[ROOM_HEIGHT-1][i] = 1;
    }
    // create left and right walls
    for (int i = 0; i < ROOM_HEIGHT; i++) {
        room[i][0] = 1;
        room[i][ROOM_WIDTH-1] = 1;
    }

    // intialize food
    room[20][20] = 4;

    // initialize snake
    initSnake();

    // connect timer timeout to player move
    connect(timer, &QTimer::timeout, this, &MainWindow::movePlayer);
    // start the timer
    timer->start(speed);

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // draw the entire room
    for (int i = 0; i < ROOM_WIDTH; i++) {
        for (int j = 0; j < ROOM_HEIGHT; j++) {
            QBrush brush;
            if (room[i][j] == 1) {// wall
                brush.setColor(QColor(0,255,255));
            }
            // don't think this does anything, the value is never set to 2
            else if (room[i][j] == 2) {// snake head
                brush.setColor(QColor(255,0,0));
            }
            // don't think this does anything, the value is never set to 3
            else if (room[i][j] == 3) {// snake
                brush.setColor(QColor(255,0,0));
            }
            else if (room[i][j] == 4) {// food
                brush.setColor(QColor(0,0,255));
            }
            else {// background
               brush.setColor(QColor(0,0,0));

            }

            // actually draw the tiles
            painter.setPen(Qt::NoPen);
            brush.setStyle(Qt::SolidPattern);
            painter.setBrush(brush);
            painter.drawRect(tileScale*roomPoints[i][j].x(), tileScale*roomPoints[i][j].y(), tileScale,tileScale);
        }
    }

    // set up snake pen
    QBrush brush;
    painter.setPen(Qt::NoPen);
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(snakeColor);
    painter.setBrush(brush);
    // draw the snake on top of the room
    for(QPoint p : snake) {
        painter.drawRect(tileScale*p.x(), tileScale*p.y(), tileScale,tileScale);
    }
}

void MainWindow::movePlayer() {
    // if the game is paused, don't move player
    if (isPaused) return;

    // this block sets the new position for the snake head based on the direction
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

    // check if snake hits self
    for (QPoint p : snake) {
        if (p == newPos&&dir!=0) {
            endRun();
            return;
        }
    }

    // if snake hits wall
    if (room[newPos.x()][newPos.y()] == 1) {
        endRun();
    }
    // if snake eats food
    else if (room[newPos.x()][newPos.y()] == 4) {
        snakeLen++;
        if (speed > 100) {
            speed -= 100;
            timer->stop();
            timer->start(speed);
        }
        snakeEat(newPos);
        room[newPos.x()][newPos.y()] = 0;
        room[QRandomGenerator::global()->bounded(ROOM_WIDTH-4)+2][(QRandomGenerator::global()->bounded(ROOM_HEIGHT-4))+2] = 4;
    }
    else {
        snakeMove(newPos);
    }

    this->update();
    return;
}

void MainWindow::snakeMove(QPoint newPos) {
    QPoint tmp[ROOM_HEIGHT*ROOM_WIDTH];

    //  copy the snake to the temporary snake
    for (int i = 0; i < ROOM_HEIGHT*ROOM_WIDTH; i++) {
        tmp[i] = snake[i];
    }

    // move the entire snake back one index in the array, DONT move the last snake segment (otherwise it would grow)
    for (int i = 0; i < snakeLen-1; i++) {
        if (snake[i+1] == QPoint(-1,-1)) {
            break;
        }
        tmp[i+1] = snake[i];
    }
    // set the head of the snake to the new position
    tmp[0] = newPos;

    // copy the edited temporary snake back to the actual snake
    for (int i = 0; i < ROOM_WIDTH*ROOM_HEIGHT; i++) {
        snake[i] = tmp[i];
    }
}

void MainWindow::snakeEat(QPoint newPos) {
    QPoint tmp[ROOM_HEIGHT*ROOM_WIDTH];

    // copy the snake to the temporary snake
    for (int i = 0; i < ROOM_HEIGHT*ROOM_WIDTH; i++) {
        tmp[i] = snake[i];
    }

    // move the entire snake back one index in the array
    for (int i = 0; i < snakeLen-1; i++) {
        if (snake[i] == QPoint(-1,-1)) {
            break;
        }
        tmp[i+1] = snake[i];
    }
    // set the snake head to the new position
    tmp[0] = newPos;

    // copy the temporary snake back to the actual snake
    for (int i = 0; i < ROOM_WIDTH*ROOM_HEIGHT; i++) {
        snake[i] = tmp[i];
    }
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
    // sets the difficulty and snake speed (both are the same at the start)
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
    // intiializes the snake

    // set the entire snake to be off the screen
    for (int i = 0; i < ROOM_HEIGHT*ROOM_WIDTH; i++) {
        snake[i] = QPoint(-1,-1);
    }

    // sets the snake to a random position
    snake[0] = QPoint(QRandomGenerator::global()->bounded(ROOM_WIDTH-4)+2,QRandomGenerator::global()->bounded(ROOM_HEIGHT-4)+2);
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
