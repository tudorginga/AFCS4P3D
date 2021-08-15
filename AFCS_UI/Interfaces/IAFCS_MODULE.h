#pragma once

#include "IAFCS_CONTROLLER.h"
#include "../Utilities/PID_CONTROLLER.h"
#include "../Utilities/UTILS.h"

class IAFCS_MODULE : public QObject
{
	Q_OBJECT

public:
	bool ENGAGEMENT_STATUS = false;

	/**
	* AFCS module interface constructor
	*
	* @param AFCS_CTL AFCS controller
	*/
	IAFCS_MODULE(IAFCS_CONTROLLER* AFCS_CTL);

	/**
	* Function to notify the module that new data has arrived from the simulator
	*
	* @param OVERRIDE_CONDITION overrides the execution of the module if it is true
	* @param ENGAGEMENT_CONDITION dictates whether the module executes or not
	* @returns true if module is engaged, false otherwise
	*/
	bool NotifyNewData(bool OVERRIDE_CONDITION, bool ENGAGEMENT_CONDITION);

protected:
	IAFCS_CONTROLLER* AFCS_CTL;
	PID_CONTROLLER* SURFACE_CONTROLLER;

	/// Virtual function for the implementation of the module
	virtual void RunModuleLogic() = 0;

signals:
	/// Function updates UI after each AFCS engagement status change
	void UpdateStatus();
};

