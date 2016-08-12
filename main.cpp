/*
 * train.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: isaac
 */

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <string>

#include "DriverMotionSensor.h"
#include "LocomotiveController.h"
#include "DerailDetector.h"
#include "LocationTracker.h"

using namespace std;
using namespace BlackLib;

int main()
{
	DriverMotionSensor dms = DriverMotionSensor();
	LocomotiveController lc=LocomotiveController();
	DerailDetector dd=DerailDetector();
	SoundSystem ss=SoundSystem();
	LocationTracker lt=LocationTracker();
	LCDVerification lv=LCDVerification();
	SystemRecorder sr=SystemRecorder();

  char t='1';
  while (1){

	switch(t){
	case '1':
		if(dd.getDerail()==true){   // check for derail
			sr.setValue(high);		// set record on
			t='4';			// go to case 4
				}

		if(lc.getSpeed()==1){	// check for speed
			t='5';
		}

		if(lt.getStatus()==0){	// check for entering zon 1
			sr.setValue(high);		// set record on
			t='2';					// go to case 2
				}
		break;
	case '2':
		if(dms.getMotion()==true){	// detect motion
			cout<<"Verification"<<endl;

			if(lv.getStatus()==true){ // got verification
				t='4';				// go to case 4
			}
			if(lv.getStatus()==false && lt.getStatus()==1)	  // driver no verify and entering zon 2
				string writeToUart="Driver no verification\n";// write to control
				t='3';				// go to case 3
			}
		break;

		if(dms.getMotion()==false){	// unable to detct motion
			ss.setValue(1);			// turn on sound alert on

			cout<<"Verification"<<endl;

			if(lv.getStatus==true){	// got verification
			ss.setValue(0);			// turn off sound alert
				t='4';				// go to case 4
			}
			if (lv.getStatus()==false && lt.getStatus()==1) {		// driver no response and entering zon 2
				string writeToUart= "DRIVER NO RESPONSE, ENTERING ZON 2 \n";	// contact control
				t='3';				// go to case 3
			}
		}
		break;
	case '3':
			string readFromUart= myUart.read(); // receive from control
			cout<<""<<readFromUart<<endl;
			if (lv.getStatus()==true){			// suddenly driver response
				t='4';							// go to case 4
			}
			if(readFromUart==false && lt.getStatus()==2 ){	// no response from control
				t='4';							// go to case 4
			}
		break;


	case '4':

		lc.setSpeed(0);				// stop pwm
		sr.setValue(low);			// stop system recorder
		cout<<"Train stop"<<endl;
		break;

	case '5':
		ss.setValue(1);			// sound alert driver
		lc.setSpeed(2);			// set pwm speed slower
		t='1';					// go back to case 1
		break;

	}
  }}

