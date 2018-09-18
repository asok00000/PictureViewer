#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include "asynchronousfile.h"
class HttpLib : public QObject
{
	Q_OBJECT

public:
	HttpLib(QObject *parent = 0);
	~HttpLib();
	const QByteArray &getData()  const{ return m_data; }
signals:
	void requestFinished(QByteArray &byteArray);
    void downloadFinished();
    void progress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFileLocalPathChanged(QString filePath);
    void write(QByteArray data);
public:
	bool post(QString url);
	bool post(QString url,QByteArray data);
	bool post(QUrl url);
	bool post(QUrl url, QUrlQuery query,QByteArray data);
	bool get(QString url);
	bool get(QString url, QString data);
	bool get(QUrl url);
	bool get(QUrl url, QUrlQuery query);
    bool download(QString url,QString savedPath);
    bool download(QUrl url,QString savedPath);
private slots:
	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void readyRead();
    void finished();
	void error(QNetworkReply::NetworkError code);
	void sslErrors(const QList<QSslError> &errors);
private:
    void initAsyncFile();
    void destroyAsyncFile();
private:
	static QNetworkAccessManager *s_manager;
	QByteArray m_data;
    QString m_savedPath = "";
    bool m_isDownload = false;
    AsynchronousFile *m_asyncFile = nullptr;
};
