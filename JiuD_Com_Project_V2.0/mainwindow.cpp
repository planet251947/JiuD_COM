#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <math.h>
#include <string>

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initPort();
    initChart();
}

MainWindow::~MainWindow()
{
    delete ui;
}
/****************************串口初始化*****************************/
void MainWindow::initPort()
{

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug()<<"Name:"<<info.portName();
        qDebug()<<"Description:"<<info.description();
        qDebug()<<"Manufacturer:"<<info.manufacturer();
        qDebug()<<"Null:"<<info.isNull();

         ui->cmbPortName->addItem(info.portName());

    }

    QStringList baudList;//波特率
    QStringList parityList;//校验位
    QStringList dataBitsList;//数据位
    QStringList stopBitsList;//停止位

    baudList<<"1200"<<"2400"<<"4800"<<"9600"
           <<"19200"<<"38400"<<"57600"
           <<"115200";

//    1200
//    2400
//    4800
//    9600
//    19200
//    38400
//    57600
//    115200

    ui->cmbBaudRate->addItems(baudList);
    ui->cmbBaudRate->setCurrentIndex(7);//115200

    parityList<<QObject::tr("无")<<QObject::tr("奇")<<QObject::tr("偶");
    parityList<<QObject::tr("标志");
    parityList<<QObject::tr("空格");

    ui->cmbParity->addItems(parityList);
    ui->cmbParity->setCurrentIndex(0);

    dataBitsList<<"5"<<"6"<<"7"<<"8";
    ui->cmbDataBits->addItems(dataBitsList);
    ui->cmbDataBits->setCurrentIndex(3);

    stopBitsList<<"1";
    stopBitsList<<"1.5";
    stopBitsList<<"2";

    ui->cmbStopBits->addItems(stopBitsList);
    ui->cmbStopBits->setCurrentIndex(0);

    //设置按钮可以被按下
    ui->btnOpen->setCheckable(true);
    ui->BtnInquire->setCheckable(true);


}

/****************************设置控件状态****************************/
void MainWindow::setSelectable()
{
    ui->cmbBaudRate->setEnabled(true);
    ui->cmbDataBits->setEnabled(true);
    ui->cmbFlowControl->setEnabled(true);
    ui->cmbParity->setEnabled(true);
    ui->cmbPortName->setEnabled(true);
    ui->cmbStopBits->setEnabled(true);

    ui->btnOpen->setText(QObject::tr("打开串口"));
}

void MainWindow::setNonSelectable()
{
    ui->cmbBaudRate->setEnabled(false);
    ui->cmbDataBits->setEnabled(false);
    ui->cmbFlowControl->setEnabled(false);
    ui->cmbParity->setEnabled(false);
    ui->cmbPortName->setEnabled(false);
    ui->cmbStopBits->setEnabled(false);

    ui->BtnInquire->setCheckable(false);
    ui->btnOpen->setText(QObject::tr("关闭串口"));
}

void MainWindow::SetParity()
{
//    if(ui->cmbParity->currentText()==QObject::tr("无"))
//         my_serialport->setParity(QSerialPort::NoParity);
//    else  if(ui->cmbParity->currentText()==QObject::tr("奇"))
//        my_serialport->setParity(QSerialPort::OddParity);
//    else  if(ui->cmbParity->currentText()==QObject::tr("偶"))
//        my_serialport->setParity(QSerialPort::EvenParity);
//    else  if(ui->cmbParity->currentText()==QObject::tr("标志"))
//        my_serialport->setParity(QSerialPort::MarkParity);
//    else  if(ui->cmbParity->currentText()==QObject::tr("空格"))
//        my_serialport->setParity(QSerialPort::SpaceParity);
/////////////////////这是另一种实现方式///////////////////////
      switch(ui->cmbParity->currentIndex())
      {
      case 0 :    my_serialport->setParity(QSerialPort::NoParity);  break;
      case 1 :    my_serialport->setParity(QSerialPort::OddParity);  break;
      case 2 :    my_serialport->setParity(QSerialPort::EvenParity);   break;
      case 3 :    my_serialport->setParity(QSerialPort::MarkParity);   break;
      case 4 :    my_serialport->setParity(QSerialPort::SpaceParity);  break;
      default: ;

       }
}

void MainWindow::SetDataBits()
{
    if(ui->cmbDataBits->currentText()==QObject::tr("8"))
         my_serialport->setDataBits(QSerialPort::Data8);
    else if(ui->cmbDataBits->currentText()==QObject::tr("7"))
            my_serialport->setDataBits(QSerialPort::Data7);
    else if(ui->cmbDataBits->currentText()==QObject::tr("6"))
            my_serialport->setDataBits(QSerialPort::Data6);
    else if(ui->cmbDataBits->currentText()==QObject::tr("5"))
                my_serialport->setDataBits(QSerialPort::Data5);

}

void  MainWindow::SetFlowControl()
{
    //很遗憾，作者暂时没有精力去实现这一功能，这和我目前没有能够使用流控的设备有关
    //或许等哪天我迫切的需要他，我会考虑重写这一段代码
    if(ui->cmbFlowControl->currentText()==QObject::tr("无"))
    my_serialport->setFlowControl(QSerialPort::NoFlowControl);

}

void MainWindow::SetStopBit()
{
          if(ui->cmbStopBits->currentText()==QObject::tr("1"))
            my_serialport->setStopBits(QSerialPort::OneStop);
           else if(ui->cmbStopBits->currentText()==QObject::tr("1.5"))
               my_serialport->setStopBits(QSerialPort::OneAndHalfStop);
            else if(ui->cmbStopBits->currentText()==QObject::tr("2"))
                 my_serialport->setStopBits(QSerialPort::TwoStop);
}
/****************************串口设置******************************/
void MainWindow::on_btnOpen_clicked()
{
    if(ui->btnOpen->text() == QObject::tr("打开串口"))
    {


        my_serialport = new QSerialPort(this);

        //设置串口号
        my_serialport->setPortName(ui->cmbPortName->currentText());
        //以读写方式打开串口
        if(my_serialport->open(QIODevice::ReadWrite))
        {
            //设置波特率
          //  qDebug()<<ui->cmbBaudRate->currentText();
          //  qDebug()<<ui->cmbBaudRate->currentText().toInt();

            my_serialport->setBaudRate(ui->cmbBaudRate->currentText().toInt());
            //设置数据位
            MainWindow::SetDataBits();
            //设置校验位
            MainWindow::SetParity();

            //设置流控制
            MainWindow::SetFlowControl();

             //设置停止位
            MainWindow::SetStopBit();

                //当有新数据时发送读取信号
            connect(my_serialport, SIGNAL(readyRead()), this, SLOT(readComDataSlot()));

            setNonSelectable();
        }
        else
        {

                QStringList QcomList;//串口列
                QSerialPortInfo info1;
                qDebug()<<"Null:"<<info1.isNull();
                qDebug()<<"Busy:"<<info1.isBusy();
                qDebug()<<"Count:"<<ui->cmbPortName->count();

                if(info1.isNull()&&ui->cmbPortName->count())
                {
                    QMessageBox::about(NULL, "提示", QObject::tr("当前设备已被占用或移除！"));

                }
                else if(info1.isNull()&&!ui->cmbPortName->count())
                {
                    QMessageBox::about(NULL, "提示", QObject::tr("未搜索到串口设备，请尝试‘获取串口’"));

                }

            QMessageBox::about(NULL, "提示", QObject::tr("串口没有打开！"));
            qDebug()<<"串口没有打开！";

            return;
        }
    }
    else //关闭串口
    {
       // timer->stop();
        setSelectable();
        my_serialport->close();
    }
}





/****************************数据传输******************************/
void MainWindow::readDatatoDataTab()
{
    QByteArray showdata=my_serialport->readAll();

    //qDebug()<<"number:"<<showdata.length();
    showdataAll.append(showdata);


       if(ui->HexDisplaycheckBox->isChecked())
       {
           QByteArray showdatalasttwo = showdataAll.toHex(' ');

           ui->textBrowser->setText(showdatalasttwo.toUpper());
         //  qDebug()<<"hex : "<<show<<"********************************";


       }else
       {

         ui->textBrowser->setText(QString(showdataAll).toUtf8());

        //   qDebug()<<"showdataAll : "<<showdataAll;
           //qDebug()<<"请发送数据: ";

       }

      // qDebug()<<"Bytearry : "<<showdataAll<<"********************************";
     //  qDebug()<<"Bytearry hex : "<<showdatalast.toHex(' ')<<"********************************";

       //光标退到最后的数据
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);
////////////////////////////////////////////////////////////////////////
    RecivedDataNum=showdataAll.length();
    ui->label_RecivedDataNum->setNum(RecivedDataNum);

}


void MainWindow::chartDataProcessing()
{
    if(chartData_oneArray.isEmpty())
    {
        return;
    }
    else
    {

        // $ 0x28 0x59 0x37 0x47 #  暂定一帧六字节
        ////////////根据具体协议获取datax datay,并去除update(),去掉timeslot()槽////////////
        if(chartData_oneArray[0]=='$'&&chartData_oneArray[5]=='#')
        {
            union Trans{
                unsigned char data_uchar[4];
                float data_float;
            }Data;

            Data.data_uchar[3] = chartData_oneArray[1];
            Data.data_uchar[2] = chartData_oneArray[2];
            Data.data_uchar[1] = chartData_oneArray[3];
            Data.data_uchar[0] = chartData_oneArray[4];

            datay = Data.data_float;     //获取datay
            datax++;//与datay对应的datax
//            qDebug("%x",chartData_oneArray[0]);
//            qDebug("%x",chartData_oneArray[1]);
//            qDebug("%x",chartData_oneArray[2]);
//            qDebug("%x",chartData_oneArray[3]);
//            qDebug("%x",chartData_oneArray[4]);
//            qDebug("%x",chartData_oneArray[5]);


//            qDebug()<<datay;
            updateData();    //更新图像显示
        }
    }
    chartData_oneArray.clear();
}


void MainWindow::readDatatoChartTab()
{

    QByteArray chartdata=my_serialport->readAll();

     for(int i=0;i<chartdata.length();i++)
     {
        if(chartdata[i]=='$')//根据帧起始符获取每帧数据，需要根据协议变化
        {
            chartDataProcessing();//进行数据处理，获取datay
        }
       chartData_oneArray.append(chartdata[i]);
     }
}


void MainWindow::readComDataSlot()
{

    if(tabBarClicked==0) //处于显示原始数据界面
    {
      readDatatoDataTab();

    }

    else if(tabBarClicked==1) //进入图形数据界面
    {
        readDatatoChartTab();
    }

}



//////////////////////////////////////////////////////
void MainWindow::on_btnSend_clicked()
{
    //有一点很重要的是，本函数支支持UTF8格式的中文字符的读写显示，
    //对于GB2312格式的字符会显示乱码
    if(ui->btnOpen->text()=="打开串口")
    {
        if(ui->TimingcheckBox->isChecked())
        {
        disconnect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::on_btnSend_clicked));
        timer->stop();
        ui->TimingcheckBox->setCheckState(Qt::CheckState(0));
        return;
        }

    }

    if(ui->btnOpen->text()=="关闭串口")
        {
            if(!ui->SendtextEdit->toPlainText().isEmpty())
            {

                if(ui->HexSendcheckBox->isChecked())//是否16进制发送
                {
                    //参照on_HexDisplaycheckBox_stateChanged
                    QString sendData = ui->SendtextEdit->toPlainText();//获取数据
                    QByteArray sendDataUtf8 = sendData.toUtf8();//将数据以UTF8格式存成字符数组
                    QByteArray sendDataUtf8Hex=QByteArray::fromHex(sendDataUtf8);//

                    my_serialport->write(sendDataUtf8Hex);

                }
               else
                {
                    QString sendData = ui->SendtextEdit->toPlainText();

                      QByteArray sendData_2=sendData.toUtf8();

                      my_serialport->write(sendData_2);
                      qDebug()<<"sendData_2"<<sendData_2;


                }
                if(ui->NewLinecheckBox->isChecked())
                {
                    my_serialport->write("\n\t");

                }
                SendDataNum+=ui->SendtextEdit->toPlainText().length();
               // QString strsenddata =
                ui->label_SendDataNum->setNum(SendDataNum);
                ui->statusBar->showMessage("send success",3000);

            }
            else
            {
                ui->statusBar->showMessage("请输入要发送的数据",3000);
            }
        }
        else
        {
        QMessageBox::about(NULL, "提示", QObject::tr("请先打开串口！"));

            ui->statusBar->showMessage("请先打开串口",3000);
        }


}




void MainWindow::on_BtnInquire_clicked()//找找有没有IO设备改变的信号，最好把这个编程自动的
{
    ui->cmbPortName->clear();//清除原有项

    //该函数会依次查询每一个正在工作的设备
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {

        qDebug()<<"Name:"<<info.portName();
        qDebug()<<"Description:"<<info.description();
        qDebug()<<"Manufacturer:"<<info.manufacturer();
            //将串口号添加到cmb
        ui->cmbPortName->addItem(info.portName());


    }

}


void MainWindow::on_btnClearRecive_clicked()
{
    ui->textBrowser->clear();
    showdataAll.clear();   //Attention, the global item must be clear;
    RecivedDataNum=0;
    ui->label_RecivedDataNum->clear();
}


void MainWindow::on_btnClearSend_clicked()
{
    ui->SendtextEdit->clear();
    SendDataNum=0;
    ui->label_SendDataNum->clear();
}

/*****************************************/



/******************************************/


void MainWindow::on_NewLinecheckBox_stateChanged(int arg1)
{
    //nothing,功能在on_btnSend_clicked里已实现
    if(arg1)
    {

    }
    else
    {

    }
}
//QTimer *timer = new QTimer(MainWindow);

void MainWindow::on_TimingcheckBox_stateChanged(int arg1)
{
    if(ui->CyclespinBox->value()!=0)
    {
     if(arg1)
         {
             connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::on_btnSend_clicked));
             timer->start(ui->CyclespinBox->value());

           }
         else
         {
                 disconnect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::on_btnSend_clicked));
                 timer->stop();
          }
    }
    else
    {
        //提示：请输入发送周期
        QMessageBox::about(NULL, "提示", QObject::tr("尚未设置发送周期！"));

    }
}

void MainWindow::on_btnSaveData_clicked()
{

    if(ui->textBrowser->toPlainText().isEmpty()){
        QMessageBox::information(this, "提示消息", tr("貌似还没有数据! 接受到数据后才能保存"), QMessageBox::Ok);
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("保存为"), tr("DataFromJiuD_Com.txt"));
    QFile file(filename);
    //如果用户取消了保存则直接退出函数
    if(file.fileName().isEmpty()){
        return;
    }
    //如果打开失败则给出提示并退出函数
    if(!file.open(QFile::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("保存文件"), tr("打开文件 %1 失败, 无法保存\n%2").arg(filename).arg(file.errorString()), QMessageBox::Ok);
        return;
    }
    //写数据到文件
    QTextStream out(&file);
    out<<ui->textBrowser->toPlainText();
    file.close();
    //修改窗口标题为保存文件路径
    setWindowTitle("saved: " + QFileInfo(filename).canonicalFilePath());
}


void MainWindow::on_HexDisplaycheckBox_stateChanged(int arg1)
{

    if(arg1)
    {
        QByteArray showdatalasttwo = showdataAll.toHex(' ');

        ui->textBrowser->setText(showdatalasttwo.toUpper());
    }
    else
    {
        ui->textBrowser->setText(QObject::tr(showdataAll));

    }
}

void MainWindow::on_HexSendcheckBox_stateChanged(int arg1)
{

    if(arg1)  //需要增加正则表达式进行输入格式限制
    {
        QString sendData = ui->SendtextEdit->toPlainText();

       QByteArray sendDataUtf8 = sendData.toUtf8();


       QByteArray sendDataUtf8Hex = sendDataUtf8.toHex(' ');

       ui->SendtextEdit->setText(sendDataUtf8Hex.toUpper());
//        qDebug()<<"sendData"<<sendData;
//        qDebug()<<"sendDataUtf8"<<sendDataUtf8;
//        qDebug()<<"sendDataUtf8Hex"<<sendDataUtf8Hex;
//        sendData "一"
//        sendDataUtf8 "\xE4\xB8\x80"
//        sendDataUtf8Hex "e4 b8 80"

    }
    else
    {
        QString sendData = ui->SendtextEdit->toPlainText();
        QByteArray sendDataUtf8 = sendData.toUtf8();
        QByteArray sendDataUtf8Hex=QByteArray::fromHex(sendDataUtf8);
        QString sendData3 = QString::fromUtf8(sendDataUtf8Hex);
        ui->SendtextEdit->setText(sendData3);



//         qDebug()<<"sendData2"<<sendData;
//         qDebug()<<"sendDataUtf82"<<sendDataUtf8;
//         qDebug()<<"sendDataUtf8Hex2"<<sendDataUtf8Hex;
//         qDebug()<<"sendData3"<<sendData3;
//         sendData2 "E4 B8 80"
//         sendDataUtf82 "E4 B8 80"
//         sendDataUtf8Hex2 "\xE4\xB8\x80"
//         sendData3 "一"
    }
}



/////////////////////////////////////////////////////////

void MainWindow::initChart()
{
    series = new QLineSeries;

    series->setUseOpenGL(true);//openGl 加速

    chart = new QChart;
    chart->addSeries(series);

//    series->setUseOpenGL(true);

    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(-10,10);
    chart->axes(Qt::Horizontal).first()->setRange(0, Display_sec_max);
    chart->axes(Qt::Horizontal).first()->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    chart->axes(Qt::Vertical).first()->setTitleFont(QFont("Microsoft YaHei", 10, QFont::Normal, true));
    chart->axes(Qt::Horizontal).first()->setTitleText("Time/sec");
    chart->axes(Qt::Vertical).first()->setTitleText("Speed/m");
    chart->axes(Qt::Horizontal).first()->setGridLineVisible(false);
    chart->axes(Qt::Vertical).first()->setGridLineVisible(false);

    /* hide legend */
    chart->legend()->hide();


    ui->graphicsView->setChart(chart);
   //这里的graphicsView已经被提升为了QChartView，所以可以直接SetChart


}

//void MainWindow::wheelEvent(QWheelEvent *event)
//{
//    if (event->delta() > 0) {
//        //chart->zoom(1.1);
//      //  cur_x_min = cur_x_min + 20;
//       // cur_x_max = cur_x_max - 20;
//        chart->axes(Qt::Horizontal).first()->setRange(cur_x_min++, cur_x_max++);

//    } else {
//       // chart->zoom(10.0/11);
//      //  cur_x_min = cur_x_min - 20;
//       // cur_x_max = cur_x_max + 20;
//        chart->axes(Qt::Horizontal).first()->setRange(cur_x_min++, cur_x_max++);

//    }

//    QWidget::wheelEvent(event);
//}



void MainWindow::updateData()
{
 /*******************

  经测试，发送频率为500hz下可正常工作。


******************/
   QVector<QPointF> oldData = series->pointsVector();
    if(oldData.size()!=Display_sec_max)
    {
        oldData.append(QPointF(datax, datay));

    }
    else if(oldData.size()==Display_sec_max)//每一百个数据更新一次数组，必须使用==，否则会发生越界错误
    {

       for(int i=0;i<49;i++)
       {
           oldData[i]=oldData[i+1];
       }
        oldData[49]=QPointF(datax, datay);

        cur_x_min++;
        cur_x_max++;

    }

        if(datay>cur_y_max)
            cur_y_max = datay+1;
         else if(datay<cur_y_min)
             cur_y_min = datay-1;

      chart->axes(Qt::Horizontal).first()->setRange(cur_x_min, cur_x_max);

      chart->axes(Qt::Vertical).first()->setRange(cur_y_min, cur_y_max);

     series->replace(oldData);//更新曲线


}


void MainWindow::timerSlot()
{
    if (QObject::sender() == timer_chart) {
        updateData();
    }
}
void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    //QT_CHARTS_USE_NAMESPACE
    tabBarClicked = index;
    qDebug()<<"tabBarClicked:"<<tabBarClicked;
    if(index)
    {
     // qDebug()<<ui->tabWidget
        qDebug()<<"开始数据绘图";
     //   connect(timer_chart, SIGNAL(timeout()), this, SLOT(timerSlot()));
      //  timer_chart->start(50);



    }
    else
    {
         qDebug()<<"停止数据绘图";
       // disconnect(timer_chart, SIGNAL(timeout()), this, SLOT(timerSlot()));
       // timer_chart->stop();

    }

}
///////////////////////////////////////////////////
/// \brief MainWindow::on_btnReadFile_clicked
///  按行读取文件中数据
///
///
void MainWindow::on_btnReadFile_clicked()
{

    QString filename = QFileDialog::getOpenFileName(this, tr("读取文件"), 0, tr("保存为 (*.txt *.log)"));  //添加更多的文件类型
    QFile file(filename);

    //如果取消打开则退出函数
    if(file.fileName().isEmpty()){
        return;
    }

    //如果打开失败则提示并退出函数
    if(!file.open(QFile::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("打开失败"),
                             tr("抱歉! JiuD_Com未能打开此文件, 这可能是由于没有足够的权限造成的. 您可以尝试使用chmod命令修改文件权限."),
                             QMessageBox::Ok);
        return;
    }

    //文件大小超过限制
    if(file.size() > MAX_FILE_SIZE){
        QMessageBox::critical(this, tr("载入失败"), tr("文件大小为 %1 字节, 超过限制大小 10000 字节").arg(file.size()), QMessageBox::Ok);
        return;
    }
    //文件太大时提示是否继续打开
//    if(file.size() > TIP_FILE_SIZE){
//        if(QMessageBox::question(this, tr("提示消息"), tr("您要打开的文件过大, 这将消耗更多一些的时间,要继续打开么?"),
//                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){

//            return;
//        }
//    }

    //读取数据并存入数组
    QByteArray line;
    int i=0;

    while(!file.atEnd())
    {
        line = file.readLine();//逐行读取
        int j=0,k=0;
        QByteArray bytex,bytey,bytez;
       for(j=0;j<line.length();j++)
       {
            if(line[j]=='\t'||line[j]=='\n') //通过特殊字符判断数据的起始位
            {
                k++;
            }
            else if(k==0)
            {
                bytex.append(line[j]);
            }
            else if(k==1)
            {
                bytey.append(line[j]);

            }
            else if(k==2)
            {
                bytez.append(line[j]);
            }
       }


       Sx.push_back(bytex.toDouble());
       Sy.push_back(bytey.toDouble());
       Sz.push_back(bytez.toDouble());


//       qDebug()<<"Sx:"<<Sx;
//       qDebug()<<"Sy:"<<Sy;
//       qDebug()<<"Sz:"<<Sz;

        i++;
    }
    file.close();
      qDebug()<<"Sx.front:"<<Sx.front();
      qDebug()<<"Sx.back:"<<Sx.back();
      qDebug()<<"Sy.front:"<<Sy.front();
      qDebug()<<"Sy.back:"<<Sy.back();
      qDebug()<<"Sz.front:"<<Sz.front();
      qDebug()<<"Sz.back:"<<Sz.back();
      qDebug()<<Sx.length();
    ui->statusBar->showMessage(tr("已经成功读取文件中的数据"));
}

unsigned int getbitu(const unsigned char *buff, int pos, int len)
{
    unsigned int bits=0;
int i;
for (i=pos;i<pos+len;i++)
{
   bits=(bits<<1)+((buff[i/8]>>(7-i%8))&1u);
}
return bits;

}
double HexToDouble(const unsigned char* buf)
{
   double value = 0;
   unsigned int i = 0;
   unsigned int num,temp;
   int num2;
   bool flags1 = true;

   num = getbitu(buf,i,1); //标志位
   i += 1;
//double型规定偏移量为1023，其表示范围为-1024-1023
   num2 = getbitu(buf,i,11) - 1023;
   i += 11;

   while(1)
   {
       if(flags1)
       {
           flags1 = false;
           value += 1 * pow(2,num2); num2--;
       }
       temp = getbitu(buf,i,1);    i += 1;
       value += temp * pow(2,num2); num2--;
       if(i == 64)
           break;
   }
   if(num == 1)
       value *= -1;

   return value;
}

void MainWindow::generateData()
{

    QByteArray sendData;


    //////////帧头///////////
    sendData[0]= 0x4E;
   ///////////命令/////////////
    sendData.append(0xE3);
    /////////ID//////////
    sendData.append(0x01);
    /////////数据长度//////////
    sendData.append(0x18);
    /////////校验//////////
    sendData.append( sendData[0]+ sendData[1]+sendData[2]+\
            sendData[3]);
    //////////帧数据///////////
   // qDebug()<<"data"<<data[SendNumber_protocol];
    unsigned char *p;
    double a = Sx[SendNumber_protocol];
    double b = Sy[SendNumber_protocol];
    double c = Sz[SendNumber_protocol];

    p=(unsigned char *)&a;
    for(int i=0;i<8;i++)
         sendData[5+i] = p[i];

     p=(unsigned char *)&b;
    for(int i=0;i<8;i++)
         sendData[13+i] = p[i];

     p=(unsigned char *)&c;
    for(int i=0;i<8;i++)
         sendData[21+i] = p[i];

    ///////////帧数据校验////////////
    sendData.append(sendData[5]+ sendData[12]+\
            sendData[13]+sendData[20]+sendData[21]+\
            sendData[27]);
    //////////////帧结尾////////////////
    sendData.append(0x0D);
    sendData.append(0x0A);


    my_serialport->write(sendData);

   QString sendstr = sendData.toHex(' ');
    ui->lineEdit_DisplayCMD->setText\
            (sendstr.toUpper());

}


QByteArray MainWindow::generateDataInt(int x, int y, int z)
{

    QByteArray sendData;

    //////////帧头///////////
    sendData[0]= 0x4E;
   ///////////命令/////////////
    sendData.append(0xE3);
    /////////ID//////////
    sendData.append(0x01);
    /////////数据长度//////////
    sendData.append(0x03);
    /////////校验//////////
    //sendData.append( sendData[0]+ sendData[1]+sendData[2]+sendData[3]);
    //////////帧数据///////////
    //sendData.append(data[SendNumber_protocol]);
    //sendData.append(0xff);
    sendData.append(reinterpret_cast<const char*>(&x), sizeof(x));
    sendData.append(reinterpret_cast<const char*>(&y), sizeof(y));
    sendData.append(reinterpret_cast<const char*>(&z), sizeof(z));
    //sendData.append(0xfe);
    ///////////帧数据校验////////////
    //sendData.append( sendData[5]+ sendData[6]+sendData[7]+sendData[8]);

//    float data = x;
//    QByteArray t2;
//    t2.append(reinterpret_cast<const char*>(&data), sizeof(data));

    //////////////帧结尾////////////////
    sendData.append(0x0D);
    sendData.append(0x0A);


    qDebug()<<"sendData"<<sendData;
    ui->lineEdit_DisplayCMD->setText(ui->lineEdit_DisplayCMD->text()+sendData.toUpper().toHex(' ')+" ");
    return sendData;

}

QByteArray MainWindow::generateData(float x, float y, float z)
{

    QByteArray sendData;

    //////////帧头///////////
    sendData[0]= 0x4E;
   ///////////命令/////////////
    sendData.append(0xE3);
    /////////ID//////////
    sendData.append(0x01);
    /////////数据长度//////////
    sendData.append(0x03);
    /////////校验//////////
    //sendData.append( sendData[0]+ sendData[1]+sendData[2]+sendData[3]);
    //////////帧数据///////////
    //sendData.append(data[SendNumber_protocol]);
    //sendData.append(0xff);
    sendData.append(reinterpret_cast<const char*>(&x), sizeof(x));
    sendData.append(reinterpret_cast<const char*>(&y), sizeof(y));
    sendData.append(reinterpret_cast<const char*>(&z), sizeof(z));
    //sendData.append(0xfe);
    ///////////帧数据校验////////////
    //sendData.append( sendData[5]+ sendData[6]+sendData[7]+sendData[8]);

//    float data = x;
//    QByteArray t2;
//    t2.append(reinterpret_cast<const char*>(&data), sizeof(data));

    //////////////帧结尾////////////////
    sendData.append(0x0D);
    sendData.append(0x0A);


    qDebug()<<"sendData"<<sendData;
    ui->lineEdit_DisplayCMD->setText(ui->lineEdit_DisplayCMD->text()+sendData.toUpper().toHex(' ')+" ");
    return sendData;

}



void MainWindow::SendPositionDataAndRefresh()//发送数据并更新显示
{
    if(ui->btnOpen->text()=="打开串口")
     {
        disconnect(timer_protocol, &QTimer::timeout, this, QOverload<>::of(&MainWindow::SendPositionDataAndRefresh));
        timer_protocol->stop();
        ui->TimingcheckBox_Protocol->setCheckState(Qt::CheckState(0));
        return;
    }
    /*
    if(ui->btnOpen->text()=="关闭串口")
    {



      ui->lineEdit_DisplayCMD->clear();//清除上一次的帧数据显示


     generateData();//发送一帧数据




      //////////////更新显示//////////////////////////
                      ui->label_Sx_Now->setNum(Sx[SendNumber_protocol]);
                      ui->label_Sy_Now->setNum(Sy[SendNumber_protocol]);
                      ui->label_Sz_Now->setNum(Sz[SendNumber_protocol]);

                      ui->label_Sx_Next->setNum(Sx[SendNumber_protocol+1]);
                      ui->label_Sy_Next->setNum(Sy[SendNumber_protocol+1]);
                      ui->label_Sz_Next->setNum(Sz[SendNumber_protocol+1]);


                      ui->label_SendDataNum_Protocol->setNum(SendNumber_protocol);
                      SendNumber_protocol++;
    }*/

    if(ui->btnOpen->text()=="关闭串口")
    {



      ui->lineEdit_DisplayCMD->clear();//清除上一次的帧数据显示

//      my_serialport->write(generateData(Sx));

//      my_serialport->write(generateData(Sy));

//      my_serialport->write(generateData(Sz));

        //my_serialport->write(generateData(0.1, -0.2, 0.3));
        //my_serialport->write(generateDataInt(0.1*1000, -0.2*1000, 0.3*1000));
        my_serialport->write(generateData(Sx[SendNumber_protocol], Sy[SendNumber_protocol], Sz[SendNumber_protocol]));




      //////////////更新显示//////////////////////////
                      ui->label_Sx_Now->setNum(Sx[SendNumber_protocol]);
                      ui->label_Sy_Now->setNum(Sy[SendNumber_protocol]);
                      ui->label_Sz_Now->setNum(Sz[SendNumber_protocol]);

                      ui->label_Sx_Next->setNum(Sx[SendNumber_protocol+1]);
                      ui->label_Sy_Next->setNum(Sy[SendNumber_protocol+1]);
                      ui->label_Sz_Next->setNum(Sz[SendNumber_protocol+1]);


                      ui->label_SendDataNum_Protocol->setNum(SendNumber_protocol);
                      SendNumber_protocol++;
    }
}



void MainWindow::on_TimingcheckBox_Protocol_stateChanged(int arg1)
{
    if(arg1)
    {
        if(ui->btnOpen->text()=="关闭串口")
        {

            if(Sx.length()!=0)//数据已读取
            {

                    if(ui->CyclespinBox_Protocol->value()!=0)
                     {
                    connect(timer_protocol, &QTimer::timeout, this, QOverload<>::of(&MainWindow::SendPositionDataAndRefresh));
                    timer_protocol->start(ui->CyclespinBox_Protocol->value());
                    }
                    else
                    {
                      QMessageBox::about(NULL, "提示", QObject::tr("尚未设置发送周期！"));

                    }






            }
            else
            {
                ui->statusBar->showMessage("请打开要发送的数据文件",3000);
            }
        }
        else
        {
        QMessageBox::about(NULL, "提示", QObject::tr("请先打开串口！"));

            ui->statusBar->showMessage("请先打开串口",3000);
        }

    }
    else
    {
        disconnect(timer_protocol, &QTimer::timeout, this, QOverload<>::of(&MainWindow::SendPositionDataAndRefresh));
        timer_protocol->stop();

    }
}

//void MainWindow::on_btnPause_clicked()
//{
//    if(timer_protocol->isActive())
//    {

//        disconnect(timer_protocol, &QTimer::timeout, this, QOverload<>::of(&MainWindow::SendPositionDataAndRefresh));
//        timer_protocol->stop();
//        qDebug()<<"timer_protocol is Stop";

//    }
//}

void MainWindow::on_btnClearSend_Protocol_clicked()
{
    SendNumber_protocol=0;

    ui->label_Sx_Now->setNum(Sx[SendNumber_protocol]);
    ui->label_Sy_Now->setNum(Sy[SendNumber_protocol]);
    ui->label_Sz_Now->setNum(Sz[SendNumber_protocol]);

    ui->label_Sx_Next->setNum(Sx[SendNumber_protocol+1]);
    ui->label_Sy_Next->setNum(Sy[SendNumber_protocol+1]);
    ui->label_Sz_Next->setNum(Sz[SendNumber_protocol+1]);


    ui->label_SendDataNum_Protocol->setNum(SendNumber_protocol);

}

void MainWindow::on_btnStartSend_clicked() //发送一次数据
{
    if(ui->btnOpen->text()=="关闭串口")
    {

        if(Sx.length()!=0)//数据已读取
        {

               if(!ui->TimingcheckBox_Protocol->isChecked())
               {
                   SendPositionDataAndRefresh();//发送数据并更新显示

               }
                else
               {
                   QMessageBox::about(NULL, "提示", QObject::tr("正在进行定时发送"));

               }





        }
        else
        {
            ui->statusBar->showMessage("请打开要发送的数据文件",3000);
        }
    }
    else
    {
    QMessageBox::about(NULL, "提示", QObject::tr("请先打开串口！"));

        ui->statusBar->showMessage("请先打开串口",3000);
    }

}

//////////////协议发送已结束////////////////////

/////////////////以下是多行发送代码//////////////////////////

void  MainWindow::btnMultiple_SendData(QLineEdit * LineEdit)
{
    if(ui->btnOpen->text()=="关闭串口")
    {

        if(!ui->lineEdit_0->text().isEmpty())//数据不为空
        {
            if(ui->HexSend_checkBoxtoMultiple->isChecked())//是否16进制发送
            {
                //参照on_HexDisplaycheckBox_stateChanged
                QString sendData = LineEdit->text();//获取数据
                QByteArray sendDataUtf8 = sendData.toUtf8();//将数据以UTF8格式存成字符数组
                QByteArray sendDataUtf8Hex=QByteArray::fromHex(sendDataUtf8);//

                my_serialport->write(sendDataUtf8Hex);

            }
           else
            {
                QString sendData =  LineEdit->text();//获取数据

                  QByteArray sendData_2=sendData.toUtf8();

                  my_serialport->write(sendData_2);
                  qDebug()<<"sendData_2"<<sendData_2;


            }
            if(ui->NewLine_checkBoxtoMultiple->isChecked())
            {
                my_serialport->write("\n\t");

            }
            SendDataNum+= LineEdit->text().length();
           // QString strsenddata =
            ui->label_SendDataNum->setNum(SendDataNum);
            ui->statusBar->showMessage("send success",3000);

        }
        else
        {
           // ui->statusBar->showMessage("请先输入数据",3000);
        }
    }
    else
    {
    QMessageBox::about(NULL, "提示", QObject::tr("请先打开串口！"));

        ui->statusBar->showMessage("请先打开串口",3000);
    }
}

void MainWindow::on_btnSend0_clicked()
{
     btnMultiple_SendData(ui->lineEdit_0);
}

void MainWindow::on_btnSend1_clicked()
{
    btnMultiple_SendData(ui->lineEdit_1);
}

void MainWindow::on_btnSend2_clicked()
{
     btnMultiple_SendData(ui->lineEdit_2);
}

void MainWindow::on_btnSend3_clicked()
{
      btnMultiple_SendData(ui->lineEdit_3);
}

void MainWindow::on_btnSend4_clicked()
{
      btnMultiple_SendData(ui->lineEdit_4);
}

void MainWindow::on_btnSend5_clicked()
{
      btnMultiple_SendData(ui->lineEdit_5);
}

void MainWindow::on_btnSend6_clicked()
{
      btnMultiple_SendData(ui->lineEdit_6);
}

void MainWindow::on_btnSend7_clicked()
{
      btnMultiple_SendData(ui->lineEdit_7);
}

void MainWindow::on_btnSend8_clicked()
{
      btnMultiple_SendData(ui->lineEdit_8);
}

void MainWindow::on_btnSend9_clicked()
{
      btnMultiple_SendData(ui->lineEdit_9);
}

void  MainWindow::HexSendCheckOn_Multiple(QLineEdit * LineEdit)
{
    if(!LineEdit->text().isEmpty())
    {
    QString sendData = LineEdit->text();
    QByteArray sendDataUtf8 = sendData.toUtf8();
    QByteArray sendDataUtf8Hex = sendDataUtf8.toHex(' ');
     LineEdit->setText(sendDataUtf8Hex.toUpper());
    }
}
void  MainWindow::HexSendCheckOff_Multiple(QLineEdit * LineEdit)
{
    if(!LineEdit->text().isEmpty())
    {
    QString sendData = LineEdit->text();
    QByteArray sendDataUtf8 = sendData.toUtf8();
    QByteArray sendDataUtf8Hex=QByteArray::fromHex(sendDataUtf8);
    QString sendData3 = QString::fromUtf8(sendDataUtf8Hex);
    LineEdit->setText(sendData3);


    }
}
void MainWindow::on_HexSend_checkBoxtoMultiple_stateChanged(int arg1)
{
    //参考自 on_HexSendcheckBox_stateChanged
    if(arg1)  //或许需要增加正则表达式进行输入格式限制
    {
     HexSendCheckOn_Multiple(ui->lineEdit_0);
     HexSendCheckOn_Multiple(ui->lineEdit_1);
     HexSendCheckOn_Multiple(ui->lineEdit_2);
     HexSendCheckOn_Multiple(ui->lineEdit_3);
     HexSendCheckOn_Multiple(ui->lineEdit_4);
     HexSendCheckOn_Multiple(ui->lineEdit_5);
     HexSendCheckOn_Multiple(ui->lineEdit_6);
     HexSendCheckOn_Multiple(ui->lineEdit_7);
     HexSendCheckOn_Multiple(ui->lineEdit_8);
     HexSendCheckOn_Multiple(ui->lineEdit_9);

    }
    else
    {
        HexSendCheckOff_Multiple(ui->lineEdit_0);
        HexSendCheckOff_Multiple(ui->lineEdit_1);
        HexSendCheckOff_Multiple(ui->lineEdit_2);
        HexSendCheckOff_Multiple(ui->lineEdit_3);
        HexSendCheckOff_Multiple(ui->lineEdit_4);
        HexSendCheckOff_Multiple(ui->lineEdit_5);
        HexSendCheckOff_Multiple(ui->lineEdit_6);
        HexSendCheckOff_Multiple(ui->lineEdit_7);
        HexSendCheckOff_Multiple(ui->lineEdit_8);
        HexSendCheckOff_Multiple(ui->lineEdit_9);

    }
}



void  MainWindow::CycleSend_Multiple()
{
    if(ui->btnOpen->text()=="打开串口")//串口已关闭
        {

        MyLineEditNumber.clear();
        checkBox_Number=0;
        CycleSend_Multiple_i=0;
        disconnect(timer_MultipleSend, &QTimer::timeout, this, QOverload<>::of(&MainWindow::CycleSend_Multiple));
        timer_MultipleSend->stop();
        ui->CycleSend_checkBox->setCheckState(Qt::CheckState(0));
        return;
        }


    CycleSend_Multiple_i = CycleSend_Multiple_i%checkBox_Number;

   if(MyLineEditNumber[CycleSend_Multiple_i]==0)
        btnMultiple_SendData(ui->lineEdit_0);
   else if(MyLineEditNumber[CycleSend_Multiple_i]==1)
       btnMultiple_SendData(ui->lineEdit_1);
   else if(MyLineEditNumber[CycleSend_Multiple_i]==2)
       btnMultiple_SendData(ui->lineEdit_2);
   else if(MyLineEditNumber[CycleSend_Multiple_i]==3)
       btnMultiple_SendData(ui->lineEdit_3);
   else if(MyLineEditNumber[CycleSend_Multiple_i]==4)
       btnMultiple_SendData(ui->lineEdit_4);
   else if(MyLineEditNumber[CycleSend_Multiple_i]==5)
       btnMultiple_SendData(ui->lineEdit_5);
   else if(MyLineEditNumber[CycleSend_Multiple_i]==6)
       btnMultiple_SendData(ui->lineEdit_6);
   else if(MyLineEditNumber[CycleSend_Multiple_i]==7)
       btnMultiple_SendData(ui->lineEdit_7);
   else if(MyLineEditNumber[CycleSend_Multiple_i]==8)
       btnMultiple_SendData(ui->lineEdit_8);
   else if(MyLineEditNumber[CycleSend_Multiple_i]==9)
       btnMultiple_SendData(ui->lineEdit_9);

    CycleSend_Multiple_i++;//下次进入就是发送下一个数据了

}

void MainWindow::on_CycleSend_checkBox_stateChanged(int arg1)
{
//    if(ui->CycleSpinBox_Multiple->value()!=0)

    if(arg1)
    {
        if(ui->btnOpen->text()=="关闭串口")
        {
                //有发送框被勾选
            if(ui->checkBox_0->isChecked()||ui->checkBox_1->isChecked()\
                    ||ui->checkBox_2->isChecked()||ui->checkBox_3->isChecked()\
                    ||ui->checkBox_4->isChecked()||ui->checkBox_5->isChecked()\
                    ||ui->checkBox_6->isChecked()||ui->checkBox_7->isChecked()\
                    ||ui->checkBox_8->isChecked()||ui->checkBox_9->isChecked())
            {

                    if(ui->CycleSpinBox_Multiple->value()!=0)
                     {


                        //先确认有哪些需要发送，即一个循环发送多少LineEdit数据

                    if(ui->checkBox_0->isChecked())
                        MyLineEditNumber.append(0);
                    if(ui->checkBox_1->isChecked())
                        MyLineEditNumber.append(1);
                    if(ui->checkBox_2->isChecked())
                        MyLineEditNumber.append(2);
                    if(ui->checkBox_3->isChecked())
                        MyLineEditNumber.append(3);
                    if(ui->checkBox_4->isChecked())
                        MyLineEditNumber.append(4);
                    if(ui->checkBox_5->isChecked())
                        MyLineEditNumber.append(5);
                    if(ui->checkBox_6->isChecked())
                        MyLineEditNumber.append(6);
                    if(ui->checkBox_7->isChecked())
                        MyLineEditNumber.append(7);
                    if(ui->checkBox_8->isChecked())
                        MyLineEditNumber.append(8);
                    if(ui->checkBox_9->isChecked())
                        MyLineEditNumber.append(9);

                    checkBox_Number = MyLineEditNumber.length();



                    connect(timer_MultipleSend, &QTimer::timeout, this, QOverload<>::of(&MainWindow::CycleSend_Multiple));
                    timer_MultipleSend->start(ui->CycleSpinBox_Multiple->value());
                    }
                    else
                    {
                      QMessageBox::about(NULL, "提示", QObject::tr("尚未设置发送周期！"));

                    }

            }
            else
            {
                QMessageBox::about(NULL, "提示", QObject::tr("请勾选发送框"));
                ui->statusBar->showMessage("请勾选发送框",3000);
            }
        }
        else
        {
        QMessageBox::about(NULL, "提示", QObject::tr("请先打开串口！"));

            ui->statusBar->showMessage("请先打开串口",3000);
        }

    }
    else
    {
        MyLineEditNumber.clear();
        checkBox_Number=0;
        CycleSend_Multiple_i=0;
        disconnect(timer_MultipleSend, &QTimer::timeout, this, QOverload<>::of(&MainWindow::CycleSend_Multiple));
        timer_MultipleSend->stop();

    }
}
