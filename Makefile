LEXER ?= pslex
PARSER ?= psparse
TARGETS := $(LEXER) $(PARSER)

LEX_SRC := $(shell find src/lexer -type f -name '*.c')
COMMON_SRC := $(shell find src/common -type f -name '*.c')
PARSE_SRC := $(shell find src/parser -type f -name '*.c')

LEX_OBJ := $(patsubst src/%.c, obj/%.o, $(LEX_SRC))
COMMON_OBJ := $(patsubst src/%.c, obj/%.o, $(COMMON_SRC))
PARSE_OBJ := $(patsubst src/%.c, obj/%.o, $(PARSE_SRC))

CC ?= cc
CFLAGS ?= -Wall -Wextra -g -std=c89
CPPFLAGS ?= -I./src/common
LDFLAGS ?= -lm

ifeq ($(COLOR),1)
	CPPFLAGS += -DCOLOR
endif
ORG2PDF := emacs --batch --no-init-file --no-site-file --visit

VLG ?= valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes
TESTS := $(patsubst test/%.pas, test/%.test, $(wildcard test/*.pas))
RESWORDS := pas/reserved_words
PDF := $(patsubst doc/%.org, doc/%.pdf, $(wildcard doc/*.org))

all: test

bin/$(LEXER): $(LEX_OBJ) $(COMMON_OBJ) | bin
	$(CC) $(CFLAGS) $(LEX_OBJ) $(COMMON_OBJ) $(LDFLAGS) -o $@
bin/$(PARSER): $(PARSE_OBJ) $(COMMON_OBJ) | bin
	$(CC) $(CFLAGS) $(PARSE_OBJ) $(COMMON_OBJ) $(LDFLAGS) -o $@


obj/lexer/%.o: src/lexer/%.c | obj/lexer
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
obj/parser/%.o: src/parser/%.c | obj/parser/prod
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
obj/common/%.o: src/common/%.c | obj/common
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

bin:
	mkdir bin
obj:
	mkdir obj
obj/parser/prod: | obj/parser
	mkdir obj/parser/prod
obj/parser: | obj
	mkdir obj/parser
obj/lexer: | obj
	mkdir obj/lexer
obj/common: | obj
	mkdir obj/common

clean: testclean
	$(RM) -rf bin obj

doc/%.pdf: doc/%.org
	$(ORG2PDF) $< --funcall org-latex-export-to-pdf

test/%.tok: test/%.list
	$(ECHO)
test/%.sym: test/%.list
	$(ECHO)
test/%.addr: test/%.list
	$(ECHO)
test/%.list: bin/$(LEXER) test/%.pas
	$(VLG) \
	$^  $(RESWORDS)  \
	2>$@.lexerr >$@.lexout \

test/%.symlist: bin/$(PARSER) test/%.sym test/%.tok test/%.list test/%.addr
	$(VLG) \
	$^ $@ \
	2>$@.symerr >$@.symout \

	./sh/callstack.sh | gdb --args \
	$^ $@ \
	| ./sh/filter.pl \
	 >$@.callstack 2>/dev/null \

test/%.test: test/%.symlist
	$(ECHO)
test: bin/$(LEXER) bin/$(PARSER) $(TESTS)
	$(ECHO)

testclean:
	$(RM) test/*.sym test/*.list \
		test/*.tok test/*.lexout  \
		test/*.err test/*.symlist \
		test/*.lexerr test/*.symerr \
		test/*.symout test/*.callstack \
		test/*.addr

pdf: test $(PDF)

.PHONY: clean test test/%.test testclean pdf pdfclean
.PRECIOUS: test/%.tok test/%.sym test/%.list test/%.symlist test/%.addr
