#pragma once
#include "basicwindow.h"
#include <QWidget>
#include "ui_SkinWindow.h"

class SkinWindow : public BasicWindow
{
	Q_OBJECT

public:
	SkinWindow(QWidget *parent = nullptr);
	~SkinWindow();
	void initControl();
public slots:
	void onShowClose(); 
private:
	Ui::SkinWindow ui;
};
