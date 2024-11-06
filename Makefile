# Name: Makefile
# Description: Makefile for the developers to use to run and test code
# Authors: Josh Park
# Other Sources: ...
# Date Created: 10/20/2024
# Last Modified: 10/31/2024

# VALGRIND_FLAGS += --leak-check=full
VALGRIND_FLAGS += --tool=memcheck
VALGRIND_FLAGS += --gen-suppressions=no
# VALGRIND_FLAGS += --track-origins=yes
# VALGRIND_FLAGS += --show-leak-kinds=all
# VALGRIND_FLAGS += -s

all: DNDCA.pro run build data

DNDCA.pro: src/*.cpp src/*.h
	qmake -project "QT += widgets" -o DNDCA.pro

build:
	mkdir build

data:
	mkdir data
	mkdir data/characters
	mkdir data/databases

run: build data DNDCA.pro
	cd build && qmake -makefile -Wall ../DNDCA.pro
	cd build && make

test:
	QT_QPA_PLATFORM=xcb ./build/DNDCA

valgrind:
	QT_QPA_PLATFORM=xcb valgrind $(VALGRIND_FLAGS) ./build/DNDCA

clean:
	cd build && \
	make distclean ;
	rm DNDCA.pro

.PHONY: all run test valgrind clean
