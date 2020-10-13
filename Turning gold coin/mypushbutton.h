#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QString>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyPushButton(QWidget *parent = nullptr);

    //开始按钮构造函数，参数1为正常显示的图片路径，参数2为按下后显示的图片路径
    MyPushButton(QString normalImg, QString pressImg = "");

    //弹跳特效
    void zoom1();  //向下跳
    void zoom2();  //向下跳

    //按钮的按下和释放事件
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);

    //成员属性，保存用户传入的默认图片显示路径以及按下后显示的图片路径
    QString normalImgPath;
    QString pressImgPath;

signals:

};

#endif // MYPUSHBUTTON_H
