#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QString>
#include <QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyCoin(QWidget *parent = nullptr);

    //参数代表传入的是金币还是银币的路径
    MyCoin(QString btnImg);

    //金币变换
    void changeFlag();
    QTimer* timer1;  //正面翻反面的定时器
    QTimer* timer2;  //反面翻正面的定时器
    int min = 1;
    int max = 8;

    //金币的属性
    int posX;  //x的坐标位置
    int posY;  //y的坐标位置
    bool flag;  //正反标识

    //执行动画标志
    bool isAnimation = false;

    //是否胜利的标志
    bool isWin = false;

    //重写按下和释放动作
    void mousePressEvent(QMouseEvent* e);

signals:

};

#endif // MYCOIN_H
