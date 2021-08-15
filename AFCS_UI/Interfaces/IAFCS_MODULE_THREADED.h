#pragma once

#include <QMutex>
#include <QWaitCondition>

#include "IAFCS_CONTROLLER.h"
#include "../Utilities/PID_CONTROLLER.h"
#include "../Utilities/UTILS.h"

class IAFCS_MODULE_THREADED : public QThread
{
	Q_OBJECT

public:
	bool ENGAGEMENT_STATUS;

	/**
	* AFCS threaded module interface constructor
	*
	* @param AFCS_CTL AFCS controller
	*/
	IAFCS_MODULE_THREADED(IAFCS_CONTROLLER* AFCS_CTL);

	/**
	* Function to notify the module that new data has arrived from the simulator
	*
	* @param OVERRIDE_CONDITION overrides the execution of the module if it is true
	* @param ENGAGEMENT_CONDITION dictates whether the module executes or not
	* @returns true if module is engaged, false otherwise
	*/
	bool NotifyNewData(bool OVERRIDE_CONDITION, bool ENGAGEMENT_CONDITION);

protected:
	QMutex SYNC;
	QWaitCondition WAIT_CONDITION;
	QWaitCondition NEW_DATA_CONDITION;
	bool INPUT_RECEIVED;

	IAFCS_CONTROLLER* AFCS_CTL;
	PID_CONTROLLER* SURFACE_CONTROLLER;

	/// Function to engage the AFCS module
	void Engage();

	/// Function to disengage the AFCS module
	void Disengage();

	/// Function to signal that new data has been fed into the controller
	void InputReceived();

	/// Function to signal that all data from simulator has been processed
	void InputProcessed();

	/*
	* Functions runs the AFCS module logic
	*
	* @overrides run method in QThread
	*/
	void run() override;

	/// Virtual function for the implementation of the module
	virtual void RunModuleLogic() = 0;

signals:
	/// Function updates UI after each AFCS engagement status change
	void UpdateStatus();
};

