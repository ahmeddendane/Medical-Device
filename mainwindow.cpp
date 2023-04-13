#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QTimer>
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->left_button->setIcon(QIcon("/media/sf_3004/arrowleftkey.png"));
    ui->right_button->setIcon(QIcon("/media/sf_3004/arrowrightkey.png"));

    ui->up_button->setIcon(QIcon("/media/sf_3004/arrowupkey.png"));
    ui->down_button->setIcon(QIcon("/media/sf_3004/arrowdownkey.png"));

    ui->on_off_button->setIcon(QIcon("/media/sf_3004/powerbutton.png"));
    ui->menu_button->setIcon(QIcon("/media/sf_3004/menubutton.png"));

    ui->back_button->setIcon(QIcon("/media/sf_3004/back-button.png"));


    ui->progressBar->setValue(100);

    QTimer *bar_light_timer = new QTimer();
    connect(bar_light_timer, SIGNAL(timeout()), this, SLOT(update_bar()));
    connect(bar_light_timer, SIGNAL(timeout()), this, SLOT(light_bar()));
    bar_light_timer->start(1000);

    createGraph();

    dataTimer = new QTimer();
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(makePlot()));

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));


    read_Coherence();







    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_menu_button_clicked()
{
    insession=false;
    ui->stackedWidget_2->setCurrentIndex(0);
}


void MainWindow::on_up_button_clicked()
{
    if(ui->stackedWidget_2->currentIndex()==0){
        if(option_number==0){
            option_number=3;
        }
        ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
        option_number-=1;
    }
    else if(ui->stackedWidget_2->currentIndex()==1){
        if(option_number==0){
            option_number=2;
        }
        ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
        option_number-=1;
    }
    else if(ui->stackedWidget_2->currentIndex()==3){
        if(option_number==0){
            option_number=4;
        }
        ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
        option_number-=1;
    }

}

void MainWindow::on_down_button_clicked()
{
    option_number+=1;
    if(ui->stackedWidget_2->currentIndex()==0){
        if(option_number==4){
            option_number=1;
        }
        ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
    }
    else if(ui->stackedWidget_2->currentIndex()==1){
        if(option_number==3){
            option_number=1;
        }
        ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
    }
    else if(ui->stackedWidget_2->currentIndex()==3){
        if(option_number==5){
            option_number=1;
        }
        ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
    }
}

void MainWindow::on_back_button_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(previous_page);
    previous_page=previous_page-1;
}


void MainWindow::on_ok_button_clicked()
{

    qInfo("%d",ui->listWidget->currentRow());
    if(insession==false){
        if(ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>("listWidget")){
            if(ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>("listWidget")->currentRow()==0){
                previous_page=ui->stackedWidget_2->currentIndex();
                insession=true;

                dataTimer->start(0);
                timer->start(1000);
                ui->stackedWidget_2->setCurrentIndex(4);
            }
            else if(ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>("listWidget")->currentRow()==1){
                previous_page=ui->stackedWidget_2->currentIndex();
                ui->stackedWidget_2->setCurrentIndex(1);
            }
            else if(ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>("listWidget")->currentRow()==3){
                previous_page=ui->stackedWidget_2->currentIndex();
                ui->stackedWidget_2->setCurrentIndex(2);
            }
        }
        else if(ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>("listWidget_3")){
            if(ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>("listWidget_3")->currentRow()==0){
                previous_page=ui->stackedWidget_2->currentIndex();
                ui->stackedWidget_2->setCurrentIndex(3);
            }
            else if(ui->stackedWidget_2->currentWidget()->findChild<QListWidget *>("listWidget_3")->currentRow()==1){
                previous_page=ui->stackedWidget_2->currentIndex();
                ui->stackedWidget_2->setCurrentIndex(3);
            }
        }
    }
    else{
        insession=false;

        timer->stop();

        dataTimer->stop();


        ui->stackedWidget_2->setCurrentIndex(5);


    }
}

void MainWindow::update_list(){
}





void MainWindow::makePlot(){
    static QTime time(QTime::currentTime());
    // calculate two new data points:

    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    length = QString::number(key);
    if (key-lastPointKey > 0.1) // at most add point every 2 ms
    {
      // add data to lines:

      current_value = qSin(key)+qrand()/(double)RAND_MAX*100;

      ui->Plot->graph(0)->addData(key, current_value);
      ui->Plot_2->graph(0)->addData(key,current_value);
      // rescale value (vertical) axis to fit the current data:
      //ui->ui->Plot->graph(0)->rescaleValueAxis();
      //ui->ui->Plot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->Plot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->Plot->replot();

    ui->Plot_2->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->Plot_2->replot();
}




void MainWindow::createGraph(){


    ui->Plot->addGraph(); // blue line
    ui->Plot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->Plot->addGraph(); // red line
    ui->Plot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QCPTextElement *title = new QCPTextElement(ui->Plot,"HRV");
    ui->Plot->plotLayout()->insertRow(0);
    ui->Plot->plotLayout()->addElement(0,0,title);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->Plot->xAxis->setTicker(timeTicker);
    ui->Plot->axisRect()->setupFullAxesBox();
    ui->Plot->yAxis->setRange(60, 100);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->Plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->Plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->Plot->yAxis2, SLOT(setRange(QCPRange)));

    ui->Plot_2->addGraph(); // blue line
    ui->Plot_2->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->Plot_2->addGraph(); // red line
    ui->Plot_2->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    ui->Plot_2->plotLayout()->insertRow(0);
    ui->Plot_2->plotLayout()->addElement(0,0,title);

    timeTicker->setTimeFormat("%h:%m:%s");
    ui->Plot_2->xAxis->setTicker(timeTicker);
    ui->Plot_2->axisRect()->setupFullAxesBox();
    ui->Plot_2->yAxis->setRange(60, 100);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->Plot_2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->Plot_2->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->Plot_2->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->Plot_2->yAxis2, SLOT(setRange(QCPRange)));




}

void MainWindow::addData(){
// Interval 0 means to refresh as fast as possible
}


void MainWindow::read_Coherence(){

    QTableWidgetItem *num = new QTableWidgetItem();
    num->setData(1,0.4);
    ui->coherence_table->setItem(0,0,num);

    QTableWidgetItem *num2 = new QTableWidgetItem();
    num2->setData(1,0.7);
    ui->coherence_table->setItem(1,0,num2);

    QTableWidgetItem *num3 = new QTableWidgetItem();
    num3->setData(1,3.3);
    ui->coherence_table->setItem(2,0,num3);

    QTableWidgetItem *num4 = new QTableWidgetItem();
    num4->setData(1,0.3);
    ui->coherence_table->setItem(3,0,num4);

    QTableWidgetItem *num5 = new QTableWidgetItem();
    num5->setData(1,1.6);
    ui->coherence_table->setItem(4,0,num5);



    qInfo("%f",ui->coherence_table->item(4,0)->data(1).toFloat());


}

void MainWindow::update(){


    coherence = (ui->coherence_table->item(i,0)->data(1).toFloat());

    ui->length_box->setText(length);
    ui->coherence_box->setText(QString::number(coherence));
    ui->achievement_box->setText(QString::number(achievement));

    if(i<ui->coherence_table->rowCount()-1){
        achievement = achievement+coherence;
        i++;
    }

}



void MainWindow::update_bar(){
    ui->progressBar->setValue(ui->progressBar->value()-1);
    ui->progressBar->update();
}

void MainWindow::light_bar(){
    if(coherence<0.5){
        ui->light_box->setStyleSheet("background-color:red;");
    }
    else if(coherence > 0.9){
        ui->light_box->setStyleSheet("background-color:green;");
    }
    else{
        ui->light_box->setStyleSheet("background-color:blue;");
    }

}
