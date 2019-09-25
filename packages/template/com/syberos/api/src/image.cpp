#include "image.h"
#include "helper.h"
#include <QObject>
#include <QDebug>

int Image::typeId = qRegisterMetaType<Image *>();
Image::Image()
{

}
Image::~Image()
{

}

void Image::request(QString callBackID, QString actionName, QVariantMap params)
{
    Q_UNUSED(callBackID);
    Q_UNUSED(actionName);
    Q_UNUSED(params);
    qDebug() << "params: " << params << endl;

    long callBackIDLong = callBackID.toLong();
    if (actionName == "saveImageToPhotosAlbum") {

        QString filePath = params.value("filePath").toString();
        saveImageToPhotosAlbum(callBackIDLong, filePath);
    }
}

void Image::submit(QString typeID, QString callBackID, QString actionName, QVariant dataRowList, QVariant attachementes)
{
    Q_UNUSED(typeID);
    Q_UNUSED(callBackID);
    Q_UNUSED(actionName);
    Q_UNUSED(dataRowList);
    Q_UNUSED(attachementes);
}

void Image::saveImageToPhotosAlbum(long callBackID, QString filePath){

    if(filePath.isEmpty()){
        qDebug() << Q_FUNC_INFO << "文件路径不能为空" << endl;
        emit failed(callBackID, 500, "文件路径不能为空");
        return;
    }

    QFile file(filePath);
    if(!file.exists()){
        qDebug() << Q_FUNC_INFO << "文件不存在：" << filePath << endl;
        emit failed(callBackID, 500, "文件不存在");
        return;
    }

    //设置系统相机路径
    QString path = Helper::instance()->getInnerStorageRootPath() + "/DCIM";
    QDir dir(path);
    if(!dir.exists()){
        dir.mkpath(path);
    }
    QFileInfo fileInfo(file);

    //设置文件名
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();                     //将当前时间转为时间戳
    QStringList filenameArr = fileInfo.fileName().split(".");
    QString filename = filenameArr[0]+"_"+ QString::number(timeT)+"."+filenameArr[1];
    QString newFile = path + "/" + filename;

    try  {
        //将照片复制到系统相册
        QFile::copy(filePath, newFile);
    } catch (QException e) {
        qDebug() << Q_FUNC_INFO << "保存图片到系统相册失败" << endl;
        emit failed(callBackID, 500, "保存图片到系统相册失败");
        return;
    }

    emit success(callBackID, newFile);
}
