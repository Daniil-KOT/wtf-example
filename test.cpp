#include <iostream>
#include <fstream>
#include <Windows.h>
#include <thread>

const int BUF_SIZE = 11;
DWORD g_BytesTransferred = 0;

VOID CALLBACK FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped)
{
	std::cerr << "Error code:\t" << dwErrorCode << std::endl;
	std::cerr << "Number of bytes:\t" << dwNumberOfBytesTransfered << std::endl;
	g_BytesTransferred = dwNumberOfBytesTransfered;
}

unsigned char* readFile(unsigned char* buf)
{
	HANDLE hFile;
	BOOL err = FALSE;
	OVERLAPPED ol = { 0 };
	LPCWSTR path = TEXT("C:/wtf/test/test/test.txt");
	DWORD bytesRead = 0;

	hFile = CreateFile(path,
					GENERIC_READ,
					FILE_SHARE_READ,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
					NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Unable to open file!" << std::endl;
		return nullptr;
	}

	if (FALSE == ReadFileEx(hFile, buf, BUF_SIZE - 1, &ol, FileIOCompletionRoutine))
	{
		std::cerr << "Unable to read from file!" << std::endl;
		CloseHandle(hFile);
		return nullptr;
	}

	SleepEx(5000, TRUE);
	bytesRead = g_BytesTransferred;

	if (0 < bytesRead && BUF_SIZE - 1 >= bytesRead)
	{
		buf[bytesRead] = '\0';
	}

	CloseHandle(hFile);

	return buf;
}

void checkBuf(unsigned char* buf)
{
	int err = 10;
	if (buf[0] == 0xaa)
		if (buf[1] == 0x9c)
			if (buf[2] == 0xf3)
				if (buf[3] == 0x01)
					if (buf[4] == 0xca)
						if (buf[5] == 0x24)
							if (buf[6] == 0xc4)
								if (buf[7] == 0xab)
									if (buf[8] == 0xb4)
										if (buf[9] == 0x4a)
											err = err / 0;
}

int main(int argc, char* argv[])
{
	static unsigned char buf[BUF_SIZE] = { 0 };

	std::this_thread::sleep_for(std::chrono::seconds(5));

	unsigned char* bufA = readFile(buf);
	if (bufA)
	{
		checkBuf(bufA);
	}

	return 0;
}
