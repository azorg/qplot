/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "main.cpp"
 */
//----------------------------------------------------------------------------
//#include <qapplication.h> // QApplication
#include <QApplication>
#include "plot_win.h" // PlotWin
#include "aini.h"     // aclass::aini   
//----------------------------------------------------------------------------
// имя INI-файла по-умолчанию
#ifdef QPLOT_WIN32
#  define QPLOT_INI_FILE "qplot.ini" // для Windows
#else
#  define QPLOT_INI_FILE ".qplotrc"  // для Linux
#  include <getopt.h>
#endif
static std::string ini_file = QPLOT_INI_FILE;
//----------------------------------------------------------------------------
#ifndef QPLOT_WIN32
static void usage(FILE *file)
{
  fprintf(file, "usage: qplot *.qplot\n");
  fprintf(file, "options:\n");
  fprintf(file, "  -c file | --config file - set config file\\n");
  fprintf(file, "  -h      | --help        - this help\n\n");
  exit(1);
}
//---------------------------------------------------------------------------
// обработать опции командной строки (в UNIX-like ОС только)
static void parse_cmd_options(int argc, char **argv)
{
  // имя конфигурационного файла по-умолчанию (в домашнем каталоге)
  ini_file = getenv("HOME");
  if (ini_file.size() > 0)
    ini_file += '/';
  ini_file += QPLOT_INI_FILE;

  // разобрать переданные опции (getopt_long)
  while (1)
  {
    int c, option_index = 0;

    static struct option long_options[] = {
      // {char *name, int has_arg, int *flag, int val}
      {"config", 1, (int*) NULL, 0},
      {"help",   0, (int*) NULL, 0},
      {(const char*) NULL, 0, (int*) NULL, 0}
    };

    c = getopt_long(argc, argv, "c:h", long_options, &option_index);
    if (c == -1) break;

    if (c == 'c' || (c == 0 && option_index == 0)) // -c or --config option
    {
      ini_file = (char*) optarg;
    }
    else if (c == 'h' || (c == 0 && option_index == 2)) // -h or --help option
    {
      usage(stdout);
    }
    else // unknown option
    {
      usage(stderr);
      exit(-1);
    }
  } // while (1)
}
#endif // !QPLOT_WIN32
//---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // обработать опции командной строки (UNIX-way)
#ifndef QPLOT_WIN32
  parse_cmd_options(argc, argv);
#endif // !QPLOT_WIN32

  qDebug("QT_VERSION=0x%08x", QT_VERSION);

  // Qt приложение
  QApplication app(argc, argv);

  // главное окно приложения (единственное)
  PlotWin pw;
 
  // открыть INI-файл
  setlocale(LC_NUMERIC, "C"); // десятичная точка = '.'
  aclass::aini ini(ini_file);
  
  // прочитать положение и размеры главного окна из INI-файла
  int x, y, w, h;
  pw.geometry().getRect(&x, &y, &w, &h);
  x = ini.read_long("PlotWin", "x", x);
  y = ini.read_long("PlotWin", "y", y);
  w = ini.read_long("PlotWin", "w", w);
  h = ini.read_long("PlotWin", "h", h);
  pw.setGeometry(x, y, w, h);
  pw.show();

  int retv = app.exec();

  // сохранить положение и размер главного окна в INI-файл
  pw.geometry().getRect(&x, &y, &w, &h);
  ini.write_long("PlotWin", "x", x);
  ini.write_long("PlotWIn", "y", y);
  ini.write_long("PlotWin", "w", w);
  ini.write_long("PlotWin", "h", h);

  return retv;
}
//----------------------------------------------------------------------------

/*** end of "main.cpp" ***/

