%{
	#include "tokens.h"
	#include "hash.h"

	int Running = 1;
	int LineNumber = 1;
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
[-,;()\[\]{}=+*/<>&|~]      {return yytext[0];    }
">="                        {return OPERATOR_GE;  }
"<="                        {return OPERATOR_LE;  }
"=="                        {return OPERATOR_EQ;  }
"!="                        {return OPERATOR_DIF; }

[a-z._]+                    {hashInsert(TK_IDENTIFIER, yytext); return TK_IDENTIFIER;}
[0-9]+						{hashInsert(LIT_INT, yytext); return LIT_INT;  }
[0-9]+\.[0-9]+              {hashInsert(LIT_REAL, yytext); return LIT_REAL;    }
"\n"                          {LineNumber++;        }
[ \t]

\\\                         {BEGIN(COMMENT);}
'.'                         {hashInsert(LIT_CHAR, yytext); return LIT_CHAR;     }
\"(\\.|[^"])*\"             {hashInsert(LIT_STRING, yytext); return LIT_STRING;   }
.                           {return TOKEN_ERROR;  }

<COMMENT>///                {BEGIN(INITIAL);}
<COMMENT>.
<COMMENT>\n                 {++LineNumber;}

\\.*

%%

void initMe(void){
	hashInit();
}

int getLineNumber(void){
	return LineNumber;
}

int isRunning(void){
	return Running;
}

int yywrap(){
    Running = 0;
    return 1;
}



