CFLAGS=-g
LEX=flex
YACC=bison
YFLAGS=-d -v -t
CC=gcc
LDLIBS=-ly -lm

all: parser.result

parser.tab.c: parser.y
	$(YACC) $(YFLAGS) $<

scan.yy.c: scan.lex
	$(LEX) -o $@ $<

parser: parser.tab.c scan.yy.c machine.c expr.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

parser.result: parser exemples.input
	./$< < $(word 2, $^) > $@ && cat $@

.PHONY: clean

clean:
	rm -f *.o core *~ *.tab.h *.yy.c *.tab.c *.output parser
