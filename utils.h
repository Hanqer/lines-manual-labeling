#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <QString>
#include "labimage.h"

QStringList getDirImageNames(QString path);
std::string getPrevFilename(QStringList nameList, int &iterpos, int &labeledNums);
std::string getNextFilename(QStringList nameList, int &iterpos, int &labeledNums);
std::string getFirstFilename(QStringList nameList);
std::string getCurrFilename(QStringList nameList, std::set<std::string> record, int &iterpos, int &labeledNums);
void addLabel(const LabImage& data);
void eraseRecord(const std::string filename, const std::string name);
void eraseLabel(const std::string filename, const std::string name);
void initRecord(const std::string filename, std::set<std::string>& record);
void addRecord(const std::string filename, std::set<std::string>& record, const std::string name);
void loadImageLabel(LabImage& labimage);
#endif // UTILS_H
