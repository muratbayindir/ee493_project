#pragma once
#include "main.h"
#include <vector>

using namespace std;

class Point
{
public:
	double x, y, z;
	Point() {}
	Point(double x, double y, double z) : x(x), y(y), z(z) {}
	string GetObject() { return "[" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "]"; }
};

class Target
{
private:
	string name;
	Point location;
	double rssi;
	uint64_t timestamp;

public:
	Target(string name, Point location, uint64_t timestamp = 0) : name(name), location(location) {}

	string Name() const { return name; }
	Point Location() const { return location; }
	uint64_t Timestamp() const { return timestamp; }
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
	Localizer(string name) : name(name) {}
	void AddTarget(Target target);
	void AddTarget(string name, Point location);
	void UpdateTargetRssi(string name, double rssiValue);
	void RemoveTarget(string name);
	void UpdateLocation();
	Point Location() const { return location; }
};
