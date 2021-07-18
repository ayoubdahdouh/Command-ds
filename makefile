cc=gcc
coptions=-g -Wall -Werror

all: clean lf

clean:
	rm -f build/* lf
	
build:
	if [ ! -d build ]; then mkdir build;fi

lf: build/common.o build/format_column.o build/format_tree.o build/format_long.o build/list.o build/lf.o build/main.o # build/view.o 
	${cc} ${coptions} $? -o $@

build/main.o: main.c list.h common.h
	${cc} ${coptions} main.c -c -o build/main.o

build/lf.o: lf.c common.h
	${cc} ${coptions} lf.c -c -o build/lf.o

# build/view.o: view.c common.h
# 	${cc} ${coptions} view.c -c -o build/view.o

build/list.o: list.c
	${cc} ${coptions} $? -c -o $@

build/format_column.o: format_column.c
	${cc} ${coptions} $? -c -o $@

build/format_tree.o: format_tree.c
	${cc} ${coptions} $? -c -o $@

build/format_long.o: format_long.c
	${cc} ${coptions} $? -c -o $@

build/common.o: common.c
	${cc} ${coptions} $? -c -o $@
	