#ifndef LABELSLIDER_H
#define LABELSLIDER_H

#include <QWidget>

namespace Ui {
class LabelSlider;
}

class LabelSlider : public QWidget
{
    Q_OBJECT

public:
    explicit LabelSlider(QWidget *parent = 0);
    ~LabelSlider();

//private:
    Ui::LabelSlider *ui;

private slots:
    void on_min_slider_valueChanged(int value);
    void on_max_slider_valueChanged(int value);

};

#endif // LABELSLIDER_H
