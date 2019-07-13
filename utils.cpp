#include "utils.h"
#include <QStringList>
#include <QDir>
#include <fstream>
QStringList getDirImageNames(QString path)
{
    QDir dir(path);
    QStringList nameList, nameFilters;
    nameFilters << "*.jpg" << "*.png" << "*.jpeg";
    nameList = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    return nameList;
}

void addLabel(const std::string filename, const LabImage& data)
{
    std::ofstream fout;
    fout.open(filename, std::ios::app);
    fout << data.name << " " << data.num_semantic_lines << " ";
    for(auto val : data.lines)
    {
        fout << val.x1 << " " << val.y1 << " " << val.x2 << " " << val.y2 << " ";
    }
    fout << std::endl;
    fout.close();
}

void initRecord(const std::string filename, std::set<std::string>& record)
{
    std::ifstream fin(filename);
    std::string val;
    while(fin >> val)
    {
        record.insert(val);
    }
    fin.close();
}

void addRecord(const std::string filename, std::set<std::string>& record, const std::string name)
{
    std::ofstream fout;
    fout.open(filename, std::ios::app);
    record.insert(name);
    fout << name << std::endl;
    fout.close();
}

std::string getNextFilename(const std::set<std::string>& record, QStringList nameList)
{
    for (int i = 0; i < nameList.size(); ++i)
    {
        std::string name = nameList[i].toStdString();
        if(name.find("label.jpg") != name.npos)
            continue;
        if(record.find(name) == record.end())
        {
            return name;
        }
    }
    return "";
}
