#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "session.h"

#include <QTimer>
#include <QDate>
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

    barTimer = new QTimer();

    connect(barTimer, SIGNAL(timeout()), this, SLOT(update_bar()));
    connect(barTimer, SIGNAL(timeout()), this, SLOT(light_bar()));


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
    for (int i = 0; i < sessions.size(); ++i) {
        delete sessions.at(i);
    }
    sessions.clear();
    delete dataTimer;
    delete timer;
    delete barTimer;
    delete ui;

}

void MainWindow::on_menu_button_clicked()
{
    insession=false;
    ui->screen->setCurrentIndex(0);
}


void MainWindow::on_up_button_clicked()
{
    //menu
    if(ui->screen->currentIndex()==0){
        if(option_number==0){
            option_number=3;
        }
        ui->screen->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
        option_number-=1;
    }
    //settings
    else if(ui->screen->currentIndex()==1){
        if(option_number==0){
            option_number=2;
        }
        ui->screen->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
        option_number-=1;
    }
    //challenge
    else if(ui->screen->currentIndex()==3){
        if(option_number==0){
            option_number=4;
        }
        ui->screen->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
        option_number-=1;
    }

}

void MainWindow::on_down_button_clicked()
{
    option_number+=1;
    //menu
    if(ui->screen->currentIndex()==0){
        if(option_number==4){
            option_number=1;
        }
        ui->screen->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
    }
    //settings
    else if(ui->screen->currentIndex()==1){
        if(option_number==3){
            option_number=1;
        }
        ui->screen->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
    }
    //challenge
    else if(ui->screen->currentIndex()==3){
        if(option_number==5){
            option_number=1;
        }
        ui->screen->currentWidget()->findChild<QListWidget *>()->setCurrentRow(option_number-1);
    }
}

void MainWindow::on_back_button_clicked()
{
    ui->screen->setCurrentIndex(previous_page);
    previous_page=previous_page-1;
}

void MainWindow::on_delete_button_clicked()
{
    if(ui->screen->currentWidget()->findChild<QListWidget *>("log_list")){
        QListWidget *log_list = ui->screen->currentWidget()->findChild<QListWidget *>("log_list");
        int selected_row = ui->screen->currentWidget()->findChild<QListWidget *>("log_list")->currentRow();
        log_list->takeItem(selected_row);


        delete sessions.at(selected_row);
        sessions.remove(selected_row);

        log_list->update();
    }

}


void MainWindow::on_ok_button_clicked()
{   
    qInfo("%d",ui->menu_screen->currentRow());
    if(insession==false){

        //Select a session log to display
        if(ui->screen->currentWidget()->findChild<QListWidget *>("log_list")){
            Session *focused_session = sessions[ui->screen->currentWidget()->findChild<QListWidget *>("log_list")->currentRow()];

            plotSummary(focused_session->get_x_data(), focused_session->get_y_data());
            ui->screen->setCurrentIndex(5);
        }

        //ok button clicked on menu screen
        else if(ui->screen->currentWidget()->findChild<QListWidget *>("menu_screen")){

            //SESSION START POINT
            if(ui->screen->currentWidget()->findChild<QListWidget *>("menu_screen")->currentRow()==0){

                //makes a session
                current_session = new Session();


                previous_page=ui->screen->currentIndex();
                insession=true;


                barTimer->start(1000);
                dataTimer->start(0);
                timer->start(5000);

                current_session->start(QTime::currentTime());
                ui->screen->setCurrentIndex(4);
            }

            //settings option
            else if(ui->screen->currentWidget()->findChild<QListWidget *>("menu_screen")->currentRow()==1){
                previous_page=ui->screen->currentIndex();
                ui->screen->setCurrentIndex(1);
            }

            //Navigate to the saved session logs
            else if(ui->screen->currentWidget()->findChild<QListWidget *>("menu_screen")->currentRow()==2){
                previous_page=ui->screen->currentIndex();
                ui->screen->setCurrentIndex(2);
            }
        }

        //ok button clicked on settings screen
        else if(ui->screen->currentWidget()->findChild<QListWidget *>("settings_screen")){
            if(ui->screen->currentWidget()->findChild<QListWidget *>("settings_screen")->currentRow()==0){
                previous_page=ui->screen->currentIndex();
                ui->screen->setCurrentIndex(3);
            }
            else if(ui->screen->currentWidget()->findChild<QListWidget *>("settings_screen")->currentRow()==1){
                previous_page=ui->screen->currentIndex();
                ui->screen->setCurrentIndex(3);
            }
        }
    }
    //SESSION END POINT
    else{
        insession=false;
        plotSummary(current_session->get_x_data(), current_session->get_y_data());
        current_session->end();
        sessions.push_back(current_session);

        //makes label for a logged session
        QString ID_log = "SID: " + QString::number(current_session->get_ID());
        QDate date = QDate::currentDate();
        QString date_log = "    " + date.toString("dd/MM/yy");
        QTime time = QTime::currentTime();
        QString time_log = "    " + time.toString("h:mm ap");

        QString log_title = ID_log + date_log + time_log;

        ui->log_list->addItem(log_title);
        timer->stop();
        dataTimer->stop();
        ui->screen->setCurrentIndex(5);
    }
}

void MainWindow::update_list(){
}


double MainWindow::getElapsedTime(){
    static QTime time(QTime::currentTime());
    return time.elapsed()/1000.0;
}



void MainWindow::makePlot(){



    // calculate two new data points:

    double key = getElapsedTime(); // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    length = QString::number(key);
    if (key-lastPointKey > 1) // at most add point every 1 second
    {

      current_session->update_duration();
      // add data to lines:

      current_value = qSin(key)+ (qrand()/(double)RAND_MAX*40.0 + 60.0);

      //stores data points in the current session to log the graph
      current_session->add_data_point(current_session->get_duration(), current_value);


      ui->Plot->graph(0)->addData(key, current_value);
      //ui->Plot_2->graph(0)->addData(key,current_value);
      // rescale value (vertical) axis to fit the current data:
      //ui->ui->Plot->graph(0)->rescaleValueAxis();
      //ui->ui->Plot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->Plot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->Plot->replot();

    //ui->Plot_2->xAxis->setRange(key, 8, Qt::AlignRight);
    //ui->Plot_2->replot();
}

void MainWindow::plotSummary(QVector<double> x_data, QVector<double> y_data){


    //fills in the graph for a session to the summary screen
    ui->graph_summary->graph(0)->data()->clear();

    for(int i = 0; i < x_data.size(); i++){
        ui->graph_summary->graph(0)->addData(i, y_data[i]);
    } 

    ui->graph_summary->xAxis->setRange(0, x_data.size() - 1);
    ui->graph_summary->replot();


    //fills in stats for a session to the summary screen
    QString label = "Achievement:\n";
    ui->achievement_summary->setText((label + QString::number(current_session->get_achievement())));

    label = "Length:\n";
    ui->length_summary->setText((label + QString::number(current_session->get_duration())));

    label = "Average:\n";
    ui->avg_summary->setText(label + QString::number(current_session->get_average()));

    label = "High:\n";
    ui->high_summary->setText(label + QString::number(current_session->get_high_duration()) + '%');

    label = "Med:\n";
    ui->med_summary->setText(label + QString::number(current_session->get_med_duration()) + '%');

    label = "low\n";
    ui->low_summary->setText(label + QString::number(current_session->get_low_duration()) + '%');

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

    ui->graph_summary->addGraph(); // blue line
    ui->graph_summary->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->graph_summary->addGraph(); // red line
    ui->graph_summary->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    ui->graph_summary->plotLayout()->insertRow(0);
    ui->graph_summary->plotLayout()->addElement(0,0,title);

    timeTicker->setTimeFormat("%h:%m:%s");
    ui->graph_summary->xAxis->setTicker(timeTicker);
    ui->graph_summary->axisRect()->setupFullAxesBox();
    ui->graph_summary->yAxis->setRange(60, 100);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->graph_summary->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->graph_summary->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->graph_summary->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graph_summary->yAxis2, SLOT(setRange(QCPRange)));


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

    current_session->update_metrics();

    ui->length_box->setText(QString::number(current_session->get_duration()));
    ui->coherence_box->setText(QString::number(current_session->get_coherence()));
    ui->achievement_box->setText(QString::number(current_session->get_achievement()));

}



void MainWindow::update_bar(){
    ui->progressBar->setValue(ui->progressBar->value()-1);
    ui->progressBar->update();
}

void MainWindow::light_bar(){
    if(current_session->get_coherence() <= 0.5){
        ui->light_box->setStyleSheet("background-color:red;");
    }
    else if(current_session->get_coherence() >= 1.4){
        ui->light_box->setStyleSheet("background-color:green;");
    }
    else{
        ui->light_box->setStyleSheet("background-color:blue;");
    }

}
