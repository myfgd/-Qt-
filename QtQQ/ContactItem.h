#pragma once

#include <QWidget>
#include "ui_ContactItem.h"

class ContactItem : public QWidget
{
	Q_OBJECT

public:
	ContactItem(QWidget *parent = nullptr);
	~ContactItem();

	void setUserName(const QString& userName);
	void setSignName(const QString& signName);
	void setHeapPixmap(const QPixmap& headPath);

	QString getUserName() const;
	QSize getHeadLabelSize() const;

private:
	Ui::ContactItem ui;
	void initControl();
};
