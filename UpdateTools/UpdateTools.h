#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <qfile.h>
#include <qdebug.h>
#include <QJsonParseError>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <QTextCodec>
#include <qcheckbox.h>
#include <qmessagebox.h>
#include <QStandardItemModel>
#include <QtWidgets/QMainWindow>
#include "ui_UpdateTools.h"
#include "x64/Debug/uic/ui_UpdateTools.h"

class UpdateTools : public QMainWindow
{
    Q_OBJECT

public:
    UpdateTools(QWidget *parent = Q_NULLPTR);

private:
    Ui::UpdateToolsClass ui;
    std::string winSCP; // winSCP·��
    std::vector<std::string> commandVector; // �洢��Ҫִ�е�winSCP����
    QStandardItemModel* remoteInfoModel;  // ����չʾԶ�̷�������Ϣ��ģ��
    std::vector<int> clickedRow;
private slots:
    void readJsonConfig();
    void upLoad();
    void showRemoteInfo();
    void addRemoteInfo();
    void deleteRemoteInfo();
    void saveRemoteInfo();
    void onCheckBoxTotalClicked(bool clicked);
};
