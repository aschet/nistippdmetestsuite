COMPILE = /itl/apps/gcc-2.95.2/SunOS-sun4u-5.6/bin/g++ -c -v -g
LINK = /itl/apps/gcc-2.95.2/SunOS-sun4u-5.6/bin/g++ -v

#INCLUDE = -Isource

bin/parserRes: ofiles/parserRes.o ofiles/IppCmdRes.o
	$(LINK) -o bin/parserRes ofiles/IppCmdRes.o ofiles/parserRes.o

ofiles/parserRes.o: source/parserRes.cc\
                 source/parserRes.h\
                 source/IppCmdRes.h
	$(COMPILE) -DPARSER_MAIN -o ofiles/parserRes.o source/parserRes.cc

ofiles/IppCmdRes.o: source/IppCmdRes.cc source/IppCmdRes.h
	$(COMPILE) -o ofiles/IppCmdRes.o source/IppCmdRes.cc
