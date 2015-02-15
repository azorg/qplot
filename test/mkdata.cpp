/*
 * Make binary/text data file to test QPlot 
 * File: "mkdata.cpp"
 */
//----------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include "aini.h"
//----------------------------------------------------------------------------
#define DATA_SIZE     (1024*10)
#define DATA_BIN_FILE "data.bin"
#define DATA_TXT_FILE "data.csv"
#define QPLOT_FILE    "test.qplot.ini"
//----------------------------------------------------------------------------
typedef struct {
  unsigned char ucVal;
  short          sVal;
  int            iVal;
  long           lVal;
  long long     llVal;
  float          fVal;
  double         dVal;
  long double   ldVal;
} custom_t;
//----------------------------------------------------------------------------
#define _OFFSET(type, field) \
 ((int) (((char*) &(((type*) 0)->field)) - ((char*) ((type*) 0))))
//----------------------------------------------------------------------------
int main()
{
  printf(
    "typedef struct {\n"
    "  unsigned char ucVal; // offset = %i, sizeof(unsigned char) = %i\n"
    "  short          sVal; // offset = %i, sizeof(short) = %i\n"
    "  int            iVal; // offset = %i, sizeof(int) = %i\n"
    "  long           lVal; // offset = %i, sizeof(long) = %i\n"
    "  long long     llVal; // offset = %i, sizeof(long long) = %i\n"
    "  float          fVal; // offset = %i, sizeof(float) = %i\n"
    "  double         dVal; // offset = %i, sizeof(doubl() = %i\n"
    "  long double   ldVal; // offset = %i, sizeof(long double) = %i\n"
    "} custom_t;\n",
    _OFFSET(custom_t, ucVal), (int) sizeof(unsigned char),
    _OFFSET(custom_t,  sVal), (int) sizeof(short),
    _OFFSET(custom_t,  iVal), (int) sizeof(int),
    _OFFSET(custom_t,  lVal), (int) sizeof(long),
    _OFFSET(custom_t, llVal), (int) sizeof(long long),
    _OFFSET(custom_t,  fVal), (int) sizeof(float),
    _OFFSET(custom_t,  dVal), (int) sizeof(double),  
    _OFFSET(custom_t, ldVal), (int) sizeof(long double));

  printf("sizeof(custom_t) = %i\n", (int) sizeof(custom_t));

  FILE *bin = fopen(DATA_BIN_FILE, "wb");
  FILE *txt = fopen(DATA_TXT_FILE, "w");
  const int data_size = DATA_SIZE;
  for (int i = 0; i < data_size; i++)
  {
    custom_t rec;

    rec.ucVal = i; // 0...255
    rec.sVal  = i * 360 / data_size;
    rec.iVal  = i;
    rec.lVal  = 1000. * cos(((double) i) / ((double) data_size) * 2*M_PI);
    rec.llVal = 1000. * sin(((double) i) / ((double) data_size) * 6*M_PI);
    rec.fVal  = cos(((double) i) / ((double) data_size) * 6*M_PI);
    rec.dVal  = 1000 * sin(((double) i) / ((double) data_size) * 2*M_PI);
    rec.ldVal = (long double) i * 360 / (double) data_size;

    fwrite((const void*) &rec, sizeof(rec), 1, bin);

    fprintf(txt, "%i, %i, %i, %li, %li, %g, %g, %g\n",
            (int) rec.ucVal, (int) rec.sVal, rec.iVal, rec.lVal,
            (long) rec.llVal, (double) rec.fVal, rec.dVal, (double) rec.ldVal); 
  }
  fclose(txt);
  fclose(bin);

  aclass::aini qini(QPLOT_FILE);
  
  // binary data
  // -----------

  qini.write_str  ("0", "legend",     "sin(x)");
  qini.write_str  ("0", "file",       DATA_BIN_FILE);
  qini.write_long ("0", "step",       data_size / 32);
  qini.write_value("0", "format",     "bin");
  qini.write_long ("0", "recordSize", sizeof(custom_t));
  qini.write_long ("0", "xOff",       _OFFSET(custom_t, sVal));
  qini.write_str  ("0", "xType",      "short");
  qini.write_long ("0", "yOff",       _OFFSET(custom_t, dVal));
  qini.write_str  ("0", "yType",      "double");

  qini.write_str  ("1", "legend",     "lissagu");
  qini.write_str  ("1", "file",       DATA_BIN_FILE);
  qini.write_long ("1", "step",       data_size / 128);
  qini.write_value("1", "format",     "bin");
  qini.write_long ("1", "recordSize", sizeof(custom_t));
  qini.write_long ("1", "xOff",       _OFFSET(custom_t, lVal));
  qini.write_str  ("1", "xType",      "long");
  qini.write_long ("1", "yOff",       _OFFSET(custom_t, llVal));
  qini.write_str  ("1", "yType",      "long long");

  qini.write_str  ("2", "legend",     "uchar 0:255");
  qini.write_str  ("2", "file",       DATA_BIN_FILE);
  qini.write_long ("2", "step",       0); // FIXME agressive test
  qini.write_value("2", "format",     "bin");
  qini.write_long ("2", "recordSize", sizeof(custom_t));
  qini.write_long ("2", "xOff",       _OFFSET(custom_t, ldVal));
  qini.write_str  ("2", "xType",      "long double");
  qini.write_long ("2", "yOff",       _OFFSET(custom_t, ucVal));
  qini.write_str  ("2", "yType",      "unsigned char");

  qini.write_str  ("3", "legend",     "cos(2x)");
  qini.write_str  ("3", "file",       DATA_BIN_FILE);
  qini.write_long ("3", "step",       1); // data_size / 32);
  qini.write_value("3", "format",     "bin");
  qini.write_long ("3", "recordSize", sizeof(custom_t));
  qini.write_long ("3", "xOff",       _OFFSET(custom_t, sVal));
  qini.write_str  ("3", "xType",      "short");
  qini.write_long ("3", "yOff",       _OFFSET(custom_t, fVal));
  qini.write_str  ("3", "yType",      "float");

  // text data
  // ---------
  qini.write_str  ("4", "legend", "sin(3x)");
  qini.write_str  ("4", "file",   DATA_TXT_FILE);
  qini.write_long ("4", "step",   1);
  qini.write_value("4", "format", "txt");
  qini.write_long ("4", "xCol",   7);
  qini.write_long ("4", "yCol",   4);

  qini.write_str  ("5", "legend", "ellipse");
  qini.write_str  ("5", "file",   DATA_TXT_FILE);
  qini.write_long ("5", "step",   1);
  qini.write_value("5", "format", "txt");
  qini.write_long ("5", "xCol",   3);
  qini.write_long ("5", "yCol",   6);

  return 0;
}
//----------------------------------------------------------------------------

/*** end of "mkdata.cpp" file ***/
