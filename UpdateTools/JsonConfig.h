#pragma once
#include <qdebug.h>
#include <QTextCodec>
#include <QJsonParseError>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qfile.h>
class JsonConfig
{
private:
	QString path;
public:
	QJsonObject configJsonObj;
public:
	JsonConfig(QString path);
	~JsonConfig();
	void loadJsonConfig();
	void getJsonConfig();
	void saveJsonConfig();
	void updateJsonConfig();
};

