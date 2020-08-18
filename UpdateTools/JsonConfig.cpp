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

// 更新指定任务
void JsonConfig::updateTask(int index,QJsonObject task) {
	QJsonValueRef tasksValue = configJsonObj.find("tasks").value();
	QJsonArray tasksArry = tasksValue.toArray();
	tasksArry.replace(index, task);
	tasksValue = tasksArry;
}
// 新增一条任务
void JsonConfig::addTask() {
	QJsonValueRef tasksValue = configJsonObj.find("tasks").value();
	QJsonArray tasksArry = tasksValue.toArray();
	QJsonObject newTaskObj;
	newTaskObj.insert("localPath","");
	newTaskObj.insert("name",QString::fromLocal8Bit("新任务"));
	QJsonArray newRemoteArry;
	newTaskObj.insert("remote", newRemoteArry);
	tasksArry.append(newTaskObj);
	tasksValue = tasksArry;
}
// 删除指定任务
void JsonConfig::deleteTask(int index) {
	QJsonValueRef tasksValue = configJsonObj.find("tasks").value();
	QJsonArray tasksArry = tasksValue.toArray();
	tasksArry.removeAt(index);
	tasksValue = tasksArry;
}

// 保存配置
void JsonConfig::saveJsonConfig(QString path) {
	QJsonDocument jsonDoc;       //构建JSON文档
	QFile fileWrite(path);
	if (fileWrite.exists()) {
		fileWrite.open(QIODevice::WriteOnly | QIODevice::Text);
		jsonDoc.setObject(configJsonObj);
		fileWrite.seek(0);
		fileWrite.write(jsonDoc.toJson());
		fileWrite.flush();
		fileWrite.close();
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("保存成功"), QMessageBox::Ok | QMessageBox::Ok);
	}
}