#include <QCoreApplication>
#include "qtcsv/stringdata.h"
#include "qtcsv/reader.h"
#include "qtcsv/writer.h"

#include <QDebug>
#include <QList>
#include <QStringList>
#include <QDir>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

#define RowCountNameLocation 25
#define RowCountValueLocation 26
#define XListLocation 27
#define DataRowBegin 28


int main(int argc, char *argv[])
{
    QString filePath = QDir::currentPath() + "/replay_40959_2020-6-24.csv";

    // read data from file
    QList<QStringList> readData = QtCSV::Reader::readToList(filePath);

    //data row count name和value位置
    QString rowCntName = readData.at(RowCountNameLocation).at(6);
    int rowCnt = readData.at(RowCountValueLocation).at(6).toInt();

    qDebug()<<rowCntName<<":"<<rowCnt;

    QStringList listX = readData.at(XListLocation);

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
    std::cerr << "Saved " << cloud.points.size () << " data points to test_pcd.pcd." << std::endl;


    return 0;
}
