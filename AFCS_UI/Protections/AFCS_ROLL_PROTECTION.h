#pragma once

#include "../Interfaces/IAFCS_MODULE_THREADED.h"

class AFCS_ROLL_PROTECTION : public IAFCS_MODULE_THREADED
{
public:
	double ROLL_REDUCED_AUTHORITY_THRESHOLD = DegreesToRadians(33);
	double ROLL_MAXIMUM_ANGLE = DegreesToRadians(67);

	/**
	* AFCS roll protection module constructor
	*
	* @param AFCS_CTL AFCS controller
	*/
	AFCS_ROLL_PROTECTION(IAFCS_CONTROLLER* AFCS_CTL);

private:
	double ROLL_AILERON_RECOVERY_FACTOR = 6.6;
	double ROLL_MAXIMUM_REQUESTABLE_RATE = 0.15;
	double ROLL_RECOVERY_RATE = DegreesToRadians(5);

	/*
	* Functions runs the AFCS roll protection
	*
	* @overrides RunModuleLogic method in AFCS module interface
	*/
	void RunModuleLogic() override;
};

