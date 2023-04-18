#include "session.h"
#include "mainwindow.h"


int Session::prev_ID = 0;

Session::Session()
{
    sessionID = ++prev_ID;

}

void Session::start(QTime time){

     start_time = time;
}

void Session::end(){

    get_duration();

}

int Session::get_duration(){
    return start_time.elapsed();
}

void Session::update_metrics(){

}


