/* C code produced by gperf version 3.0.3 */
/* Command-line: gperf dap4.gperf  */
/* Computed positions: -k'1-2,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif


#include <string.h>
#include "dap4tab.h"
struct dap4_keyword {
	char *name;
	int opentag;
	int closetag;
	int attrtag;
};
#include <string.h>

#define TOTAL_KEYWORDS 45
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 11
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 79
/* maximum key range = 76, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned char asso_values[] =
    {
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      10, 80,  0, 80, 25, 80, 45, 80, 80, 80,
      80, 80, 80, 80, 80, 50, 35,  5,  0,  0,
       0, 30, 80,  0, 80, 80,  5,  5, 50, 10,
      80, 80,  0, 15, 80,  0, 25, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80,  5, 25, 80,
      10,  5, 10,  0, 30, 20, 80, 80,  0,  0,
       0,  5,  0, 80, 20,  5, 15, 80, 20, 80,
       0, 30, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
      80, 80, 80, 80, 80, 80
    };
  return len + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[0]] + asso_values[(unsigned char)str[len - 1]];
}

static const struct dap4_keyword wordlist[] =
  {
    {""}, {""}, {""}, {""},
    {"Enum", ENUM_, _ENUM, 0},
    {"Int64", INT64_, _INT64, 0},
    {"UInt64", UINT64_, _UINT64, 0},
    {"Float64", FLOAT64_, _FLOAT64, 0},
    {"URL", URL_, _URL, 0},
    {"enum", 0, 0, ATTR_ENUM},
    {"xmlns", 0, 0, ATTR_XMLNS},
    {"Enumeration", ENUMERATION_, _ENUMERATION, 0},
    {"ns", 0, 0, ATTR_NS},
    {"Map", MAP_, _MAP, 0},
    {"name", 0, 0, ATTR_NAME},
    {"Int32", INT32_, _INT32, 0},
    {"UInt32", UINT32_, _UINT32, 0},
    {"Float32", FLOAT32_, _FLOAT32, 0},
    {""},
    {"namespace", 0, 0, ATTR_NAMESPACE},
    {"dmrVersion", 0, 0, ATTR_DMRVERSION},
    {"Opaque", OPAQUE_, _OPAQUE, 0},
    {"Message", MESSAGE_, _MESSAGE, 0},
    {"Dim", DIM_, _DIM, 0},
    {"EnumConst", ENUMCONST_, _ENUMCONST, 0},
    {"dapVersion", 0, 0, ATTR_DAPVERSION},
    {""},
    {"Dataset", DATASET_, _DATASET, 0},
    {""},
    {"Dimension", DIMENSION_, _DIMENSION, 0},
    {"Int16", INT16_, _INT16, 0},
    {"UInt16", UINT16_, _UINT16, 0},
    {"Context", CONTEXT_, _CONTEXT, 0},
    {"Sequence", SEQUENCE_, _SEQUENCE, 0},
    {"size", 0, 0, ATTR_SIZE},
    {"value", 0, 0, ATTR_VALUE},
    {"String", STRING_, _STRING, 0},
    {""},
    {"OtherXML", OTHERXML_, _OTHERXML, 0},
    {"base", 0, 0, ATTR_BASE},
    {"Value", VALUE_, _VALUE, 0},
    {""}, {""},
    {"basetype", 0, 0, ATTR_BASETYPE},
    {"Structure", STRUCTURE_, _STRUCTURE, 0},
    {"Error", ERROR_, _ERROR, 0},
    {""}, {""}, {""},
    {"Int8", INT8_, _INT8, 0},
    {"UInt8", UINT8_, _UINT8, 0},
    {""}, {""}, {""},
    {"type", 0, 0, ATTR_TYPE},
    {"Group", GROUP_, _GROUP, 0},
    {""}, {""}, {""},
    {"Char", CHAR_, _CHAR, 0},
    {""}, {""}, {""}, {""},
    {"href", 0, 0, ATTR_HREF},
    {""}, {""}, {""}, {""},
    {"Namespace", NAMESPACE_, _NAMESPACE, 0},
    {""}, {""}, {""}, {""},
    {"Byte", BYTE_, _BYTE, 0},
    {""}, {""}, {""}, {""},
    {"Attribute", ATTRIBUTE_, _ATTRIBUTE, 0}
  };

#ifdef __GNUC__
__inline
#ifdef __GNUC_STDC_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
const struct dap4_keyword *
dap4_keyword_lookup (str, len)
     register const char *str;
     register unsigned int len;
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}

