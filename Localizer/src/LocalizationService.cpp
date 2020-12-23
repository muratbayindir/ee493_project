#include "main.h"
#include "LocalizationService.h"

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
	DWORD dwRead;
	char buffer[1024];
	HANDLE hPipe_;

	hPipe_ = CreateNamedPipe(TEXT("\\\\.\\pipe\\StronicsLocalizer"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);

	while (!serviceThreadExit_ && hPipe_ != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe_, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			log << "Connected to a pipe\n";

			uint32_t dataLen;

			while (ReadFile(hPipe_, &dataLen, 4, &dwRead, NULL))
			{
				if (dwRead == 4 && ReadFile(hPipe_, buffer, dataLen, &dwRead, NULL))
				{
					buffer[dwRead] = 0;
					string message(buffer);
					log << message << "\n";
				}
			}
		}

		log << "Disconnected from pipe\n";
		DisconnectNamedPipe(hPipe_);
	}
}
