#include "CourseCalculation.h"
#include <unistd.h>
#include <iostream>

int main() {

	CourseCalculation cc;


	double boatLats [5] 		= {5.23, 10.52, 160.25, 56.24, 179.244};
	double boatLongs [5] 		= {10.442, 65.24, 24.44, 89.10, 25.25};
	double waypointLats [5] 	= {63.234, 42.53, 66.22, 90.34, 120.42};
	double waypointLongs [5] 	= {12.41, 42.52, 88.22, 54.52, 39.52};

	double boatLat = 5.2121;
	double boatLong = 145.2322;
	double waypointLat = 10;
	double waypointLong = 60.123;

	cc.setTACK_ANGLE(45);
	cc.setSECTOR_ANGLE(5);
	cc.setTWD(68);

	for(int i = 0; i < 5; i++) {

		cc.calculateBTW(boatLats[i], boatLongs[i], waypointLats[i], waypointLongs[i]);
		//cc.calculateDTW(boatLats[i], boatLongs[i], waypointLats[i], waypointLongs[i]);
		cc.calculateCTS();

		std::cout << "--------------------" << std::endl;
		std::cout << "BOAT-LAT________:" << boatLats[i] << std::endl;
		std::cout << "BOAT-LONG:______:" << boatLongs[i] << std::endl;
		std::cout << "WAYPOINT-LAT:___:" << waypointLats[i] << std::endl;
		std::cout << "WAYPOINT-LONG:__:" << waypointLongs[i] << std::endl;
		std::cout << "TWD:____________:" << cc.getTWD() << std::endl;
		std::cout << "BTW:____________:" << cc.getBTW() << std::endl;
		std::cout << "TACK:___________:" << cc.getTACK() << std::endl;
		//std::cout << "DTW: 	" << cc.getDTW() << std::endl;
		std::cout << "CTS:____________:" << cc.getCTS() << std::endl;
		std::cout << "--------------------" << std::endl;

		sleep(1);
	}
}
