// Copyright (c) 2014 Sze Howe Koh
// This code is licensed under the MIT license (see LICENSE.MIT for details)

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QMap>

class QNetworkAccessManager;

class Downloader : public QObject
{
	Q_OBJECT

public:
	explicit Downloader(QObject* parent = nullptr);
	void loadMirrors();

signals:
	void mirrorsFound(const QStringList& mirrors) const;
	void metadataLoaded(const QString& sdkArch, const QString& rootUrl) const;

public slots:
	void useMirror(const QString& sdkArch, const QString& mirrorDomain);

private:
	QNetworkAccessManager* nam;

	QMap<QString, QString> mirrorMap;
	QString _sdkArch;
	QString _rootUrl;

	int expectedFileCount;
	int loadedFileCount;
	int expectedIndexCount;
	int loadedIndexCount;

	void initCounters();
	void getFilesFromIndex(const QString& rootUrl, const QString& relPath, const QStringList& wantedExtensions);
	void recordDownload();
};

#endif // DOWNLOADER_H
