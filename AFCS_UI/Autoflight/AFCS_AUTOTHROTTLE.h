#pragma once

#include "../Interfaces/IAFCS_MODULE.h"
#include "../Utilities/PID_CONTROLLER_RESTRICTED_OUTPUT.h"

class AFCS_AUTOTHROTTLE : public IAFCS_MODULE
{
	Q_OBJECT

public:
	int SPEED_SELECT = 0;

	/**
	* AFCS autothrottle module constructor
	*
	* @param AFCS_CTL AFCS controller
	*/
	AFCS_AUTOTHROTTLE(IAFCS_CONTROLLER* AFCS_CTL);

private:
	double THROTTLE_CORRECTION_RATE = 25;

	/*
	* Functions runs the AFCS autothrottle
	*
	* @overrides RunModuleLogic method in AFCS module interface
	*/
	void RunModuleLogic() override;
};

