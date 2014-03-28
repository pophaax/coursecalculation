#include "RudderCalculation.h"

const double PI = 3.1415926;
const double radiusOfEarth = 6378.1;

RudderCalculation::RudderCalculation() {
  PREVIOUS_ITERATION_TACK = false;
}

RudderCalculation::~RudderCalculation() {
}

void RudderCalculation::radianConversion(double  boatLat, double boatLong, double wpLat, double wpLong) {
 
  deltaLatitudeRadians = (wpLat-boatLat) * PI / 180;
  deltaLongitudeRadians = (wpLong - boatLong) * PI / 180;
  latitudeRadians = boatLat * PI / 180;
  wpLatitudeRadians = wpLat * PI / 180;
  longitudeRadians = boatLat * PI / 180;
  wpLongitudeRadians = wpLong * PI / 180;
}

void RudderCalculation::calculateBWP(double  boatLat, double boatLong, double wpLat, double wpLong) {
  
  radianConversion(boatLat, boatLong, wpLat, wpLong);
  
  double y = sin(deltaLongitudeRadians) * cos(wpLatitudeRadians);
  double x = cos(latitudeRadians) * sin(wpLatitudeRadians) -
	      sin(latitudeRadians) * cos(wpLatitudeRadians) * cos(deltaLongitudeRadians);
	      
  double bwp = atan2(y, x) / PI * 180;
  
  if (bwp < 0) {
    
    bwp = 360 + bwp;
  }
  
  this->BWP = bwp;
}

void RudderCalculation::calculateDWP(double  boatLat, double boatLong, double wpLat, double wpLong) {
 
  radianConversion(boatLat, boatLong, wpLat, wpLong);
  
  double a = sin(deltaLatitudeRadians/2) * sin(deltaLatitudeRadians/2) +
          sin(deltaLongitudeRadians/2) * sin(deltaLongitudeRadians/2) * 
          cos(latitudeRadians) * cos(wpLatitudeRadians);
  double c = 2 * atan2(sqrt(a), sqrt(1-a)); 
  double d = radiusOfEarth * c;
  d = d*1000;
  
  this->DWP = d;
}

int RudderCalculation::determineFirstCTS() {
  
  int cts;
  int port = countDown();
  int starboard = countUp();
    
  if (port > starboard) {
    cts = calculatePortCTS();
    GOING_STARBOARD = false;
  }
  
  else if (starboard >= port) {
    cts = calculateStarboardCTS();
    GOING_STARBOARD = true;
  }
  
  return cts;
}

bool RudderCalculation::continuePort() {
 
  int tmp_BWP = round(BWP);
  int tmp_TWD = TWD;
  bool continueDirection = false;
  int sectorBegin = tmp_TWD - TACK_ANGLE;
  
  if (sectorBegin < 0) {
   
    sectorBegin += 360;
  }

  for (int i=0; i < (TACK_ANGLE + SECTOR_ANGLE); i++) {
    
    if (tmp_BWP == sectorBegin) {
     
      continueDirection = true;
      break;
    }
    
    sectorBegin++;
    
    if (sectorBegin == 360) {
     
      sectorBegin = 0;
    }
  }
  
  return continueDirection;
}

bool RudderCalculation::continueStarboard() {
 
  int tmp_BWP = round(BWP);
  int tmp_TWD = TWD;
  bool continueDirection = false;
  int sectorBegin = tmp_TWD + TACK_ANGLE;
  
  if (sectorBegin >= 360) {
   
    sectorBegin -= 360;
  }

  for (int i=0; i < (TACK_ANGLE + SECTOR_ANGLE); i++) {

    if (tmp_BWP == sectorBegin) {
      continueDirection = true;
      break;
    }
    
    sectorBegin--;
    
    if (sectorBegin == -1) {
     
      sectorBegin = 359;
    }
  }
  
  return continueDirection;
}

double RudderCalculation::calculateStarboardCTS() {
  
  double cts;
  
  if (TWD + TACK_ANGLE > 360) {
    cts = TWD + TACK_ANGLE - 360;
  }
  else {
    cts = TWD + TACK_ANGLE;
  }
  
  return cts;
}

double RudderCalculation::calculatePortCTS() {
  
  double cts;
  
  if (TWD - TACK_ANGLE < 0) {
    cts = TWD - TACK_ANGLE + 360;
  }
  else {
    cts = TWD - TACK_ANGLE;
  }
  return cts;
}

void RudderCalculation::calculateCTS() {
  
  double cts;
   
  calculateTACK();
  
  if (TACK == true) {
    // We are going against the wind direction
    if (PREVIOUS_ITERATION_TACK == false) {
      // We just ended up in tack position
      cts = determineFirstCTS();
    }
    
    else {
      // We are still in tack position
      if (GOING_STARBOARD) {
	// Wind is coming from port but we are going in starboard direction relative to the wind
	if(!continueStarboard()) {
	   // CTS should switch to port side
	  GOING_STARBOARD = false;
	  cts = calculatePortCTS();
	}
	
	else {
	  // CTS should remain about the same
	  cts = calculateStarboardCTS();
	}
      }
      
      else if (!GOING_STARBOARD) {
	// Wind is coming from starboard but we are going in port direction relative to the wind
	if (!continuePort()) {
	  // CTS should switch to starboard side
	  GOING_STARBOARD = true;
	  cts = calculateStarboardCTS();
	}
	
	else {
	  // CTS should remain about the same
	  cts = calculatePortCTS();
	}
      }
    }
  }
  
  else {
    // We are not going against the wind direction
    cts = BWP;
  }
  
  CTS = cts;
  PREVIOUS_ITERATION_TACK = TACK;
  
}

int RudderCalculation::countUp() {
  
  int count = 0;
  int tmp_BWP = round(BWP);
  int tmp_TWD = TWD;
  bool go = true;
  
  for (int i=0; i<TACK_ANGLE && go == true; i++) {
   
    if (tmp_TWD == tmp_BWP) {
     go = false; 
    }
    
    count++;
    tmp_TWD++;
    
    if (tmp_TWD == 360) {
     
      tmp_TWD = 0;
    }
  }
  
  return count;
}

int RudderCalculation::countDown() {
  
  int count = 0;
  int tmp_BWP = round(BWP);
  int tmp_TWD = TWD;
  bool go = true;
  
  for (int i=0; i<TACK_ANGLE && go == true; i++) {
   
    if (tmp_TWD == tmp_BWP) {
     go = false; 
    }
    
    count++;
    tmp_TWD--;
    
    if (tmp_TWD == -1) {
     
      tmp_TWD = 359;
    }
  }
  
  return count;
}

bool RudderCalculation::calcUp() {
 
  int tmp_TWD = TWD;
  int tmp_BWP = round(BWP);
  
  bool tack = false;
  
  for (int i=0; i<TACK_ANGLE; i++) {
    
    if (tmp_TWD == tmp_BWP) {
    tack = true;
    }
    
    tmp_TWD++;
    
    if (tmp_TWD == 360) {
      tmp_TWD = 0;
    }
    
  }
  
  return tack;
}

bool RudderCalculation::calcDown() {
 
  int tmp_TWD = TWD;
  int tmp_BWP = round(BWP);
  bool tack = false;
  
  for (int i=0; i<TACK_ANGLE; i++) {
    
    if (tmp_TWD == tmp_BWP) {
    tack = true;
    }
    
    tmp_TWD--;
    
    if (tmp_TWD == -1) {
      tmp_TWD = 359;
    }
    
  }
  
  return tack;
}

bool RudderCalculation::calculateTACK() {
  
  if (calcUp() || calcDown()) {
    TACK = true;
  }
  
  else {
    TACK = false;
  }

  return TACK;
}

void RudderCalculation::setTWD(double degrees) {
  this->TWD = degrees;
}

void RudderCalculation::setBWP(double degrees) {
  this->BWP = degrees;
}

void RudderCalculation::setTACK_ANGLE(double degrees) {
  this->TACK_ANGLE = degrees;
}

void RudderCalculation::setSECTOR_ANGLE(double degrees) {
  this->SECTOR_ANGLE = degrees;
}

double RudderCalculation::getCTS() {
 
  return CTS;
}

double RudderCalculation::getBWP() {
  
  return BWP;
}

double RudderCalculation::getDWP() {
  
  return DWP;
}

double RudderCalculation::getTWD() {
  
  return TWD;
}

bool RudderCalculation::getTACK() {
 
  return TACK;
}