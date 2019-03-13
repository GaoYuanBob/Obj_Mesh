#include "slices.h"

#include <QSlider>
#include <QLayout>
#include <QLabel>
#include <QSpinBox>
#include <QTextEdit>

Slices::Slices(QWidget *parent) : QWidget(parent)
{
	label = new QLabel("Value:");
	slider = new QSlider(Qt::Horizontal, this);
	spinBox = new QSpinBox;


	auto hLayout = new QHBoxLayout;
	hLayout->addWidget(label);
	hLayout->addWidget(slider);
	hLayout->addWidget(spinBox);


	auto layout = new QVBoxLayout;

	// 按顺序从上到下
	//layout->addStretch();
	auto edit = new QTextEdit;
	layout->addWidget(edit);
	layout->addLayout(hLayout);
	
	
    setLayout(layout);
}
