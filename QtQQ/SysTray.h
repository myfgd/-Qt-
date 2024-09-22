#pragma once

#include <QSystemTrayIcon>
#include <QWidget>
class SysTray  :
	public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTray(QWidget *parent);
	~SysTray();
	void initSystemTray(); 
private:
	void addSysTrayMenu();
public slots:
	void onIconActivated(QSystemTrayIcon::ActivationReason reason);
private:
	QWidget* m_parent; 
 };
