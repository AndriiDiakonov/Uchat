.PHONY: all install uchat clean uninstall reinstall

SRCS := $(wildcard src/*.c)
HDRS := $(wildcard inc/*.h)

WFLGS := $(addprefix -W, all extra error pedantic) \
	-Wno-unused-command-line-argument -Wno-unused-variable \
    -Wno-unused-function -Wno-unused-parameter \
    -fno-omit-frame-pointer -O0

all: install

install: uchat_server

uchat_server:
	@clang -lsqlite3 -lpthread $(WFLGS) $(SRCS) $(HDRS) ../libraries/libmx/libmx.a
	@mv a.out ../$@

clean:
	@rm -rf inc/*.gch

uninstall: clean
	@rm -rf ../uchat_server

reinstall: uninstall install
