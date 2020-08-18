#pragma once
#include <qdebug.h>
#include <QTextCodec>
#include <QJsonParseError>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qfile.h>
#include <qmessagebox.h>
class JsonConfig
{
private:
	QString path;
public:
	QJsonObject configJsonObj;
public:
	JsonConfig(QString path);
	~JsonConfig();
	// 读取配置
	void loadJsonConfig();
	// 更新指定任务
	void updateTask(int index, QJsonObject task);
	// 新增一条任务
	void addTask();
	// 删除指定任务
	void deleteTask(int index);
	// 保存配置
	void saveJsonConfig(QString path);
};

