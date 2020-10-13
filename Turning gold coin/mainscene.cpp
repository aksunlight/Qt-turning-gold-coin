#include "mainscene.h"
#include "ui_mainscene.h"
#include "mypushbutton.h"
#include "chooselevelscene.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QSound>  //多媒体模块下

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景
    //设置主场景
    setFixedSize(350, 566);

    //设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置标题
    setWindowTitle("翻金币游戏");

    //退出按钮的实现
    connect(ui->actionquit, &QAction::triggered, [=](){
        this->close();
    });

    //准备开始按钮的音效
    QSound* startSound = new QSound(":/res/TapButtonSound.wav", this);

    //开始按钮
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width() * 0.5 - startBtn->width() * 0.5, this->height() * 0.75);

    //实例化选择关卡场景
    chooseScene = new ChooseLevelScene;

    connect(startBtn, &MyPushButton::clicked, [=](){
        //播放开始音效
        startSound->play();

        startBtn->zoom1();
        startBtn->zoom2();

        //延时进入到选择关卡场景
        QTimer::singleShot(200, this, [=](){
            //设置选关窗口位置
            chooseScene->setGeometry(this->geometry());

            //进入选择关卡场景中
            //自身隐藏
            this->hide();
            //显示选择关卡场景
            chooseScene->show();
        });
    });

    //监听选择关卡界面的返回按钮
    connect(chooseScene, &ChooseLevelScene::chooseSceneBack, [=](){
        this->setGeometry(chooseScene->geometry());
        chooseScene->hide();  //隐藏选择关卡场景
        this->show();  //重新显示主场景
    });
}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //画背景图
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    //画标题图标
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.7, pix.height() * 0.7);  //缩放图片
    painter.drawPixmap(10, 30, pix);
}
