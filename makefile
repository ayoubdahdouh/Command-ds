cc=gcc
coptions=-g -Wall -Werror

all: clean build lf

clean:
	rm -f build/* lf
	
build:
	if [ ! -d build ]; then mkdir build;fi

lf: build/color.o build/common.o build/display.o build/format_column.o build/format_list.o build/format_tree.o build/format_long.o build/list.o build/lf.o build/main.o 
	${cc} ${coptions} $? -o $@

build/main.o: src/main.c src/list.h src/common.h
	${cc} ${coptions} src/main.c -c -o build/main.o

build/lf.o: src/lf.c src/common.h
	${cc} ${coptions} src/lf.c -c -o build/lf.o

build/display.o: src/display.c src/common.h src/color.h
	${cc} ${coptions} src/display.c -c -o build/display.o

build/color.o: src/color.c src/common.h
	${cc} ${coptions} src/color.c -c -o build/color.o

build/list.o: src/list.c
	${cc} ${coptions} $? -c -o $@

build/format_column.o: src/format_column.c
	${cc} ${coptions} $? -c -o $@

build/format_tree.o: src/format_tree.c
	${cc} ${coptions} $? -c -o $@

build/format_list.o: src/format_list.c
	${cc} ${coptions} $? -c -o $@

build/format_long.o: src/format_long.c
	${cc} ${coptions} $? -c -o $@

build/common.o: src/common.c
	${cc} ${coptions} $? -c -o $@
	