#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsrand(time(NULL));     //随机数种子，撒在与使用随机数函数所在类以外的类中（使用函数在QPiece类中）

    this->setFixedSize(450, 600);
    this->setWindowTitle(tr("龙哥俄罗斯方块"));

    this->runGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::runGame()
{
    boardData = new QBoardData;     //开数组
    this->drawDesktop();            //做界面

    this->initGameSlot();
}

void MainWindow::initGameSlot()
{
    isPause = 0;
    boardData->initBoard();
    boardData->newPiece();
    showBoard->update();
}

void MainWindow::suspendGameSlot()          //暂停游戏
{
    if ( !isPause )
    {
        boardData->suspendGame();
        isPause = 1;
    }
    else
    {
        boardData->goOnGame();
        isPause = 0;
    }
}

void MainWindow::drawDesktop()
{
    window = new QWidget;
    this->setCentralWidget(window);

    showBoard = new QShowBoard;
    showBoard->setBoardState(boardData->getBoardState());

    QPushButton *startButton = new QPushButton;          //三个按钮
    QPushButton *quitButton = new QPushButton;
    QPushButton *suspendButton = new QPushButton;
    startButton->setText(tr("开始"));
    quitButton->setText(tr("退出"));
    suspendButton->setText(tr("暂停"));

    labelNext = new QLabel;                                         //信息栏
    labelNext->setFrameStyle(QFrame::Box | QFrame::Raised);

    LCDScore = new QLCDNumber(4);           //显示4位数字
    LCDScore->setSegmentStyle(QLCDNumber::Filled);

    LCDLevel = new QLCDNumber(2);
    LCDLevel->setSegmentStyle(QLCDNumber::Filled);

    QVBoxLayout *rightTopLayout = new QVBoxLayout;
    rightTopLayout->addWidget(this->createLabel(tr("Next Form")));
    rightTopLayout->addWidget(labelNext);
    rightTopLayout->addWidget(this->createLabel(tr("Score")));
    rightTopLayout->addWidget(LCDScore);
    rightTopLayout->addWidget(this->createLabel(tr("Level")));
    rightTopLayout->addWidget(LCDLevel);

    QVBoxLayout *rightDownLayout = new QVBoxLayout;
    rightDownLayout->addWidget(startButton);
    rightDownLayout->addWidget(suspendButton);
    rightDownLayout->addWidget(quitButton);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(rightTopLayout);
    rightLayout->addLayout(rightDownLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(showBoard);
    mainLayout->addLayout(rightLayout);

    window->setLayout(mainLayout);

    //    showBoard->setFocus();          //使游戏区域获得焦点 ，以接受键盘事件

    connect(startButton, SIGNAL(clicked()), this, SLOT(initGameSlot()));        /*---------点击开始，初始化游戏*/
    connect(suspendButton, SIGNAL(clicked()), this, SLOT(suspendGameSlot()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(boardData, SIGNAL(showNextPieceSignal()), this, SLOT(showNextPieceLabelSlot()));
    connect(boardData, SIGNAL(updateShowBoardSignal()), this, SLOT(updateShowBoardSlot()));

    connect(boardData, SIGNAL(scoreChangeSignal()), this, SLOT(scoreChangeSlot()));
    connect(boardData, SIGNAL(levelChangeSignal()), this, SLOT(levelChangeSlot()));
}

QLabel *MainWindow::createLabel(const QString text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    return lbl;
}


void MainWindow::showNextPieceLabelSlot()
{
    labelNext->setPixmap( showBoard->drawNextPieceLablePixmap(boardData->getNextPiece()) );
}

void MainWindow::updateShowBoardSlot()
{
    showBoard->setShowValue(boardData->getPiece(), boardData->getCurX(), boardData->getCurY());
    showBoard->update();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ( !boardData->isGameStart() )
    {
        return;
    }

    if ( Qt::Key_Up == event->key() )
    {
        boardData->rotateForm();
    }
    if ( Qt::Key_Down == event->key() )     //向下键 直接到底
    {
        boardData->fastDropDown();
    }
    if ( Qt::Key_Space == event->key() )    //空格键 直接到底
    {
        boardData->fastDropDown();
    }
    if ( Qt::Key_Left == event->key() )
    {
        boardData->moveLeft();
    }
    if ( Qt::Key_Right == event->key() )
    {
        boardData->moveRight();
    }
    if ( Qt::Key_Z == event->key() )
    {
        boardData->oneLineDown();
    }
}

void MainWindow::scoreChangeSlot()
{
    LCDScore->display(boardData->getScore());
}

void MainWindow::levelChangeSlot()
{
    LCDLevel->display(boardData->getLevel());
}
