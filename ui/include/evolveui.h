#ifndef EVOLVEUI_H
#define EVOLVEUI_H

#include <QMainWindow>
#include <QThread>
#include "map"
#include "speciesinfo.h"

namespace Ui {
class EvolveUI;
}

class EcosystemThread;

class EvolveUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit EvolveUI(QWidget *parent = 0);
    ~EvolveUI();
    std::shared_ptr<EcosystemThread> ecosystem_thread_;
    void run_ecosystem();
    Ui::EvolveUI *ui;
    std::map<std::string, SpeciesInfo*> speciesinfo_;

private:
    bool ecosystem_running_ = false;

private slots:
    void on_run_button_clicked();

};


class EcosystemThread : public QThread
{
public:
    bool ecosystem_running_ = false;
    EvolveUI *app;
protected:
    void run();
};



#endif // EVOLVEUI_H
