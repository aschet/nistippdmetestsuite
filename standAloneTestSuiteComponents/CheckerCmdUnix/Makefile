COMPILE = /itl/apps/gcc-2.95.2/SunOS-sun4u-5.6/bin/g++ -c -v -g
LINK = /itl/apps/gcc-2.95.2/SunOS-sun4u-5.6/bin/g++ -v

#INCLUDE = -Isource

bin/checkerCmd: ofiles/checkerCmd.o\
                ofiles/parserCmd.o\
                ofiles/world.o\
                ofiles/IppCmdRes.o
	$(LINK) -o bin/checkerCmd ofiles/IppCmdRes.o\
                                  ofiles/parserCmd.o\
                                  ofiles/world.o\
                                  ofiles/checkerCmd.o

ofiles/checkerCmd.o: source/checkerCmd.cc\
                     source/checkerCmd.h\
                     source/tools.h\
                     source/world.h\
                     source/IppCmdRes.h
	$(COMPILE) -DCHECKER_MAIN -o ofiles/checkerCmd.o source/checkerCmd.cc

ofiles/parserCmd.o: source/parserCmd.cc\
                 source/parserCmd.h\
                 source/IppCmdRes.h
	$(COMPILE) -o ofiles/parserCmd.o source/parserCmd.cc

ofiles/IppCmdRes.o: source/IppCmdRes.cc source/IppCmdRes.h
	$(COMPILE) -o ofiles/IppCmdRes.o source/IppCmdRes.cc

ofiles/world.o: source/world.cc\
                source/tools.h\
                source/world.h\
                source/IppCmdRes.h
	$(COMPILE) -o ofiles/world.o source/world.cc

