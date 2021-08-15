#pragma once

#include "../Interfaces/IAFCS_MODULE.h"

class AFCS_OVERSPEED_PROTECTION : public IAFCS_MODULE
{
public:
	/**
	* AFCS overspeed protection module constructor
	*
	* @param AFCS_CTL AFCS controller
	*/
	AFCS_OVERSPEED_PROTECTION(IAFCS_CONTROLLER* AFCS_CTL);

private:
	double PITCH_ELEVATOR_RECOVERY_FACTOR = 10;
	double PITCH_RECOVERY_RATE = DegreesToRadians(5);

	/// Function sets power to flight idle
	inline void SetIdleThrust();

	/*
	* Functions runs the AFCS overspeed protection
	*
	* @overrides RunModuleLogic method in AFCS module interface
	*/
	void RunModuleLogic() override;
};

