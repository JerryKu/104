%{

#include "lyutils.h"
#include "auxlib.h"
#include "astree.h"

#define YY_USER_ACTION  { scanner_useraction(); }

%}

%option 8bit
%option debug
%option nodefault
%option noinput
%option nounput
%option noyywrap
%option verbose
%option warn


LETTER          		[A-Za-z_]
DIGIT           		[0-9]
IDENT       				({LETTER}({LETTER}|{DIGIT})*)
INTCON          		({DIGIT}+)
CHARCON							('([^\\'\n]|\\[\\'"0nt])')
STRINGCON						(\"([^\\"\n]|\\[\\'"0nt])*\")
BADIDENT        		({DIGIT}({LETTER}|{DIGIT})*)
BADCHARCON1         (('((\\([^\\'"0nt]'*|[\\'"0nt]
BADCHARCON2         [^']+'*))|[^\\'][^']+'*))|'')
BADCHARCON      		{BADCHARCON1}{BADCHARCON2}
BADSTRINGCON    		(\"((.*\\[^\\'"0nt].*)|([^"]*[^\"])))

%%

"#".*           		{ scanner_include(); }
\n              		{ scanner_newline(); }
([ \t]+)         		{  }

"void" 							{ return yylval_token(TOK_VOID); }
"bool" 							{ return yylval_token(TOK_BOOL); }
"char" 							{ return yylval_token(TOK_CHAR); }
"int" 							{ return yylval_token(TOK_INT); }
"string" 						{ return yylval_token(TOK_STRING); }
"if" 								{ return yylval_token(TOK_IF); }
"else" 							{ return yylval_token(TOK_ELSE); }
"while" 						{ return yylval_token(TOK_WHILE); }
"return" 						{ return yylval_token(TOK_RETURN); }
"struct" 						{ return yylval_token(TOK_STRUCT); }
"false" 						{ return yylval_token(TOK_FALSE); }
"true" 							{ return yylval_token(TOK_TRUE); }
"null" 							{ return yylval_token(TOK_NULL); }
"new" 							{ return yylval_token(TOK_NEW); }
"ord" 							{ return yylval_token(TOK_ORD); }
"chr" 							{ return yylval_token(TOK_CHR); }

{INTCON}       		 { return yylval_token(TOK_INTCON); }
{IDENT}        		 { return yylval_token(TOK_IDENT); }
{CHARCON}				   { return yylval_token(TOK_CHARCON); }
{STRINGCON}    		 { return yylval_token(TOK_STRINGCON); }

"="            		 { return yylval_token('='); }
"+"            		 { return yylval_token('+'); }
"-"            		 { return yylval_token('-'); }
"*"            		 { return yylval_token('*'); }
"/"            		 { return yylval_token('/'); }
"^"            		 { return yylval_token('^'); }
"("            		 { return yylval_token('('); }
")"            		 { return yylval_token(')'); }
";"            		 { return yylval_token(';'); }

"[]"						 	 { return yylval_token(TOK_ARRAY); }
"["							 	 { return yylval_token('['); }
"]"								 { return yylval_token(']'); }
"{"								 { return yylval_token('{'); }
"}"								 { return yylval_token('}'); }
","								 { return yylval_token(','); }
"."								 { return yylval_token('.'); }
"=="							 { return yylval_token(TOK_EQ); }
"!="							 { return yylval_token(TOK_NE); }
"<"								 { return yylval_token(TOK_LT); }
"<="							 { return yylval_token(TOK_LE); }
">"								 { return yylval_token(TOK_GT); }
">="							 { return yylval_token(TOK_GE); }
"%"								 { return yylval_token('%'); }
"!"								 { return yylval_token('!'); }

.               	 { scanner_badchar (*yytext); \
									  	return yylval_token(TOK_BADCHAR); }

%%
