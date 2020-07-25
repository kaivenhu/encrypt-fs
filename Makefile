
CXXFLAGS += -std=c++17 -g -Wall -W -Werror -fPIC \
			-DFUSE_USE_VERSION=30 `pkg-config fuse --cflags`
LDFLAGS += `pkg-config fuse --libs`

EXEC=encfs
EXE_SRC = main.cc
EXE_OBJ = $(EXE_SRC:.cc=.o)


.PHONY: all clean

all: $(EXEC)

clean:
	rm -f $(EXEC) $(EXE_OBJ)

$(EXEC): $(EXE_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	rm -f $^

