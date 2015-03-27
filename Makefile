#######################################################
#
#    Aland Sailing Robot
#    ===========================================
#    coursecalculation
#    -------------------------------------------
#
#######################################################

CC = g++
FLAGS = -Wall -pedantic -Werror
LIBS =

SOURCES = CourseCalculation.cpp
HEADERS = CourseCalculation.h
FILE = CourseCalculation.o



all : $(FILE)
$(FILE) : $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) $(FLAGS) $(LIBS) -c -o $(FILE)

example : $(SOURCES) $(HEADERS) example.cpp
	$(CC) $(SOURCES) example.cpp $(FLAGS) $(LIBS) -o example

test : $(SOURCES) $(HEADERS) catch.hpp testCourseCalculation.cpp
	$(CC) $(SOURCES) testCourseCalculation.cpp $(LIBS) -o test


clean :
	rm -f $(FILE)
	rm -f example
	rm -f test