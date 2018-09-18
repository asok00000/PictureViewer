#ifndef ASYNCHRONOUSFILE_H
#define ASYNCHRONOUSFILE_H

#include <QThread>
#include <QFile>
#include <QDebug>
class AsynchronousFile : public QThread
{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)

public:
    AsynchronousFile(QObject *parent = 0);
    ~AsynchronousFile();
    // QThread interface
    QString filePath() const;

public slots:
    void setFilePath(QString filePath);
    int write(QByteArray data);
signals:
    void filePathChanged(QString filePath);

protected:
    void run() override;
private:
    QString m_filePath = "";
    QFile *m_file = nullptr;
};

#endif // ASYNCHRONOUSFILE_H
