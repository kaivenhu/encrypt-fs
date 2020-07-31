include ../Makefile.inc

SRC = $(wildcard ./*.cc)
OBJ = $(SRC:.cc=.o)

all: $(SHLIB)

$(SHLIB): $(OBJ)
	$(CXX) -shared -o $@ $^

$(OBJ): %.o:%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJ) $(SHLIB)

