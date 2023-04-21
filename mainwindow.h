#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "session.h"

#include <QMap>
#include <QVector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int i=0;

    QTimer *timer;
    QTimer *dataTimer;
    QTimer *barTimer;

    bool insession=false;;

    float coherence;
    QString length;
    float achievement;

    int bar_value;

    int option_number=0;
    float current_value;

    QMap <float, float> numbers;

    int previous_page;

   void createGraph();

   void addData();

   void read_Coherence();

   double getElapsedTime();

   void plotSummary(QVector<double>, QVector<double>);



private slots:

    void on_delete_button_clicked();

    void on_menu_button_clicked();

    void update_list();

    void makePlot();

    void update_bar();

    void on_up_button_clicked();

    void on_back_button_clicked();

    void on_down_button_clicked();

    void on_ok_button_clicked();

    void update();

    void  light_bar();



private:
    Ui::MainWindow *ui;
    QVector<double> coherence_scores;
    QVector<Session*> sessions;
    Session *current_session;
    int current_session_index;
};
#endif // MAINWINDOW_H
