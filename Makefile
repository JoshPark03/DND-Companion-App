# Name: Makefile
# Description: Makefile for the developers to use to run and test code
# Authors: Josh Park
# Other Sources: ...
# Date Created: 10/20/2024
# Last Modified: 10/25/2024

all: DNDCA.pro run build

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
	QT_QPA_PLATFORM=xcb ./build/DNDCA

clean:
	cd build && \
	make distclean ;
	rm DNDCA.pro

.PHONY: all run clean
