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
    void save_graph();
    void update_metrics();


private:
    static int prev_ID;
    int sessionID;
    double coherence_score;
    double achievment;
    int duation;
    QTime start_time;
    QCPGraph *HRV_graph;

};

#endif // SESSION_H
