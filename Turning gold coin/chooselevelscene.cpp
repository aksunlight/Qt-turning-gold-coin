#include "chooselevelscene.h"
#include "mypushbutton.h"
#include "playscene.h"
#include <QMenuBar>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QString>
#include <QSound>

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    //配置选择关卡场景
    this->setFixedSize(350, 566);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择关卡");

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

    //返回按钮
    MyPushButton * backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(), this->height()-backBtn->height());

    //点击返回到开始界面
    connect(backBtn, &MyPushButton::clicked, [=](){
        //qDebug() << "执行了返回按钮";
        //告诉主场景我要返回了，主场景要监听ChooseLevelScene的返回按钮发出的信号

        //播放返回按钮音效
        quitSound->play();

        QTimer::singleShot(200, this, [=](){  //延时返回
            emit this->chooseSceneBack();
        });
    });

    //设置选择关卡音效
    QSound* chooseSound = new QSound(":/res/TapButtonSound.wav", this);

    //创建选择关卡的按钮
    for(int i = 0; i < 20; i++)
    {
        //设置关卡按钮
        MyPushButton* menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(45 + (i%4) * 70, 140 + (i/4) * 70);

       //设置关卡按钮上的文字
        QLabel* label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(), menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(45 + (i%4) * 70, 140 + (i/4) * 70);
        //设置按钮上的文字的对其方式
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);  //设置水平居中和垂直居中
        //设置让鼠标进行穿透  51号属性
        label->setAttribute(Qt::WA_TransparentForMouseEvents);

        //监听每个按钮的点击事件
        connect(menuBtn, &MyPushButton::clicked, [=](){
//          QString str = QString("您选择的是第 %1 关").arg(i + 1);
//          qDebug() << str;

            //播放选择关卡音效
            chooseSound->play();

            //进入游戏场景
            this->hide();  //将关卡界面隐藏
            playscene = new PlayScene(i + 1);  //传入参数为选择的关卡
            playscene->setGeometry(this->geometry());
            playscene->show();  //显示游戏场景

            //监听游戏界面的返回按钮
            connect(playscene, &PlayScene::playSceneBack, [=](){
                this->setGeometry(playscene->geometry());
                this->show();  //重新显示选择场景
                delete playscene;  //销毁游戏界面
                playscene = NULL;
            });
        });
    }
}

void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    //设置背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    //设置标题图
    pix.load(":/res/Title.png");
    //pix = pix.scaled(pix.width() * 1.2, pix.height() * 1.2);
    //第一个参数把标题图标放在居中位置了，倒数第二第三个参数可放可不放
    painter.drawPixmap((this->width()-pix.width()) * 0.5, 30, pix.width(), pix.height(), pix);
}
