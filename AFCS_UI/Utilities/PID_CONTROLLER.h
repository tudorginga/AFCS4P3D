#pragma once

class PID_CONTROLLER
{
public:
	/**
	* PID controller constructor
	*
	* @param PROPORTIONAL_COEFFICIENT
	* @param INTEGRAL_COEFFICIENT
	* @param DERIVATE_COEFFICIENT
	*/
	PID_CONTROLLER(double PROPORTIONAL_COEFFICIENT, double INTEGRAL_COEFFICIENT, double DERIVATE_COEFFICIENT);

	/**
	* Update the PID controller parameters with a new input
	*
	* @param INPUT
	* @returns new correction accounting for the input
	*/
	virtual double Update(double INPUT);

protected:
	double PROPORTIONAL_COEFFICIENT;
	double INTEGRAL_COEFFICIENT;
	double DERIVATE_COEFFICIENT;
	double OLD_ERROR;
	double INTEGRAL_ERROR;
};

