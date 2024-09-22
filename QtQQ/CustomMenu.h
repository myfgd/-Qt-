#pragma once

#include <QMenu>
#include <QMap>
class CustomMenu  : public QMenu
{
	Q_OBJECT
private:
	QMap<QString, QAction*> m_menuActionMap;
public:
	CustomMenu(QWidget *parent);
	~CustomMenu();
	QAction* getAction(const QString& text);
	void addCustomMenu(const QString& text,const QString& icon,const QString& name);
};
