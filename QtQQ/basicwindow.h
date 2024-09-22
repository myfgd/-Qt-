#pragma once
#include <QFile>
#include "titlebar.h"
#include <QDialog>
#include <QMainWindow>

class BasicWindow  : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent);
	virtual ~BasicWindow();
public:
	void loadStyleSheet(const QString& sheetName);
	QPixmap getRoundImage(const QPixmap& src,QPixmap& mask,QSize masksize=QSize(0,0));


private:
	void initBackGroundColor();

protected:
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

protected:
	void initTitleBar(ButtonType buttontype=MIN_BUTTON);
	void setTitleBarTitle(const QString& title,const QString& icon="");

public slots:
	void onShowClose(bool);
	void onShowMin(bool);
	void onShowHide(bool);
	void onShowNormal(bool);
	void onShowQuit(bool);
	void onSignalSkinChanged(const QColor& color);


	void  onButtonMinClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();
	void onButtonRestoreClicked();

protected:
	QPoint m_mousePoint;
	bool m_mosuePressed;
	QColor m_colorBackGround;
	QString m_styleName;
	titleBar* _titleBar;


};

