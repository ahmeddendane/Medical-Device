#include "session.h"
#include "mainwindow.h"
#include <cmath>


int Session::prev_ID = 0;
QVector<double> Session::coherence_scores = {0.1, 0.3, 0.5, 0.7, 0.9, 1.0, 1.4, 1.8, 2.0, 2.5, 3.0};

Session::Session()
{
    sessionID = ++prev_ID;
    coherence_score = 0;
    achievment = 0;
    length = 0;
    avg_coherence = 0;
    high_time = 0;
    med_time = 0;
    low_time = 0;
    x_data = {};
    y_data = {};

}

void Session::start(QTime time)
{
     start_time = time;

}

void Session::end()
{
    get_duration();
}


void Session::update_metrics()
{
    coherence_score = coherence_scores[qrand() % coherence_scores.size()];
    achievment += coherence_score;
    avg_coherence = achievment/length;

}

void Session::update_duration(){
    //length += 1;

    if(coherence_score < 0.5){
       low_time += 1;
    }
    else if(coherence_score <= 1.4){
        med_time += 1;
    }
    else{
        high_time += 1;
    }
}

void Session::add_data_point(double x, double y)
{
    length += 1;    //can be multiplied by 1/(# of updates per second)
    x_data.push_back(x);
    y_data.push_back(y);
}


//getters
int Session::get_ID()
{
    return sessionID;
}

int Session::get_duration()
{
    return length;
}

double Session::get_coherence()
{
    return coherence_score;
}

double Session::get_achievement()
{
    return achievment;
}

double Session::get_average()
{
    return round(avg_coherence * 10)/10;
}

double Session::get_low_duration()
{
    return round((float(low_time)/length * 100)*10)/10;
}

double Session::get_med_duration()
{
    return round((float(med_time)/length * 100)*10)/10;
}

double Session::get_high_duration()
{
    return round((float(high_time)/length * 100)*10)/10;
}

QVector<double>& Session::get_x_data()
{
    return x_data;
}

QVector<double>& Session::get_y_data()
{
    return y_data;
}





