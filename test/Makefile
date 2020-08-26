include ../Makefile.inc

SRC = $(wildcard ./*.cc)
EXEC = $(SRC:.cc=.exe)

LDFLAGS += -lgtest -lgtest_main -lpthread

.PHONY: all clean

all: $(EXEC)

clean:
	rm -f $(EXEC) $(EXE_OBJ)

$(EXEC): %.exe:%.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	rm -f $^

