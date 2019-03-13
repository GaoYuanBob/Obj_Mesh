#ifndef SLICES_H
#define SLICES_H

#include <QWidget>
class QSlider;
class QLabel;
class QSpinBox;

class Slices : public QWidget
{
    Q_OBJECT
public:
    explicit Slices(QWidget *parent = 0);
private:
	QLabel * label;
    QSlider * slider;
	QSpinBox * spinBox;

signals:

public slots:
};

#endif // SLICES_H
