#pragma once

#include "../Interfaces/IAFCS_MODULE.h"
#include "../Utilities/PID_CONTROLLER_RESTRICTED_OUTPUT.h"

class AFCS_COMMAND_PITCH : public IAFCS_MODULE
{
public:
	int ALTITUDE_SELECT = 0;
	int VERTICAL_SPEED_SELECT = 0;

	/**
	* AFCS command pitch module constructor
	*
	* @param AFCS_CTL AFCS controller
	*/
	AFCS_COMMAND_PITCH(IAFCS_CONTROLLER* AFCS_CTL);

	/**
	* Vertical speed setting setter
	*
	* @param VERTICAL_SPEED new VERTICAL_SPEED to be set
	*/
	void SetVerticalSpeed(double VERTICAL_SPEED);

private:
	PID_CONTROLLER_RESTRICTED_OUTPUT* VERTICAL_SPEED_CONTROLLER;
	double VERTICAL_SPEED_CORRECTION_RATE = 0.1;
	double ELEVATOR_CORRECTION_RATE = 0.04;

	/*
	* Functions runs the AFCS command pitch
	*
	* @overrides RunModuleLogic method in AFCS module interface
	*/
	void RunModuleLogic() override;
};

