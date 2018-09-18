#include "picture.h"

Picture::Picture(int delayLoadMSecs, QWidget *parent)
	: QWidget(parent)
{
    QDir _dir;
    _dir.mkpath(SAVED_DIR);
    setStyleSheet("border:2px solid #FFF;padding:2px 2px 2px 2px;");
    if(delayLoadMSecs > 0)
    {
        QTimer::singleShot(delayLoadMSecs<100?100:delayLoadMSecs,[&](){
            load();
        });
    }
}

Picture::~Picture()
{
}

void Picture::paintEvent(QPaintEvent *event)
{
    QPainter _p(this);
    initHighPerformancePainter(&_p);
    if(m_imageLoaded){
        paintImage(&_p);
    }
    QStyleOption _opt;
    _opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &_opt, &_p, this);
}

void Picture::paintImage(QPainter *painter)
{
    QRect &&_contentRect = contentsRect();
    int _contentWidth = _contentRect.width();
    int _contentHeight = _contentRect.height();
    int _imageWidth = m_image.width();
    int _imageHeight = m_image.height();
    if(_contentWidth>=_imageWidth  && _contentHeight >= _imageHeight)
    {
        int x = 0 ,y = 0;
        x = _contentWidth / 2 - _imageWidth / 2;
        y = _contentHeight / 2 - _imageHeight / 2;
        painter->drawImage(x,y,m_image);
    }
    else
    {
        QImage && _scaledImage = m_image.scaledToHeight(_contentHeight,Qt::SmoothTransformation);
        if(_scaledImage.width() > _contentWidth)
        {
            QImage && _scaledImage = m_image.scaledToWidth(_contentWidth,Qt::SmoothTransformation);
            int y = _contentHeight / 2 - _scaledImage.height() / 2;
            painter->drawImage(0,y,_scaledImage);
        }
        else
        {
            int x = _contentWidth / 2 - _scaledImage.width() / 2;
            painter->drawImage(x,0,_scaledImage);
        }
    }
}

bool Picture::event(QEvent *event)
{
    return QWidget::event(event);
}

QString Picture::imagePath() const
{
    return m_imagePath;
}

void Picture::initHighPerformancePainter(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
}

bool Picture::autoResize() const
{
    return m_autoResize;
}

void Picture::setFixedSize(const QSize &fixedSize)
{
    if(!m_autoResize)
    {
       QWidget::setFixedSize(fixedSize);
    }
}

void Picture::setImagePath(QString imagePath)
{
    if (m_imagePath == imagePath)
        return;
    m_imagePath = imagePath;
    emit imagePathChanged(m_imagePath);
}

void Picture::load()
{
    QUrl _url = QUrl::fromUserInput(m_imagePath);
    if(_url.isValid()){
        if(_url.isLocalFile()){
            if(_url.isRelative()){
                QDir _dir;
                m_imagePath =_dir.absoluteFilePath(m_imagePath);
            }
            m_savedPath = m_imagePath;
            bool _bRet = m_image.load(m_savedPath);
            if(!_bRet)
                qWarning()<<QString("Load %1 failed.").arg(m_savedPath);
            else
            {
                m_imageLoaded = true;
                if(m_imageLoaded)QWidget::setFixedSize(m_image.size());
                update();
            }
        }
        else{
            m_savedPath = SAVED_DIR+"/"+QString("%1").arg(QDateTime::currentSecsSinceEpoch());
            HttpLib *_httpLib = new HttpLib();
            connect(_httpLib,&HttpLib::downloadFinished,this,[&](){
                bool _bRet = m_image.load(m_savedPath);
                if(!_bRet)
                    qWarning()<<QString("Load %1 failed.").arg(m_savedPath);
                else
                {
                    m_imageLoaded = true;
                    if(m_imageLoaded)QWidget::setFixedSize(m_image.size());
                    update();
                }
            });
            _httpLib->download(_url,m_savedPath);

        }
    }
}

void Picture::setAutoResize(bool autoResize)
{
    m_autoResize = autoResize;
    if(m_imageLoaded)QWidget::setFixedSize(m_image.size());
}
