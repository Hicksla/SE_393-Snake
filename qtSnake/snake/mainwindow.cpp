#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // multiplayer
//    connectToServer(QHostAddress("ordos.xorsoftworks.com"), 8000);
    connectToServer(QHostAddress("3.139.31.84"), 8000);

    // timer to send and read tcp data
    connect(multTimer, &QTimer::timeout, this, &MainWindow::readData);
    connect(multTimer, &QTimer::timeout, this, &MainWindow::sendData);

    // start the timer
    multTimer->start(1000);

    // end mutliplayer

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

    // initialize food positions
    for (int i = 0; i < MAX_CLIENTS; i++) {
        foodPos[i] = QPoint(-1,-2);
    }

    // initialize snake
    initSnake();

    // connect timer timeout to player move
    connect(timer, &QTimer::timeout, this, &MainWindow::movePlayer);
    // start the timer
    timer->start(speed);

    //set up the mainwindow after we're done with the start menu
    //ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event) {
    // if the game hasn't started, don't draw anything
    static bool startOpen =false;
    if (startup)
    {
        if(!startOpen) {startMenu();startOpen = true;}
        return;
    }
    else
    {
        if(startOpen) {ui->setupUi(this); startOpen =false;}
    }

    QPainter painter(this);

    // draw the entire room
    for (int i = 0; i < ROOM_WIDTH; i++) {
        for (int j = 0; j < ROOM_HEIGHT; j++) {
            QBrush brush;
            if (room[i][j] == 1) {// wall (draw same color as snake so it is clear which one is you)
                brush.setColor(((socketId==-1)?QColor(0,255,0):snakeColors[socketId]));
            }
            else {// background
               brush.setColor(QColor(0,0,0));
            }

            // actually draw the tiles
            painter.setPen(Qt::NoPen);
            brush.setStyle(Qt::SolidPattern);
            painter.setBrush(brush);
            painter.drawRect(tileScale*roomPoints[i][j].x(), tileScale*roomPoints[i][j].y(), tileScale,tileScale); // don't think this needs to be roomPoints
        }
    }

    /*
    // if the game hasn't started, don't draw anything
    static bool startOpen =false;
    if (startup)
    {
        if(!startOpen) {startMenu();startOpen = true;}
        return;
    }
    */

    // set up snake pen
    QBrush brush;
    painter.setPen(Qt::NoPen);
    brush.setStyle(Qt::SolidPattern);
    // this sets the color to green if client isn't connected yet
    brush.setColor(((socketId==-1)?QColor(0,255,0):snakeColors[socketId]));
    painter.setBrush(brush);
    // draw the snake on top of the room
    for(QPoint p : snake) {
        if (p == noSnake) break; // the snake is terminated by noSnake
        painter.drawRect(tileScale*p.x(), tileScale*p.y(), tileScale,tileScale);
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        // paint the food
        brush.setColor(QColor(0,0,255));
        painter.setBrush(brush);
        painter.drawRect(tileScale*foodPos[i].x(), tileScale*foodPos[i].y(), tileScale, tileScale);

        //set up altSnake pen
        brush.setColor(snakeColors[i]);
        painter.setBrush(brush);
        // draw the altSnake on top of the room
        for(QPoint p : altSnake[i]) {
            if (p == noSnake) break; // the snake is terminated by noSnake
            painter.drawRect(tileScale*p.x(), tileScale*p.y(), tileScale,tileScale);
        }
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
        if (p == noSnake) break; // the snake is terminated by noSnake
        if (p == newPos&&dir!=0) {
            endRun();
            return;
        }
    }

    // check if snake hits other snakes or other foods
    for (int i = 0; i < MAX_CLIENTS; i++) {
        for (QPoint p : altSnake[i]) {
            if (p == noSnake) break;
            if (p==newPos) {
                endRun();
                return;
            }
            // each client is in charge of changing its food tile
            if (p == foodPos[socketId]) {
                genNewFood();
                qDebug() << "alt eat";
            }
        }
    }

    // if snake hits wall
    if (room[newPos.x()][newPos.y()] == 1) {
        endRun();
    }
    // if snake eats food
    else if (snakeEatFood(newPos)) {
        snakeLen++;
        if (speed > 100) {
            speed -= 100;
            timer->stop();
            timer->start(speed);
        }
        snakeEat(newPos);
        if (newPos == foodPos[socketId]) genNewFood();
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

    //if we haven't left the start menu, button presses will only cause problems
    if(startup) {return;}

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
        pauseMenu();
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
    for (int i = 0; i < ROOM_HEIGHT*ROOM_WIDTH; i++) {
        if (snake[i] == noSnake) break; // the snake is terminated by noSnake
        snake[i] = noSnake;
    }
    foodPos[socketId] = noFood;
    sendData();
    timer->stop();
    if (QMessageBox::question(this,"You died", "Your length was: "+QString::number(snakeLen)+"\nWould you like to restart?") == QMessageBox::Yes) {
        restartRun();
    } else {

        this->update();
        QCoreApplication::quit();
    }
}

void MainWindow::initSnake() {
    // intiializes the snake

    // set the entire snake to be off the screen
    for (int i = 0; i < ROOM_HEIGHT*ROOM_WIDTH; i++) {
        // might not be able terminate at noSnake here because the snake may grow extra length when eating
        // that may be prevented by snakeLen though
        snake[i] = noSnake;
    }

    // sets the snake to a random position
    snake[0] = QPoint(QRandomGenerator::global()->bounded(ROOM_WIDTH-4)+2,QRandomGenerator::global()->bounded(ROOM_HEIGHT-4)+2);
    snake[1] = snake[0]+=QPoint(1,0);
    snakeLen = 2;
    dir = 0;
    speed = difficulty;
    timer->stop();
    timer->start(speed);

    // multiplayer
    // set the entire snake to be off the screen
    for (int j = 0; j<MAX_CLIENTS; j++) {
        for (int i = 0; i < ROOM_HEIGHT*ROOM_WIDTH; i++) {
            if (altSnake[j][i] == noSnake) break; // we can terminate here because we never change the length of altSnake, that is done by the other clients
            altSnake[j][i] = noSnake;
        }
    }

    genNewFood();
    this->update();
}

void MainWindow::restartRun() {
    initSnake();
}

//void MainWindow::setSnakeColor(QColor color) {
//    snakeColor = color;
//}

//void MainWindow::setSnakeColor(int red, int green, int blue) {
//    snakeColor = QColor(red, green, blue);
//}

// multiplayer
void MainWindow::connectToServer(const QHostAddress &address, quint16 port) {
    clientSocket.connectToHost(address, port);
}

void MainWindow::readData() {
    if (socketId == -1) {
        setId();
        return;
    }

    QString data(clientSocket.readAll());
    if (data=="") return;
    QStringList strList = data.split("_");

    // figure out socket id of incoming snake
    bool ok;
    int altSnakeSocketId = strList[0].toInt(&ok);
    if (!ok) return;

    // figure out food position of this altSnake
    QStringList newFoodData = strList[1].split(",");
    bool foodXOk, foodYOk;
    int foodX = newFoodData[0].toInt(&foodXOk);
    int foodY = newFoodData[1].toInt(&foodYOk);
    if (foodXOk && foodYOk) {
        foodPos[altSnakeSocketId].setX(foodX);
        foodPos[altSnakeSocketId].setY(foodY);
    }

    // set altsnake array
    QStringList altSnakeStrList1 = strList[2].split(":");
    for (int i = 0; i < ROOM_HEIGHT*ROOM_WIDTH; i++) {
        QStringList altSnakeStrList2 = altSnakeStrList1[i].split(",");
        bool xOk, yOk;
        int altSnakeX = altSnakeStrList2[0].toInt(&xOk);
        int altSnakeY = altSnakeStrList2[1].toInt(&yOk);
        if (xOk&&yOk) {
            altSnake[altSnakeSocketId][i] = QPoint(altSnakeX,altSnakeY);
            if (QPoint(altSnakeX, altSnakeY) == noSnake) break; // break after the terminator is put in the array
        }
    }

    multTimer->stop();
    multTimer->start(50);
    this->update();
}

void MainWindow::setId() {
    // split at '$' because the first sent data will be the socketId, but there may be extra data in the buffer (from other clients)
    QByteArrayList data = clientSocket.readAll().split('$');
    bool ok;
    int tmpSocketId = data[0].toInt(&ok);
    if (ok) {
        socketId = tmpSocketId;
        foodPos[0] = QPoint(-1,-2);
        genNewFood();
    }
    qDebug() << socketId;
}

void MainWindow::sendData() {
    // don't send data if we don't have a socketId yet
    if (socketId == -1) {
        return;
    }

    // construct our food position as a string
    QString foodStr = QString::number(foodPos[socketId].x()) + "," + QString::number(foodPos[socketId].y());

    // construct our snake as a string
    QString snakeStr = QString::number(snake[0].x())+","+QString::number(snake[0].y());
    for(int i = 1; i < ROOM_HEIGHT*ROOM_WIDTH; i++) {
        snakeStr+=":"+QString::number(snake[i].x())+","+QString::number(snake[i].y());
        if (snake[i] == noSnake) break; // break after adding the terminator (this should hopefully reduce latency)
    }

    // construct main data string
    QString strData = QString::number(socketId)+"_" +foodStr+"_"+snakeStr+"_";

    // write the data as a QByteArray (that's the kind of data QTcpSocket deals with)
    clientSocket.write(strData.toLocal8Bit());

    multTimer->stop();
    multTimer->start(50);
}

void MainWindow::genNewFood() {
    // generate a new food position
    // if this client isn't connected, use address 0 for food position
    // generates a random point inside the room
    foodPos[(socketId==-1)?0:socketId] = QPoint(QRandomGenerator::global()->bounded(ROOM_WIDTH-4)+2,(QRandomGenerator::global()->bounded(ROOM_HEIGHT-4))+2);
}

bool MainWindow::snakeEatFood(QPoint newPos) {
    // checks if the snake is going to be on top of some food
    // this is to clean up the movePlayer logic

    for (QPoint p : foodPos) {
        if (newPos == p) {
            return true;
        }
    }
    return false;
}
void MainWindow::pauseMenu()
{
    //pause menu here
    //options: Continue (isPaused = !isPaused;), Restart (restartRun), Exit (endRun)
    //see score here?
    //what happens if an altSnake hits us when we're paused?
    //should this pause all snakes?

    QColor snakeColor = (((socketId==-1)?QColor(0,255,0):snakeColors[socketId]));
    QPalette pal = palette();
    pal.setColor(QPalette::WindowText, snakeColor);
    pal.setColor(backgroundRole(), Qt::black);

    pauseScreen = new QFrame;
    pauseScreen->resize(600,400);
    pauseScreen->setWindowTitle("Pause Screen");
    pauseScreen->setFrameStyle(QFrame::Box|QFrame::Plain);
    pauseScreen->setPalette(pal);
    pauseScreen->setLineWidth(10);

    QLabel *pauseMessage = new QLabel("Game Paused",pauseScreen);
    pauseMessage->setGeometry(200,25,250,50);
    pauseMessage->setFont(QFont("MS Shell Dlg 2",24));
    //pauseMessage->setAlignment(Qt::AlignCenter);

    QLabel *scoreMessage = new QLabel("Your score: "+QString::number(snakeLen),pauseScreen);
    scoreMessage->setGeometry(230,75,250,50);
    scoreMessage->setFont(QFont("MS Shell Dlg 2",18));
    //scoreMessage->setAlignment(Qt::AlignCenter);

    QPushButton *continueButton;
    continueButton = new QPushButton("Continue", pauseScreen);
    continueButton->setGeometry(QRect(50,175,500,50));
    connect(continueButton, &QPushButton::clicked, this,&MainWindow::continueFromPause);

    QPushButton *restartButton;
    restartButton = new QPushButton("Restart", pauseScreen);
    restartButton->setGeometry(QRect(50,250,500,50));
    connect(restartButton, &QPushButton::clicked,this,&MainWindow::restartFromPause);

    QPushButton *exitButton;
    exitButton = new QPushButton("Exit", pauseScreen);
    exitButton->setGeometry(QRect(50,325,500,50));
    connect(exitButton, &QPushButton::clicked,this,&MainWindow::exitFromPause);

    pauseScreen->show();


}
void MainWindow::continueFromPause()
{
    //continue button from pause
    if(startup)
    {startScreen->close();startup=false;}
    else{pauseScreen->close();isPaused = !isPaused;}
    update();
}
void MainWindow::exitFromPause()
{
    //exit button from pause
    isPaused = !isPaused;
    if(startup)
    {startScreen->close();startup=false;QCoreApplication::quit();}
    else{pauseScreen->close();endRun();}
}
void MainWindow::restartFromPause()
{
    //restart button from pause
    if(startup)
    {startScreen->close();startup=false;}
    else{pauseScreen->close();isPaused = !isPaused;}
    restartRun();
}
void MainWindow::startMenu()
{
    //start menu goes here
    //startup=false;
    QColor snakeColor = (((socketId==-1)?QColor(0,255,0):snakeColors[socketId]));
    QPalette pal = palette();
    pal.setColor(QPalette::WindowText, snakeColor);
    pal.setColor(backgroundRole(), Qt::black);

    startScreen = new QFrame;
    startScreen->resize(800,800);
    startScreen->setWindowTitle("Start Screen");
    startScreen->setFrameStyle(QFrame::Box|QFrame::Plain);
    startScreen->setPalette(pal);
    startScreen->setLineWidth(10);

    QLabel *titleMessage = new QLabel("Snake",startScreen);
    titleMessage->setGeometry(320,50,250,75);
    titleMessage->setFont(QFont("MS Shell Dlg 2",40));

    diffMessage = new QLabel("Select Difficulty: Easy",startScreen);
    diffMessage->setGeometry(250,250,400,75);
    diffMessage->setFont(QFont("MS Shell Dlg 2",24));

    diff = new QSlider(Qt::Horizontal,startScreen);
    diff->setMaximum(2);
    diff->setMinimum(0);
    diff->setValue(0);
    diff->setTickPosition(QSlider::TicksBelow);
    diff->setTickInterval(1);
    diff->setGeometry(250,350,300,50);
    connect(diff, &QSlider::valueChanged,this,&MainWindow::sliderLogic);

    QPushButton *continueButton;
    continueButton = new QPushButton("Start Game", startScreen);
    continueButton->setGeometry(QRect(50,550,700,50));
    connect(continueButton, &QPushButton::clicked, this,&MainWindow::continueFromPause);

    QPushButton *exitButton;
    exitButton = new QPushButton("Exit Game", startScreen);
    exitButton->setGeometry(QRect(50,650,700,50));
    connect(exitButton, &QPushButton::clicked,this,&MainWindow::exitFromPause);



    startScreen->show();
    //need to update(); when we leave
}
int MainWindow::sliderLogic()
{
    //slider logic here
    QString tempMsg;
    switch(diff->value())
    {
    case 0:
        tempMsg="Easy";
        //setDifficulty(300);
        break;
    case 1:
        tempMsg="Medium";
        //setDifficulty(500);
        break;
    case 2:
        tempMsg="Hard";
        //setDifficulty(800);
        break;
    }

    diffMessage->setText("Select Difficulty: " + tempMsg);
}
