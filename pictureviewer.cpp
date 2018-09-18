#include "pictureviewer.h"

PictureViewer::PictureViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    ui.scrollAreaWidgetContents->setLayout(&m_flowLayout);
//    Picture *_pic = new Picture(this);
//    _pic->resize(400/3,600/3);
//    _pic->setImagePath("http://img.vip.kanimg.com/img/banner/201608111301527900.jpg");
//    _pic->load();
//	_pic->show();
    HttpLib *_httpLib = new HttpLib;
    connect(_httpLib,&HttpLib::requestFinished,[&](QByteArray &data){
        QString _data = data;
        _data.remove("var ugirlsData=");
        QJsonDocument doc = QJsonDocument::fromJson(_data.toUtf8());
        QJsonArray jAlbums = doc.array();
        foreach(QJsonValue value, jAlbums)
        {
            QJsonObject _obj = value.toObject();
//            QZuesNetImg *img = new QZuesNetImg(obj["img_upload"].toString(), ui.wdgAlbumContainer);
//            connect(img, &QZuesNetImg::signalDbClicked, this, [&](AlbumInfo albumInfo) {
//                ui.stkContent->setCurrentWidget(ui.pageDetailes);
//                QString urlTemp = "http://data.meitu.xunlei.com/data/image/%1/%2.jpg";
//                urlTemp = urlTemp.arg(albumInfo.resourceId);
//                for (int i = 0 ; i<albumInfo.counts; ++ i)
//                {
//                    QZuesNetImg *img = new QZuesNetImg(urlTemp.arg(i), ui.wdgAlbumContainer);
//                    img->setMinimumSize(300, 600);
//                    detailsLayout->addWidget(img);
//                }
//            });
            Picture *_picture = new Picture(500,this);
            _picture->setImagePath(_obj["img_upload"].toString());
            _picture->setAutoResize(true);
            _picture->setFixedSize(QSize(300, 600));
            m_flowLayout.addWidget(_picture);
        }
    });
    _httpLib->get(ALBUM_URL);
}
