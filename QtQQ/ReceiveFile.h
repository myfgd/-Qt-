#pragma once

#include <QWidget>
#include "ui_ReceiveFile.h"
#include "basicwindow.h"

class ReceiveFile : public BasicWindow
{
	Q_OBJECT

public:
	ReceiveFile(QWidget *parent = nullptr);
	~ReceiveFile();
	void setMsg(QString& msgLabel);

signals:
	void refuseFile();
private slots:
	void on_cancelBtn_clicked();
	void on_okBtn_clicked(); 

private:
	Ui::ReceiveFileClass ui;
};
