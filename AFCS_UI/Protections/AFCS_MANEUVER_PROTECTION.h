#pragma once

#include "../Interfaces/IAFCS_MODULE.h"

class AFCS_MANEUVER_PROTECTION : public IAFCS_MODULE
{
public:
	double MANEUVER_MAXIMUM_VALUE = 2.5;
	double MANEUVER_MINIMUM_VALUE = -1.0;
	double MANEUVER_REDUCED_AUTHORITY_MAXIMUM_THRESHOLD = 2.0;
	double MANEUVER_REDUCED_AUTHORITY_MINIMUM_THRESHOLD = -0.5;

	/**
	* AFCS maneuver protection module constructor
	*
	* @param AFCS_CTL AFCS controller
	*/
	AFCS_MANEUVER_PROTECTION(IAFCS_CONTROLLER* AFCS_CTL);

private:
	double PITCH_ELEVATOR_RECOVERY_FACTOR = 10;
	double PITCH_MAXIMUM_REQUESTABLE_RATE = 0.15;

	/*
	* Functions runs the AFCS maneuver protection
	*
	* @overrides RunModuleLogic method in AFCS module interface
	*/
	void RunModuleLogic() override;
};

