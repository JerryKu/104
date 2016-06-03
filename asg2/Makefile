DEPSFILE  = Makefile.dep
NOINCLUDE = ci clean spotless
NEEDINCL  = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}
CPP       = g++ -g -O0 -Wall -Wextra -std=gnu++14
MKDEPS    = g++ -MM -std=gnu++14
GRIND     = valgrind --leak-check=full --show-reachable=yes
FLEX      = flex --header-file=${LEXHDR} --outfile=${LEXCPP}
BISON     = bison --defines=${PARSEHDR} --output=${PARSECPP}

MODULES   = astree stringset auxlib lyutils
FLEXSRC   = scanner.l
BISONSRC  = parser.y
LEXHDR    = yylex.h
LEXCPP    = yylex.cpp
PARSEHDR  = yyparse.h
PARSECPP  = yyparse.cpp
LEXOUT    = yylex.output
PARSEOUT  = yyparse.output
EXECBIN   = oc

HDRSRC    = ${MODULES:=.h}
OBJECTS   = ${ALLCSRC:.cpp=.o}
CPPSRC    = ${MODULES:=.cpp} main.cpp
MODSRC    = ${foreach MOD, ${MODULES}, ${MOD}.h ${MOD}.cpp}
MISCSRC   = ${filter-out ${MODSRC}, ${HDRSRC} ${CPPSRC}}
CGENS     = ${LEXCPP} ${PARSECPP}
REPORTS   = ${LEXOUT} ${PARSEOUT}
TESTINS   = ${wildcard test*.in}
EXECTEST  = ${EXECBIN} -ly
LISTSRC   = ${ALLSRC} ${DEPSFILE} ${PARSEHDR}
ALLGENS   = ${LEXHDR} ${PARSEHDR} ${CGENS}
ALLCSRC   = ${CPPSRC} ${CGENS}
ALLSRC    = README ${FLEXSRC} ${BISONSRC} ${MODSRC} ${MISCSRC} Makefile

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CPP} -o${EXECBIN} ${OBJECTS}

%.o : %.cpp
	${CPP} -c $<

${LEXCPP} : ${FLEXSRC}
	${FLEX} ${FLEXSRC} 2>${LEXOUT}
	- grep -v '^  ' ${LEXOUT}

${PARSECPP} ${PARSEHDR} : ${BISONSRC}
	${BISON} ${BISONSRC}
	echo -e '1,$$!cpp -fpreprocessed -dD\nw\nq\n' | ex ${PARSEHDR}

ci :
	- git remote add assgn2 https://CGTheLegend@bitbucket.org/CGTheLegend/cmps104a-assignment_2.git
	- git checkout master

lis : ${LISTSRC} tests
	mkpspdf List.source.ps ${LISTSRC}
	mkpspdf List.output.ps ${REPORTS} \
		${foreach test, ${TESTINS:.in=}, \
		${patsubst %, ${test}.%, in out err log}}

clean :
	- rm -f ${OBJECTS} ${ALLGENS} ${REPORTS} ${DEPSFILE} core
	- rm -f ${foreach test, ${TESTINS:.in=}, \
		${patsubst %, ${test}.%, out err log}}

spotless : clean
	- rm -f ${EXECBIN} List.*.ps List.*.pdf
	- rm -f *.str
	- rm -f *.tok

deps : ${ALLCSRC}
	@ echo "# ${DEPSFILE} created `date` by ${MAKE}" >${DEPSFILE}
	${MKDEPS} ${ALLCSRC} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps

tests : ${EXECBIN}
	touch ${TESTINS}
	make --no-print-directory ${TESTINS:.in=.out}

%.out %.err : %.in ${EXECBIN}
	${GRIND} --log-file=$*.log ${EXECTEST} $< 1>$*.out 2>$*.err; \
	echo EXIT STATUS = $$? >>$*.log

again :
	gmake --no-print-directory spotless deps ci all lis

submit :
	- submit cmps104a-wm.s16 asg2 ${CHECKINS}

ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif
