#include "main.h"
#include "Localizer.h"
#include <math.h>

using namespace std;

void Localizer::AddTarget(Target target)
{
	targets.push_back(target);
}

void Localizer::AddTarget(string name, Point location)
{
	Target target(name, location);
	this->AddTarget(target);
}

void Localizer::UpdateTargetRssi(string name, double rssiValue)
{
	for (auto it = begin(targets); it != end(targets); ++it) {
		if (it->Name() == name) {
			it->SetRssi(rssiValue);
			break;
		}
	}
}

void Localizer::RemoveTarget(string name)
{
	for (auto it = begin(targets); it != end(targets); ++it) {
		if (it->Name() == name) {
			targets.erase(it);
			break;
		}
	}
}

void Localizer::UpdateLocation()
{
	/*
x
^
.
.
ESP3
.
.
.
ESP1 . . . ESP2. . .>y
*/
	double z1;
	double z2;
	double z3;
	int x = 0;

	for (auto it = begin(targets); it != end(targets); ++it) {
		if (x == 0) {
			z1 = RSSI_to_Meter(it->Rssi());
			x++;
		}
		else if (x == 1) {
			z2 = RSSI_to_Meter(it->Rssi());
			x++;
		}
		else if (x == 2) {
			z3 = RSSI_to_Meter(it->Rssi());
			x++;
		}
		else {
			x = 0;
		}
	}

	z1 = z1 * z1;
	z2 = z2 * z2;
	z3 = z3 * z3;

	location.x = (z1 - z2 + 9) / 6; //for 1 meter square area 6->2, 9->1
	location.y = (z1 - z3 + 9) / 6; //for 1 meter square area 6->2, 9->1
	location.z = 0;
}

double Localizer::RSSI_to_Meter(double rssi)
{
	return pow(10, ((-69 - (rssi)) / (10 * 2)));

	double from_source;
	if (rssi < 10) {
		//linearization with the remaining values.
		from_source = 0;
		return from_source;
	}
	else if (rssi < 20) {
		from_source = 0;
		return from_source;
	}
	else if (rssi < 30) {
		from_source = 0;
		return from_source;
	}
	else if (rssi < 40) {
		from_source = 0;
		return from_source;
	}
	else if (rssi < 50) {
		from_source = 0;
		return from_source;
	}
	else if (rssi < 60) {
		from_source = 0;
		return from_source;
	}
	else if (rssi < 70) {
		from_source = 0;
		return from_source;
	}
	else if (rssi < 80) {
		from_source = 0;
		return from_source;
	}
	else {
		from_source = 0;
		return from_source;
	}

	return 0.0;
}
