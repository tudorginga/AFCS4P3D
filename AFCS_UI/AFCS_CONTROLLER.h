#pragma once

#include "Interfaces/IAFCS_CONTROLLER.h"
#include "Autoflight/AFCS_AUTOTHROTTLE.h"
#include "Autoflight/AFCS_COMMAND_PITCH.h"
#include "Autoflight/AFCS_COMMAND_ROLL.h"
#include "Protections/AFCS_ALFA_PROTECTION.h"
#include "Protections/AFCS_MANEUVER_PROTECTION.h"
#include "Protections/AFCS_OVERSPEED_PROTECTION.h"
#include "Protections/AFCS_PITCH_PROTECTION.h"
#include "Protections/AFCS_ROLL_PROTECTION.h"

class AFCS_CONTROLLER : public IAFCS_CONTROLLER
{
	Q_OBJECT

public:
	AFCS_AUTOTHROTTLE* AFCS_AT = new AFCS_AUTOTHROTTLE(this);
	AFCS_COMMAND_PITCH* AFCS_CMD_PITCH = new AFCS_COMMAND_PITCH(this);
	AFCS_COMMAND_ROLL* AFCS_CMD_ROLL = new AFCS_COMMAND_ROLL(this);
	AFCS_ALFA_PROTECTION* AFCS_ALFA_PROT = new AFCS_ALFA_PROTECTION(this);
	AFCS_MANEUVER_PROTECTION* AFCS_MANEUVER_PROT = new AFCS_MANEUVER_PROTECTION(this);
	AFCS_OVERSPEED_PROTECTION* AFCS_OVERSPEED_PROT = new AFCS_OVERSPEED_PROTECTION(this);
	AFCS_PITCH_PROTECTION* AFCS_PITCH_PROT = new AFCS_PITCH_PROTECTION(this);
	AFCS_ROLL_PROTECTION* AFCS_ROLL_PROT = new AFCS_ROLL_PROTECTION(this);

signals:
	/// Function updates UI according to autoflight disengaged AFCS state
	void AutoflightDisengaged();

	/// Function initializes UI after AFCS start
	void InitializeUI();

	/// Function updates UI with new flight parameters at each new simulation frame update
	void UpdateFlightParameters();

private:
	bool CONNECTION_STATUS = false;
	double JOYSTICK_RESOLUTION = 32768;

	enum EVENTS
	{
		JOYSTICK_EVENT_X_AXIS,
		JOYSTICK_EVENT_Y_AXIS
	};

	enum GROUPS
	{
		GROUP_0
	};

	enum INPUTS
	{
		JOYSTICK_INPUT_X_AXIS,
		JOYSTICK_INPUT_Y_AXIS
	};

	enum REQUESTS
	{
		REQUEST_FLIGHT_PARAMETERS
	};

	/// Function sets up simulator interaction
	void SetupPrepar3DInteractions();

	/**
	* Function executes autoflight system logic
	*
	* @param OVERRIDE_CONDITION dictates whether autoflight system will execute or not
	*/
	void RunAutoflightLogic(bool OVERRIDE_CONDITION);

	/**
	* Function executes AFCS protections logic
	*
	* @returns true if any protection is engaged, false otherwise
	*/
	bool RunProtectionsLogic();

	/**
	* Function performs simulator communication and variable refreshing
	*
	* @param pData Pointer to a SIMCONNECT_RECV structure, containing the data
	* @param cbData The size of the structure that inherits the SIMCONNECT_RECV structure, in bytes
	* @param pContext Contains the pointer specified by the client in the function call
	*/
	void SimConnectDispatchHandler(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);

	/**
	* Callback function to SimConnectDispatch to maintain OOP character
	*
	* @param pData Pointer to a SIMCONNECT_RECV structure, containing the data
	* @param cbData The size of the structure that inherits the SIMCONNECT_RECV structure, in bytes
	* @param pContext Contains the pointer specified by the client in the function call
	*/
	static void CALLBACK SimConnectDispatchHandlerInterface(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);

	/*
	* Functions runs the AFCS controller logic
	*
	* @overrides run method in QThread
	*/
	void run() override;
};

