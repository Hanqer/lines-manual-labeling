#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <QString>
#include "labimage.h"
QStringList getDirImageNames(QString path);
std::string getNextFilename(const std::set<std::string>& record, QStringList nameList);
void addLabel(const std::string filename, const LabImage& data);
void initRecord(const std::string filename, std::set<std::string>& record);
void addRecord(const std::string filename, std::set<std::string>& record, const std::string name);
#endif // UTILS_H
