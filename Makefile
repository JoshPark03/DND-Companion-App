


all: DNDCP.pro run build

DNDCP.pro: *.cpp #*.h
	qmake -project "QT += widgets" -o DNDCP.pro

build:
	mkdir build

run: build DNDCP.pro
	chmod 0700 /run/user/1000
	cd build && qmake -makefile -Wall ../DNDCP.pro
	cd build && make
	./build/DNDCP

clean:
	cd build && \
	make distclean ;
	rm DNDCP.pro

.PHONY: all run clean
