#include "httplib.h"

QNetworkAccessManager *HttpLib::s_manager = new QNetworkAccessManager;
HttpLib::HttpLib(QObject *parent)
	: QObject(parent)
{
}

HttpLib::~HttpLib()
{
}

bool HttpLib::post(QString url)
{
	if (url.indexOf("http://") != 0)url = "http://" + url;
	QUrl _url(url);
	if (_url.isValid())
		return post(_url);
	return false;
}

bool HttpLib::post(QString url, QByteArray data)
{
	if (data == "")
	{
		return post(url);
	}
	else
	{
		QUrl _url(url);
		if (_url.isValid())
			return post(_url, QUrlQuery(),data);
	}
	return false;
}

bool HttpLib::post(QUrl url)
{
	if (url.isValid())
		return post(url, QUrlQuery(),QByteArray());
	return false;
}

bool HttpLib::post(QUrl url, QUrlQuery query, QByteArray data)
{
	if (url.isValid())
	{
		QNetworkRequest _request;
		url.setQuery(query);
		_request.setUrl(url);
		QNetworkReply *_reply =  s_manager->post(_request,data);
		connect(_reply, &QNetworkReply::downloadProgress, this, &HttpLib::downloadProgress, Qt::UniqueConnection);
		connect(_reply, &QNetworkReply::readyRead, this, &HttpLib::readyRead, Qt::UniqueConnection);
		connect(_reply, &QNetworkReply::finished, this, &HttpLib::finished, Qt::UniqueConnection);
		connect(_reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError) > (&QNetworkReply::error), this, &HttpLib::error, Qt::UniqueConnection);
		connect(_reply, &QNetworkReply::sslErrors, this, &HttpLib::sslErrors, Qt::UniqueConnection);
	}
	return false;
}

bool HttpLib::get(QString url)
{
	if (url.indexOf("http://") != 0)url = "http://" + url;
	QUrl _url(url);
	if (_url.isValid())
	{
		return get(_url);
	}
	return false;
}

bool HttpLib::get(QString url, QString data)
{
	QUrl _url(url);
	if (_url.isValid())
	{
		return get(_url, QUrlQuery(data));
	}
	return false;
}

bool HttpLib::get(QUrl url)
{
	if (url.isValid())
	{
		return get(url, QUrlQuery());
	}
	return false;
}

bool HttpLib::get(QUrl url, QUrlQuery query)
{
	if (url.isValid())
	{
		QNetworkRequest _request;
		url.setQuery(query);
		_request.setUrl(url);
		QNetworkReply *_reply = s_manager->get(_request);
		connect(_reply, &QNetworkReply::downloadProgress, this, &HttpLib::downloadProgress, Qt::UniqueConnection);
		connect(_reply, &QNetworkReply::readyRead, this, &HttpLib::readyRead, Qt::UniqueConnection);
		connect(_reply, &QNetworkReply::finished, this, &HttpLib::finished, Qt::UniqueConnection);
		connect(_reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)> (&QNetworkReply::error), this, &HttpLib::error, Qt::UniqueConnection);
		connect(_reply, &QNetworkReply::sslErrors, this, &HttpLib::sslErrors, Qt::UniqueConnection);
	}
    return false;
}

bool HttpLib::download(QString url, QString savedPath)
{
    QUrl _url(url);
    if(_url.isValid()){
        return download(_url,savedPath);
    }
    return false;
}

bool HttpLib::download(QUrl url, QString savedPath)
{
    if(url.isValid()){
        m_savedPath = savedPath;
		m_isDownload = true;
		initAsyncFile();
		emit downloadFileLocalPathChanged(m_savedPath);
        QNetworkRequest _request;
        _request.setUrl(url);
        QNetworkReply *_reply = s_manager->get(_request);
        connect(_reply, &QNetworkReply::downloadProgress, this, &HttpLib::downloadProgress, Qt::UniqueConnection);
        connect(_reply, &QNetworkReply::readyRead, this, &HttpLib::readyRead, Qt::UniqueConnection);
        connect(_reply, &QNetworkReply::finished, this, &HttpLib::finished, Qt::UniqueConnection);
        connect(_reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)> (&QNetworkReply::error), this, &HttpLib::error, Qt::UniqueConnection);
        connect(_reply, &QNetworkReply::sslErrors, this, &HttpLib::sslErrors, Qt::UniqueConnection);
    }
    return false;
}

void HttpLib::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit progress(bytesReceived,bytesTotal);
}

void HttpLib::readyRead()
{
    auto _reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(m_isDownload){
        emit write(_reply->readAll());
    }
    else{
        m_data.append(_reply->readAll());
    }
}

void HttpLib::finished()
{
    auto _reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if(m_isDownload){
        QByteArray && _data= _reply->readAll();
        if(!_data.isEmpty()) emit write(_data);
        destroyAsyncFile();
        emit downloadFinished();
    }
    else{
        m_data.append(_reply->readAll());
        emit requestFinished(m_data);
    }

}

void HttpLib::error(QNetworkReply::NetworkError code)
{
	qDebug() << "http error:" << code;
}

void HttpLib::sslErrors(const QList<QSslError>& errors)
{
    foreach (QSslError error, errors)
	{
		qDebug() << "http ssl error:" << error;
    }
}

void HttpLib::initAsyncFile()
{
    if(m_asyncFile == nullptr){
        m_asyncFile = new AsynchronousFile;
        connect(this,&HttpLib::downloadFileLocalPathChanged,m_asyncFile,&AsynchronousFile::setFilePath,Qt::QueuedConnection);
        connect(this,&HttpLib::write,m_asyncFile,&AsynchronousFile::write,Qt::QueuedConnection);
        m_asyncFile->start();
    }
}

void HttpLib::destroyAsyncFile()
{
    if(m_asyncFile != nullptr){
        m_asyncFile->deleteLater();
        m_asyncFile = nullptr;
    }
}
