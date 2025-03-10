COMPILE = /itl/apps/gcc-2.95.2/SunOS-sun4u-5.6/bin/g++ -c -v -g
LINK = /itl/apps/gcc-2.95.2/SunOS-sun4u-5.6/bin/g++ -v

#INCLUDE = -Isource

bin/parserCmd: ofiles/parserCmd.o ofiles/IppCmdRes.o
	$(LINK) -o bin/parserCmd ofiles/IppCmdRes.o ofiles/parserCmd.o

ofiles/parserCmd.o: source/parserCmd.cc\
                 source/parserCmd.h\
                 source/IppCmdRes.h
	$(COMPILE) -DPARSER_MAIN -o ofiles/parserCmd.o source/parserCmd.cc

ofiles/IppCmdRes.o: source/IppCmdRes.cc source/IppCmdRes.h
	$(COMPILE) -o ofiles/IppCmdRes.o source/IppCmdRes.cc
