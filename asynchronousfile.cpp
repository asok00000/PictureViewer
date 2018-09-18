#include "asynchronousfile.h"

AsynchronousFile::AsynchronousFile(QObject *parent)
    :QThread(parent)
{
    moveToThread(this);
}

AsynchronousFile::~AsynchronousFile()
{
    if(m_file != nullptr){
        m_file->flush();
        m_file->close();
        delete m_file;
        m_file = nullptr;
    }
	terminate();
}

QString AsynchronousFile::filePath() const
{
    return m_filePath;
}

void AsynchronousFile::setFilePath(QString filePath)
{
    if (m_filePath == filePath)
        return;

    m_filePath = filePath;
    emit filePathChanged(m_filePath);
    if(m_file != nullptr){
        m_file->flush();
        m_file->close();
        m_file->deleteLater();
    }
    m_file = new QFile(m_filePath);
    if(!m_file->open(QIODevice::ReadWrite)){
        qDebug()<<"Open file error "<<m_file->errorString();
    }
}

int AsynchronousFile::write(QByteArray data)
{
    if(data.isEmpty()) return 0;
    if(m_file != nullptr && m_file->isOpen()){
        return m_file->write(data);
    }
    else{
        qDebug()<<"File is not opened.";
    }
}

void AsynchronousFile::run()
{
    exec();
}
