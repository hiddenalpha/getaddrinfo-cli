
getaddrinfo-cli
===============

Simple commandline frontend for [getaddrinfo](https://pubs.opengroup.org/onlinepubs/9699919799/functions/getaddrinfo.html).

[Home](https://git.hiddenalpha.ch/getaddrinfo-cli.git/tree/README.txt).

[Downloads](https://github.com/hiddenalpha/getaddrinfo-cli/releases).



## Build/Install From Source

Make sure you have at least:
- C compiler
- libc
- make

Then take a look if you need to tune anything in the Makefile.

Then to build do:
```sh
make clean
make
```

And to install:
```sh
make install
```

Or install even simpler:
```sh
tar -C /usr/local -xzf path/to/dist/your-build.tgz -- bin
```

