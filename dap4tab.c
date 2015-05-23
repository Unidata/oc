/* A Bison parser, made by GNU Bison 3.0.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */
#line 16 "dap4.y" /* yacc.c:316  */

#include "config.h"
#include <stdlib.h>
#include <stdio.h>

#line 69 "dap4tab.c" /* yacc.c:316  */

/* Substitute the variable and function names.  */
#define yyparse         dap4parse
#define yylex           dap4lex
#define yyerror         dap4error
#define yydebug         dap4debug
#define yynerrs         dap4nerrs


/* Copy the first part of user declarations.  */

#line 81 "dap4tab.c" /* yacc.c:339  */

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "dap4.tab.h".  */
#ifndef YY_DAP4_DAP4_TAB_H_INCLUDED
# define YY_DAP4_DAP4_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int dap4debug;
#endif
/* "%code requires" blocks.  */
#line 22 "dap4.y" /* yacc.c:355  */

#include "dap4parselex.h"
#include "dap4tab.h"

#line 116 "dap4tab.c" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    DATASET_ = 258,
    _DATASET = 259,
    GROUP_ = 260,
    _GROUP = 261,
    ENUMERATION_ = 262,
    _ENUMERATION = 263,
    ENUMCONST_ = 264,
    _ENUMCONST = 265,
    NAMESPACE_ = 266,
    _NAMESPACE = 267,
    DIMENSION_ = 268,
    _DIMENSION = 269,
    DIM_ = 270,
    _DIM = 271,
    ENUM_ = 272,
    _ENUM = 273,
    MAP_ = 274,
    _MAP = 275,
    STRUCTURE_ = 276,
    _STRUCTURE = 277,
    SEQUENCE_ = 278,
    _SEQUENCE = 279,
    VALUE_ = 280,
    _VALUE = 281,
    ATTRIBUTE_ = 282,
    _ATTRIBUTE = 283,
    OTHERXML_ = 284,
    _OTHERXML = 285,
    ERROR_ = 286,
    _ERROR = 287,
    MESSAGE_ = 288,
    _MESSAGE = 289,
    CONTEXT_ = 290,
    _CONTEXT = 291,
    OTHERINFO_ = 292,
    _OTHERINFO = 293,
    CHAR_ = 294,
    _CHAR = 295,
    BYTE_ = 296,
    _BYTE = 297,
    INT8_ = 298,
    _INT8 = 299,
    UINT8_ = 300,
    _UINT8 = 301,
    INT16_ = 302,
    _INT16 = 303,
    UINT16_ = 304,
    _UINT16 = 305,
    INT32_ = 306,
    _INT32 = 307,
    UINT32_ = 308,
    _UINT32 = 309,
    INT64_ = 310,
    _INT64 = 311,
    UINT64_ = 312,
    _UINT64 = 313,
    FLOAT32_ = 314,
    _FLOAT32 = 315,
    FLOAT64_ = 316,
    _FLOAT64 = 317,
    STRING_ = 318,
    _STRING = 319,
    URL_ = 320,
    _URL = 321,
    OPAQUE_ = 322,
    _OPAQUE = 323,
    ATTR_BASE = 324,
    ATTR_BASETYPE = 325,
    ATTR_DAPVERSION = 326,
    ATTR_DMRVERSION = 327,
    ATTR_ENUM = 328,
    ATTR_HREF = 329,
    ATTR_NAME = 330,
    ATTR_NAMESPACE = 331,
    ATTR_NS = 332,
    ATTR_SIZE = 333,
    ATTR_TYPE = 334,
    ATTR_VALUE = 335,
    ATTR_XMLNS = 336,
    TEXT = 337,
    UNKNOWN_ATTR = 338,
    UNKNOWN_ELEMENT_ = 339,
    _UNKNOWN_ELEMENT = 340,
    ERROR = 341,
    UNKNOWN = 342,
    UNEXPECTED = 343
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 31 "dap4.y" /* yacc.c:355  */

OClist* List;
yax_token* Token;
OCnode* Node;
OCDOM* Dom;

#line 224 "dap4tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int dap4parse (DAP4parser* state);

#endif /* !YY_DAP4_DAP4_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 238 "dap4tab.c" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 27 "dap4.y" /* yacc.c:359  */

int dap4debug = 0;

#line 244 "dap4tab.c" /* yacc.c:359  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   454

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  89
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  190
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  244

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   343

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   103,   103,   104,   110,   108,   119,   117,   128,   130,
     131,   132,   133,   134,   140,   138,   147,   148,   152,   154,
     161,   159,   168,   170,   175,   176,   177,   178,   185,   183,
     195,   193,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   240,   242,   243,   244,   250,   248,   259,
     257,   265,   267,   268,   269,   270,   276,   274,   282,   284,
     285,   286,   287,   290,   292,   296,   297,   301,   302,   311,
     308,   319,   316,   326,   327,   332,   342,   339,   350,   351,
     356,   357,   361,   363,   368,   376,   378,   383,   388,   397,
     398,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   423,   424,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   468,   469,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   492,   489,   499,   501,   505,   507,
     509
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "DATASET_", "_DATASET", "GROUP_",
  "_GROUP", "ENUMERATION_", "_ENUMERATION", "ENUMCONST_", "_ENUMCONST",
  "NAMESPACE_", "_NAMESPACE", "DIMENSION_", "_DIMENSION", "DIM_", "_DIM",
  "ENUM_", "_ENUM", "MAP_", "_MAP", "STRUCTURE_", "_STRUCTURE",
  "SEQUENCE_", "_SEQUENCE", "VALUE_", "_VALUE", "ATTRIBUTE_", "_ATTRIBUTE",
  "OTHERXML_", "_OTHERXML", "ERROR_", "_ERROR", "MESSAGE_", "_MESSAGE",
  "CONTEXT_", "_CONTEXT", "OTHERINFO_", "_OTHERINFO", "CHAR_", "_CHAR",
  "BYTE_", "_BYTE", "INT8_", "_INT8", "UINT8_", "_UINT8", "INT16_",
  "_INT16", "UINT16_", "_UINT16", "INT32_", "_INT32", "UINT32_", "_UINT32",
  "INT64_", "_INT64", "UINT64_", "_UINT64", "FLOAT32_", "_FLOAT32",
  "FLOAT64_", "_FLOAT64", "STRING_", "_STRING", "URL_", "_URL", "OPAQUE_",
  "_OPAQUE", "ATTR_BASE", "ATTR_BASETYPE", "ATTR_DAPVERSION",
  "ATTR_DMRVERSION", "ATTR_ENUM", "ATTR_HREF", "ATTR_NAME",
  "ATTR_NAMESPACE", "ATTR_NS", "ATTR_SIZE", "ATTR_TYPE", "ATTR_VALUE",
  "ATTR_XMLNS", "TEXT", "UNKNOWN_ATTR", "UNKNOWN_ELEMENT_",
  "_UNKNOWN_ELEMENT", "ERROR", "UNKNOWN", "UNEXPECTED", "$accept",
  "response", "dataset", "$@1", "group", "$@2", "groupbody", "enumdef",
  "$@3", "enumconst_list", "enumconst", "dimdef", "$@4", "dimref",
  "variable", "atomicvariable", "$@5", "enumvariable", "$@6",
  "atomictype_", "_atomictype", "varbody", "mapref", "$@7",
  "structurevariable", "$@8", "structbody", "sequencevariable", "$@9",
  "sequencebody", "metadatalist", "metadata", "attribute",
  "atomicattribute", "$@10", "$@11", "namespace_list", "namespace",
  "containerattribute", "$@12", "attributelist", "valuelist", "value",
  "otherxml", "xml_body", "element_or_text", "xml_attribute_map",
  "xml_attribute", "xml_open", "xml_close", "error_response", "$@13",
  "error_body", "error_element", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343
};
# endif

#define YYPACT_NINF -131

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-131)))

#define YYTABLE_NINF -97

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     107,  -131,  -131,    67,  -131,  -131,   371,   371,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,   101,   195,  -131,   -44,  -131,
    -131,  -131,    29,    32,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,    34,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,   -13,    44,    50,  -131,
    -131,   371,   371,   371,  -131,  -131,   371,   114,  -131,    77,
      98,   109,  -131,   127,  -131,  -131,  -131,  -131,    -6,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
     196,  -131,  -131,  -131,  -131,  -131,  -131,   260,   -66,    92,
    -131,    -4,    -3,   327,   372,    71,   124,   123,  -131,   143,
    -131,  -131,   114,   311,  -131,    94,    97,  -131,  -131,  -131,
    -131,   -12,  -131,   103,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,   164,   -63,    40,
    -131,  -131,  -131,   -21,    31,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,   170,   172,   179,   184,  -131,  -131,   176,   178,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,    -9,  -131
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   109,   109,     0,     2,     3,     4,   184,     1,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,     8,   110,   186,     0,     0,     5,     0,   109,
     109,   109,     0,     0,   109,   109,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    13,     9,    10,    11,    24,    25,     0,    26,    27,
      12,    85,    87,    88,    86,   185,     0,     0,     0,   187,
       6,    14,    20,    30,    69,    76,    93,     0,    28,     0,
       0,     0,     8,     0,    83,    63,    71,    78,    89,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   108,   153,
       0,   105,   109,    63,   188,   189,   190,     0,     0,     0,
      16,     0,     0,     0,     0,     0,     0,     0,    94,     0,
     104,   106,     0,     0,     7,     0,     0,    15,    17,    21,
      84,     0,    31,     0,    64,    65,    66,    70,    72,    73,
      74,    75,    77,    79,    80,    81,    82,     0,     0,     0,
     100,    92,    98,     0,     0,    62,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    29,     0,     0,     0,     0,    67,    95,     0,     0,
      90,   101,    97,    99,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   107,    18,    19,    22,    23,    83,
     103,   102,     0,    68
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -131,  -131,  -131,  -131,  -131,  -131,   126,  -131,  -131,  -131,
      81,  -131,  -131,   -31,   100,  -131,  -131,  -131,  -131,  -131,
    -131,    83,   135,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
     -27,  -130,  -109,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,    45,  -131,    74,  -112,    -2,  -131,  -131,  -131,
    -131,  -131,  -131,  -131
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,    22,    51,    82,    25,    52,    83,   129,
     130,    53,    84,   154,    54,    55,   123,    56,    85,    57,
     191,   132,   155,   239,    58,    86,   133,    59,    87,   134,
     131,    60,    61,    62,   136,   137,    88,   138,    63,   139,
     173,   169,   170,    64,   120,   121,     6,    23,   122,   234,
       5,    24,    26,    69
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       7,   150,   156,   161,   166,   135,    34,   202,   141,   145,
     149,   243,   151,   156,   146,   152,   153,   198,    34,   199,
      35,   -96,   -91,    34,    34,    35,    35,    71,    72,    73,
     172,    70,    76,    77,    89,   204,    90,   205,    91,   206,
      92,   207,    93,   208,    94,   209,    95,   210,    96,   211,
      97,   212,    98,   213,    99,   214,   100,   215,   101,   216,
     102,   217,   141,   194,   203,   168,   195,     8,   200,    79,
     103,   218,   104,   219,   105,   220,   106,   221,   107,   222,
     108,   223,   109,   224,   110,   225,   111,   226,   112,   227,
     113,   228,   114,   229,   115,   230,   116,   231,   117,   232,
     147,   128,   158,   163,    74,    27,    28,    75,    29,    78,
       1,   124,   150,   118,    30,   119,   233,    89,    31,    90,
     142,    91,    32,    92,    33,    93,    80,    94,    34,    95,
      35,    96,    81,    97,   125,    98,   128,    99,     2,   100,
      36,   101,    37,   102,    38,   167,    39,   126,    40,   168,
      41,   171,    42,   103,    43,   104,    44,   105,    45,   106,
      46,   107,    47,   108,    48,   109,    49,   110,    50,   111,
      34,   112,   193,   113,   192,   114,   197,   115,   196,   116,
     235,   117,   236,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,   237,   118,    21,   119,    89,
     238,    90,   240,    91,   241,    92,   143,    93,   127,    94,
     148,    95,   242,    96,   201,    97,   174,    98,     0,    99,
       0,   100,     0,   101,     0,   102,   140,    65,    66,     0,
      67,     0,    68,   159,   164,   103,     0,   104,     0,   105,
       0,   106,     0,   107,     0,   108,     0,   109,     0,   110,
       0,   111,     0,   112,     0,   113,     0,   114,     0,   115,
       0,   116,     0,   117,     0,    28,   144,    29,   160,   165,
       0,     0,     0,    30,     0,     0,     0,    31,   118,     0,
     119,    32,     0,    33,     0,     0,     0,    34,     0,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
       0,    37,     0,    38,     0,    39,     0,    40,     0,    41,
       0,    42,     0,    43,     0,    44,     0,    45,     0,    46,
       0,    47,     0,    48,     0,    49,   151,    50,     0,   175,
     153,     0,     0,     0,     0,     0,     0,     0,    34,     0,
      35,     0,   151,     0,    31,     0,   153,     0,    32,   157,
      33,   176,     0,   177,    34,   178,    35,   179,     0,   180,
       0,   181,     0,   182,     0,   183,    36,   184,    37,   185,
      38,   186,    39,   187,    40,   188,    41,   189,    42,   190,
      43,     0,    44,     0,    45,     0,    46,   151,    47,    31,
      48,   153,    49,    32,    50,    33,   162,     0,     0,    34,
       0,    35,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,     0,    37,     0,    38,     0,    39,     0,    40,
       0,    41,     0,    42,     0,    43,     0,    44,     0,    45,
       0,    46,     0,    47,     0,    48,     0,    49,     0,    50,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,     0,     0,    21
};

static const yytype_int16 yycheck[] =
{
       2,   131,   132,   133,   134,    11,    27,    28,   120,    75,
      14,    20,    15,   143,    80,    18,    19,    80,    27,    82,
      29,    27,    28,    27,    27,    29,    29,    29,    30,    31,
     139,    75,    34,    35,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,   174,    75,   173,    25,    78,     0,    28,    82,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
       8,     9,   133,   134,    75,     4,     5,    75,     7,    75,
       3,    34,   242,    82,    13,    84,    85,     3,    17,     5,
     122,     7,    21,     9,    23,    11,    82,    13,    27,    15,
      29,    17,    82,    19,    36,    21,     9,    23,    31,    25,
      39,    27,    41,    29,    43,    74,    45,    38,    47,    25,
      49,    28,    51,    39,    53,    41,    55,    43,    57,    45,
      59,    47,    61,    49,    63,    51,    65,    53,    67,    55,
      27,    57,    75,    59,    80,    61,    12,    63,    75,    65,
      10,    67,    10,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    16,    82,    83,    84,     3,
      16,     5,    26,     7,    26,     9,   123,    11,    82,    13,
     129,    15,   239,    17,   169,    19,   142,    21,    -1,    23,
      -1,    25,    -1,    27,    -1,    29,    30,    32,    33,    -1,
      35,    -1,    37,   133,   134,    39,    -1,    41,    -1,    43,
      -1,    45,    -1,    47,    -1,    49,    -1,    51,    -1,    53,
      -1,    55,    -1,    57,    -1,    59,    -1,    61,    -1,    63,
      -1,    65,    -1,    67,    -1,     5,     6,     7,   133,   134,
      -1,    -1,    -1,    13,    -1,    -1,    -1,    17,    82,    -1,
      84,    21,    -1,    23,    -1,    -1,    -1,    27,    -1,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    41,    -1,    43,    -1,    45,    -1,    47,    -1,    49,
      -1,    51,    -1,    53,    -1,    55,    -1,    57,    -1,    59,
      -1,    61,    -1,    63,    -1,    65,    15,    67,    -1,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      29,    -1,    15,    -1,    17,    -1,    19,    -1,    21,    22,
      23,    40,    -1,    42,    27,    44,    29,    46,    -1,    48,
      -1,    50,    -1,    52,    -1,    54,    39,    56,    41,    58,
      43,    60,    45,    62,    47,    64,    49,    66,    51,    68,
      53,    -1,    55,    -1,    57,    -1,    59,    15,    61,    17,
      63,    19,    65,    21,    67,    23,    24,    -1,    -1,    27,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    41,    -1,    43,    -1,    45,    -1,    47,
      -1,    49,    -1,    51,    -1,    53,    -1,    55,    -1,    57,
      -1,    59,    -1,    61,    -1,    63,    -1,    65,    -1,    67,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    -1,    83
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    31,    90,    91,   139,   135,   135,     0,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    83,    92,   136,   140,    95,   141,     4,     5,     7,
      13,    17,    21,    23,    27,    29,    39,    41,    43,    45,
      47,    49,    51,    53,    55,    57,    59,    61,    63,    65,
      67,    93,    96,   100,   103,   104,   106,   108,   113,   116,
     120,   121,   122,   127,   132,    32,    33,    35,    37,   142,
      75,   135,   135,   135,    75,    75,   135,   135,    75,    82,
      82,    82,    94,    97,   101,   107,   114,   117,   125,     3,
       5,     7,     9,    11,    13,    15,    17,    19,    21,    23,
      25,    27,    29,    39,    41,    43,    45,    47,    49,    51,
      53,    55,    57,    59,    61,    63,    65,    67,    82,    84,
     133,   134,   137,   105,    34,    36,    38,    95,     9,    98,
      99,   119,   110,   115,   118,    11,   123,   124,   126,   128,
      30,   134,   135,   110,     6,    75,    80,     8,    99,    14,
     120,    15,    18,    19,   102,   111,   120,    22,   102,   103,
     111,   120,    24,   102,   103,   111,   120,    74,    25,   130,
     131,    28,   121,   129,   133,    18,    40,    42,    44,    46,
      48,    50,    52,    54,    56,    58,    60,    62,    64,    66,
      68,   109,    80,    75,    75,    78,    75,    12,    80,    82,
      28,   131,    28,   121,     4,     6,     8,    10,    12,    14,
      16,    18,    20,    22,    24,    26,    28,    30,    40,    42,
      44,    46,    48,    50,    52,    54,    56,    58,    60,    62,
      64,    66,    68,    85,   138,    10,    10,    16,    16,   112,
      26,    26,   119,    20
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    89,    90,    90,    92,    91,    94,    93,    95,    95,
      95,    95,    95,    95,    97,    96,    98,    98,    99,    99,
     101,   100,   102,   102,   103,   103,   103,   103,   105,   104,
     107,   106,   108,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   110,   110,   110,   110,   112,   111,   114,
     113,   115,   115,   115,   115,   115,   117,   116,   118,   118,
     118,   118,   118,   119,   119,   120,   120,   121,   121,   123,
     122,   124,   122,   125,   125,   126,   128,   127,   129,   129,
     130,   130,   131,   131,   132,   133,   133,   134,   134,   135,
     135,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   140,   139,   141,   141,   142,   142,
     142
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     0,     5,     0,     5,     0,     2,
       2,     2,     2,     2,     0,     5,     1,     2,     4,     4,
       0,     5,     3,     3,     1,     1,     1,     1,     0,     5,
       0,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     2,     2,     2,     0,     5,     0,
       5,     0,     2,     2,     2,     2,     0,     5,     0,     2,
       2,     2,     2,     0,     2,     1,     1,     1,     1,     0,
       6,     0,     5,     0,     2,     3,     0,     6,     1,     2,
       1,     2,     3,     3,     4,     1,     2,     4,     1,     0,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     5,     0,     2,     3,     3,
       3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (state, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, state); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, DAP4parser* state)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (state);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, DAP4parser* state)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, state);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, DAP4parser* state)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              , state);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, state); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, DAP4parser* state)
{
  YYUSE (yyvaluep);
  YYUSE (state);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (DAP4parser* state)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, state);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 110 "dap4.y" /* yacc.c:1646  */
    {if(dap4_enterdataset(state,(yyvsp[0].List))) YYABORT;}
#line 1567 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 113 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leavedataset(state)) YYABORT;}
#line 1573 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 119 "dap4.y" /* yacc.c:1646  */
    {if(dap4_entergroup(state,(yyvsp[0].Token))) YYABORT;}
#line 1579 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 122 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leavegroup(state)) YYABORT;}
#line 1585 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 140 "dap4.y" /* yacc.c:1646  */
    {if(dap4_enterenumdef(state,(yyvsp[0].List))) YYABORT;}
#line 1591 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 143 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leaveenumdef(state)) YYABORT;}
#line 1597 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 153 "dap4.y" /* yacc.c:1646  */
    {if(dap4_enumconst(state,(yyvsp[-2].Token),(yyvsp[-1].Token))) YYABORT;}
#line 1603 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 155 "dap4.y" /* yacc.c:1646  */
    {if(dap4_enumconst(state,(yyvsp[-1].Token),(yyvsp[-2].Token))) YYABORT;}
#line 1609 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 161 "dap4.y" /* yacc.c:1646  */
    {if(dap4_enterdimdef(state,(yyvsp[0].List))) YYABORT;}
#line 1615 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 164 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leavedimdef(state)) YYABORT;}
#line 1621 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 169 "dap4.y" /* yacc.c:1646  */
    {if(dap4_dimref(state,(yyvsp[-1].Token))) YYABORT;}
#line 1627 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 171 "dap4.y" /* yacc.c:1646  */
    {if(dap4_dimref(state,(yyvsp[-1].Token))) YYABORT;}
#line 1633 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 185 "dap4.y" /* yacc.c:1646  */
    {if(dap4_enteratomicvariable(state,(yyvsp[-1].Token),(yyvsp[0].Token))) YYABORT;}
#line 1639 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 188 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leaveatomicvariable(state,(yyvsp[0].Token))) YYABORT;}
#line 1645 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 195 "dap4.y" /* yacc.c:1646  */
    {if(dap4_enterenumvariable(state,(yyvsp[0].List))) YYABORT;}
#line 1651 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 198 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leaveenumvariable(state)) YYABORT;}
#line 1657 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 204 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1663 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 205 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1669 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 206 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1675 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 207 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1681 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 208 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1687 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 209 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1693 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 210 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1699 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 211 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1705 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 212 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1711 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 213 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1717 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 214 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1723 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 215 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1729 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 216 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1735 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 217 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1741 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 218 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1747 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 222 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1753 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 223 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1759 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 224 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1765 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 225 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1771 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 226 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1777 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 227 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1783 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 228 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1789 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 229 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1795 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 230 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1801 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 231 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1807 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 232 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1813 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 233 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1819 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 234 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1825 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 235 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1831 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 236 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1837 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 237 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[0].Token));}
#line 1843 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 250 "dap4.y" /* yacc.c:1646  */
    {if(dap4_entermap(state,(yyvsp[0].Token))) YYABORT;}
#line 1849 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 253 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leavemap(state)) YYABORT;}
#line 1855 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 259 "dap4.y" /* yacc.c:1646  */
    {if(dap4_enterstructurevariable(state,(yyvsp[0].Token))) YYABORT;}
#line 1861 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 262 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leavestructurevariable(state)) YYABORT;}
#line 1867 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 276 "dap4.y" /* yacc.c:1646  */
    {if(dap4_entersequencevariable(state,(yyvsp[0].Token))) YYABORT;}
#line 1873 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 279 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leavesequencevariable(state)) YYABORT;}
#line 1879 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 311 "dap4.y" /* yacc.c:1646  */
    {if(dap4_enteratomicattribute(state,(yyvsp[-1].List),(yyvsp[0].List))) YYABORT;}
#line 1885 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 314 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leaveatomicattribute(state)) YYABORT;}
#line 1891 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 319 "dap4.y" /* yacc.c:1646  */
    {if(dap4_enteratomicattribute(state,(yyvsp[-1].List),(yyvsp[0].List))) YYABORT;}
#line 1897 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 321 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leaveatomicattribute(state)) YYABORT;}
#line 1903 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 326 "dap4.y" /* yacc.c:1646  */
    {(yyval.List)=dap4_namespace_list(state,NULL,NULL);}
#line 1909 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 328 "dap4.y" /* yacc.c:1646  */
    {(yyval.List)=dap4_namespace_list(state,(yyvsp[-1].List),(yyvsp[0].Token));}
#line 1915 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 335 "dap4.y" /* yacc.c:1646  */
    {(yyval.Token)=((yyvsp[-1].Token));}
#line 1921 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 342 "dap4.y" /* yacc.c:1646  */
    {if(dap4_entercontainerattribute(state,(yyvsp[-1].List),(yyvsp[0].List))) YYABORT;}
#line 1927 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 345 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leavecontainerattribute(state)) YYABORT;}
#line 1933 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 362 "dap4.y" /* yacc.c:1646  */
    {if(dap4_value(state,(yyvsp[-1].Token))) YYABORT;}
#line 1939 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 364 "dap4.y" /* yacc.c:1646  */
    {if(dap4_value(state,(yyvsp[-1].Token))) YYABORT;}
#line 1945 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 372 "dap4.y" /* yacc.c:1646  */
    {if(dap4_otherxml(state,(yyvsp[-2].List),(yyvsp[-1].List))) YYABORT;}
#line 1951 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 377 "dap4.y" /* yacc.c:1646  */
    {(yyval.List)=dap4_xmlbody(state,NULL,(yyvsp[0].Dom));}
#line 1957 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 379 "dap4.y" /* yacc.c:1646  */
    {(yyval.List)=dap4_xmlbody(state,(yyvsp[-1].List),(yyvsp[0].Dom));}
#line 1963 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 387 "dap4.y" /* yacc.c:1646  */
    {(yyval.Dom)=dap4_xmlelement(state,(yyvsp[-3].Token),(yyvsp[-2].List),(yyvsp[-1].List),(yyvsp[0].Token));}
#line 1969 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 389 "dap4.y" /* yacc.c:1646  */
    {(yyval.Dom)=dap4_xmltext(state,(yyvsp[0].Token));}
#line 1975 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 397 "dap4.y" /* yacc.c:1646  */
    {(yyval.List)=dap4_xml_attributes(state,NULL,NULL);}
#line 1981 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 399 "dap4.y" /* yacc.c:1646  */
    {(yyval.List)=dap4_xml_attributes(state,(yyvsp[-1].List),(yyvsp[0].Token));}
#line 1987 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 492 "dap4.y" /* yacc.c:1646  */
    {if(dap4_entererror(state,(yyvsp[0].List))) YYABORT;}
#line 1993 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 496 "dap4.y" /* yacc.c:1646  */
    {if(dap4_leaveerror(state)) YYABORT;}
#line 1999 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 506 "dap4.y" /* yacc.c:1646  */
    {if(dap4_errormessage(state,(yyvsp[-1].Token))) YYABORT;}
#line 2005 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 508 "dap4.y" /* yacc.c:1646  */
    {if(dap4_errorcontext(state,(yyvsp[-1].Token))) YYABORT;}
#line 2011 "dap4tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 510 "dap4.y" /* yacc.c:1646  */
    {if(dap4_errorotherinfo(state,(yyvsp[-1].Token))) YYABORT;}
#line 2017 "dap4tab.c" /* yacc.c:1646  */
    break;


#line 2021 "dap4tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (state, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (state, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, state);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, state);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (state, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, state);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, state);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
