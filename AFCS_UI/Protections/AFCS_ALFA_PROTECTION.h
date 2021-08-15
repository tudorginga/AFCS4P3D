#pragma once

#include "../Interfaces/IAFCS_MODULE.h"

class AFCS_ALFA_PROTECTION : public IAFCS_MODULE
{
public:
	double ALFA_MAXIMUM_ANGLE = DegreesToRadians(10.2);
	double ALFA_REDUCED_AUTHORITY_THRESHOLD = DegreesToRadians(8.787);

	/**
	* AFCS alfa protection module constructor
	*
	* @param AFCS_CTL AFCS controller
	*/
	AFCS_ALFA_PROTECTION(IAFCS_CONTROLLER* AFCS_CTL);

private:
	double PITCH_ELEVATOR_RECOVERY_FACTOR = 10;
	double PITCH_MAXIMUM_REQUESTABLE_RATE = 0.15;
	double PITCH_RECOVERY_RATE = DegreesToRadians(5);

	/// Function sets power to maximum thrust
	inline void SetTOGAThrust();

	/*
	* Functions runs the AFCS alfa protection
	*
	* @overrides RunModuleLogic method in AFCS module interface
	*/
	void RunModuleLogic() override;
};

