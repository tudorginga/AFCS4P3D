#include "PID_CONTROLLER_RESTRICTED_OUTPUT.h"

PID_CONTROLLER_RESTRICTED_OUTPUT::PID_CONTROLLER_RESTRICTED_OUTPUT(double PROPORTIONAL_COEFFICIENT, double INTEGRAL_COEFFICIENT, double DERIVATE_COEFFICIENT, double LOWER_BOUND, double UPPER_BOUND) : PID_CONTROLLER(PROPORTIONAL_COEFFICIENT, INTEGRAL_COEFFICIENT, DERIVATE_COEFFICIENT)
{
	this->LOWER_BOUND = LOWER_BOUND;
	this->UPPER_BOUND = UPPER_BOUND;
}

void PID_CONTROLLER_RESTRICTED_OUTPUT::SetBound(double LOWER_BOUND, double UPPER_BOUND)
{
	this->LOWER_BOUND = LOWER_BOUND;
	this->UPPER_BOUND = UPPER_BOUND;
}

double PID_CONTROLLER_RESTRICTED_OUTPUT::Update(double INPUT)
{
	double OUTPUT = PID_CONTROLLER::Update(INPUT);
	if (OUTPUT < LOWER_BOUND) OUTPUT = LOWER_BOUND;
	if (OUTPUT > UPPER_BOUND) OUTPUT = UPPER_BOUND;
	return OUTPUT;
}
