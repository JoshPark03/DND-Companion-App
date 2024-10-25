


all: DNDCP.pro run build

DNDCP.pro: src/*.cpp src/*.h
	qmake -project "QT += widgets" -o DNDCP.pro

build:
	mkdir build

data:
	mkdir data
	mkdir data/characters
	mkdir data/databases

run: build data DNDCP.pro
	cd build && qmake -makefile -Wall ../DNDCP.pro
	cd build && make
	QT_QPA_PLATFORM=xcb ./build/DNDCP

clean:
	cd build && \
	make distclean ;
	rm DNDCP.pro

.PHONY: all run clean
