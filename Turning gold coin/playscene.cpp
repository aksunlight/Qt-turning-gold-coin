#include "playscene.h"
#include "mypushbutton.h"
#include "mycoin.h"
#include "dataconfig.h"
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QMenuBar>
#include <QPainter>
#include <QPropertyAnimation>
#include <QSound>

//PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
PlayScene::PlayScene(int levelNum)
{
//  QString str = QString("进入了第 %1 关").arg(levelNum + 1);
//  qDebug() << str;
    this->levelIndex = levelNum;

    //配置选择关卡场景
    this->setFixedSize(350, 566);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("开始游戏");

    //创建菜单栏，可以用设计师界面类
    QMenuBar * bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu* startMenu = bar->addMenu("开始");

    //创建退出菜单项
    QAction* quitAction = startMenu->addAction("退出");

    //点击退出实现退出游戏
    connect(quitAction, &QAction::triggered, [=](){
        this->close();
    });

    //设置返回按钮音效
    QSound* quitSound = new QSound(":/res/BackButtonSound.wav", this);
    //设置翻金币音效
    QSound* flipSound = new QSound(":/res/ConFlipSound.wav", this);
    //设置胜利音效
    QSound* winSound = new QSound(":/res/LevelWinSound.wav", this);

    //返回按钮
    MyPushButton * backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(), this->height()-backBtn->height());

    //点击返回到选择界面
    connect(backBtn, &MyPushButton::clicked, [=](){
        //qDebug() << "执行了返回按钮";
        //告诉选择场景我要返回了，选择景要监听playScene的返回按钮发出的信号

        //播放返回音效
        quitSound->play();

        QTimer::singleShot(200, this, [=](){  //延时返回
            emit this->playSceneBack();
        });
    });

    //显示当前的关卡数
    QLabel* label = new QLabel;
    label->setParent(this);

    QFont font;  //font设置字体
    font.setFamily("华文新魏");
    font.setPointSize(20);

    QString str1 = QString("Level %1").arg(this->levelIndex);
    label->setFont(font);
    label->setText(str1);
    label->setGeometry(30, this->height()-50, 120, 50);  //设置标签开始位置和大小

    //初始化每个关卡的二维数组
    dataConfig config;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }

    //胜利图片显示
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0, 0, tmpPix.width(), tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width()-tmpPix.width()) * 0.5, -tmpPix.height());

    //显示金币图案和背景图案
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            //创建背景图案
            QLabel* label= new QLabel;
            label->setGeometry(0, 0, 50, 50);  //设置矩形标签起点和大小
            label->setPixmap(QPixmap(":/res/BoardNode.png"));
            label->setParent(this);
            label->move(75 + i * 50, 200 + j * 50);

            //创建金币图案
            QString str;
            if(this->gameArray[i][j] == 1)
            {
                //显示金币
                str = ":/res/Coin0001.png";
            }
            else
            {
                //显示银币
                str = ":/res/Coin0008.png";
            }
            MyCoin* coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(77 + i * 50, 203 + j * 50);

            //给金币属性赋值
            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j];  //1为正，0为反

            //将金币放入到金币的二维数组中
            coinBtn[i][j] = coin;

            //点击金币进行翻转
            connect(coin, &MyCoin::clicked, [=](){
                //播放翻金币音效
                flipSound->play();

                //点击按钮瞬间屏蔽其他金币按钮
                for(int i = 0; i < 4; i++)
                    for(int j = 0; j < 4; j++)
                    {
                        coinBtn[i][j]->isWin = true;
                    }

                coin->changeFlag();
                this->gameArray[i][j] = (this->gameArray[i][j] == 0) ? 1 : 0;

                //翻转周围的金币
                QTimer::singleShot(200, this, [=](){
                    if(coin->posX + 1 <= 3)  //翻转右侧金币
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY] = (this->gameArray[coin->posX+1][coin->posY] == 0) ? 1 : 0;
                    }
                    if(coin->posX - 1 >= 0)  //翻转左侧金币
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY] = (this->gameArray[coin->posX-1][coin->posY] == 0) ? 1 : 0;
                    }
                    if(coin->posY + 1 <= 3)  //翻转下侧金币
                    {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1] = (this->gameArray[coin->posX][coin->posY+1] == 0) ? 1 : 0;
                    }
                    if(coin->posY - 1 >= 0)  //翻转上侧金币
                    {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1] = (this->gameArray[coin->posX][coin->posY-1] == 0) ? 1 : 0;
                    }

                    //翻完周围金币后将所有金币解禁
                    for(int i = 0; i < 4; i++)
                        for(int j = 0; j < 4; j++)
                        {
                            coinBtn[i][j]->isWin = false;
                        }

                    //判断是否胜利
                    this->isWin = true;
                    for(int i = 0; i < 4; i++)
                    {
                        for(int j = 0; j < 4; j++)
                        {
                            if(coinBtn[i][j]->flag == false)
                            {
                                this->isWin = false;
                                break;
                            }
                        }
                        if(this->isWin == false)
                        {
                            break;
                        }
                    }
                    if(this->isWin == true)
                    {
                        //胜利了
                        //qDebug() << "游戏胜利";

                        //播放胜利音效
                        winSound->play();

                        //将所有按钮的胜利标志改为true
                        for(int i = 0; i < 4; i++)
                            for(int j = 0; j < 4; j++)
                            {
                                coinBtn[i][j]->isWin = true;
                            }
                        //将胜利的图片显示
                        QPropertyAnimation* animation = new QPropertyAnimation(winLabel, "geometry");
                        //设置时间间隔
                        animation->setDuration(800);
                        //设置开始位置
                        animation->setStartValue(QRect(winLabel->x(), winLabel->y(), winLabel->width(), winLabel->height()));
                        //设置开始位置
                        animation->setEndValue(QRect(winLabel->x(), winLabel->y() + 2 * winLabel->height(), winLabel->width(), winLabel->height()));
                        //设置缓和曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        //执行动画
                        animation->start();
                    }
                });
            });
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *)
{
    //设置背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    //设置标题图
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap(10, 30, pix.width(), pix.height(), pix);
}
