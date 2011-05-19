#
# Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

ifndef BOOST_ROOT
	BOOST_ROOT = ../boost
endif

TARGET = proxy

DSYM = $(TARGET).dSYM

CXX = g++-4.6

ifndef OPTIM_LEVEL
	OPTIM_LEVEL = 0
endif

ifeq ($(OPTIM_LEVEL),0)
	OPTIM_CXXFLAGS = -fno-inline -O0
else
	OPTIM_CXXFLAGS = -O$(OPTIM_LEVEL)
endif

CXXFLAGS = -std=c++0x -Wall -Wextra -g $(OPTIM_CXXFLAGS) -I$(BOOST_ROOT) -DBOOST_ASIO_ENABLE_HANDLER_TRACKING

LD = g++-4.6

LDFLAGS = -std=c++0x

LIBS = $(BOOST_ROOT)/stage/lib/libboost_system.a

CXXFILES = main.cpp new.cpp server.cpp

OBJFILES = $(CXXFILES:.cpp=.o)

all: $(TARGET) $(DSYM)

$(TARGET): $(OBJFILES)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJFILES) $(LIBS)

$(DSYM): $(TARGET)
	dsymutil $(TARGET)

clean:
	rm -rf $(OBJFILES) $(TARGET) $(DSYM)

depends:
	makedepend $(CXXFILES)
	rm Makefile.bak

# DO NOT DELETE THIS LINE - make depend depends on it

main.o: server.hpp allocator.hpp coroutine.hpp
server.o: server.hpp allocator.hpp coroutine.hpp yield.hpp unyield.hpp
