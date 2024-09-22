#pragma once

//#include <QtWidgets/QMainWindow>
#include "ui_CCMainWindow.h"
#include "basicwindow.h"
#include <QMap>
#include <QTreeWidget>
#include <QMouseEvent>
#include <QEvent>
#include <QApplication>
class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QString account,bool isAccountLogin,QWidget *parent = nullptr);
    ~CCMainWindow();
    void initControl();
    void setUserName(const QString& username);
    void setLevelPixmap(int level);
    void setHeadPixmap(const QString& headPath);
    void setStatusMenIcon(const QString& statusPath);
    QWidget* addOtherAppExtension(const QString& appPath,const QString& appName);
    void initTimer();
    void initContactTree();
private:
    QString getHeadPicturePath();
private slots:
    void onItemClicked(QTreeWidgetItem* item , int column);
    void onItemExpand(QTreeWidgetItem* item);
    void onItemCollapsed(QTreeWidgetItem* item);
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);
private:
    void updateSearchStyle();
    void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, int DepID);
private:
    void resizeEvent(QResizeEvent* event);
    bool eventFilter(QObject* obj,QEvent* event);
    void mousePressEvent(QMouseEvent* event);
private:
    Ui::CCMainWindowClass ui;
    bool m_isAccountLogin;
    QString m_account;
private slots:
    void onAppIconClicked();
private:
    //QMap<QTreeWidgetItem*,  QString> m_groupMap;
};
