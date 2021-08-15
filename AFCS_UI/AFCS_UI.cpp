#include "AFCS_UI.h"

AFCS_UI::AFCS_UI(QWidget* parent) : QWidget(parent)
{
	UI.setupUi(this);
	SetupControllerConnections();
	SetupUIConnections();
}

inline void AFCS_UI::SetLabelTextAndStyleSheet(QLabel* ELEMENT, const QString& TEXT, const QString& STYLE_SHEET)
{
	ELEMENT->setText(TEXT);
	ELEMENT->setStyleSheet(STYLE_SHEET);
}

void AFCS_UI::InitializeUI()
{
	SetLabelTextAndStyleSheet(UI.CONNECTION_STATUS_LABEL, "Connected to Prepar3D!", "QLabel {color: green}");
	UI.TEST_CONNECTION_PUSH_BUTTON->setStyleSheet("");
	UI.TEST_CONNECTION_PUSH_BUTTON->setEnabled(false);

	SetLabelTextAndStyleSheet(UI.AT_MODE_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.PITCH_MODE_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.ROLL_MODE_LABEL, "", "");

	SetLabelTextAndStyleSheet(UI.SPD_VAL_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.SPD_SEL_VAL_LABEL, "N/A", "color: orange");
	SetLabelTextAndStyleSheet(UI.ALFA_VAL_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.ALFA_LIM_VAL_LABEL, QString::number(RadiansToDegrees(AFCS_CTL->AFCS_ALFA_PROT->ALFA_MAXIMUM_ANGLE)), "QLabel {color: red}");

	SetLabelTextAndStyleSheet(UI.PITCH_VAL_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.PITCH_LIM_VAL_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.G_VAL_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.G_LIM_VAL_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.ALT_VAL_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.ALT_SEL_VAL_LABEL, "N/A", "color: orange");
	SetLabelTextAndStyleSheet(UI.VS_VAL_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.VS_SEL_VAL_LABEL, "N/A", "color: orange");

	SetLabelTextAndStyleSheet(UI.BANK_VAL_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.BANK_LIM_VAL_LABEL, QString::number(RadiansToDegrees(AFCS_CTL->AFCS_ROLL_PROT->ROLL_MAXIMUM_ANGLE)), "QLabel {color: red}");
	SetLabelTextAndStyleSheet(UI.HDG_VAL_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.HDG_SEL_VAL_LABEL, "N/A", "color: orange");

	SetLabelTextAndStyleSheet(UI.AFCS_ACTIVE_MODE_LABEL, "NORMAL MODE", "QLabel {color: green}");
	SetLabelTextAndStyleSheet(UI.AFCS_CMD_PITCH_STATUS_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.AFCS_CMD_ROLL_STATUS_LABEL, "", "");
	SetLabelTextAndStyleSheet(UI.AFCS_AT_STATUS_LABEL, "", "");

	UI.AT_INHIBIT_PUSH_BUTTON->setEnabled(true);
	UI.MODE_OVRD_PUSH_BUTTON->setEnabled(true);
}

void AFCS_UI::SetupControllerConnections()
{
	connect(AFCS_CTL, &AFCS_CONTROLLER::InitializeUI, this, &AFCS_UI::InitializeUI);
	connect(AFCS_CTL, &AFCS_CONTROLLER::UpdateFlightParameters, this, &AFCS_UI::UpdateFlightParameters);
	connect(AFCS_CTL, &AFCS_CONTROLLER::AutoflightDisengaged, this, &AFCS_UI::AutoflightDisengaged);
	connect(AFCS_CTL->AFCS_AT, &AFCS_AUTOTHROTTLE::UpdateStatus, this, &AFCS_UI::UpdateStatus);
	connect(AFCS_CTL->AFCS_CMD_PITCH, &AFCS_COMMAND_PITCH::UpdateStatus, this, &AFCS_UI::UpdateStatus);
	connect(AFCS_CTL->AFCS_CMD_ROLL, &AFCS_COMMAND_ROLL::UpdateStatus, this, &AFCS_UI::UpdateStatus);
	connect(AFCS_CTL->AFCS_ALFA_PROT, &AFCS_ALFA_PROTECTION::UpdateStatus, this, &AFCS_UI::UpdateStatus);
	connect(AFCS_CTL->AFCS_MANEUVER_PROT, &AFCS_MANEUVER_PROTECTION::UpdateStatus, this, &AFCS_UI::UpdateStatus);
	connect(AFCS_CTL->AFCS_OVERSPEED_PROT, &AFCS_OVERSPEED_PROTECTION::UpdateStatus, this, &AFCS_UI::UpdateStatus);
	connect(AFCS_CTL->AFCS_PITCH_PROT, &AFCS_PITCH_PROTECTION::UpdateStatus, this, &AFCS_UI::UpdateStatus);
	connect(AFCS_CTL->AFCS_ROLL_PROT, &AFCS_ROLL_PROTECTION::UpdateStatus, this, &AFCS_UI::UpdateStatus);
}

void AFCS_UI::SetupUIConnections()
{
	QObject::connect(UI.TEST_CONNECTION_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		AFCS_CTL->start();
	});

	QObject::connect(UI.SPEED_DIAL, &QDial::valueChanged, [&]() {
		SetLabelTextAndStyleSheet(UI.SPEED_WINDOW_LABEL, QString::number(UI.SPEED_DIAL->value()), "");
	});
	QObject::connect(UI.SPD_SEL_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		AFCS_CTL->AFCS_AT->SPEED_SELECT = UI.SPEED_DIAL->value();
		SetLabelTextAndStyleSheet(UI.SPD_SEL_VAL_LABEL, QString::number(UI.SPEED_DIAL->value()), "");

		/// If autothrottle is disengaged, make it available for engagement
		if (AFCS_CTL->AFCS_AT->ENGAGEMENT_STATUS == false) UI.AT_SPEED_PUSH_BUTTON->setEnabled(true);
	});
	QObject::connect(UI.AT_SPEED_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		UI.AT_SPEED_PUSH_BUTTON->setEnabled(false);
		UI.AT_DISENG_PUSH_BUTTON->setEnabled(true);
		AFCS_CTL->AFCS_ENGAGEMENT_STATUS.AUTOTHROTTLE_SPEED = true;
	});

	QObject::connect(UI.HEADING_DIAL, &QDial::valueChanged, [&]() {
		SetLabelTextAndStyleSheet(UI.HEADING_WINDOW_LABEL, QString::number(UI.HEADING_DIAL->value()), "");
	});
	QObject::connect(UI.HDG_SEL_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		AFCS_CTL->AFCS_CMD_ROLL->HEADING_SELECT = UI.HEADING_DIAL->value();
		SetLabelTextAndStyleSheet(UI.HDG_SEL_VAL_LABEL, QString::number(UI.HEADING_DIAL->value()), "");

		/// If command roll is disengaged, make it available for engagement
		if (AFCS_CTL->AFCS_ENGAGEMENT_STATUS.COMMAND_ROLL == false) UI.CMD_ROLL_PUSH_BUTTON->setEnabled(true);
	});
	QObject::connect(UI.CMD_ROLL_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		UI.CMD_ROLL_PUSH_BUTTON->setEnabled(false);
		UI.CMD_DISENG_PUSH_BUTTON->setEnabled(true);
		AFCS_CTL->AFCS_ENGAGEMENT_STATUS.COMMAND_ROLL = true;
	});

	QObject::connect(UI.ALTITUDE_DIAL, &QDial::valueChanged, [&]() {
		SetLabelTextAndStyleSheet(UI.ALTITUDE_WINDOW_LABEL, QString::number(UI.ALTITUDE_DIAL->value()), "");
	});
	QObject::connect(UI.ALT_SEL_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		AFCS_CTL->AFCS_CMD_PITCH->ALTITUDE_SELECT = UI.ALTITUDE_DIAL->value();
		UI.VERTICAL_SPEED_SEL_PUSH_BUTTON->setEnabled(true);
		SetLabelTextAndStyleSheet(UI.ALT_SEL_VAL_LABEL, QString::number(UI.ALTITUDE_DIAL->value()), "");
	});
	QObject::connect(UI.CMD_PITCH_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		UI.CMD_PITCH_PUSH_BUTTON->setEnabled(false);
		UI.CMD_DISENG_PUSH_BUTTON->setEnabled(true);
		AFCS_CTL->AFCS_ENGAGEMENT_STATUS.COMMAND_PITCH = true;
	});

	QObject::connect(UI.VERTICAL_SPEED_DIAL, &QDial::valueChanged, [&]() {
		SetLabelTextAndStyleSheet(UI.VERTICAL_SPEED_WINDOW_LABEL, QString::number(UI.VERTICAL_SPEED_DIAL->value()), "");
	});
	QObject::connect(UI.VERTICAL_SPEED_SEL_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		AFCS_CTL->AFCS_CMD_PITCH->SetVerticalSpeed(UI.VERTICAL_SPEED_DIAL->value());
		SetLabelTextAndStyleSheet(UI.VS_SEL_VAL_LABEL, QString::number(UI.VERTICAL_SPEED_DIAL->value()), "");

		/// If command pitch is disengaged, make it available for engagement
		if (AFCS_CTL->AFCS_ENGAGEMENT_STATUS.COMMAND_PITCH == false) UI.CMD_PITCH_PUSH_BUTTON->setEnabled(true);
	});

	/// PUSH TO LEVEL OFF button will memorize current altitude and impose a 500 feet per minute vertical speed to attain it
	QObject::connect(UI.PUSH_TO_LEVEL_OFF_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		AFCS_CTL->AFCS_CMD_PITCH->ALTITUDE_SELECT = AFCS_CTL->FLIGHT_PARAMETERS.INDICATED_ALTITUDE;
		UI.ALTITUDE_DIAL->setValue(AFCS_CTL->AFCS_CMD_PITCH->ALTITUDE_SELECT);
		SetLabelTextAndStyleSheet(UI.ALTITUDE_WINDOW_LABEL, QString::number(AFCS_CTL->AFCS_CMD_PITCH->ALTITUDE_SELECT), "");
		SetLabelTextAndStyleSheet(UI.ALT_SEL_VAL_LABEL, QString::number(AFCS_CTL->AFCS_CMD_PITCH->ALTITUDE_SELECT), "");
		AFCS_CTL->AFCS_CMD_PITCH->SetVerticalSpeed(500);
		UI.VERTICAL_SPEED_DIAL->setValue(500);
		SetLabelTextAndStyleSheet(UI.VERTICAL_SPEED_WINDOW_LABEL, "500", "");
		SetLabelTextAndStyleSheet(UI.VS_SEL_VAL_LABEL, "500", "");
	});

	QObject::connect(UI.CMD_DISENG_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		UI.CMD_PITCH_PUSH_BUTTON->setEnabled(true);
		UI.CMD_ROLL_PUSH_BUTTON->setEnabled(true);
		UI.CMD_DISENG_PUSH_BUTTON->setEnabled(false);
		AFCS_CTL->AFCS_ENGAGEMENT_STATUS.COMMAND_ROLL = false;
		AFCS_CTL->AFCS_ENGAGEMENT_STATUS.COMMAND_PITCH = false;
	});
	QObject::connect(UI.AT_DISENG_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		UI.AT_SPEED_PUSH_BUTTON->setEnabled(true);
		UI.AT_DISENG_PUSH_BUTTON->setEnabled(false);
		AFCS_CTL->AFCS_ENGAGEMENT_STATUS.AUTOTHROTTLE_SPEED = false;
	});
	QObject::connect(UI.AT_INHIBIT_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		UI.AT_SPEED_PUSH_BUTTON->setEnabled(false);
		UI.AT_INHIBIT_PUSH_BUTTON->setEnabled(false);
		SetLabelTextAndStyleSheet(UI.AFCS_AT_STATUS_LABEL, "AT INHIBIT", "QLabel {color: red}");
		AFCS_CTL->AFCS_ENGAGEMENT_STATUS.AUTOTHROTTLE_SPEED = false;
		AFCS_CTL->AFCS_ENGAGEMENT_STATUS.AUTOTHROTTLE_INHIBIT = true;
	});
	QObject::connect(UI.MODE_OVRD_PUSH_BUTTON, &QPushButton::clicked, [&]() {
		UI.AT_SPEED_PUSH_BUTTON->setEnabled(false);
		UI.AT_DISENG_PUSH_BUTTON->setEnabled(false);
		UI.AT_INHIBIT_PUSH_BUTTON->setEnabled(false);
		UI.CMD_PITCH_PUSH_BUTTON->setEnabled(false);
		UI.CMD_ROLL_PUSH_BUTTON->setEnabled(false);
		UI.CMD_DISENG_PUSH_BUTTON->setEnabled(false);
		UI.MODE_OVRD_PUSH_BUTTON->setEnabled(false);
		SetLabelTextAndStyleSheet(UI.AFCS_ACTIVE_MODE_LABEL, "DIRECT MODE", "QLabel {color: red}");
		SetLabelTextAndStyleSheet(UI.AFCS_AT_STATUS_LABEL, "AT INHIBIT", "QLabel {color: red}");
		AFCS_CTL->AFCS_ENGAGEMENT_STATUS.AUTOTHROTTLE_INHIBIT = true;
		AFCS_CTL->AFCS_ENGAGEMENT_STATUS.DIRECT_MODE = true;
	});
}

void AFCS_UI::UpdateFlightParameters()
{
	SetLabelTextAndStyleSheet(UI.SPD_VAL_LABEL, QString::number(floor(AFCS_CTL->FLIGHT_PARAMETERS.AIRSPEED_INDICATED)), "");
	if (AFCS_CTL->FLIGHT_PARAMETERS.INCIDENCE_ALPHA > AFCS_CTL->AFCS_ALFA_PROT->ALFA_MAXIMUM_ANGLE)
		SetLabelTextAndStyleSheet(
			UI.ALFA_VAL_LABEL, 
			QString::number(floor(RadiansToDegrees(AFCS_CTL->FLIGHT_PARAMETERS.INCIDENCE_ALPHA) * 10) / 10), 
			"QLabel {background-color: red; color: white}"
		);
	else
		if (AFCS_CTL->FLIGHT_PARAMETERS.INCIDENCE_ALPHA >= AFCS_CTL->AFCS_ALFA_PROT->ALFA_REDUCED_AUTHORITY_THRESHOLD)
			SetLabelTextAndStyleSheet(
				UI.ALFA_VAL_LABEL,
				QString::number(floor(RadiansToDegrees(AFCS_CTL->FLIGHT_PARAMETERS.INCIDENCE_ALPHA) * 10) / 10),
				"QLabel {color: orange}"
			);
		else
			SetLabelTextAndStyleSheet(
				UI.ALFA_VAL_LABEL,
				QString::number(floor(RadiansToDegrees(AFCS_CTL->FLIGHT_PARAMETERS.INCIDENCE_ALPHA) * 10) / 10),
				""
			);

	if (AFCS_CTL->FLIGHT_PARAMETERS.PLANE_PITCH_DEGREES < AFCS_CTL->AFCS_PITCH_PROT->PITCH_MAXIMUM_POSITIVE_ANGLE || 
		AFCS_CTL->FLIGHT_PARAMETERS.PLANE_PITCH_DEGREES > AFCS_CTL->AFCS_PITCH_PROT->PITCH_MAXIMUM_NEGATIVE_ANGLE)
		SetLabelTextAndStyleSheet(
			UI.PITCH_VAL_LABEL,
			QString::number(floor(RadiansToDegrees(-1 * AFCS_CTL->FLIGHT_PARAMETERS.PLANE_PITCH_DEGREES))),
			"QLabel {background-color: red; color: white}"
		);
	else
		if (AFCS_CTL->FLIGHT_PARAMETERS.PLANE_PITCH_DEGREES < AFCS_CTL->AFCS_PITCH_PROT->PITCH_REDUCED_AUTHORITY_POSITIVE_THRESHOLD || 
			AFCS_CTL->FLIGHT_PARAMETERS.PLANE_PITCH_DEGREES > AFCS_CTL->AFCS_PITCH_PROT->PITCH_REDUCED_AUTHORITY_NEGATIVE_THRESHOLD)
			SetLabelTextAndStyleSheet(
				UI.PITCH_VAL_LABEL,
				QString::number(floor(RadiansToDegrees(-1 * AFCS_CTL->FLIGHT_PARAMETERS.PLANE_PITCH_DEGREES))),
				"QLabel {color: orange}"
			);
		else
			SetLabelTextAndStyleSheet(
				UI.PITCH_VAL_LABEL,
				QString::number(floor(RadiansToDegrees(-1 * AFCS_CTL->FLIGHT_PARAMETERS.PLANE_PITCH_DEGREES))),
				""
			);
	if (AFCS_CTL->FLIGHT_PARAMETERS.PLANE_PITCH_DEGREES < 0)
		SetLabelTextAndStyleSheet(
			UI.PITCH_LIM_VAL_LABEL,
			QString::number(RadiansToDegrees(-AFCS_CTL->AFCS_PITCH_PROT->PITCH_MAXIMUM_POSITIVE_ANGLE)),
			"QLabel {color: red}"
		);
	else
		SetLabelTextAndStyleSheet(
			UI.PITCH_LIM_VAL_LABEL,
			QString::number(RadiansToDegrees(-AFCS_CTL->AFCS_PITCH_PROT->PITCH_MAXIMUM_NEGATIVE_ANGLE)),
			"QLabel {color: red}"
		);
	if (AFCS_CTL->FLIGHT_PARAMETERS.G_FORCE > AFCS_CTL->AFCS_MANEUVER_PROT->MANEUVER_MAXIMUM_VALUE || 
		AFCS_CTL->FLIGHT_PARAMETERS.G_FORCE < AFCS_CTL->AFCS_MANEUVER_PROT->MANEUVER_MINIMUM_VALUE)
		SetLabelTextAndStyleSheet(
			UI.G_VAL_LABEL,
			QString::number(floor(AFCS_CTL->FLIGHT_PARAMETERS.G_FORCE * 10) / 10),
			"QLabel {background-color: red; color: white}"
		);
	else
		if (AFCS_CTL->FLIGHT_PARAMETERS.G_FORCE >= AFCS_CTL->AFCS_MANEUVER_PROT->MANEUVER_REDUCED_AUTHORITY_MAXIMUM_THRESHOLD || 
			AFCS_CTL->FLIGHT_PARAMETERS.G_FORCE <= AFCS_CTL->AFCS_MANEUVER_PROT->MANEUVER_REDUCED_AUTHORITY_MINIMUM_THRESHOLD)
			SetLabelTextAndStyleSheet(
				UI.G_VAL_LABEL,
				QString::number(floor(AFCS_CTL->FLIGHT_PARAMETERS.G_FORCE * 10) / 10),
				"QLabel {color: orange}"
			);
		else
			SetLabelTextAndStyleSheet(
				UI.G_VAL_LABEL,
				QString::number(floor(AFCS_CTL->FLIGHT_PARAMETERS.G_FORCE * 10) / 10),
				""
			);
	if (AFCS_CTL->FLIGHT_PARAMETERS.G_FORCE >= 0)
		SetLabelTextAndStyleSheet(
			UI.G_LIM_VAL_LABEL,
			QString::number(AFCS_CTL->AFCS_MANEUVER_PROT->MANEUVER_MAXIMUM_VALUE),
			"QLabel {color: red}"
		);
	else
		SetLabelTextAndStyleSheet(
			UI.G_LIM_VAL_LABEL,
			QString::number(AFCS_CTL->AFCS_MANEUVER_PROT->MANEUVER_MINIMUM_VALUE),
			"QLabel {color: red}"
		);
	SetLabelTextAndStyleSheet(UI.ALT_VAL_LABEL, QString::number(floor(AFCS_CTL->FLIGHT_PARAMETERS.INDICATED_ALTITUDE)), "");
	SetLabelTextAndStyleSheet(UI.VS_VAL_LABEL, QString::number(floor(FeetPerSecondToFeetPerMinute(AFCS_CTL->FLIGHT_PARAMETERS.VERTICAL_SPEED))), "");

	if (abs(AFCS_CTL->FLIGHT_PARAMETERS.PLANE_BANK_DEGREES) > AFCS_CTL->AFCS_ROLL_PROT->ROLL_MAXIMUM_ANGLE)
		SetLabelTextAndStyleSheet(
			UI.BANK_VAL_LABEL,
			QString::number(floor(abs(RadiansToDegrees(AFCS_CTL->FLIGHT_PARAMETERS.PLANE_BANK_DEGREES)))),
			"QLabel {background-color: red; color: white}"
		);
	else
		if (abs(AFCS_CTL->FLIGHT_PARAMETERS.PLANE_BANK_DEGREES) > AFCS_CTL->AFCS_ROLL_PROT->ROLL_REDUCED_AUTHORITY_THRESHOLD)
			SetLabelTextAndStyleSheet(
				UI.BANK_VAL_LABEL,
				QString::number(floor(abs(RadiansToDegrees(AFCS_CTL->FLIGHT_PARAMETERS.PLANE_BANK_DEGREES)))),
				"QLabel {color: orange}"
			);
		else
			SetLabelTextAndStyleSheet(
				UI.BANK_VAL_LABEL,
				QString::number(floor(abs(RadiansToDegrees(AFCS_CTL->FLIGHT_PARAMETERS.PLANE_BANK_DEGREES)))),
				""
			);
	SetLabelTextAndStyleSheet(UI.HDG_VAL_LABEL, QString::number(floor(RadiansToDegrees(AFCS_CTL->FLIGHT_PARAMETERS.PLANE_HEADING_DEGREES_TRUE))), "");
}

void AFCS_UI::UpdateStatus()
{
	if (AFCS_CTL->AFCS_ALFA_PROT->ENGAGEMENT_STATUS == true)
	{
		SetLabelTextAndStyleSheet(UI.AT_MODE_LABEL, "ALFA PROTECTION", "QLabel {background-color: red; color: white}");
		if (AFCS_CTL->AFCS_ENGAGEMENT_STATUS.AUTOTHROTTLE_INHIBIT != true)
		{
			SetLabelTextAndStyleSheet(UI.AFCS_AT_STATUS_LABEL, "", "");
		}
	}
	else
		if (AFCS_CTL->AFCS_OVERSPEED_PROT->ENGAGEMENT_STATUS == true)
		{
			SetLabelTextAndStyleSheet(UI.AT_MODE_LABEL, "OVERSPEED PROTECTION", "QLabel {background-color: red; color: white}");
			if (AFCS_CTL->AFCS_ENGAGEMENT_STATUS.AUTOTHROTTLE_INHIBIT != true)
			{
				SetLabelTextAndStyleSheet(UI.AFCS_AT_STATUS_LABEL, "", "");
			}
		}
		else
			if (AFCS_CTL->AFCS_AT->ENGAGEMENT_STATUS == true)
			{
				SetLabelTextAndStyleSheet(UI.AT_MODE_LABEL, "SPEED", "QLabel {border: 3px solid green}");
				SetLabelTextAndStyleSheet(UI.AFCS_AT_STATUS_LABEL, "AT SPEED", "QLabel {color: green}");
			}
			else
			{
				SetLabelTextAndStyleSheet(UI.AT_MODE_LABEL, "", "");
				if (AFCS_CTL->AFCS_ENGAGEMENT_STATUS.AUTOTHROTTLE_INHIBIT != true)
				{
					SetLabelTextAndStyleSheet(UI.AFCS_AT_STATUS_LABEL, "", "");
				}
			}
	
	if (AFCS_CTL->AFCS_PITCH_PROT->ENGAGEMENT_STATUS == true)
	{
		SetLabelTextAndStyleSheet(UI.PITCH_MODE_LABEL, "PITCH PROTECTION", "QLabel {border: 3px solid orange}");
		SetLabelTextAndStyleSheet(UI.AFCS_CMD_PITCH_STATUS_LABEL, "", "");
	}
	else
		if (AFCS_CTL->AFCS_MANEUVER_PROT->ENGAGEMENT_STATUS == true)
		{
			SetLabelTextAndStyleSheet(UI.PITCH_MODE_LABEL, "MANEUVER PROTECTION", "QLabel {background-color: red; color: white}");
			SetLabelTextAndStyleSheet(UI.AFCS_CMD_PITCH_STATUS_LABEL, "", "");
		}
		else
			if (AFCS_CTL->AFCS_ENGAGEMENT_STATUS.COMMAND_PITCH == true)
			{
				SetLabelTextAndStyleSheet(UI.PITCH_MODE_LABEL, "ALTITUDE", "QLabel {border: 3px solid green}");
				SetLabelTextAndStyleSheet(UI.AFCS_CMD_PITCH_STATUS_LABEL, "CMD PITCH", "QLabel {color: green}");
			}
			else
			{
				SetLabelTextAndStyleSheet(UI.PITCH_MODE_LABEL, "", "");
				SetLabelTextAndStyleSheet(UI.AFCS_CMD_PITCH_STATUS_LABEL, "", "");
			}

	if (AFCS_CTL->AFCS_ROLL_PROT->ENGAGEMENT_STATUS == true)
	{
		SetLabelTextAndStyleSheet(UI.ROLL_MODE_LABEL, "ROLL PROTECTION", "QLabel {border: 3px solid orange}");
		SetLabelTextAndStyleSheet(UI.AFCS_CMD_ROLL_STATUS_LABEL, "", "");
	}
	else
		if (AFCS_CTL->AFCS_ENGAGEMENT_STATUS.COMMAND_ROLL == true)
		{
			SetLabelTextAndStyleSheet(UI.ROLL_MODE_LABEL, "HEADING", "QLabel {border: 3px solid green}");
			SetLabelTextAndStyleSheet(UI.AFCS_CMD_ROLL_STATUS_LABEL, "CMD ROLL", "QLabel {color: green}");
		}
		else
		{
			SetLabelTextAndStyleSheet(UI.ROLL_MODE_LABEL, "", "");
			SetLabelTextAndStyleSheet(UI.AFCS_CMD_ROLL_STATUS_LABEL, "", "");
		}
}

void AFCS_UI::AutoflightDisengaged()
{
	if (AFCS_CTL->AFCS_ENGAGEMENT_STATUS.DIRECT_MODE != true)
	{
		if (AFCS_CTL->AFCS_ENGAGEMENT_STATUS.AUTOTHROTTLE_INHIBIT != true)
		{
			UI.AT_SPEED_PUSH_BUTTON->setEnabled(true);
			UI.AT_DISENG_PUSH_BUTTON->setEnabled(false);
		}
		UI.CMD_PITCH_PUSH_BUTTON->setEnabled(true);
		UI.CMD_ROLL_PUSH_BUTTON->setEnabled(true);
		UI.CMD_DISENG_PUSH_BUTTON->setEnabled(false);
	}
}
