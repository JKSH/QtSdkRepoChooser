// Copyright (c) 2014 Sze Howe Koh
// This code is licensed under the MIT license (see LICENSE.MIT for details)

#ifndef GUI_H
#define GUI_H

#include "ui_gui.h"

class Gui : public QWidget, private Ui::Gui
{
	Q_OBJECT

public:
	explicit Gui(QWidget* parent = nullptr);

public slots:
	void setMirrorList(const QStringList& mirrors);
	void setLocalRepoUrl(const QString& url);

signals:
	void mirrorSelected(const QString& sdkArch, const QString& mirrorDomain) const;
};

#endif // GUI_H
