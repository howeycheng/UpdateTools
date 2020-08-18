#include "JsonConfig.h"

JsonConfig::JsonConfig(QString path) : path(path){
}
JsonConfig::~JsonConfig() {
}

// ��ȡjson���õ�jsonObj
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
		qDebug() << QObject::tr("����json�ļ�����");
		return;
	}
	configJsonObj = document.object();
}

// ����ָ������
void JsonConfig::updateTask(int index,QJsonObject task) {
	QJsonValueRef tasksValue = configJsonObj.find("tasks").value();
	QJsonArray tasksArry = tasksValue.toArray();
	tasksArry.replace(index, task);
	tasksValue = tasksArry;
}
// ����һ������
void JsonConfig::addTask() {
	QJsonValueRef tasksValue = configJsonObj.find("tasks").value();
	QJsonArray tasksArry = tasksValue.toArray();
	QJsonObject newTaskObj;
	newTaskObj.insert("localPath","");
	newTaskObj.insert("name",QString::fromLocal8Bit("������"));
	QJsonArray newRemoteArry;
	newTaskObj.insert("remote", newRemoteArry);
	tasksArry.append(newTaskObj);
	tasksValue = tasksArry;
}
// ɾ��ָ������
void JsonConfig::deleteTask(int index) {
	QJsonValueRef tasksValue = configJsonObj.find("tasks").value();
	QJsonArray tasksArry = tasksValue.toArray();
	tasksArry.removeAt(index);
	tasksValue = tasksArry;
}

// ��������
void JsonConfig::saveJsonConfig(QString path) {
	QJsonDocument jsonDoc;       //����JSON�ĵ�
	QFile fileWrite(path);
	if (fileWrite.exists()) {
		fileWrite.open(QIODevice::WriteOnly | QIODevice::Text);
		jsonDoc.setObject(configJsonObj);
		fileWrite.seek(0);
		fileWrite.write(jsonDoc.toJson());
		fileWrite.flush();
		fileWrite.close();
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����ɹ�"), QMessageBox::Ok | QMessageBox::Ok);
	}
}