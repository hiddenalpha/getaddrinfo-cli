
getaddrinfo-cli
===============

Simple commandline frontend for [getaddrinfo](https://pubs.opengroup.org/onlinepubs/9699919799/functions/getaddrinfo.html).

[Home](https://git.hiddenalpha.ch/getaddrinfo-cli.git/tree/README.md).

[Downloads](https://github.com/hiddenalpha/getaddrinfo-cli/releases).


## Stats for Nerds

```txt
github.com/AlDanial/cloc
---------------------------------------------
Language    files    blank    comment    code
---------------------------------------------
C               1        7          0     136
make            1       13          0      43
Markdown        1       14          0      30
---------------------------------------------
SUM:            3       34          0     209
---------------------------------------------
```


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

