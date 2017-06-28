// Copyright (c) 2017 Sze Howe Koh
// This code is licensed under the MIT license (see LICENSE.MIT for details)

#include "downloader.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QDir>

Downloader::Downloader(QObject* parent)
	: QObject(parent)
	, nam(new QNetworkAccessManager(this))
{}

void
Downloader::loadMirrors()
{
	qDebug() << "Downloading mirror list...";

	// ASSUMPTION: All mirrors host packages for all architectures
	// ASSUMPTION: Updates.xml is the core file
	QNetworkRequest req(QUrl("http://download.qt.io/online/qtsdkrepository/windows_x86/root/qt/Updates.xml.mirrorlist"));
	auto reply = nam->get(req);
	connect(reply, &QNetworkReply::finished, [=]
	{
		QString page = reply->readAll();
		reply->deleteLater();
		if (reply->error() != QNetworkReply::NoError)
			return;

		const QStringList mirrorGroups
		{
			"mirrors which handle this country", // Country-wide group
			"mirrors in other countries, but same continent", // Continent-wide group
			"mirrors in other parts of the world" // Everywhere else
		};

		QStringList tmp;
		for (const QString& group : mirrorGroups)
		{
			if (page.contains(group))
			{
				// Found the closest group. Stop looking.
				tmp = page.split(group);
				break;
			}
		}

		if (tmp.size() < 2)
		{
			qDebug() << "\tERROR: Malformed file:\n" << page;
			return;
		}

		// ASSUMPTION: After the heading "Found N mirrors in other parts of the world", only
		//             the mirrored file URLs are in a list
		QStringList mirrors = tmp[1].split("<li><a href=\"");
		mirrors.removeFirst();

		for (int i = 0; i < mirrors.size(); ++i)
		{
			QString rootUrl = mirrors[i].split("online/qtsdkrepository").first();

			// Replace list with domains only
			QString& domain = mirrors[i];
			domain = rootUrl;
			domain.remove("http://");
			domain = domain.split('/').first();

			mirrorMap[domain] = rootUrl;
		}

		qDebug() << "\tDone";

		// The original order, with the "best" mirror on top, is preserved
		emit mirrorsFound(mirrors);

		// ASSUMPTION: There will be at least 1 mirror found
	});

	connect(reply, ( void (QNetworkReply::*)(QNetworkReply::NetworkError) )&QNetworkReply::error, [=]
	{
		qDebug() << "\tERROR:" << reply->errorString();
	});
}


//=============
// PUBLIC SLOTS

void
Downloader::useMirror(const QString& sdkArch, const QString& mirrorDomain)
{
	_sdkArch = sdkArch;
	_rootUrl = mirrorMap[mirrorDomain];

	QString subdir = "online/qtsdkrepository/" + sdkArch + "/root/qt/";

	// Delete old files
	QDir place(subdir + "qt/");
	place.removeRecursively();
	place.mkpath(".");

	initCounters();

	qDebug() << "Downloading raw metadata...";
	getFilesFromIndex(_rootUrl, subdir,
			{".xml"});
	getFilesFromIndex(_rootUrl, subdir + "qt/",
			{".7z", ".sha1"});
}


// =======
// PRIVATE

void
Downloader::initCounters()
{
	expectedFileCount = 0;
	loadedFileCount = 0;
	expectedIndexCount = 0;
	loadedIndexCount = 0;
}

static bool
isWanted(const QString& link, const QStringList& wantedExtensions)
{
	for (const QString& ext : wantedExtensions)
	{
		if (link.endsWith(ext))
			return true;
	}
	return false;
}

void
Downloader::getFilesFromIndex(const QString& rootUrl, const QString& relPath, const QStringList& wantedExtensions)
{
	// ASSUMPTION: relPath ends with '/'

	++expectedIndexCount;
	QNetworkRequest indexReq(QUrl(rootUrl + relPath));
	QNetworkReply* indexReply = nam->get(indexReq);
	connect(indexReply, &QNetworkReply::finished, [=]
	{
		++loadedIndexCount;

		QString page = indexReply->readAll();
		indexReply->deleteLater();

		// ASSUMPTION: There's no whitespace around the '='
		QStringList links = page.split("a href=\"");
		links.removeFirst();

		QStringList wantedFiles;
		for (int i = 0; i < links.size(); ++i)
		{
			QString link = links[i].split('"').at(0);
			if (isWanted(link, wantedExtensions))
				wantedFiles << link;
		}

		expectedFileCount += wantedFiles.size();
		for (const QString& filename : wantedFiles)
		{
			QNetworkRequest fileReq(QUrl(rootUrl + relPath + filename));
			auto fileReply = nam->get(fileReq);
			connect(fileReply, &QNetworkReply::finished, [=]
			{
				qDebug() << "\tLoaded" << fileReply->url().toString();

				QByteArray data = fileReply->readAll();
				fileReply->deleteLater();

				QFile fileOut(relPath + filename);
				if (!fileOut.open(QFile::WriteOnly))
				{
					qDebug() << "\tERROR: Unable to open file for writing:" << fileOut.fileName();
					return;
				}

				fileOut.write(data);
				fileOut.close();

				this->recordDownload();
			});

			connect(fileReply, ( void (QNetworkReply::*)(QNetworkReply::NetworkError) )&QNetworkReply::error, [=]
			{
				qDebug() << "\tERROR:" << fileReply->errorString();
			});
		}
	});

	connect(indexReply, ( void (QNetworkReply::*)(QNetworkReply::NetworkError) )&QNetworkReply::error, [=]
	{
		qDebug() << "\tERROR:" << indexReply->errorString();
	});
}

void
Downloader::recordDownload()
{
	++loadedFileCount;

	if (loadedFileCount == expectedFileCount && loadedIndexCount == expectedIndexCount)
		emit metadataLoaded(_sdkArch, _rootUrl);
}
