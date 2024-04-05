
CC=gcc
LD=gcc
MKDIR_P=mkdir -p
RIMRAF=rm -rf
COPYTO=cp -t
DIRNAME=dirname
MD5SUM=md5sum -b
TAR=tar
BINEXT=
INSTALL_ROOT=/usr/local

CFLAGS=-Wall -Werror -pedantic -std=c89 -fPIC -Os -fmax-errors=1 -DPROJECT_VERSION=$(PROJECT_VERSION) -D_POSIX_C_SOURCE=200112L

LDFLAGS=-Wl,-s,--as-needed,--gc-sections,-pie

LIBS=-dy -lc

PROJECT_VERSION=$(shell (git describe --tags 2>/dev/null|sed 's,^v,,'|egrep .) || date -u +0.0.0-%Y%m%d.%H%M%S)

ARCH=$(shell $(CC) -v 2>&1 | egrep '^Target: ' | sed -E 's,^Target: +(.*)$$,\1,')

.SILENT:

default:
default: dist/getaddrinfo-$(PROJECT_VERSION)+$(ARCH).tgz

.PHONY: clean
clean:
	echo "  RM    build dist"
	$(RIMRAF) build dist

build/bin/getaddrinfo$(BINEXT):
build/bin/getaddrinfo$(BINEXT): build/obj/main/c/getaddrinfocli.o
	echo "  LN    $@"
	$(MKDIR_P) "$(shell dirname "$@")"
	$(LD) -o "$@" $(LDFLAGS) $^ $(LIBS)

build/obj/main/c/getaddrinfocli.o:
build/obj/main/c/getaddrinfocli.o: src/main/c/getaddrinfocli.c
	echo "  CC    $@"
	$(MKDIR_P) "$(shell dirname "$@")"
	$(CC) -c -o "$@" $(CFLAGS) $^

dist/getaddrinfo-$(PROJECT_VERSION)+$(ARCH).tgz:
dist/getaddrinfo-$(PROJECT_VERSION)+$(ARCH).tgz: build/bin/getaddrinfo$(BINEXT)
	echo "  PKG   $@"
	$(MKDIR_P) "$(shell dirname "$@")" build/tar build/tar/bin
	$(COPYTO) build/tar/bin/. build/bin/getaddrinfo$(BINEXT)
	echo "version=$(PROJECT_VERSION)" > build/tar/METADATA.INI
	echo "arch=$(ARCH)" >> build/tar/METADATA.INI
	echo "builtAt=$(shell date -u +%Y-%m-%dT%H:%M:%SZ)" >> build/tar/METADATA.INI
	$(TAR) --owner=0 --group=0 -czf "$@" -C build/tar METADATA.INI bin/getaddrinfo$(BINEXT)

.PHONY: install
install:
	$(TAR) -f dist/getaddrinfo-*+$(ARCH).tgz -C "$(INSTALL_ROOT)" -x bin

