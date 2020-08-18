#include "UpdateTools.h"
#include "Config.h"


UpdateTools::UpdateTools(QWidget* parent)
	: QMainWindow(parent), localPath(""), taskModel(NULL), jsCfg(NULL)
{
	ui.setupUi(this);
	connect(ui.pushButton_upLoad, SIGNAL(clicked()), this, SLOT(upLoad()));
	connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(addRemoteInfo()));
	connect(ui.pushButton_delete, SIGNAL(clicked()), this, SLOT(deleteRemoteInfo()));
	connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(saveRemoteInfo()));
	connect(ui.toolButton_choseLocalPath, SIGNAL(clicked()), this, SLOT(onCheckChoseLocalFileButton()));
	connect(ui.listView_task, SIGNAL(clicked(QModelIndex)), this, SLOT(showTaskDetail(QModelIndex)));
	connect(ui.action_newTask,SIGNAL(triggered()),this,SLOT(onCheckActionNewTask()));
	connect(ui.action_deleteTask,SIGNAL(triggered()),this,SLOT(onCheckActionDeleteTask()));
	connect(ui.action_saveTask,SIGNAL(triggered()),this,SLOT(onCheckActionSaveCfg()));
	remoteInfoModel = new QStandardItemModel(this);
	QStringList title;
	title << QString("ip") << QString("user") << QString("passwd") << QString("path") << QString::fromLocal8Bit("勾选");
	/*设置列字段名*/
	remoteInfoModel->setColumnCount(5);
	remoteInfoModel->setHorizontalHeaderLabels(title);
	ui.tableView_remote->setModel(remoteInfoModel);
	jsCfg = JsonConfig("config.json");
	showTasks();
}

void UpdateTools::showTasks() {
	/*解析json文件*/
	jsCfg.loadJsonConfig();
	QJsonObject jsonObject = jsCfg.configJsonObj;
	winSCP = std::string(jsonObject["winSCP"].toString().toLocal8Bit());
	if (jsonObject.contains(QStringLiteral("tasks")))
	{
		QStringList l;
		QJsonValue arrayValue = jsonObject.value(QStringLiteral("tasks"));
		if (arrayValue.isArray())
		{
			QJsonArray array = arrayValue.toArray();
			for (int i = 0; i < array.size(); i++)
			{
				QJsonValue taskArray = array.at(i);
				QJsonObject taskObject = taskArray.toObject();
				QString taskName = taskObject["name"].toString();
				l.append(taskName);
			}
		}
		taskModel = new QStringListModel(l);
		ui.listView_task->setModel(taskModel);
	}
}

void UpdateTools::showTaskDetail(QModelIndex index)
{
	QString task = ui.listView_task->model()->data(index).toString();
	//QWidget* newTab = new QWidget();
	//ui.tabWidget_task->addTab(newTab, task);
	remoteInfoModel->clear();
	QStringList title;
	title << QString("ip") << QString("user") << QString("passwd") << QString("path") << QString::fromLocal8Bit("勾选");
	/*设置列字段名*/
	remoteInfoModel->setColumnCount(5);
	remoteInfoModel->setHorizontalHeaderLabels(title);
	ui.tableView_remote->setModel(remoteInfoModel);
	ui.tabWidget_task->setTabText(0,task);
	QJsonObject jsonObject = jsCfg.configJsonObj;
	if (jsonObject.contains(QStringLiteral("tasks")))
	{
		QJsonValue tasksValue = jsonObject.value(QStringLiteral("tasks"));
		QJsonValue taskValue = tasksValue.toArray().at(index.row());
		localPath = taskValue.toObject().value(QStringLiteral("localPath")).toString();
		ui.lineEdit_localPath->setText(localPath);
		QJsonValue remotesArray = taskValue.toObject().value(QStringLiteral("remote"));
		QJsonArray remotes = remotesArray.toArray();
		for (int i = 0; i < remotes.size(); i++)
		{
			QJsonValue remoteArray = remotes.at(i);
			QJsonObject remoteObject = remoteArray.toObject();
			QString ip = remoteObject["ip"].toString();
			QString user = remoteObject["user"].toString();
			QString passwd = remoteObject["passwd"].toString();
			QString path = remoteObject["path"].toString();
			remoteInfoModel->setItem(i, 0, new QStandardItem(ip));
			remoteInfoModel->setItem(i, 1, new QStandardItem(user));
			remoteInfoModel->setItem(i, 2, new QStandardItem(passwd));
			remoteInfoModel->setItem(i, 3, new QStandardItem(path));
			// 最后一列增加勾选框
			QCheckBox* checkBox = new QCheckBox();
			// 每次在最后一行增加勾选框
			ui.tableView_remote->setIndexWidget(remoteInfoModel->index(i, 4), checkBox);
		}

	}
}

void UpdateTools::upLoad() {
	commandVector.clear();
	localPath = ui.lineEdit_localPath->text();
	QString localPathFinal;
	if (localPath.contains("\\"))
	{
		localPathFinal = localPath.replace("\\", "\\\\");
	}
	else
	{
		localPathFinal = localPath.replace("\/", "\\\\");
	}
	for (size_t i = 0; i < remoteInfoModel->rowCount(); i++)
	{
		QString ip = remoteInfoModel->data(remoteInfoModel->index(i, 0)).toString();
		QString user = remoteInfoModel->data(remoteInfoModel->index(i, 1)).toString();
		QString passwd = remoteInfoModel->data(remoteInfoModel->index(i, 2)).toString();
		QString path = remoteInfoModel->data(remoteInfoModel->index(i, 3)).toString();
		QString remote = user + ":" + passwd + "@" + ip;
		std::string command = " \"open " + std::string(remote.toLocal8Bit()) + "\" \"option transfer binary\" \"put " + std::string(localPathFinal.toLocal8Bit()) + " " + std::string(path.toLocal8Bit()) + "\"" + " \"close\"";
		commandVector.push_back(command);
	}
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

// 新增一行远程服务器信息
void UpdateTools::addRemoteInfo() {
	int row = remoteInfoModel->rowCount();
	remoteInfoModel->setItem(row, 0, new QStandardItem(""));
	remoteInfoModel->setItem(row, 1, new QStandardItem(""));
	remoteInfoModel->setItem(row, 2, new QStandardItem(""));
	remoteInfoModel->setItem(row, 3, new QStandardItem(""));
	remoteInfoModel->setItem(row, 4, new QStandardItem(""));
	QCheckBox* checkBox = new QCheckBox();
	connect(checkBox, SIGNAL(clicked(bool)), this, SLOT(onCheckBoxTotalClicked(bool)));
	// 每次在最后一行增加勾选框
	ui.tableView_remote->setIndexWidget(remoteInfoModel->index(remoteInfoModel->rowCount() - 1, 4), checkBox);
}

// 删除选中行远程服务器信息
void UpdateTools::deleteRemoteInfo() {
	int row = remoteInfoModel->rowCount();
	// 遍历远程服务器信息表
	for (size_t i = 0; i < row; i++)
	{
		QCheckBox* box = (QCheckBox*)ui.tableView_remote->indexWidget(remoteInfoModel->index(i, 4));
		// 若当前行服务器信息表已勾选，则操作
		if (box->checkState())
		{
			remoteInfoModel->removeRow(i);
			--row;
			--i;
		}
	}
}

// 将界面修改的远程服务器信息保存至配置文件
void UpdateTools::saveRemoteInfo() {
	int index = ui.listView_task->currentIndex().row();
	QJsonObject newTaskObj;
	newTaskObj.insert("name", ui.tabWidget_task->tabText(0));
	localPath = ui.lineEdit_localPath->text();
	newTaskObj.insert("localPath", localPath);
	// 删除原有远程服务器配置
	QJsonArray remoteArray;
	// 遍历远程服务器信息表
	for (size_t i = 0; i < remoteInfoModel->rowCount(); i++)
	{
		QString ip = remoteInfoModel->data(remoteInfoModel->index(i, 0)).toString();
		QString user = remoteInfoModel->data(remoteInfoModel->index(i, 1)).toString();
		QString passwd = remoteInfoModel->data(remoteInfoModel->index(i, 2)).toString();
		QString path = remoteInfoModel->data(remoteInfoModel->index(i, 3)).toString();
		QJsonObject obj;
		obj.insert("id", (int)i);
		obj.insert("ip", ip);
		obj.insert("user", user);
		obj.insert("passwd", passwd);
		obj.insert("path", path);
		remoteArray.append(obj);
	}
	newTaskObj.insert("remote", remoteArray);
	jsCfg.updateTask(index, newTaskObj);
	jsCfg.saveJsonConfig("config.json");
}


// 勾选全选按钮触发事件
void UpdateTools::onCheckBoxTotalClicked(bool clicked) {

}

// 点击加载本地目录事件
void UpdateTools::onCheckChoseLocalFileButton() {
	localPath = QFileDialog::getExistingDirectory();
	ui.lineEdit_localPath->setText(localPath);
}

void UpdateTools::onCheckActionNewTask() {
	taskModel->insertRow(taskModel->rowCount()); //在尾部插入一空行
	QModelIndex index = taskModel->index(taskModel->rowCount() - 1, 0);//获取最后一行
	taskModel->setData(index, QString::fromLocal8Bit("新任务"), Qt::DisplayRole);//设置显示文字
	jsCfg.addTask();
}

void UpdateTools::onCheckActionDeleteTask() {
	QModelIndex index = ui.listView_task->currentIndex();
	taskModel->removeRow(index.row());
	jsCfg.deleteTask(index.row());
}

void UpdateTools::onCheckActionSaveCfg() {
	jsCfg.saveJsonConfig("config.json");
}