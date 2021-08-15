#include "AFCS_ALFA_PROTECTION.h"

AFCS_ALFA_PROTECTION::AFCS_ALFA_PROTECTION(IAFCS_CONTROLLER* AFCS_CTL) : IAFCS_MODULE(AFCS_CTL)
{
	SURFACE_CONTROLLER = new PID_CONTROLLER(PITCH_ELEVATOR_RECOVERY_FACTOR, 0, 0);
}

inline void AFCS_ALFA_PROTECTION::SetTOGAThrust()
{
	/// Send command for 100% Throttle Lever Angle (TLA) to the simulator
	AFCS_CTL->GENERAL_ENG_THROTTLE_LEVER_POSITION.GENERAL_ENG_THROTTLE_LEVER_POSITION_1 = 100.0f;
	AFCS_CTL->GENERAL_ENG_THROTTLE_LEVER_POSITION.GENERAL_ENG_THROTTLE_LEVER_POSITION_2 = 100.0f;
	AFCS_CTL->GENERAL_ENG_THROTTLE_LEVER_POSITION.GENERAL_ENG_THROTTLE_LEVER_POSITION_3 = 100.0f;
	AFCS_CTL->GENERAL_ENG_THROTTLE_LEVER_POSITION.GENERAL_ENG_THROTTLE_LEVER_POSITION_4 = 100.0f;

	SimConnect_SetDataOnSimObject(AFCS_CTL->hSimConnect, AFCS_CTL->DEFINITION_GENERAL_ENG_THROTTLE_LEVER_POSITION, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(AFCS_CTL->GENERAL_ENG_THROTTLE_LEVER_POSITION), &AFCS_CTL->GENERAL_ENG_THROTTLE_LEVER_POSITION);
}

void AFCS_ALFA_PROTECTION::RunModuleLogic()
{
	double FLIGHT_PARAMETERS_INCIDENCE_ALPHA = AFCS_CTL->FLIGHT_PARAMETERS.INCIDENCE_ALPHA;
	double ALFA_DESIRED_RATE = AFCS_CTL->JOYSTICK_INPUT.Y_AXIS * PITCH_MAXIMUM_REQUESTABLE_RATE;
	bool GOES_TOWARD_LIMIT = (Sign(FLIGHT_PARAMETERS_INCIDENCE_ALPHA) == Sign(ALFA_DESIRED_RATE));

	/// If the angle of attack is greater than the critical angle, use maximum elevator available to reduce it
	if (FLIGHT_PARAMETERS_INCIDENCE_ALPHA >= ALFA_MAXIMUM_ANGLE)
	{
		ALFA_DESIRED_RATE = (-1) * PITCH_RECOVERY_RATE * Sign(FLIGHT_PARAMETERS_INCIDENCE_ALPHA);

		/// If the autothrust connection to the simulator is not inhibited, set power to maximum thrust
		if (AFCS_CTL->AFCS_ENGAGEMENT_STATUS.AUTOTHROTTLE_INHIBIT != true) SetTOGAThrust();
	}
	else
		/// If the angle of attack is greater than the protection engagement threshold, linearly reduce the pitch autority to reach 0 at the critical angle
		if (FLIGHT_PARAMETERS_INCIDENCE_ALPHA > ALFA_REDUCED_AUTHORITY_THRESHOLD && GOES_TOWARD_LIMIT == true && AFCS_CTL->JOYSTICK_INPUT.Y_AXIS != 0)
		{
			double ALFA_MAXIMUM_RATE = PITCH_MAXIMUM_REQUESTABLE_RATE + (FLIGHT_PARAMETERS_INCIDENCE_ALPHA - ALFA_REDUCED_AUTHORITY_THRESHOLD) * (0 - PITCH_MAXIMUM_REQUESTABLE_RATE) / (ALFA_MAXIMUM_ANGLE - ALFA_REDUCED_AUTHORITY_THRESHOLD);

			if (abs(ALFA_DESIRED_RATE) > ALFA_MAXIMUM_RATE)
				ALFA_DESIRED_RATE = ALFA_MAXIMUM_RATE * Sign(ALFA_DESIRED_RATE);

			/// If the autothrust connection to the simulator is not inhibited, set power to maximum thrust
			if (AFCS_CTL->AFCS_ENGAGEMENT_STATUS.AUTOTHROTTLE_INHIBIT != true) SetTOGAThrust();
		}

	auto ALFA_CONTROLLER_OUTPUT = SURFACE_CONTROLLER->Update(ALFA_DESIRED_RATE - AFCS_CTL->FLIGHT_PARAMETERS.ROTATION_VELOCITY_BODY_Y);

	if (ALFA_CONTROLLER_OUTPUT > 1) ALFA_CONTROLLER_OUTPUT = 1;
	if (ALFA_CONTROLLER_OUTPUT < -1) ALFA_CONTROLLER_OUTPUT = -1;

	/// Send updated elevator position to the simulator
	AFCS_CTL->ELEVATOR_POSITION.ELEVATOR_POSITION = ALFA_CONTROLLER_OUTPUT;
	SimConnect_SetDataOnSimObject(AFCS_CTL->hSimConnect, AFCS_CTL->DEFINITION_ELEVATOR_POSITION, SIMCONNECT_OBJECT_ID_USER, 0, 1, sizeof(AFCS_CTL->ELEVATOR_POSITION), &AFCS_CTL->ELEVATOR_POSITION);
}
