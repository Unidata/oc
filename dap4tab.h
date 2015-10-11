/* A Bison parser, made by GNU Bison 3.0.  */

/* Bison interface for Yacc-like parsers in C

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
#line 22 "dap4.y" /* yacc.c:1909  */

#include "dap4parselex.h"
#include "dap4tab.h"

#line 49 "dap4tab.h" /* yacc.c:1909  */

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
#line 31 "dap4.y" /* yacc.c:1909  */

OClist* List;
yax_token* Token;
OCnode* Node;
OCDOM* Dom;

#line 157 "dap4tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int dap4parse (DAP4parser* state);

#endif /* !YY_DAP4_DAP4_TAB_H_INCLUDED  */
