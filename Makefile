.PHONY: all install uninstall reinstall

all: install

install:
	@$(MAKE) -C libraries/libmx
	@$(MAKE) -C client/
	@$(MAKE) -C server/

uninstall:
	@$(MAKE) -C server/ uninstall
	@$(MAKE) -C client/ uninstall
	@$(MAKE) -C libraries/libmx uninstall

reinstall: uninstall install
