#ifndef EVOLVEUI_H
#define EVOLVEUI_H

#include <QMainWindow>

namespace Ui {
class EvolveUI;
}

class EvolveUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit EvolveUI(QWidget *parent = 0);
    ~EvolveUI();

private slots:
    void on_run_button_clicked();

    void on_temperature_min_slider_valueChanged(int value);

    void on_temperature_max_slider_valueChanged(int value);

private:
    Ui::EvolveUI *ui;
};

#endif // EVOLVEUI_H
