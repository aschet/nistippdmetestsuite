COMPILE = /itl/apps/gcc-2.95.2/SunOS-sun4u-5.6/bin/g++ -c -v -g

LINK = /itl/apps/gcc-2.95.2/SunOS-sun4u-5.6/bin/g++ -v

#INCLUDE = -Isource

bin/CSYmover: ofiles/CSYmover.o
	$(LINK) -o bin/CSYmover ofiles/CSYmover.o

ofiles/CSYmover.o: source/CSYmover.cc
	$(COMPILE) -o ofiles/CSYmover.o source/CSYmover.cc
