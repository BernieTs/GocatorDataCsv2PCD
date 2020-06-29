#include <QCoreApplication>
#include "qtcsv/stringdata.h"
#include "qtcsv/reader.h"
#include "qtcsv/writer.h"

#include <QDebug>
#include <QList>
#include <QStringList>
#include <QDir>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    // prepare data that you want to save to csv-file
    QStringList strList;
    strList << "one" << "two" << "three";

    QtCSV::StringData strData;
    strData.addRow(strList);
    strData.addEmptyRow();
    strData << strList << "this is the last row";

    // write to file
    QString filePath = QDir::currentPath() + "/test.csv";
    QtCSV::Writer::write(filePath, strData);

    // read data from file
    QList<QStringList> readData = QtCSV::Reader::readToList(filePath);
    for ( int i = 0; i < readData.size(); ++i )
    {
        qDebug() << readData.at(i).join(",");
    }


    return a.exec();
}
