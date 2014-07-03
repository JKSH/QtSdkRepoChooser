// Copyright (c) 2014 Sze Howe Koh
// This code is licensed under the MIT license (see LICENSE.MIT for details)

#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include "gui.h"
#include "downloader.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// cd into the folder which contains the repo
	QString dataPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
	QDir dir(dataPath);
	dir.mkpath(".");
	QDir::setCurrent(dataPath);

	Gui gui;
	Downloader downloader;

	QObject::connect(&downloader, &Downloader::mirrorsFound,
			&gui, &Gui::setMirrorList);

	QObject::connect(&gui, &Gui::mirrorSelected,
			&downloader, &Downloader::useMirror);

	QObject::connect(&downloader, &Downloader::metadataLoaded, [&gui](const QString& sdkArch, const QString& rootUrl)
	{
		qDebug() << "Patching Updates.xml...";
		QString subdir = "online/qtsdkrepository/" + sdkArch + "/root/qt/";

		QFile updateFile(subdir + "Updates.xml");
		if (!updateFile.open(QFile::ReadWrite|QFile::Text))
		{
			qDebug() << "\tERROR: Unable to open Updates.xml";
			return;
		}

		// ASSUMPTION: rootUrl ends with '/'
		QString contents = updateFile.readAll();
		contents.replace("http://download.qt-project.org/", rootUrl);
		updateFile.resize(0);
		updateFile.write(contents.toUtf8());
		updateFile.close();

		qDebug() << "\tDone\n";

		qDebug() << "===========";
		qDebug() << "Restart this program to choose a new mirror.\n";
		qDebug() << "Copy the Local Repo URL below. Paste it as your User Defined Repository in your online installer (Maintenance Tool). See the User Manual for instructions.";

		QFileInfo fi(updateFile);
		gui.setLocalRepoUrl( QUrl::fromLocalFile(fi.canonicalPath()).toString() );
	});

	gui.show();
	downloader.loadMirrors();
	return a.exec();
}
