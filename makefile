cc=gcc
coptions=-g -Wall -Werror

all: clean build ds

clean:
	rm -f build/* ds
	
build:
	if [ ! -d build ]; then mkdir build;fi

ds: build/color.o build/args.o build/common.o build/display.o build/useColumn.o build/useList.o build/useTree.o build/useLong.o build/linkedList.o build/ds.o build/main.o 
	${cc} ${coptions} $? -o $@

build/main.o: src/main.c src/linkedList.h src/common.h
	${cc} ${coptions} src/main.c -c -o build/main.o

build/ds.o: src/ds.c src/common.h
	${cc} ${coptions} src/ds.c -c -o build/ds.o

build/display.o: src/display.c src/common.h src/color.h
	${cc} ${coptions} src/display.c -c -o build/display.o

build/color.o: src/color.c src/common.h
	${cc} ${coptions} src/color.c -c -o build/color.o

build/linkedList.o: src/linkedList.c
	${cc} ${coptions} $? -c -o $@

build/useColumn.o: src/useColumn.c
	${cc} ${coptions} $? -c -o $@

build/useTree.o: src/useTree.c
	${cc} ${coptions} $? -c -o $@

build/useList.o: src/useList.c
	${cc} ${coptions} $? -c -o $@

build/useLong.o: src/useLong.c
	${cc} ${coptions} $? -c -o $@

build/common.o: src/common.c
	${cc} ${coptions} $? -c -o $@

build/args.o: src/args.c
	${cc} ${coptions} $? -c -o $@
	