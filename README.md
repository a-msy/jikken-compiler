# jikken-compiler
There is a viewing fee for this repository. Donate a pizza to me.

## Supports syntax up to final exam 6
## Supports compilation up to final exam 3

## How to Use bro.

You need gcc, Make, MAPS, lex and bison.

- compile and run

```
make run N=1
```

- only compile

```
make compile 
```

- test on MAPS up to final exam 3

```
make test
```

- debug compile and run

```
make rung N=1
```

- debug on gdb

```
make debug N=1
```

- Identify the number of lines with addr2line when a segmentation fault occurs.

http://www.koikikukan.com/archives/2017/03/14-000300.php

First, You should add environment variables.

```
export LD_PRELOAD="/lib64/libSegFault.so"
export SEGFAULT_SIGNALS="all"
```

```
make line L=0x000000
```