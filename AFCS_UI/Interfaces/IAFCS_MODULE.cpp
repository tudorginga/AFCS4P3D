#include "IAFCS_MODULE.h"

IAFCS_MODULE::IAFCS_MODULE(IAFCS_CONTROLLER* AFCS_CTL)
{
	this->AFCS_CTL = AFCS_CTL;
}

bool IAFCS_MODULE::NotifyNewData(bool OVERRIDE_CONDITION, bool ENGAGEMENT_CONDITION)
{
	bool OLD_ENGAGEMENT_STATUS = ENGAGEMENT_STATUS;
	ENGAGEMENT_STATUS = OVERRIDE_CONDITION == false && ENGAGEMENT_CONDITION == true;
	if (OLD_ENGAGEMENT_STATUS != ENGAGEMENT_STATUS) emit UpdateStatus();
	if (ENGAGEMENT_STATUS) RunModuleLogic();
	return ENGAGEMENT_STATUS;
}
