#pragma once

#include "../Interfaces/IAFCS_MODULE_THREADED.h"
#include "../Utilities/PID_CONTROLLER_RESTRICTED_OUTPUT.h"

class AFCS_COMMAND_ROLL : public IAFCS_MODULE_THREADED
{
public:
	int HEADING_SELECT = 0;

	/**
	* AFCS command roll module constructor
	*
	* @param AFCS_CTL AFCS controller
	*/
	AFCS_COMMAND_ROLL(IAFCS_CONTROLLER* AFCS_CTL);

private:
	PID_CONTROLLER_RESTRICTED_OUTPUT* HEADING_CONTROLLER;
	double HEADING_CORRECTION_RATE = DegreesToRadians(3);
	double AILERON_CORRECTION_RATE = 0.08 / DegreesToRadians(1);

	/*
	* Functions runs the AFCS command roll
	*
	* @overrides RunModuleLogic method in AFCS module interface
	*/
	void RunModuleLogic() override;
};

