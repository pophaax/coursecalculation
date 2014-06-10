#include "CourseCalculation.h"
#include <unistd.h>
#include <iostream>

int main() {

	CourseCalculation cc;

	double boatLats [3] 		= {60.836881, 60.073063, 59.539888 };
	double boatLongs [3] 		= {19.143219, 22.185974, 19.894409};
	double waypointLats [3] 	= {60.103333, 60.103172, 60.103362};
	double waypointLongs [3] 	= {19.928306, 19.92834, 19.928601};


	cc.setTACK_ANGLE(45);
	cc.setSECTOR_ANGLE(5);
	cc.setTWD(49);

	for(int i = 0; i < 3; i++) {

		cc.calculateBTW(boatLats[i], boatLongs[i], waypointLats[i], waypointLongs[i]);
		cc.calculateDTW(boatLats[i], boatLongs[i], waypointLats[i], waypointLongs[i]);
		
		cc.calculateCTS();

		std::cout << "--------------------" << std::endl;
		std::cout.precision(10);
		std::cout << "BOAT-LAT________:" << boatLats[i] << std::endl;
		std::cout << "BOAT-LONG:______:" << boatLongs[i] << std::endl;
		std::cout << "WAYPOINT-LAT:___:" << waypointLats[i] << std::endl;
		std::cout << "WAYPOINT-LONG:__:" << waypointLongs[i] << std::endl;
		std::cout << "TWD:____________:" << cc.getTWD() << std::endl;
		std::cout << "BTW:____________:" << cc.getBTW() << std::endl;
		std::cout << "TACK:___________:" << cc.getTACK() << std::endl;
		std::cout << "DTW:____________:" << cc.getDTW() << std::endl;
		std::cout << "CTS:____________:" << cc.getCTS() << std::endl;
		std::cout << "--------------------" << std::endl;

		//sleep(1);
	}
}
