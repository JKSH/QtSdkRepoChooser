// Copyright (c) 2014 Sze Howe Koh
// This code is licensed under the MIT license (see LICENSE.MIT for details)

#include "gui.h"
#include <QDebug>

QTextEdit* logWidget;
static void printMessage(QtMsgType, const QMessageLogContext&, const QString &msg)
{
	logWidget->append(msg);
}

Gui::Gui(QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);
	logWidget = textEdit;
	qInstallMessageHandler(&printMessage);

	comboBox_os->addItems({
			"linux_x64",
			"linux_x86",
			"mac_x64",
			"windows_x86"
	});

#if defined(Q_OS_LINUX)
	comboBox_os->setCurrentText("linux_x64");
#elif defined(Q_OS_OSX)
	comboBox_os->setCurrentText("mac_x64");
#elif defined(Q_OS_WIN32)
	comboBox_os->setCurrentText("windows_x86");
#endif

	connect(pushButton_mirror, &QPushButton::clicked, [=]
	{
		pushButton_mirror->setEnabled(false);
		emit mirrorSelected(comboBox_os->currentText(), comboBox_mirror->currentText());
	});
}

void
Gui::setMirrorList(const QStringList& mirrors)
{
	comboBox_mirror->clear();
	comboBox_mirror->addItems(mirrors);
	pushButton_mirror->setEnabled(mirrors.count() > 0);
}

void
Gui::setLocalRepoUrl(const QString& url)
{
	lineEdit_localUrl->setText(url);
	lineEdit_localUrl->setEnabled(true);
}
