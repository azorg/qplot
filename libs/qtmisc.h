/*
 * Вспомагательные функции на базе Qt4 (Qt5 ???)
 * File: "qtmisc.h"
 * Last update: 2008.04.07
 */

#ifndef QTMISC_H
#define QTMISC_H
//---------------------------------------------------------------------------
#include <string>
#include <QString>
#include <QTextCodec>
//---------------------------------------------------------------------------
#define _T(s) qtmisc::str_from_8bit(s)
//---------------------------------------------------------------------------
namespace qtmisc {
//---------------------------------------------------------------------------
// преобразовать строку из локальной кодировки к QString
inline QString str_from_local8bit(const std::string &str)
{
  return QString::fromLocal8Bit(str.c_str());
}
//---------------------------------------------------------------------------
// преобразовать QString к локальной кодироке
inline std::string str_to_local8bit(const QString &str)
{
  return std::string(str.toLocal8Bit().data());
}
//---------------------------------------------------------------------------
// преобразовать строку в указанной кодировке к QString
// charset: UTF8, KOI8-R, ...
inline QString str_from_8bit(const std::string &str,
                             const char *charset="UTF8")
{
  QTextCodec *codec = QTextCodec::codecForName(charset);
  return codec->toUnicode(str.c_str());
}
//---------------------------------------------------------------------------
// преобразовать QString к заданной кодировке
// charset: UTF8, KOI8-R, ...
inline std::string str_to_8bit(const QString &str,
                               const char *charset="UTF8")
{
  QTextCodec *codec = QTextCodec::codecForName(charset);
  return std::string(codec->fromUnicode(str).data());
}
//---------------------------------------------------------------------------
} // namespace qtmisc
//---------------------------------------------------------------------------
#endif // QTMISC_H

/*** end of "qtmisc.h" file ***/

