#pragma once

#include "PID_CONTROLLER.h"
#include "UTILS.h"

class PID_CONTROLLER_RESTRICTED_OUTPUT : public PID_CONTROLLER
{
public:
	/**
	* PID controller with a restricted output controller
	*
	* @param PROPORTIONAL_COEFFICIENT
	* @param INTEGRAL_COEFFICIENT
	* @param DERIVATE_COEFFICIENT
	* @param LOWER_BOUND
	* @param UPPER_BOUND
	*/
	PID_CONTROLLER_RESTRICTED_OUTPUT(double PROPORTIONAL_COEFFICIENT, double INTEGRAL_COEFFICIENT, double DERIVATE_COEFFICIENT, double LOWER_BOUND, double UPPER_BOUND);

	/**
	* Bound setter for the PID controller
	*
	* @param LOWER_BOUND new LOWER_BOUND
	* @param UPPER_BOUND new UPPER_BOUND
	*/
	void SetBound(double LOWER_BOUND, double UPPER_BOUND);

	/**
	* Update the PID controller parameters with a new input
	*
	* @param INPUT
	* @overrides Update method from base PID_CONTROLLER class
	* @returns new correction accounting for the input
	*/
	double Update(double INPUT) override;

private:
	double LOWER_BOUND;
	double UPPER_BOUND;
};

