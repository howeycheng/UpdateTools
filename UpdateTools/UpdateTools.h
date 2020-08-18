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
#include <qfiledialog.h>
#include <QStandardItemModel>
#include <QtWidgets/QMainWindow>
#include <QStandardItemModel>
#include <qstringlistmodel.h>
#include "ui_UpdateTools.h"
#include "x64/Debug/uic/ui_UpdateTools.h"

#include "JsonConfig.h"
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
    QString localPath;
    QStringListModel* taskModel;
    JsonConfig jsCfg;

private slots:
    void showTasks();
    void showTaskDetail(QModelIndex index);
    void upLoad();
    void addRemoteInfo();
    void deleteRemoteInfo();
    void saveRemoteInfo();
    void onCheckBoxTotalClicked(bool clicked);
    void onCheckChoseLocalFileButton();
    void onCheckActionNewTask();
    void onCheckActionDeleteTask();
    void onCheckActionSaveCfg();
};
