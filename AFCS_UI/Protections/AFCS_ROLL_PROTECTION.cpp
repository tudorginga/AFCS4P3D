#include "AFCS_ROLL_PROTECTION.h"

AFCS_ROLL_PROTECTION::AFCS_ROLL_PROTECTION(IAFCS_CONTROLLER* AFCS_CTL) : IAFCS_MODULE_THREADED(AFCS_CTL)
{
	SURFACE_CONTROLLER = new PID_CONTROLLER(ROLL_AILERON_RECOVERY_FACTOR, 0, 0);
}

void AFCS_ROLL_PROTECTION::RunModuleLogic()
{
	double ABS_FLIGHT_PARAMETERS_PLANE_BANK_DEGREES = abs(AFCS_CTL->FLIGHT_PARAMETERS.PLANE_BANK_DEGREES);
	double ROLL_DESIRED_RATE = AFCS_CTL->JOYSTICK_INPUT.X_AXIS * ROLL_MAXIMUM_REQUESTABLE_RATE;
	bool GOES_TOWARD_LIMIT = (Sign(AFCS_CTL->FLIGHT_PARAMETERS.PLANE_BANK_DEGREES) != Sign(AFCS_CTL->JOYSTICK_INPUT.X_AXIS));

	/// If the bank angle is greater than the maximum bank angle, use maximum aileron available to reduce it
	if (ABS_FLIGHT_PARAMETERS_PLANE_BANK_DEGREES >= ROLL_MAXIMUM_ANGLE)
	{
		ROLL_DESIRED_RATE = ROLL_RECOVERY_RATE * Sign(AFCS_CTL->FLIGHT_PARAMETERS.PLANE_BANK_DEGREES);
	}
	else
		/// If the bank angle is greater than the protection engagement threshold, linearly reduce the roll autority to reach 0 at the maximum bank angle
		if (ABS_FLIGHT_PARAMETERS_PLANE_BANK_DEGREES > ROLL_REDUCED_AUTHORITY_THRESHOLD && AFCS_CTL->JOYSTICK_INPUT.X_AXIS != 0 && GOES_TOWARD_LIMIT == true)
		{
			double ROLL_MAXIMUM_RATE = ROLL_MAXIMUM_REQUESTABLE_RATE + (ABS_FLIGHT_PARAMETERS_PLANE_BANK_DEGREES - ROLL_REDUCED_AUTHORITY_THRESHOLD) * (0 - ROLL_MAXIMUM_REQUESTABLE_RATE) / (ROLL_MAXIMUM_ANGLE - ROLL_REDUCED_AUTHORITY_THRESHOLD);

			if (abs(ROLL_DESIRED_RATE) > ROLL_MAXIMUM_RATE)
			{
				ROLL_DESIRED_RATE = ROLL_MAXIMUM_RATE * Sign(ROLL_DESIRED_RATE);
			}
		}
		else
			/// If the stick is released with the bank angle above the protection engagement threshold, recover to the nominal bank angle in order to exit the protection zone
			if (ABS_FLIGHT_PARAMETERS_PLANE_BANK_DEGREES > ROLL_REDUCED_AUTHORITY_THRESHOLD && AFCS_CTL->JOYSTICK_INPUT.X_AXIS == 0)
			{
				ROLL_DESIRED_RATE = ROLL_RECOVERY_RATE * Sign(AFCS_CTL->FLIGHT_PARAMETERS.PLANE_BANK_DEGREES);
			}

	auto ROLL_CONTROLLER_OUTPUT = SURFACE_CONTROLLER->Update(ROLL_DESIRED_RATE - AFCS_CTL->FLIGHT_PARAMETERS.ROTATION_VELOCITY_BODY_X);

	if (ROLL_CONTROLLER_OUTPUT > 1) ROLL_CONTROLLER_OUTPUT = 1;
	if (ROLL_CONTROLLER_OUTPUT < -1) ROLL_CONTROLLER_OUTPUT = -1;

	/// Send updated aileron position to the simulator
	AFCS_CTL->AILERON_POSITION.AILERON_POSITION = ROLL_CONTROLLER_OUTPUT;
	SimConnect_SetDataOnSimObject(AFCS_CTL->hSimConnect, AFCS_CTL->DEFINITION_AILERON_POSITION, SIMCONNECT_OBJECT_ID_USER, 0, 1, sizeof(AFCS_CTL->AILERON_POSITION), &AFCS_CTL->AILERON_POSITION);
}
