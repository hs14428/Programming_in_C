CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS= -lm
SDL= `sdl2-config --libs` `sdl2-config --cflags`

all: parse parse_s interp interp_s interp_v extension

parse: parser.c parser_fn.c parser_fn.h parser_test.c General/general.c General/general.h
	$(CC) parser.c parser_fn.c parser_test.c General/general.c -o parse $(PRODUCTION) $(LDLIBS)

parse_s: parser.c parser_fn.c parser_fn.h parser_test.c General/general.c General/general.h
	$(CC) parser.c parser_fn.c parser_test.c General/general.c -o parse_s $(SANITIZE) $(LDLIBS)

interp: interp.c interp_fn.c interp_fn.h interp_test.c neillsdl2.c neillsdl2.h General/general.c General/general.h Stack/Linked/linked.c Stack/Linked/specific.h Stack/stack.h
	$(CC) interp.c interp_fn.c interp_test.c neillsdl2.c General/general.c Stack/Linked/linked.c -o interp $(PRODUCTION) $(LDLIBS) $(SDL)

interp_s: interp.c interp_fn.c interp_fn.h interp_test.c neillsdl2.c neillsdl2.h General/general.c General/general.h Stack/Linked/linked.c Stack/Linked/specific.h Stack/stack.h
	$(CC) interp.c interp_fn.c interp_test.c neillsdl2.c General/general.c Stack/Linked/linked.c -o interp_s $(SANITIZE) $(LDLIBS) $(SDL)

interp_v: interp.c interp_fn.c interp_fn.h interp_test.c neillsdl2.c neillsdl2.h General/general.c General/general.h Stack/Linked/linked.c Stack/Linked/specific.h Stack/stack.h
	$(CC) interp.c interp_fn.c interp_test.c neillsdl2.c General/general.c Stack/Linked/linked.c -o interp_v $(VALGRIND) $(LDLIBS) $(SDL)

extension: extension.c extension_fn.c extension_fn.h extension_test.c neillsdl2.c neillsdl2.h General/general.c General/general.h Stack/Linked/linked.c Stack/Linked/specific.h Stack/stack.h
	$(CC) extension.c extension_fn.c extension_test.c neillsdl2.c General/general.c Stack/Linked/linked.c -o extension $(PRODUCTION) $(LDLIBS) $(SDL)

clean:
	rm -f parse
	rm -f parse_s
	rm -f interp
	rm -f interp_s
	rm -f interp_v
	rm -f extension
