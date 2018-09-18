#pragma once

#include <QtWidgets/QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTimer>
#include "ui_pictureviewer.h"

#include "httplib.h"
#include "picture.h"
#include "flowlayout.h"

#define ALBUM_URL "http://act.vip.xunlei.com/ugirls/js/ugirlsdata.js"
class PictureViewer : public QMainWindow
{
	Q_OBJECT

public:
	PictureViewer(QWidget *parent = Q_NULLPTR);

private:
	Ui::PictureViewerClass ui;
    FlowLayout m_flowLayout;
};
