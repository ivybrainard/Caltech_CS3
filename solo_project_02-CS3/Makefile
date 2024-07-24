LIBS = $(shell ls library | grep -E '.*\.c' | sed 's/\.c//g')
OBJS = $(addprefix out/,$(LIBS:=.o))

TEST_BINS = bin/test_str_util bin/test_ll bin/test_http
TEST_SERVER_DEPS = bin/test_server bin/web_server$(WS)
TEST_SERVER_CMD = $(TEST_SERVER_DEPS) $(shell cs3-port)


task0: bin/test_str_util bin/test_ll
	bin/test_str_util
	bin/test_ll

task1: bin/test_http
	bin/test_http init

task2: bin/test_http
	bin/test_http init parse

task3: bin/test_http
	bin/test_http

task4: $(TEST_SERVER_DEPS)
	$(TEST_SERVER_CMD)

bin/test_%: out/test_%.o out/test_util.o $(OBJS) $(STAFF_OBJS) | bin
	$(CC) $(CFLAGS) $^ -o $@

all: $(TEST_BINS) server

server: bin/web_server$(WS)
	@echo "Go to \033[0;32mhttp://labradoodle.caltech.edu:$(shell cs3-port)/\033[0m to access your web server!" && \
	$^ $(shell cs3-port)

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

bin/test_server: out/test_server.o out/test_util.o out/server_test_util.o $(OBJS) | bin

bin/test_suite_%: out/test_suite_%.o out/test_util.o $(OBJS) |  bin
	$(CC) $(CFLAGS) $^ -o $@

bin/%: out/%.o $(OBJS) |  bin
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_BINS) $(TEST_SERVER_DEPS)
	set -e; for f in $(TEST_BINS); do echo $$f; $$f; echo; done; $(TEST_SERVER_CMD)

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
