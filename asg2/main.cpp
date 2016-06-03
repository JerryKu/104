// $Id: cppstrtok.cpp,v 1.4 2016-03-24 14:34:10-07 - - $

// Use cpp to scan a file and print line numbers.
// Print out each input line read in, then strtok it for
// tokens.

#include <string>
#include <iostream>
using namespace std;

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
#include "auxlib.h"
#include "stringset.h"
#include "lyutils.h"
#include "astree.h"
#include "yylex.h"
#include "yyparse.h"
#include <assert.h>

int c;
//int yydebug  = 0;
//int yy_flex_debug = 0;
int Dval = 0;
int atVal = 0;
string D;
string at;

const char* asg1Out;
const char* asg2Out;
const string CPP = "/usr/bin/cpp";
constexpr size_t LINESIZE = 1024;

// Chomp the last character from a buffer if it is delim.
void chomp (char* string, char delim) {
   size_t len = strlen (string);
   if (len == 0) return;
   char* nlpos = string + len - 1;
   if (*nlpos == delim) *nlpos = '\0';
}

string removeExt (const std::string &filename){
   size_t period = filename.find_last_of(".");
   if(period == std::string::npos) return filename;
   return filename.substr(0,period);

}

FILE* preprocess(string ocfname){
   string cmd = "cpp ";
   cmd += ocfname + " ";
   return(popen(cmd.c_str(), "r"));
}



// Run cpp against the lines of the file.
void cpplines (FILE* pipe, char* filename) {
   int linenr = 1;
   char inputname[LINESIZE];
   strcpy (inputname, filename);
   for (;;) {
      char buffer[LINESIZE];
      char* fgets_rc = fgets (buffer, LINESIZE, pipe);
      if (fgets_rc == NULL) break;
      chomp (buffer, '\n');
      int sscanf_rc = sscanf (buffer, "# %d \"%[^\"]\"",
                              &linenr, filename);
      if (sscanf_rc == 2) {
         continue;
      }
      char* savepos = NULL;
      char* bufptr = buffer;
      for (int tokenct = 1;; ++tokenct) {
         char* token = strtok_r (bufptr, " \t\n", &savepos);
         bufptr = NULL;
         if (token == NULL) break;
         intern_stringset(token);
      }
      ++linenr;
   }
}

int flexScanner(int argc, char** argv){
   int token = 1;
   set_execname (argv[0]);
   /*for (int argi = 1; argi < argc; ++argi) {
      char* filename = argv[argc-1];
      */
      char* filename = argv[argc-1];
      string command = CPP + " " + filename;
      yyin = popen (command.c_str(), "r");

      if (yyin == NULL) {
         syserrprintf (command.c_str());
         exit(EXIT_FAILURE);
      }else {
         while((token = yylex())!= YYEOF){  
      }
         int pclose_rc = pclose (yyin);
         eprint_status (command.c_str(), pclose_rc);
      }    
   
   return get_exitstatus();

}

void fill_string_table(){
   unsigned token_type;
   while((token_type = yylex())){
      if(token_type == YYEOF)
         break;
   }
}



int main (int argc, char **argv) {
   set_execname (argv[0]);
   while((c = getopt(argc, argv, "lyD:@:")) != -1)
   switch(c){
         case 'l':
            yy_flex_debug = 1;
            break;
         case 'y':
            yydebug = 1;
            break;
         case 'D':
            D = optarg;
            Dval = 1;
            break;
         case '@':
            at = optarg;
            atVal = 1;
            break;
         default:
            fprintf(stderr, "Unknown Options");
   }
   for (int argi = 1; argi < argc; ++argi) {
      char* filename = argv[argc-1];
      string command = CPP + " " + filename;
      FILE* pipe = popen (command.c_str(), "r");
      if (pipe == NULL) {
         syserrprintf (command.c_str());
      }else {
         cpplines (pipe, filename);
         int pclose_rc = pclose (pipe);
         eprint_status (command.c_str(), pclose_rc);
         if (pclose_rc != 0) set_exitstatus (EXIT_FAILURE);
      }


   string noFileExt = filename;
   noFileExt = removeExt(noFileExt);
   string asg1 = noFileExt;
   string asg2 = asg1;
   asg1 = asg1 + ".str";
   asg2 = asg2 + ".tok";
   asg1Out = asg1.c_str();
   asg2Out = asg2.c_str();


   FILE* strfile = NULL;
   strfile = fopen(asg1Out, "w");
   //strfile.open(filename);
   dump_stringset(strfile);
   fclose(strfile);
   

   /*yyin = preprocess(filename);
   yyparse();
   fclose(yyin);
   fill_string_table();
   strcat(filename, ".tok");
   FILE* scannerfile = fopen(filename, "w");
   dump_astree(scannerfile, yylval);
   fclose(scannerfile);
   return get_exitstatus();
   */

   FILE* tokfile = NULL;
   tokfile = fopen(asg2Out, "w");
   flexScanner(argc, argv);
   fclose(tokfile);

   set_exitstatus(EXIT_SUCCESS);
   return get_exitstatus();
   }
}
