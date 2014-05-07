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

example : $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) example.cpp $(FLAGS) $(LIBS) -o example

