STUDENT_ID=2298962

all: valgrind-test-build build

build:
	cd bash-4.2; ./configure; cd ..
	make -C ./bash-4.2 -j8 CFLAGS=-g

valgrind-test-build:
	gcc -O0 -Wall -g valgrind_test.c -o valgrind-test

test:
	./bash-4.2/bash finder.sh ./bash-4.2 execute 20

tar:
	-rm -f valgrind-test
	-make distclean -C bash-4.2
	mkdir $(2298962)-gdb-lab
	cp -r bash-4.2 Makefile finder.sh $(2298962)-gdb-lab
	tar cvzf $(2298962)-gdb-lab.tar.gz $(2298962)-gdb-lab
	rm -rf $(2298962)-gdb-lab

