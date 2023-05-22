CC=g++
OBJARGS=
CCARGS=
HASHSRCS = main.cpp hashtable.cpp
##HASHSRCS = Main.c HashTable.c
TESTSRCS = tester.cpp
TESTGENSRCS=testGenerator.cpp
GNUPLOTSRC=gnuPlotScript.gp
DEMOSRCS=demo.cpp
OBJDIR=obj/
DEPDIR=dep/

all: $(OBJDIR) $(DEPDIR) hash.exe test.exe testGen.exe 

$(OBJDIR)%.o: %.cpp
	$(CC) -M $(CCARGS) $< -o $(DEPDIR)$(<:.cpp=.d)
	echo "$(OBJDIR)" > line.tmp
	mv $(DEPDIR)$(<:.cpp=.d) input.tmp
	head -c -1 -q line.tmp input.tmp > $(DEPDIR)$(<:.cpp=.d)
	rm input.tmp line.tmp
	$(CC) -c $(CCARGS) $< -o $@

include $(wildcard $(DEPDIR)/*.d)

hash.exe: $(addprefix $(OBJDIR), $(HASHSRCS:.cpp=.o))
	$(CC) $(OBJARGS) $^ -o $@

test.exe: $(addprefix $(OBJDIR), $(TESTSRCS:.cpp=.o))
	$(CC) $(OBJARGS) $^ -o $@

testGen.exe: $(addprefix $(OBJDIR), $(TESTGENSRCS:.cpp=.o))
	$(CC) $(OBJARGS) $^ -o $@

graph:
	gnuplot $(GNUPLOTSRC)

demo: $(DEMOSRCS)
	make all
	$(CC) $(CC_ARGS) $(OBJ_ARGS) $^ -o hash.exe
	touch $(HASHSRCS)

$(OBJDIR):
	mkdir $@

$(DEPDIR):
	mkdir $@

clean:
	rm -rf *.o *.d *.exe
	rm -rf $(DEPDIR) $(OBJDIR)
	clear

run: all
	./test.exe

.PHONY: clean all graph