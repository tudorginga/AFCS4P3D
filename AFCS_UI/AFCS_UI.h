#pragma once

#include <QString>
#include <QThread>
#include <QWidget>

#include "ui_AFCS_UI.h"
#include "AFCS_CONTROLLER.h"

class AFCS_UI : public QWidget
{
public:
	/// AFCS_UI constructor
	AFCS_UI(QWidget *parent = Q_NULLPTR);

public slots:
	/// Function updates UI according to autoflight disengaged AFCS state
	void AutoflightDisengaged();

	/// Function initializes UI after AFCS start
	void InitializeUI();

	/// Function updates UI with new flight parameters at each new simulation frame update
	void UpdateFlightParameters();

	/// Function updates UI after each AFCS engagement status change
	void UpdateStatus();

private:
	Ui::AFCS_UIClass UI;
	AFCS_CONTROLLER* AFCS_CTL = new AFCS_CONTROLLER();

	/**
	* Function initializes QLabel with given parameters
	*
	* @param ELEMENT QLabel to be initialized
	* @param TEXT text to be set
	* @param STYLE_SHEET styleSheet to be set
	*/
	inline void SetLabelTextAndStyleSheet(QLabel* ELEMENT, const QString& TEXT, const QString& STYLE_SHEET);

	/// Function sets up callbacks from the AFCS controller to the UI
	void SetupControllerConnections();

	/// Function sets up the UI interactions
	void SetupUIConnections();
};

