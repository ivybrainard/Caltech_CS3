LIBS = ll mystr strarray
OBJS = $(addprefix out/,$(LIBS:=.o))

TEST_BINS = bin/test_str_util bin/test_ll

task0a: bin/test_str_util
	bin/test_str_util strarray

task0b: bin/test_str_util
	bin/test_str_util strindexof

task0c: bin/test_str_util
	bin/test_str_util split

task0: task0a task0b task0c

task1b: bin/test_ll
	bin/test_ll basic

task1c: bin/test_ll
	bin/test_ll

task1: task1b task1c

bin/test_%: out/test_%.o out/test_util.o $(OBJS) $(STAFF_OBJS) | bin
	$(CC) $(CFLAGS) $^ -o $@

all: $(TEST_BINS) server

server: ./bin/web_server
	@echo "Go to \033[0;32mhttp://labradoodle.caltech.edu:$(shell cs3-port)/\033[0m to access your web server!" && \
	./bin/web_server $(shell cs3-port)


CC = clang
ifndef NO_ASAN
  CFLAGS = -fsanitize=address,undefined,leak
  ifeq ($(wildcard .debug),)
    $(shell $(CLEAN_COMMAND))
    $(shell touch .debug)
  endif
# Compiling without asan (run 'make NO_ASAN=true all')
else
  CFLAGS = -O3
  ifneq ($(wildcard .debug),)
    $(shell $(CLEAN_COMMAND))
    $(shell rm -f .debug)
  endif
endif

CFLAGS += -Iinclude -Wall -Wextra -g -fno-omit-frame-pointer

out/%.o: library/%.c | out
	@git commit -am "Autocommit of library for ${USER}" > /dev/null || true
	$(CC) -c $(CFLAGS) $^ -o $@
out/%.o: server/%.c | out
	@git commit -am "Autocommit of server for ${USER}" > /dev/null || true
	$(CC) -c $(CFLAGS) $^ -o $@
out/%.o: tests/%.c | out
	$(CC) -c $(CFLAGS) $^ -o $@

bin/web_server: out/web_server.o $(OBJS) | out
	$(CC) $(CFLAGS) $^ -o $@

bin/test_suite_%: out/test_suite_%.o out/test_util.o $(OBJS) | bin
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_BINS)
	set -e; for f in $(TEST_BINS); do echo $$f; $$f; echo; done

out:
	mkdir -p $@

bin:
	mkdir -p $@

# Removes all compiled files.
CLEAN_COMMAND = find out/ ! -name .gitignore -type f -delete && \
find bin/ ! -name .gitignore -type f -delete

clean:
	$(CLEAN_COMMAND)

# This special rule tells Make that "all", "clean", and "test" are rules
# that don't build a file.
.PHONY: all clean test out bin
# Tells Make not to delete the .o files after the executable is built
.PRECIOUS: out/%.o
