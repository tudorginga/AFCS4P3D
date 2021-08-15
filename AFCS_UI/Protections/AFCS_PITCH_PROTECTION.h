#pragma once

#include "../Interfaces/IAFCS_MODULE_THREADED.h"

class AFCS_PITCH_PROTECTION : public IAFCS_MODULE_THREADED
{
public:
	double PITCH_MAXIMUM_POSITIVE_ANGLE = DegreesToRadians(-30);
	double PITCH_MAXIMUM_NEGATIVE_ANGLE = DegreesToRadians(15);
	double PITCH_REDUCED_AUTHORITY_POSITIVE_THRESHOLD = DegreesToRadians(-25);
	double PITCH_REDUCED_AUTHORITY_NEGATIVE_THRESHOLD = DegreesToRadians(10);

	/**
	* AFCS pitch protection module constructor
	*
	* @param AFCS_CTL AFCS controller
	*/
	AFCS_PITCH_PROTECTION(IAFCS_CONTROLLER* AFCS_CTL);

private:
	double PITCH_ELEVATOR_RECOVERY_FACTOR = 10;
	double PITCH_MAXIMUM_REQUESTABLE_RATE = 0.15;
	double PITCH_RECOVERY_RATE = DegreesToRadians(5);

	/*
	* Functions runs the AFCS pitch protection
	*
	* @overrides RunModuleLogic method in AFCS module interface
	*/
	void RunModuleLogic() override;
};

