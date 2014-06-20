# Makefile for the libKNM libray.
# Version: 3
#
# Author       : Jean-Philippe Clipffel
# Creation date: 29/04/2014
# Last modified: 05/05/2014
#
#
# libKNM
# Copyright (C) 2014 Jean-Philippe Clipffel
# Email: jp.clipffel@gmail.com
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
# USA


NAME 				=		libKNM.so
SONAME				=		$(NAME).1
RLNAME				= 		$(SONAME).0.0
ARCH 				= 		$(shell getconf LONG_BIT)
DYNALIB_INSPATH 	=		/usr/local/lib
HEADERS_INSPATH 	=		/usr/local/include/libKNM
BUILD				=		build

SRC_LIB 			=		.
SRC_CPP 			=		$(SRC_LIB)/KAlert/KAlert.cpp			\
							$(SRC_LIB)/KAlert/kalert.c 				\
							$(SRC_LIB)/KConfig/KConfig.cpp			\
							$(SRC_LIB)/KDylib/KDylib.cpp			\
							$(SRC_LIB)/KError/KError.cpp			\
							$(SRC_LIB)/KFS/KFS.cpp					\
							$(SRC_LIB)/KLog/KLog.cpp				\
							$(SRC_LIB)/KMutex/KMutex.cpp			\
							$(SRC_LIB)/KProcess/KProcess.cpp		\
							$(SRC_LIB)/KRegistry/KRegistry.cpp		\
							$(SRC_LIB)/KSem/KSem.cpp				\
							$(SRC_LIB)/KSocket/KSocket.cpp 			\
							$(SRC_LIB)/KSocket/KSaddr.cpp 			\
							$(SRC_LIB)/KSocket/KSocket_pool.cpp 	\
							$(SRC_LIB)/KTime/KTime.cpp

SRC_HEADERS 		=		$(SRC_LIB)/KAlert/KAlert.hh				\
							$(SRC_LIB)/KAlert/kalert.h 				\
							$(SRC_LIB)/KConfig/KConfig.hh			\
							$(SRC_LIB)/KDylib/KDylib.hh				\
							$(SRC_LIB)/KError/KError.hh				\
							$(SRC_LIB)/KFS/KFS.hh					\
							$(SRC_LIB)/KLog/KLog.hh					\
							$(SRC_LIB)/KMutex/KMutex.hh				\
							$(SRC_LIB)/KProcess/KProcess.hh			\
							$(SRC_LIB)/KRegistry/KRegistry.hh		\
							$(SRC_LIB)/KSem/KSem.hh					\
							$(SRC_LIB)/KSocket/KSocket.hh 			\
							$(SRC_LIB)/KSocket/KSaddr.hh 			\
							$(SRC_LIB)/KSocket/KSocket_pool.hh 		\
							$(SRC_LIB)/KThread/KThread.hpp			\
							$(SRC_LIB)/KTime/KTime.hh				\
							$(SRC_LIB)/KModule/KModule.hpp			\
							$(SRC_LIB)/KTree/KTree.hpp

CC					=		g++
CFLAGS				=		-W -Wall -ansi -pedantic -O3
CLIBS				=		-pthread -ldl
KNMFLAGS 			= 
INCLUDES			=		$(BUILD)/includes

DOXYFILE 			=		Doxyfile
DOXYPATH			=		Documentation/doxygen_html



.PHONY: 	all clean fclean re install doc info
.SILENT:	all clean fclean re install doc info

$(NAME):
		@echo "---"
		@echo "Compiling $(RLNAME) for x$(ARCH) architecture"
		@echo "KNM flags : $(KNMFLAGS)"
		@echo "G++ flags : $(CFLAGS)"
		@echo "Libraires : $(CLIBS)"
		@echo "---"
		mkdir -p $(BUILD)
		mkdir -p $(INCLUDES)
		cp $(SRC_HEADERS) $(INCLUDES)
		$(CC) -I $(INCLUDES) $(CFLAGS) $(CLIBS) $(KNMFLAGS) -fPIC -shared $(SRC_CPP) -o $(BUILD)/$(RLNAME)

all:	$(NAME)

clean: 	

fclean:	clean
		@echo "[*] Removing build temporary directory"
		if [ -d $(BUILD) ]; then rm -r $(BUILD); fi
		@echo "[*] Removing generated documentation"
		if [ -d $(DOXYPATH) ]; then rm -r $(DOXYPATH); fi

re:		fclean all

install:	all
		@echo "Installing libKNM in $(DYNALIB_INSPATH)"
		mkdir -p $(DYNALIB_INSPATH)
		cp $(BUILD)/$(RLNAME) $(DYNALIB_INSPATH)
		if [ -L $(DYNALIB_INSPATH)/$(NAME) ]; then rm $(DYNALIB_INSPATH)/$(NAME); fi
		ln -s $(DYNALIB_INSPATH)/$(RLNAME) $(DYNALIB_INSPATH)/$(NAME)

		@echo "Installing libKNM headers in $(HEADERS_INSPATH)/"
		mkdir -p $(HEADERS_INSPATH)
		cp $(INCLUDES)/* $(HEADERS_INSPATH)/

doc:
		@echo "--- Generating libKNM documentation ---"
		doxygen $(DOXYFILE)

info:
		@echo "--- Installation configuration ---"
		@echo "'linker name' installation path : $(DYNALIB_INSPATH)/$(NAME)"
		@echo "'soname' installation path      : $(DYNALIB_INSPATH)/$(RLNAME)"
		@echo "Includes installation path      : $(HEADERS_INSPATH)"
		@echo ""
		@echo "--- Compilation tips ---"
		@echo "1) Add $(DYNALIB_INSPATH) to your LD_LIBRAY_PATH variable:"
		@echo "   export LD_LIBRARY_PATH=/usr/local/lib:\$$LD_LIBRARY_PATH"
		@echo "OR"
		@echo "2) Compile with:"
		@echo "   g++ <sources> -lKNM -Wl,-rpath $(DYNALIB_INSPATH)"
