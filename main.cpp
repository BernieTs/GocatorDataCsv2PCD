#include <QCoreApplication>
#include "qtcsv/stringdata.h"
#include "qtcsv/reader.h"
#include "qtcsv/writer.h"

#include <QDebug>
#include <QList>
#include <QStringList>
#include <QDir>
#include <QString>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

#define RowCountNameLocation 25
#define RowCountValueLocation 26
#define XListLocation 27
#define DataRowBegin 28


int main(int argc, char *argv[])
{

    bool fileExists;
    bool isCSVfile;
    QString filePath;
    do{
        std::cout<<"Enter Gocator SnapShot sensor data file name(.csv):";
        std::string fileName;
        std::cin>>fileName;
        filePath = QString::fromStdString(fileName);
        fileExists = (QFileInfo::exists(filePath) && QFileInfo(filePath).isFile())? true: false;   //確認file是否存在
        isCSVfile = (QFileInfo(filePath).suffix() == "csv")? true :false;   //確認file是否為csv
    }while(!fileExists || !isCSVfile);

    // read data from file
    QList<QStringList> readData = QtCSV::Reader::readToList(filePath);

    //data row count name和value位置
    if(readData.size()<RowCountNameLocation)
    {
        qDebug()<<"This csv file might not be Gocator SnapeShot data format! Please check the file.(wrong RowCountNameLocation)";
        return 0;
    }
    QString rowCntName = readData.at(RowCountNameLocation).at(6);
    if(rowCntName.isEmpty() )
    {
        qDebug()<<"Cannot find rowCntName";
        return 0;
    }
    bool ok;
    int rowCnt = readData.at(RowCountValueLocation).at(6).toInt(&ok);
    if(!ok)
    {
        qDebug()<<"Cannot read row count value";
        return 0;
    }

    qDebug()<<rowCntName<<":"<<rowCnt;

    QString colCntName = readData.at(RowCountNameLocation).at(7);
    if(colCntName.isEmpty() )
    {
        qDebug()<<"Cannot find colCntName";
        return 0;
    }
    int colCnt = readData.at(RowCountValueLocation).at(7).toInt(&ok);
    if(!ok)
    {
        qDebug()<<"Cannot read row count value";
        return 0;
    }
    qDebug()<<colCntName<<":"<<colCnt;


    if(readData.size()<XListLocation)
    {
        qDebug()<<"This csv file might not be Gocator SnapeShot data format! Please check the file.(wrong XListLocation)";
        return 0;
    }
    QStringList listX = readData.at(XListLocation);

    if(readData.size()<DataRowBegin)
    {
        qDebug()<<"This csv file might not be Gocator SnapeShot data format! Please check the file.(wrong DataRowBegin)";
        return 0;
    }


    // Fill in the cloud data
    pcl::PointCloud<pcl::PointXYZ> cloud;
    cloud.width    = colCnt;
    cloud.height   = rowCnt;
    cloud.is_dense = false;
    cloud.points.resize (cloud.width * cloud.height);

    //bad point
    const float bad_point = std::numeric_limits<float>::quiet_NaN();

    int cnt = 0;
    for ( int i = 0; i <rowCnt; ++i )
    {
        QStringList listY = readData.at(DataRowBegin + i);

        for(int j=1; j<listY.size(); j++)
        {
            cloud.points[cnt].x = listX.at(j).toDouble()/1000.0;    //pcl單位是m
            cloud.points[cnt].y = listY.at(0).toDouble()/1000.0;    //pcl單位是m
            cloud.points[cnt].z = (!listY.at(j).isEmpty()) ? (listY.at(j).toDouble()/1000.0) : bad_point;    //pcl單位是m
            cnt++;
        }
    }

    qDebug()<<"cloud.size():"<<cloud.size();

    // Save cloud data
    QString saveName = QFileInfo(filePath).baseName() + ".pcd";
    pcl::io::savePCDFileASCII (saveName.toStdString(), cloud);
    std::cerr << "Saved " << cloud.points.size () << " data points to "<<saveName.toStdString() << std::endl;

    system("PAUSE");
    return 0;
}
