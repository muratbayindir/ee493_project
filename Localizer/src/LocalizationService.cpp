#include "main.h"
#include "LocalizationService.h"
#include "Localizer.h"

using namespace rapidjson;

LocalizationService::LocalizationService()
{
	serviceThreadExit_ = false;
	serviceThread_ = thread(bind(&LocalizationService::serviceThreadLoop, this));
}

LocalizationService::~LocalizationService()
{
	serviceThreadExit_ = true;

	if (serviceThread_.joinable())
		serviceThread_.join();
}

void LocalizationService::serviceThreadLoop()
{
	DWORD dwRead, dwWrite;
	char buffer[1024];
	HANDLE hPipe;

	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\StronicsLocalizer"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);

	while (!serviceThreadExit_ && hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			log << "Connected to a pipe\n";

			uint32_t dataLen = 0;

			while (ReadFile(hPipe, &dataLen, 4, &dwRead, NULL))
			{
				if (dwRead == 4 && ReadFile(hPipe, buffer, dataLen, &dwRead, NULL))
				{
					buffer[dwRead] = 0;
					string message((char *)buffer);
					log << "Received Message : " << message << "\n";

					// {"name":"test", "targets":[{"name":"1","location":[1,2,3],"rssi":15},{}]}

					Document doc;
					doc.Parse(message.c_str());

					Localizer localizer(doc["name"].GetString());

					Value& targets = doc["targets"];
					for (int i = 0; i < targets.GetArray().Size(); ++i)
					{
						Value& targetJson = targets[i];
						Value& locationJson = targetJson["location"];
						Target target(targetJson["name"].GetString(), Point(locationJson[0].GetDouble()
							, locationJson[0].GetDouble(), locationJson[0].GetDouble()));
						localizer.AddTarget(target);
					}

					localizer.UpdateLocation();
					string response = "{\"location\":" + localizer.Location().GetObject() + "}";
					uint32_t responseLength = response.length();

					WriteFile(hPipe, &responseLength, 4, &dwWrite, NULL);
					WriteFile(hPipe, response.c_str(), responseLength, &dwWrite, NULL);
				}
			}
		}

		log << "Disconnected from pipe\n";
		DisconnectNamedPipe(hPipe);
	}
}
