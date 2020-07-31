#include "UpdateTools.h"
#include "Config.h"
#include <QStandardItemModel>

UpdateTools::UpdateTools(QWidget* parent)
	: QMainWindow(parent)
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
	/*����json�ļ�*/
	QFile file("config.json");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextCodec* codec = QTextCodec::codecForName("UTF8");
	QString value = codec->toUnicode(file.readAll());
	file.close();

	QJsonParseError parseJsonErr;
	QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), &parseJsonErr);
	if (!(parseJsonErr.error == QJsonParseError::NoError))
	{
		qDebug() << tr("����json�ļ�����");
		return;
	}
	QJsonObject jsonObject = document.object();
	winSCP = std::string(jsonObject["winSCP"].toString().toLocal8Bit());
	QString localPath = jsonObject["localPath"].toString();
	ui.lineEdit_localPath->setText(localPath);
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
				remoteInfoModel->setItem(i, 0, new QStandardItem(ip));
				remoteInfoModel->setItem(i, 1, new QStandardItem(user));
				remoteInfoModel->setItem(i, 2, new QStandardItem(passwd));
				remoteInfoModel->setItem(i, 3, new QStandardItem(path));
				// ���һ�����ӹ�ѡ��
				QCheckBox* checkBox = new QCheckBox();
				// ÿ�������һ�����ӹ�ѡ��
				ui.tableView_remote->setIndexWidget(remoteInfoModel->index(i, 4), checkBox);
			}
		}
	}
}

void UpdateTools::showRemoteInfo() {
	remoteInfoModel = new QStandardItemModel(this);
	/*�������ֶ���*/
	remoteInfoModel->setColumnCount(5);
	remoteInfoModel->setHeaderData(0, Qt::Horizontal, "ip");
	remoteInfoModel->setHeaderData(1, Qt::Horizontal, "user");
	remoteInfoModel->setHeaderData(2, Qt::Horizontal, "passwd");
	remoteInfoModel->setHeaderData(3, Qt::Horizontal, "path");
	remoteInfoModel->setHeaderData(4, Qt::Horizontal, "clicked");
	ui.tableView_remote->setModel(remoteInfoModel);
	ui.tableView_remote->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

// ����һ��Զ�̷�������Ϣ
void UpdateTools::addRemoteInfo() {
	int row = remoteInfoModel->rowCount();
	remoteInfoModel->setItem(row, 0, new QStandardItem(""));
	remoteInfoModel->setItem(row, 1, new QStandardItem(""));
	remoteInfoModel->setItem(row, 2, new QStandardItem(""));
	remoteInfoModel->setItem(row, 3, new QStandardItem(""));
	remoteInfoModel->setItem(row, 4, new QStandardItem(""));
	QCheckBox* checkBox = new QCheckBox();
	connect(checkBox, SIGNAL(clicked(bool)), this, SLOT(onCheckBoxTotalClicked(bool)));
	// ÿ�������һ�����ӹ�ѡ��
	ui.tableView_remote->setIndexWidget(remoteInfoModel->index(remoteInfoModel->rowCount() - 1, 4), checkBox);
}

// ɾ��ѡ����Զ�̷�������Ϣ
void UpdateTools::deleteRemoteInfo() {
	// ����Զ�̷�������Ϣ��
	for (size_t i = 0; i < remoteInfoModel->rowCount(); i++)
	{
		QCheckBox* box = (QCheckBox*)ui.tableView_remote->indexWidget(remoteInfoModel->index(i, 4));
		// ����ǰ�з�������Ϣ���ѹ�ѡ�������
		if (box != NULL && box->checkState())
		{
			remoteInfoModel->removeRow(i);
		}
	}
}

// �������޸ĵ�Զ�̷�������Ϣ�����������ļ�
void UpdateTools::saveRemoteInfo() {

}


// ��ѡȫѡ��ť�����¼�
void UpdateTools::onCheckBoxTotalClicked(bool clicked) {

}