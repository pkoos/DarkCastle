VERSION := "$(shell git describe --tags --always --abbrev=8)"
BUILD_TIME := "$(shell date +'%Y-%m-%d %H:%M:%S')"

CXX = g++-11
SRCS = $(shell find . -maxdepth 2 -name showplayerxp.cpp -prune -o -name '*.cpp' -print)

OBJS = $(SRCS:cpp=o)
SPX_OBJS = DC.o interp.o Leaderboard.o vault.o utility.o const.o 
DEBUG_FLAGS = -ggdb3
PROF_FLAGS = $(DEBUG_FLAGS) -pg

LDFLAGS = -L/usr/local/lib -lcrypt -lmcheck -lz -lpq
CYGWIN_LDFLAGS = -L/emul/linux/x86/lib -L/usr/local/lib32 -lcrypt -lz -lpq /usr/local/lib/libXmlRpc.a
IN_CXXFLAGS = -std=c++20 -Wall -Wno-write-strings -DLINUX -m32 -Iinclude -I/usr/include/pgsql/ -I/usr/include/x86_64-linux-gnu/

BASENAME = dcastle# Base product name -- NO SPACES

default:
	@echo "Type make followed by one of:"
	@echo ""
	@echo "debug	Make with debug libraries/flags"
	@echo "simple	Make with no libraries/flags"
	@echo ""

DC.o: DC.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DVERSION=\"$(VERSION)\" -DBUILD_TIME=\"$(BUILD_TIME)\" -c -o $@ $<

%.C: %.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

install:
	mv /srv/dcastle/bin/$(BASENAME) /srv/dcastle/bin/$(BASENAME).old
	cp $(BASENAME) /srv/dcastle/bin/$(BASENAME)
	cp showplayer /srv/dcastle/bin/
	cp ../scripts/lastcore.sh /srv/dcastle/bin/

	mv /srv/dcastle2/bin/$(BASENAME) /srv/dcastle2/bin/$(BASENAME).old
	cp $(BASENAME) /srv/dcastle2/bin/$(BASENAME)
	cp showplayer /srv/dcastle2/bin/
	cp ../scripts/lastcore.sh /srv/dcastle2/bin/

	mv /srv/dcastle_test/bin/$(BASENAME) /srv/dcastle_test/bin/$(BASENAME).old
	cp $(BASENAME) /srv/dcastle_test/bin/$(BASENAME)
	cp showplayer /srv/dcastle_test/bin/
	cp ../scripts/lastcore.sh /srv/dcastle_test/bin/

all:	debug

debug:
	@$(MAKE) darkcastle PROGNAME=$(BASENAME) CXXFLAGS="$(IN_CXXFLAGS) $(DEBUG_FLAGS)"

simple:
	@$(MAKE) darkcastle PROGNAME=$(BASENAME).simple CXXFLAGS="$(IN_CXXFLAGS)"

darkcastle: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROGNAME) $(OBJS) $(LDFLAGS)

showplayerxp: $(OBJS)
	$(CXX) $(IN_CXXFLAGS) -o showplayerxp $(OBJS)

cygwin_darkcastle: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROGNAME) $(OBJS) $(CYGWIN_LDFLAGS)

clean:
	/bin/rm -f `find . -name '*.[oa]' -print`

