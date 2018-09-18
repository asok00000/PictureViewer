#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QEvent>
#include <QMouseEvent>
#include <QDir>
#include <QDebug>
#include <QStyle>
#include <QStyleOption>
#include <QTimer>
#include "httplib.h"
#define SAVED_DIR QString("./downloaded")

class Picture : public QWidget
{
	Q_OBJECT
    Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged)
    Q_PROPERTY(bool autoResize READ autoResize WRITE setAutoResize)
public:
    Picture(int delayLoadMSecs = -1,QWidget *parent = nullptr);
	~Picture();
    bool event(QEvent *event) override;
    QString imagePath() const;
    static void initHighPerformancePainter(QPainter *painter);
    bool autoResize() const;
    void setFixedSize(const QSize &fixedSize);
public slots:
    void setImagePath(QString imagePath);
    void load();
    void setAutoResize(bool autoResize);

private:
    bool m_imageLoaded = false;
    QImage m_image = QImage();
    QString m_imagePath = "";
    QString m_savedPath = "";
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void paintImage(QPainter *painter);
    bool m_autoResize = false;

signals:
    void imagePathChanged(QString imagePath);
};
