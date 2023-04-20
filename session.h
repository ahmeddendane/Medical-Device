#ifndef SESSION_H
#define SESSION_H
#include <QTime>
#include <QListWidget>
#include "qcustomplot.h"

class Session{

public:
    Session();
    void start(QTime);
    void end();
    int get_duration();
    double get_coherence();
    double get_achievement();
    double get_length();
    double get_average();
    int get_low_duration();
    int get_med_duration();
    int get_high_duration();
    void save_graph();
    void update_metrics();
    void update_duration();
    void add_data_point(double x, double y);
    QVector<double>& get_x_data();
    QVector<double>& get_y_data();


private:
    //time spent in each coherence level
    int high_time;
    int low_time;
    int med_time;

    static int prev_ID;
    int sessionID;
    double coherence_score;
    double achievment;
    double avg_coherence;
    int length;
    QTime start_time;
    QVector<double> x_data;
    QVector<double> y_data;
    static QVector<double> coherence_scores;

};

#endif // SESSION_H
