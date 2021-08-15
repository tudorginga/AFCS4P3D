#include "AFCS_MANEUVER_PROTECTION.h"

AFCS_MANEUVER_PROTECTION::AFCS_MANEUVER_PROTECTION(IAFCS_CONTROLLER* AFCS_CTL) : IAFCS_MODULE(AFCS_CTL)
{
	SURFACE_CONTROLLER = new PID_CONTROLLER(PITCH_ELEVATOR_RECOVERY_FACTOR, 0, 0);
}

void AFCS_MANEUVER_PROTECTION::RunModuleLogic()
{
	double FLIGHT_PARAMETERS_G_FORCE = AFCS_CTL->FLIGHT_PARAMETERS.G_FORCE;
	double PITCH_DESIRED_RATE = AFCS_CTL->JOYSTICK_INPUT.Y_AXIS * PITCH_MAXIMUM_REQUESTABLE_RATE;

	/// If the overload is greater than the protection engagement threshold, linearly reduce the pitch autority to reach 0 at the critical overload
	if (FLIGHT_PARAMETERS_G_FORCE < MANEUVER_REDUCED_AUTHORITY_MINIMUM_THRESHOLD || FLIGHT_PARAMETERS_G_FORCE > MANEUVER_REDUCED_AUTHORITY_MAXIMUM_THRESHOLD)
	{
		double PITCH_MAXIMUM_RATE = 0;
		if (FLIGHT_PARAMETERS_G_FORCE > MANEUVER_REDUCED_AUTHORITY_MAXIMUM_THRESHOLD)
		{
			PITCH_MAXIMUM_RATE = PITCH_MAXIMUM_REQUESTABLE_RATE + (FLIGHT_PARAMETERS_G_FORCE - MANEUVER_REDUCED_AUTHORITY_MAXIMUM_THRESHOLD) * (0 - PITCH_MAXIMUM_REQUESTABLE_RATE) / (MANEUVER_MAXIMUM_VALUE - MANEUVER_REDUCED_AUTHORITY_MAXIMUM_THRESHOLD);
		}
		if (FLIGHT_PARAMETERS_G_FORCE < MANEUVER_REDUCED_AUTHORITY_MINIMUM_THRESHOLD)
		{
			PITCH_MAXIMUM_RATE = PITCH_MAXIMUM_REQUESTABLE_RATE + (FLIGHT_PARAMETERS_G_FORCE - MANEUVER_REDUCED_AUTHORITY_MINIMUM_THRESHOLD) * (0 - PITCH_MAXIMUM_REQUESTABLE_RATE) / (MANEUVER_MINIMUM_VALUE - MANEUVER_REDUCED_AUTHORITY_MINIMUM_THRESHOLD);
		}
		if (abs(PITCH_DESIRED_RATE) > PITCH_MAXIMUM_RATE)
		{
			PITCH_DESIRED_RATE = PITCH_MAXIMUM_RATE * Sign(PITCH_DESIRED_RATE);
		}
	}

	auto PITCH_CONTROLLER_OUTPUT = SURFACE_CONTROLLER->Update(PITCH_DESIRED_RATE - AFCS_CTL->FLIGHT_PARAMETERS.ROTATION_VELOCITY_BODY_Y);

	if (PITCH_CONTROLLER_OUTPUT > 1) PITCH_CONTROLLER_OUTPUT = 1;
	if (PITCH_CONTROLLER_OUTPUT < -1) PITCH_CONTROLLER_OUTPUT = -1;

	/// Send updated elevator position to the simulator
	AFCS_CTL->ELEVATOR_POSITION.ELEVATOR_POSITION = PITCH_CONTROLLER_OUTPUT;
	SimConnect_SetDataOnSimObject(AFCS_CTL->hSimConnect, AFCS_CTL->DEFINITION_ELEVATOR_POSITION, SIMCONNECT_OBJECT_ID_USER, 0, 1, sizeof(AFCS_CTL->ELEVATOR_POSITION), &AFCS_CTL->ELEVATOR_POSITION);
}
