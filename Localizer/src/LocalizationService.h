#pragma once
#include "main.h"
#include <thread>

using namespace std;

class LocalizationService
{
private:
	std::thread serviceThread_;
	bool serviceThreadExit_;

	void serviceThreadLoop();

public:
	LocalizationService();
	~LocalizationService();
};

