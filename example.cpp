#include "CourseCalculation.h"
#include <unistd.h>
#include <iostream>

int main() {

	CourseCalculation cc;

	double boatLat = 5.2121;
	double boatLong = 145.2322;
	double waypointLat = 10;
	double waypointLong = 60.123;

	cc.setTACK_ANGLE(45);
	cc.setSECTOR_ANGLE(5);
	cc.setTWD(5);

	while (true) {

		cc.calculateBTW(boatLat, boatLong, waypointLat, waypointLong);
		cc.calculateDTW(boatLat, boatLong, waypointLat, waypointLong);
		cc.calculateCTS();

		std::cout << "--------------------" << std::endl;
		std::cout << "TWD: 	" << cc.getTWD() << std::endl;
		std::cout << "BTW: 	" << cc.getBTW() << std::endl;
		std::cout << "TACK: " << cc.getTACK() << std::endl;
		std::cout << "DTW: 	" << cc.getDTW() << std::endl;
		std::cout << "CTS: 	" << cc.getCTS() << std::endl;
		std::cout << "--------------------" << std::endl;

		sleep(1);
	}
}
