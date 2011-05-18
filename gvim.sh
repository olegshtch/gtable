#!/bin/sh
gvim src/*.h src/*.cpp src/3rd_party/*.{h,hpp,cpp} src/db/*.h src/db/*.cpp src/orm/*.h src/orm/*.cpp src/ga/*.h src/ga/*.cpp \
	  src/Makefile.am src/3rd_party/Makefile.am src/orm/Makefile.am src/db/Makefile.am src/ga/Makefile.am configure.ac
