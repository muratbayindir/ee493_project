#include "main.h"
#include "Localizer.h"

using namespace std;

Target::Target(string name, Point location) : name(name), location(location)
{
}

void Localizer::AddTarget(Target target)
{
	targets.push_back(target);
}

void Localizer::AddTarget(string name, Point location)
{
	Target target(name, location);
	targets.push_back(target);
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
}

Point Localizer::GetLocation()
{
	return location;
}
