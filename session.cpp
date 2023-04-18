#include "session.h"
#include "mainwindow.h"


int Session::prev_ID = 0;
QVector<double> Session::coherence_scores = {0.1, 0.3, 0.5, 0.7, 0.9, 1.0, 1.4, 1.8, 2.0, 2.5, 3.0};

Session::Session()
{
    sessionID = ++prev_ID;
    coherence_score = 0;
    achievment = 0;
    length = 0;

}

void Session::start(QTime time)
{
     start_time = time;

}

void Session::end()
{
    get_duration();
}

double Session::get_duration()
{
    return start_time.elapsed()/1000.0;
}

double Session::get_coherence()
{
    return coherence_score;
}

double Session::get_achievement()
{
    return achievment;
}

void Session::update_metrics()
{
    coherence_score = coherence_scores[qrand() % coherence_scores.size()];
    length = get_duration()/1000.0;
    achievment += coherence_score;


    if(coherence_score < 0.5){
       low_time += 5;
    }
    else if(coherence_score >= 1.4){
        med_time += 5;
    }
    else{
        high_time += 5;
    }
}

QVector<double>& Session::get_x_data()
{
    return x_data;
}

QVector<double>& Session::get_y_data()
{
    return y_data;
}

void Session::add_data_point(double x, double y)
{
    x_data.push_back(x);
    y_data.push_back(y);
}



