#include "AFCS_COMMAND_PITCH.h"

AFCS_COMMAND_PITCH::AFCS_COMMAND_PITCH(IAFCS_CONTROLLER* AFCS_CTL) : IAFCS_MODULE(AFCS_CTL)
{
	VERTICAL_SPEED_CONTROLLER = new PID_CONTROLLER_RESTRICTED_OUTPUT(VERTICAL_SPEED_CORRECTION_RATE, 0, 0, -VERTICAL_SPEED_SELECT / 60, VERTICAL_SPEED_SELECT / 60);
	SURFACE_CONTROLLER = new PID_CONTROLLER_RESTRICTED_OUTPUT(ELEVATOR_CORRECTION_RATE, 0, 0, -0.5, 0.5);
}

void AFCS_COMMAND_PITCH::SetVerticalSpeed(double VERTICAL_SPEED)
{
	VERTICAL_SPEED_CONTROLLER->SetBound(-VERTICAL_SPEED / 60, VERTICAL_SPEED / 60);
	VERTICAL_SPEED_SELECT = VERTICAL_SPEED;
}

void AFCS_COMMAND_PITCH::RunModuleLogic()
{
	/// Vertical speed controller output is adjusted according to the difference between target altitude and actual altitude
	double ALTITUDE_ERROR = ALTITUDE_SELECT - AFCS_CTL->FLIGHT_PARAMETERS.INDICATED_ALTITUDE;
	double VERTICAL_SPEED_CONTROLLER_OUTPUT = VERTICAL_SPEED_CONTROLLER->Update(ALTITUDE_ERROR);

	/// Elevator position is adjusted according to the difference between target vertical speed and actual vertical speed
	double VERTICAL_SPEED_ERROR = VERTICAL_SPEED_CONTROLLER_OUTPUT - AFCS_CTL->FLIGHT_PARAMETERS.VERTICAL_SPEED;
	double ELEVATOR_POSITION = SURFACE_CONTROLLER->Update(VERTICAL_SPEED_ERROR);

	/// Send updated elevator position to the simulator
	AFCS_CTL->ELEVATOR_POSITION.ELEVATOR_POSITION = ELEVATOR_POSITION;
	SimConnect_SetDataOnSimObject(AFCS_CTL->hSimConnect, AFCS_CTL->DEFINITION_ELEVATOR_POSITION, SIMCONNECT_OBJECT_ID_USER, 0, 1, sizeof(AFCS_CTL->ELEVATOR_POSITION), &AFCS_CTL->ELEVATOR_POSITION);
}
