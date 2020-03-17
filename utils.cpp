#include "utils.h"
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <fstream>
QStringList getDirImageNames(QString path)
{
    QDir dir(path);
    QStringList nameList, nameFilters;
    nameFilters << "*.jpg" << "*.png" << "*.jpeg";
    nameList = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    return nameList;
}

void addLabel(const LabImage& data)
{
    std::ofstream fout;
    std::string abs_path = data.abs_path;
    qDebug() << abs_path.data();
    std::string filename = abs_path.replace(abs_path.end()-4, abs_path.end(), ".txt");
    qDebug() << filename.data();
    fout.open(filename);
    fout << data.num_semantic_lines << " ";
    for(auto val : data.lines)
    {
        fout << val.x1 << " " << val.y1 << " " << val.x2 << " " << val.y2 << " ";
    }
    fout << std::endl;
    fout.close();

}

void eraseLabel(const std::string filename, const std::string name)
{
    std::ifstream fin(filename);
    int len = 0;
    char names[9999][100], labels[9999][200];
    while(fin >> names[len])
    {
        fin.getline(labels[len], 200);
        len++;
    }
    fin.close();

    std::ofstream fout(filename);
    for (int i = 0; i < len; ++i)
    {
        if(name == std::string(names[i]))
        {
            continue;
        }
        fout << names[i] << labels[i] << std::endl;
    }
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

void eraseRecord(const std::string filename, const std::string name)
{
    std::ifstream fin(filename);
    int len = 0;
    char names[9999][100];
    while(fin >> names[len++])
    {
        //pass
        ;
    }
    fin.close();
    std::ofstream fout(filename);
    for (int i = 0; i < len; ++i)
    {
        if(name == std::string(names[i]))
        {
            continue;
        }
        fout << names[i] << std::endl;
    }
    fout.close();
}


std::string getPrevFilename(QStringList nameList, int &iterpos, int &labeledNums)
{
    if (iterpos == 0) return "";
    std::string name = nameList[--iterpos].toStdString();
    while(name.find("label.jpg") != name.npos && iterpos > 0)
    {
        labeledNums--;
        name = nameList[--iterpos].toStdString();
    }
    if (name.find("label.jpg") == name.npos)
        return name;
    else return "";
}

std::string getNextFilename(QStringList nameList, int &iterpos, int &labeledNums)
{
    qDebug() << iterpos;
    if (iterpos == nameList.size()-1) return "";
    std::string name = nameList[++iterpos].toStdString();
    while(name.find("label.jpg") != name.npos && iterpos < nameList.size()-1)
    {
        labeledNums++;
        name = nameList[++iterpos].toStdString();
    }
    if (name.find("label.jpg") == name.npos)
        return name;
    else return "";
}

std::string getFirstFilename(QStringList nameList)
{
    for (int i = 0; i < nameList.size(); ++i)
    {
        std::string name = nameList[i].toStdString();
        if(name.find("label.jpg") == name.npos)
            return name;
    }
    return "";
}

std::string getCurrFilename(QStringList nameList, std::set<std::string> record, int &iterpos, int &labeledNums)
{
    int pos1 = -1, pos2 = 0;
    for (int i = 0; i < nameList.size(); ++i)
    {
        std::string name = nameList[i].toStdString();
        pos1++;
        if (name.find("label.jpg") != name.npos)
        {
            pos2++;
            continue;
        }
        if (record.find(name) == record.end())
        {
            iterpos = pos1;
            labeledNums = pos2;
            return name;
        }

    }
    iterpos = pos1;
    labeledNums = pos2;
    return "";
}

void loadImageLabel(LabImage& labimage)
{
    std::string abs_path = labimage.abs_path;
    std::string labelfile = abs_path.replace(abs_path.end()-4, abs_path.end(), ".txt");
    std::ifstream fin(labelfile);
    int n;
    fin >> n;
    labimage.num_semantic_lines = n;
    for (int i = 0; i < n; ++i)
    {
        int x1, y1, x2, y2;
        fin >> x1 >> y1 >> x2 >> y2;
        labimage.lines.push_back(mLine(x1, y1, x2, y2));
    }
}
