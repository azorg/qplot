/*
 * QPlot - 2d plotting tool based on Qwt
 * File: "main.cpp"
 */
//----------------------------------------------------------------------------
#include <QApplication>
#include "plot_win.h"  // PlotWin
#include "plot_area.h" // PlotArea, PlotAreaConf
#include "aini.h"      // aclass::aini
#include "qplot.h"     // qplot_read_conf()
//----------------------------------------------------------------------------
// имя INI-файла по-умолчанию
#ifdef QPLOT_WIN32
#  define QPLOT_INI_FILE "qplot.ini" // для Windows
#else
#  define QPLOT_INI_FILE ".qplotrc.ini"  // для Linux
#  include <getopt.h>
#endif
//----------------------------------------------------------------------------
static std::string ini_file; // имя INI-файла конфигурации qplot
//----------------------------------------------------------------------------
#ifndef QPLOT_WIN32
static void usage(FILE *file)
{
  fprintf(file, "usage: qplot [options]\n");
  fprintf(file, "  options:\n");
  fprintf(file, "    -r file | --run file    - run mission *.qplot INI-file\n");
  fprintf(file, "    -c file | --config file - set qplot config file\n");
  fprintf(file, "    -h      | --help        - this help\n\n");
  exit(1);
}
//---------------------------------------------------------------------------
// обработать опции командной строки (в UNIX-like ОС только)
// функция фозвращает имя файла задания
static std::string parse_cmd_options(int argc, char **argv)
{
  std::string mission_file;

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
      {"run",    1, (int*) NULL, 0},
      {"config", 1, (int*) NULL, 0},
      {"help",   0, (int*) NULL, 0},
      {(const char*) NULL, 0, (int*) NULL, 0}
    };

    c = getopt_long(argc, argv, "r:c:h", long_options, &option_index);
    if (c == -1) break;

    if (c == 'r' || (c == 0 && option_index == 0)) // -r or --run
    {
      mission_file = (char*) optarg;
    }
    else if (c == 'c' || (c == 0 && option_index == 1)) // -c or --config
    {
      ini_file = (char*) optarg;
    }
    else if (c == 'h' || (c == 0 && option_index == 2)) // -h or --help
    {
      usage(stdout);
    }
    else // unknown option
    {
      usage(stderr);
      exit(-1);
    }
  } // while (1)

  return mission_file;
}
#endif // !QPLOT_WIN32
//---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  qDebug("QT_VERSION=0x%08x", QT_VERSION);

  std::string mission_file;

  // обработать опции командной строки (UNIX-way)
#ifndef QPLOT_WIN32
  mission_file = parse_cmd_options(argc, argv);
#else
  ini_file = QPLOT_INI_FILE;
  if (argc == 2)
     mission_file = argv[1];
#endif

  // Qt приложение
  QApplication app(argc, argv);

  // главное окно приложения (единственное)
  PlotWin pw;
 
  // открыть INI-файл
  setlocale(LC_NUMERIC, "C"); // десятичная точка = '.'
  aclass::aini f(ini_file);
  
  // прочитать название главного окна приложения
  std::string title = f.read_str("", "title", "");
  if (title.size()) pw.setWindowTitle(_QS(title));
  
  // прочитать положение и размеры главного окна приложения
  int w = pw.size().width();
  int h = pw.size().height();
  int x = f.read_long("", "x", -1);
  int y = f.read_long("", "y", -1);
  w = f.read_long("", "width",  w);
  h = f.read_long("", "height", h);
  if (x >= 0 && y >= 0) pw.move(x, y);
  if (w >  0 && h >  0) pw.resize(w, h);

  // прочитать секцию [area] и настроить PlotAreaConf
  PlotAreaConf conf = pw.pa()->getConf();
  qplot_read_conf(&f, "area", &conf);
  pw.pa()->setConf(conf);

  // показать главное окно
  pw.show();

  // построить графики по заданию или включить демо
  if (mission_file != "")
  { // файл задания указан в командной строке
    if (qplot_run(mission_file.c_str(), pw.pa(), &pw))
      pw.showInfo("Success"); // FIXME
    else
      qplot_demo(pw.pa());
  }
  else
    qplot_demo(pw.pa());
  
  int retv = app.exec();

  // сохранить положение и размер главного окна приложения
  x = pw.pos().x();
  y = pw.pos().y();
  w = pw.size().width();
  h = pw.size().height();
  f.write_long("", "x",      x);
  f.write_long("", "y",      y);
  f.write_long("", "width",  w);
  f.write_long("", "height", h);

  return retv;
}
//----------------------------------------------------------------------------

/*** end of "main.cpp" ***/

