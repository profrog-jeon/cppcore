#include "stdafx.h"
#include "Information.h"
#include "Log.h"
#include "Environment.h"

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	std::tstring GetWeekString(WORD wDayOfWeek)
	{
		switch(wDayOfWeek)
		{
		case 0:			return TEXT("Sunday");
		case 1:			return TEXT("Monday");
		case 2:			return TEXT("Tuesday");
		case 3:			return TEXT("Wednesday");
		case 4:			return TEXT("Thursday");
		case 5:			return TEXT("Friday");
		case 6:			return TEXT("Saturday");
		}

		return Format(TEXT("Unknown Week:%u"), wDayOfWeek);
	}

	//////////////////////////////////////////////////////////////////////////
	std::tstring GetMonthString(WORD wMonth)
	{
		switch(wMonth)
		{
		case 1:			return TEXT("January");
		case 2:			return TEXT("February");
		case 3:			return TEXT("March");
		case 4:			return TEXT("April");
		case 5:			return TEXT("May");
		case 6:			return TEXT("June");
		case 7:			return TEXT("July");
		case 8:			return TEXT("August");
		case 9:			return TEXT("September");
		case 10:		return TEXT("October");
		case 11:		return TEXT("November");
		case 12:		return TEXT("December");
		}

		return Format(TEXT("Undefined Month:%u"), wMonth);
	}
};