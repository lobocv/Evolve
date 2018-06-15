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

private:
    Ui::LabelSlider *ui;
};

#endif // LABELSLIDER_H
