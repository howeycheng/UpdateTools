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
	// ��ȡ����
	void loadJsonConfig();
	// ����ָ������
	void updateTask(int index, QJsonObject task);
	// ����һ������
	void addTask();
	// ɾ��ָ������
	void deleteTask(int index);
	// ��������
	void saveJsonConfig(QString path);
};

