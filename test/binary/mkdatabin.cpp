/*
 * Make binary data file to test QPlot 
 * File: "mkdatabin.cpp"
 */
//----------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include "aini.h"
//----------------------------------------------------------------------------
#define DATA_SIZE  1024
#define DATA_FILE  "data.bin"
#define QPLOT_FILE "binary.qplot.ini"
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

  FILE *file = fopen(DATA_FILE, "wb");
  const int data_size = DATA_SIZE;
  for (int i = 0; i < data_size; i++)
  {
    custom_t rec;

    rec.ucVal = i * 256 / data_size; // 0...255
    rec.sVal  = i * 360 / data_size;
    rec.iVal  = i % (data_size / 10);
    rec.lVal  = 1000. * cos(((double) i) / ((double) data_size) * 2*M_PI);
    rec.llVal = 1000. * sin(((double) i) / ((double) data_size) * 4*M_PI);
    rec.fVal  = cos(((double) i) / ((double) data_size) * 2*M_PI);
    rec.dVal  = sin(((double) i) / ((double) data_size) * 2*M_PI);
    rec.ldVal = (long double) i;

    fwrite((const void*) &rec, sizeof(rec), 1, file);
  }
  fclose(file);

  aclass::aini qini(QPLOT_FILE);
  
  qini.write_str  ("0", "file",       DATA_FILE);
  qini.write_long ("0", "step",       data_size / 32);
  qini.write_value("0", "format",     "bin");
  qini.write_long ("0", "recordSize", sizeof(custom_t));
  qini.write_long ("0", "xOff",       _OFFSET(custom_t, sVal));
  qini.write_str  ("0", "xType",      "short");
  qini.write_long ("0", "yOff",       _OFFSET(custom_t, dVal));
  qini.write_str  ("0", "yType",      "double");
  qini.write_str  ("0", "legend",     "sin(x)");

  qini.write_str  ("1", "file",       DATA_FILE);
  qini.write_long ("1", "step",       data_size / 32);
  qini.write_value("1", "format",     "bin");
  qini.write_long ("1", "recordSize", sizeof(custom_t));
  qini.write_long ("1", "xOff",       _OFFSET(custom_t, lVal));
  qini.write_str  ("1", "xType",      "long");
  qini.write_long ("1", "yOff",       _OFFSET(custom_t, llVal));
  qini.write_str  ("1", "yType",      "long long");
  qini.write_str  ("1", "legend",     "lissagu");


  return 0;
}
//----------------------------------------------------------------------------

/*** end of "mkdatabin.cpp" file ***/
