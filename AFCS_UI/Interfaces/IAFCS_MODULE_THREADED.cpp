#include "IAFCS_MODULE_THREADED.h"

IAFCS_MODULE_THREADED::IAFCS_MODULE_THREADED(IAFCS_CONTROLLER* AFCS_CTL)
{
	this->AFCS_CTL = AFCS_CTL;

	/// The AFCS module should start in disengaged state
	Disengage();
	start();
}

void IAFCS_MODULE_THREADED::Engage()
{
	SYNC.lock();
	ENGAGEMENT_STATUS = true;
	INPUT_RECEIVED = true;
	emit UpdateStatus();
	SYNC.unlock();
	WAIT_CONDITION.wakeAll();
}

void IAFCS_MODULE_THREADED::Disengage()
{
	SYNC.lock();
	ENGAGEMENT_STATUS = false;
	emit UpdateStatus();
	SYNC.unlock();
}

void IAFCS_MODULE_THREADED::InputReceived()
{
	SYNC.lock();
	INPUT_RECEIVED = true;
	SYNC.unlock();
	NEW_DATA_CONDITION.wakeAll();
}

void IAFCS_MODULE_THREADED::InputProcessed()
{
	SYNC.lock();
	INPUT_RECEIVED = false;
	SYNC.unlock();
}

bool IAFCS_MODULE_THREADED::NotifyNewData(bool OVERRIDE_CONDITION, bool ENGAGEMENT_CONDITION)
{
	if (OVERRIDE_CONDITION == false && ENGAGEMENT_CONDITION == true && ENGAGEMENT_STATUS == false) Engage();
	if (OVERRIDE_CONDITION == false && ENGAGEMENT_CONDITION == true && ENGAGEMENT_STATUS == true) InputReceived();
	if ((OVERRIDE_CONDITION == true || ENGAGEMENT_CONDITION == false) && ENGAGEMENT_STATUS == true) Disengage();

	return ENGAGEMENT_STATUS;
}

void IAFCS_MODULE_THREADED::run()
{
	while (true)
	{
		SYNC.lock();
		while (ENGAGEMENT_STATUS == false) WAIT_CONDITION.wait(&SYNC);
		while (INPUT_RECEIVED == false) NEW_DATA_CONDITION.wait(&SYNC);
		SYNC.unlock();
		RunModuleLogic();
		InputProcessed();
	}
}
