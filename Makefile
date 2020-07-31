include Makefile.inc

EXEC=encfs
EXE_SRC = main.cc
EXE_OBJ = $(EXE_SRC:.cc=.o)


.PHONY: all clean $(SUB_DIR) check

all: $(SUB_DIR) $(EXEC)

clean: $(SUB_DIR)
	rm -f $(EXEC) $(EXE_OBJ)

$(SUB_DIR):
	$(MAKE) -C $@ $(MAKECMDGOALS);

$(EXEC): $(EXE_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	rm -f $^

check:
	@test/script.sh

