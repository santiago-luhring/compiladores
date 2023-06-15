%{
	#include "y.tab.h"
	#include "hash.h"

	int Running = 1;
	int LineNumber = 1;

	int getLineNumber(void) {
    return LineNumber;
	}

	int isRunning(void) {
    	return Running;
	}
%}

%x COMMENT

%%

char						{return KW_CHAR;      }
int                         {return KW_INT;       }
real                        {return KW_REAL;      }
if                          {return KW_IF;        }
then                        {return KW_THEN;      }
else                        {return KW_ELSE;      }
bool                        {return KW_BOOL;      }
loop                        {return KW_LOOP;      }
input                       {return KW_INPUT;     }
output                      {return KW_OUTPUT;    }
return                      {return KW_RETURN;    }
[-,;()\[\]{}=+*/<>&|~\%]    {return yytext[0];    }
">="                        {return OPERATOR_GE;  }
"<="                        {return OPERATOR_LE;  }
"=="                        {return OPERATOR_EQ;  }
"!="                        {return OPERATOR_DIF; }

[a-z._]+                    {hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER;}
[0-9]+						{hashInsert(yytext, LIT_INT); return LIT_INT;  }
[0-9]+\.[0-9]+              {hashInsert(yytext, LIT_REAL); return LIT_REAL;    }
"\n"                        {LineNumber++;        }
[ \t]

'([^'])'                    {hashInsert(yytext, LIT_CHAR); return LIT_CHAR;}
\"(\\.|[^"\\\n])*\"         {hashInsert(yytext, LIT_STRING); return LIT_STRING;}

"\\\\\\"                    { BEGIN(COMMENT); }
<COMMENT>"\/\/\/"           { BEGIN(INITIAL); }
<COMMENT>"\n"               { ++LineNumber; }
<COMMENT>.
"\\\\".*

.                           {return TOKEN_ERROR;}

%%

#include "y.tab.c"
#include "main.c"
#include "hash.c"

int yywrap(){
    Running = 0;
    return 1;
}

void initMe(void) {
    LineNumber = 1;
    Running = 1;
    hashInit();
}



