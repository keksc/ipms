CXX = $(shell wx-config --cxx)
WXFLAGS = $(shell wx-config --cxxflags)
WXLIBS = $(shell wx-config --libs)

EXE = ipms.out
SRCS = App.cpp Main.cpp
OBJS = $(SRCS:.cpp=.o)
#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG -Wall

#
# Release build settings
#
RELDIR = release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3 -DNDEBUG -Wall

.PHONY: all clean debug prep release remake

all: prep debug


#
# Debug rules
#

debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CXX) $(DBGCFLAGS) -o $(DBGEXE) $^ $(WXLIBS)

$(DBGDIR)/%.o: %.cpp
	$(CXX) -c $(WXFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#

release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CXX) $(RELCFLAGS) -o $(RELEXE) $^ $(WXLIBS)

$(RELDIR)/%.o: %.cpp
	$(CXX) -c $(WXFLAGS) $(RELCFLAGS) -o $@ $<

#
# Other rules
#
prep:
	mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
