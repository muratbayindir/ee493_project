#pragma once
#include "main.h"
#include <vector>

using namespace std;

class Point
{
public:
	double x, y, z;
};

class Target
{
private:
	string name;
	Point location;
	double rssi;

public:
	Target(string name, Point location);

	string Name() const { return name; }
	Point Location() const { return location; }
	double Rssi() const { return rssi; }

	void SetRssi(double value) { rssi = value; }
};

class Localizer
{
private:
	string name;
	Point location;
	vector<Target> targets;
public:
	void AddTarget(Target target);
	void AddTarget(string name, Point location);
	void UpdateTargetRssi(string name, double rssiValue);
	void RemoveTarget(string name);
	void UpdateLocation();
	Point GetLocation();
};
