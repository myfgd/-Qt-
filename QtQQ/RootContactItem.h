#pragma once
#include <QPropertyAnimation>
#include <QLabel>
#include <QPaintEvent>
class RootContactItem  : public QLabel
{
	Q_OBJECT
	Q_PROPERTY(int rotation READ rotation WRITE setRotation)
public:
	RootContactItem(bool hasArrow=true,QWidget *parent=nullptr);
	~RootContactItem();
public:
	void setText(const QString& title);
	void setExpanded(bool exapnd);

private:
	int rotation();
	void setRotation(int rotation);
	void paintEvent(QPaintEvent* event);
protected:
	QPropertyAnimation* m_animation;
	QString m_titleText;
	int m_rotation;
	bool m_hasArrow;
};
