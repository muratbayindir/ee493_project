#include "main.h"
#include "LocalizationService.h"

int main(char argc, char **argv)
{
	LocalizationService lService;

	while (true)
	{
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}