#include "RudderCalculation.h"
#include <unistd.h>

int main() {
 
  RudderCalculation rc;
  
  double boatLat = 0;
  double boatLong = 0;
  double waypointLat = 10;
  double waypointLong = 0;
  
  rc.setTACK_ANGLE(45);
  rc.setSECTOR_ANGLE(5);
  rc.setTWD(5);
  
  while (true) {
    
  rc.calculateBWP(boatLat,boatLong,waypointLat, waypointLong);
  rc.calculateDWP(boatLat,boatLong,waypointLat, waypointLong);
  rc.calculateCTS();
  
  std::cout << "--------------------" << std::endl;
  std::cout << "TWD: 	" << rc.getTWD()	 << std::endl;
  std::cout << "BWP: 	" << rc.getBWP()	 << std::endl;
  std::cout << "TACK: 	" << rc.getTACK()	 << std::endl;
  std::cout << "DWP: 	" << rc.getDWP()	 << std::endl;
  std::cout << "CTS: 	" << rc.getCTS()	 << std::endl;
  std::cout << "--------------------" << std::endl;
  
  sleep(1);
  }
} 
