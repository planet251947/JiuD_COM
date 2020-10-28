#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QString>
#include <QLineEdit>




//载入文件时，如果文件大小超过TIP_FILE_SIZE字节则提示文件过大是否继续打开
#define TIP_FILE_SIZE 10000
//载入文件最大长度限制在MAX_FILE_SIZE字节内
#define MAX_FILE_SIZE 65535

QT_CHARTS_USE_NAMESPACE


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnOpen_clicked();
    //读取串口数据
    void readComDataSlot();

    void on_btnSend_clicked();



    void on_BtnInquire_clicked();


    void on_btnClearRecive_clicked();

    void on_btnClearSend_clicked();


    void on_NewLinecheckBox_stateChanged(int arg1);

    void on_TimingcheckBox_stateChanged(int arg1);

    void on_btnSaveData_clicked();


    void on_HexDisplaycheckBox_stateChanged(int arg1);

    void on_HexSendcheckBox_stateChanged(int arg1);

    void on_tabWidget_tabBarClicked(int index);

     void timerSlot();



     void on_btnReadFile_clicked();

     void on_TimingcheckBox_Protocol_stateChanged(int arg1);
     void SendPositionDataAndRefresh();//发送数据并更新显示


   //  void on_btnPause_clicked();

     void on_btnClearSend_Protocol_clicked();

     void on_btnStartSend_clicked();

     void on_btnSend0_clicked();

     void on_btnSend1_clicked();

     void on_btnSend2_clicked();

     void on_btnSend3_clicked();

     void on_btnSend4_clicked();

     void on_btnSend5_clicked();

     void on_btnSend6_clicked();

     void on_btnSend7_clicked();

     void on_btnSend8_clicked();

     void on_btnSend9_clicked();

     void on_HexSend_checkBoxtoMultiple_stateChanged(int arg1);

     void on_CycleSend_checkBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    //实例化串口指针，用来对串口进行操作
    QSerialPort * my_serialport;
    QTimer *timer = new QTimer(this);
    QTimer *timer_protocol = new QTimer(this);
    QTimer *timer_chart = new QTimer(this);
    QTimer *timer_MultipleSend = new QTimer(this);

    QByteArray showdataAll;

    QVector<unsigned char> chartData_oneArray;
    QString SendDataGlobal;
    int SendDataNum=0;
    int RecivedDataNum=0;

    int SendNumber_protocol=0;//

    int tabBarClicked = 0;//默认在串口原始数据界面
    //图表参数
    QChart *chart;
    QLineSeries *series;
    int Display_sec_max =50; //显示数据个数
    int cur_x_min = 0;
    int cur_x_max = Display_sec_max;//x轴显示范围
    int cur_y_min = -10;
    int cur_y_max = 10;//y轴显示范围

     qreal datax,datay;


    quint16 count = 0;


    ///////////
    QVector<double>  Sx,Sy,Sz;  //用于存储读取的Sx Sy Sz数据


    /////////////多行发送
    int CycleSend_Multiple_i=0; //在单个循环中已发送到第 i 个数据
    int checkBox_Number;//一个循环共有几个需发送的linedata
     QVector<int> MyLineEditNumber;//存入了需要发送的lineEdit的序号



    //初始化串口信息
    void initPort();
    void SetParity();
    void SetDataBits();
    void SetFlowControl();
    void SetStopBit();


    //设置控件的状态
    void setSelectable();
    void setNonSelectable();

    //关于图表的函数
    void initChart();
    void updateData();
   //void wheelEvent(QWheelEvent *event);


    void readDatatoDataTab();
    void readDatatoChartTab();
    void chartDataProcessing();

    void generateData();
    QByteArray generateData(float x, float y, float z);
    QByteArray generateDataInt(int x, int y, int z);



    ////////多行发送/////
    void  btnMultiple_SendData(QLineEdit * LineEdit);
    void  HexSendCheckOn_Multiple(QLineEdit * LineEdit);
    void  HexSendCheckOff_Multiple(QLineEdit * LineEdit);
    void  CycleSend_Multiple();

};

#endif // MAINWINDOW_H
