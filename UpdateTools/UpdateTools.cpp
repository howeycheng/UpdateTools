#include "UpdateTools.h"
#include "Config.h"
#include <QStandardItemModel>

UpdateTools::UpdateTools(QWidget* parent)
	: QMainWindow(parent), remoteCount(0)
{
	ui.setupUi(this);
	connect(ui.pushButton_upLoad, SIGNAL(clicked()), this, SLOT(upLoad()));
	connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(addRemoteInfo()));
	connect(ui.pushButton_delete, SIGNAL(clicked()), this, SLOT(deleteRemoteInfo()));
	showRemoteInfo();
	readJsonConfig();
}

void UpdateTools::upLoad() {
	std::string exe = winSCP + " /console /command \"option batch continue\" \"option confirm off\"";
	auto iter = commandVector.begin();
	while (iter != commandVector.end())
	{
		exe += *iter;
		++iter;
	}
	exe += " /log=log_file.txt";
	WinExec(exe.c_str(), SW_SHOW);
}

void UpdateTools::readJsonConfig() {
	/*解析json文件*/
	QFile file("config.json");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextCodec* codec = QTextCodec::codecForName("UTF8");
	QString value = codec->toUnicode(file.readAll());
	file.close();

	QJsonParseError parseJsonErr;
	QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
	if (!(parseJsonErr.error == QJsonParseError::NoError))
	{
		qDebug() << tr("解析json文件错误！");
		return;
	}
	QJsonObject jsonObject = document.object();
	winSCP = std::string(jsonObject["winSCP"].toString().toLocal8Bit());
	QString localPath = jsonObject["localPath"].toString();
	ui.textEdit_localPath->append(localPath);
	if (jsonObject.contains(QStringLiteral("remote")))
	{
		QJsonValue arrayValue = jsonObject.value(QStringLiteral("remote"));
		if (arrayValue.isArray())
		{
			QJsonArray array = arrayValue.toArray();
			for (int i = 0; i < array.size(); i++)
			{
				QJsonValue remoteArray = array.at(i);
				QJsonObject remoteObject = remoteArray.toObject();
				QString ip = remoteObject["ip"].toString();
				QString user = remoteObject["user"].toString();
				QString passwd = remoteObject["passwd"].toString();
				QString path = remoteObject["path"].toString();
				QString remote = user + ":" + passwd + "@" + ip;
				std::string command = " \"open " + std::string(remote.toLocal8Bit()) + "\" \"option transfer binary\" \"put " + std::string(localPath.toLocal8Bit()) + " " + std::string(path.toLocal8Bit()) + "\"" + " \"close\"";
				commandVector.push_back(command);
				remoteCount = i;
				remoteInfoModel->setItem(i, 0, new QStandardItem(ip));
				remoteInfoModel->setItem(i, 1, new QStandardItem(user));
				remoteInfoModel->setItem(i, 2, new QStandardItem(passwd));
				remoteInfoModel->setItem(i, 3, new QStandardItem(path));
			}
		}
	}
}

void UpdateTools::showRemoteInfo() {
	remoteInfoModel = new QStandardItemModel(this);
	/*设置列字段名*/
	remoteInfoModel->setColumnCount(4);
	remoteInfoModel->setHeaderData(0, Qt::Horizontal, "ip");
	remoteInfoModel->setHeaderData(1, Qt::Horizontal, "user");
	remoteInfoModel->setHeaderData(2, Qt::Horizontal, "passwd");
	remoteInfoModel->setHeaderData(3, Qt::Horizontal, "path");
	ui.tableView_remote->setModel(remoteInfoModel);
}

// 新增一行远程服务器信息
void UpdateTools::addRemoteInfo() {
	remoteInfoModel->setItem(remoteCount + 1, 0, new QStandardItem(""));
	remoteInfoModel->setItem(remoteCount + 1, 1, new QStandardItem(""));
	remoteInfoModel->setItem(remoteCount + 1, 2, new QStandardItem(""));
	remoteInfoModel->setItem(remoteCount + 1, 3, new QStandardItem(""));
	++remoteCount;
}

// 删除选中行远程服务器信息
void UpdateTools::deleteRemoteInfo() {

	int rowToDel = ui.tableView_remote->currentIndex().row();
	remoteInfoModel->removeRow(rowToDel);
	--remoteCount;
}

// 将界面修改的远程服务器信息保存至配置文件
void UpdateTools::saveRemoteInfo() {
	


}