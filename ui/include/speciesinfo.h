#ifndef SPECIESINFO_H
#define SPECIESINFO_H

#include <QWidget>

namespace Ui {
class SpeciesInfo;
}

class SpeciesInfo : public QWidget
{
    Q_OBJECT

public:
    explicit SpeciesInfo(QWidget *parent = nullptr);
    ~SpeciesInfo();

//private:
    Ui::SpeciesInfo *ui;
};

#endif // SPECIESINFO_H
