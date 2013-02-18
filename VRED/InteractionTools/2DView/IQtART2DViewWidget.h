#pragma once
#include "qwidget.h"
#include "IART2DView.h"

class IQtART2DViewWidget : public QWidget, public IART2DView {

	Q_OBJECT

public:
	IQtART2DViewWidget(QWidget *parent = 0, const char *name = 0, WFlags flags = 0);
	~IQtART2DViewWidget(void);
};
