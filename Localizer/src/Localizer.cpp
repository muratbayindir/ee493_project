#include "main.h"
#include "Localizer.h"

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

	for (auto it = begin(targets); it != end(targets); ++it) {
		z1 = RSSI_to_Meter(it->Rssi);
	}

	z1 = z1 * z1;
	z2 = z2 * z2;
	z3 = z3 * z3;

	location.x = (z1 - z2 + 9) / 6; //for 1 meter square area 6->2, 9->1
	location.y = (z1 - z3 + 9) / 6; //for 1 meter square area 6->2, 9->1
}

double Localizer::RSSI_to_Meter(double rssi)
{
	return 0.0;
}
