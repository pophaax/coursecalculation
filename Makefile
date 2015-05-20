#######################################################
#
#    Aland Sailing Robot
#    ===========================================
#    coursecalculation
#    -------------------------------------------
#
#######################################################

CC = g++
FLAGS = -Wall -pedantic -Werror -std=c++14
LIBS =

SOURCES = CourseCalculation.cpp
HEADERS = CourseCalculation.h
FILE = CourseCalculation.o



all : $(FILE)
$(FILE) : $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) $(FLAGS) $(LIBS) -c -o $(FILE)

example : $(SOURCES) $(HEADERS) example.cpp
	$(CC) $(SOURCES) example.cpp $(FLAGS) $(LIBS) -o example

metatest : $(SOURCES) $(HEADERS) $(SAILINGROBOTS_HOME)/tests/catch.hpp $(SAILINGROBOTS_HOME)/tests/testsuite/testCourseCalculation.cpp
	$(CC) $(SOURCES) $(SAILINGROBOTS_HOME)/tests/testsuite/testCourseCalculation.cpp -fprofile-arcs -ftest-coverage $(LIBS) -o metatest 


clean :
	rm -f $(FILES)
	rm -f example
	rm -f metatest
	rm -f *.gcda
	rm -f *.gcno

metalog :
	make metatest
	./metatest
	gcov -r CourseCalculation.cpp
	grep -wE "(#####)" CourseCalculation.cpp.gcov > metatestlog.txt
	rm -f *.gcov
	make clean
	sed -i '1s/^/Codelines below not tested by test*.cpp\n/' metatestlog.txt