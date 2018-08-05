#ifndef EVOLVEUI_H
#define EVOLVEUI_H

#include <QMainWindow>
#include <QThread>

namespace Ui {
class EvolveUI;
}


class EcosystemThread : public QThread
{
public:
    bool ecosystem_running_ = false;
    Ui::EvolveUI *ui_;
protected:
    void run();
};

class EvolveUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit EvolveUI(QWidget *parent = 0);
    ~EvolveUI();
    std::shared_ptr<EcosystemThread> ecosystem_thread_;
    void run_ecosystem();

private:
    bool ecosystem_running_ = false;

private slots:
    void on_run_button_clicked();

private:
    Ui::EvolveUI *ui;
};




#endif // EVOLVEUI_H
