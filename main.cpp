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

    if(readData.size()<RowCountValueLocation)
    {
        qDebug()<<"This csv file might not be Gocator SnapeShot data format! Please check the file.(wrong RowCountValueLocation)";
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

    //data to pcd
    pcl::PointCloud<pcl::PointXYZ> cloud;

    for ( int i = 0; i <rowCnt; ++i )
    {
        QStringList listY = readData.at(DataRowBegin + i);

        for(int j=1; j<listY.size(); j++)
        {
            if(!listY.at(j).isEmpty())
            {
                pcl::PointXYZ p;
                p.x = listX.at(j).toDouble()/1000.0;    //pcl單位是m
                p.y = listY.at(0).toDouble()/1000.0;    //pcl單位是m
                p.z = listY.at(j).toDouble()/1000.0;    //pcl單位是m

                cloud.push_back(p);
            }
        }
    }

    qDebug()<<"cloud.size():"<<cloud.size();

    for (std::size_t i = 0; i < 10; ++i)
        std::cerr << "    " << cloud.points[i].x << " " << cloud.points[i].y << " " << cloud.points[i].z << std::endl;

    // Save cloud data
    pcl::io::savePCDFileASCII ("test_pcd.pcd", cloud);
    std::cerr << "Saved " << cloud.points.size () << " data points to test_pcd.pcd" << std::endl;

    return 0;
}
