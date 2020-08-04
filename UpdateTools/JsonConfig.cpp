#include "JsonConfig.h"

JsonConfig::JsonConfig(QString path) : path(path){
}
JsonConfig::~JsonConfig() {
}

// 读取json配置到jsonObj
void JsonConfig::loadJsonConfig() {
	QFile file(path);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextCodec* codec = QTextCodec::codecForName("UTF8");
	QString value = codec->toUnicode(file.readAll());
	file.close();
	QJsonParseError parseJsonErr;
	QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
	if (!(parseJsonErr.error == QJsonParseError::NoError))
	{
		qDebug() << QObject::tr("解析json文件错误！");
		return;
	}
	configJsonObj = document.object();

}
void JsonConfig::getJsonConfig(){}
void JsonConfig::saveJsonConfig(){}
void JsonConfig::updateJsonConfig(){}