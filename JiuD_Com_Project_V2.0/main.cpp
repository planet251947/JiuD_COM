#include "mainwindow.h"
#include <QApplication>
/*
 Project: JiuD_Com
 Version: V2.0
 Author : JiuDing
 Time   : 2020/3/30
 Contact: jdingwang@outlook.com
          3195661858@qq.com
*/



/**********
 1：基本功能已经完工；
 2：界面布局可继续调整
 3：虚拟示波器显示效果不佳，难以实现高速传输，
 目前只能在较低的速度下工作，这可能与画图函数占用时间过高有关
 4：可以添加多路示波器

************/


int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;

    //由于布局问题，子控件不能跟随窗口缩放，遂禁止改变窗口大小
  //  w.setWindowFlags(w.windowFlags() &~ (Qt::WindowMinMaxButtonsHint ));
  //  w.setFixedSize(w.width(),w.height());


    w.setWindowTitle("JiuD_Com");
    w.show();

    return a.exec();
}
