#pragma once

#include <QtWidgets/QDialog>
#include "ui_QtQQ_Server.h"
#include "TcpServer.h"
#include <QSqlQueryModel>
#include <QTimer>
#include <QUdpSocket>
class QtQQ_Server : public QDialog
{
    Q_OBJECT

public:
    QtQQ_Server(QWidget *parent = nullptr);
    ~QtQQ_Server();

private:
    void initComboxData();
    void initTcpSocket();
    void initUdpSocket();
    void setDepNameMap();
    void setStatusMap();
    void setOnlineMap();
    bool connectMySql();
    int getComDepID();
    void updateTableData(int depID=0,int employeeID=0);
private slots:
    void  onUDPbroadMsg(QByteArray& btData);
    void onRefresh();
    void on_queryDepartmentBtn_clicked();
    void on_queryIDBtn_clicked();
    void on_logoutBtn_clicked();
    void on_selectPictureBtn_clicked();
    void on_addBtn_clicked();
private:
    Ui::QtQQ_ServerClass ui;
    QTimer* m_timer;
    int m_comDepID;
    int m_depID;
    int m_employeeID;
    QString m_pixPath;
    QMap<QString, QString> m_statusMap;
    QMap<QString, QString> m_depNameMap;
    QMap<QString, QString> m_onlineMap;
    QSqlQueryModel m_queryInfoModel;
    TcpServer* m_tcpServer;
    QUdpSocket* m_udpSender;
};
