#include <QCoreApplication>
#include "qtcsv/stringdata.h"
#include "qtcsv/reader.h"
#include "qtcsv/writer.h"

#include <QDebug>
#include <QList>
#include <QStringList>
#include <QDir>

#define RowCountNameLocation 25
#define RowCountValueLocation 26
#define XListLocation 27
#define DataRowBegin 28


class Point
{
public:
    Point(){}
    Point(double x, double y, double z)
    {xx = x; yy = y; zz = z;}

    void Print()
    {qDebug()<<"x:"<<xx<<" y:"<<yy<<" z:"<<zz;}

public:
    double xx;
    double yy;
    double zz;
};

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
    std::vector<Point> vPoints;

    for ( int i = 0; i <rowCnt; ++i )
    {
        QStringList listY = readData.at(DataRowBegin + i);

        for(int j=1; j<listY.size(); j++)
        {
            if(!listY.at(j).isEmpty())
            {
                Point p;
                p.xx = listX.at(j).toDouble();
                p.yy = listY.at(0).toDouble();
                p.zz = listY.at(j).toDouble();

                vPoints.push_back(p);
            }
        }
    }

    qDebug()<<"vPoints.size():"<<vPoints.size();

    for (int var = 0; var < 10; ++var) {
        vPoints.at(var).Print();
    }

    return 0;
}
