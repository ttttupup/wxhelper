// ConsoleApplication.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include "getopt.h"



std::wstring Utf8ToUnicode(const char* buffer) {
	int c_size = MultiByteToWideChar(CP_UTF8, 0, buffer, -1, NULL, 0);
	if (c_size > 0) {
		wchar_t* temp = new wchar_t[c_size + 1];
		MultiByteToWideChar(CP_UTF8, 0, buffer, -1, temp, c_size);
		temp[c_size] = L'\0';
		std::wstring ret(temp);
		delete[] temp;
		temp = NULL;
		return ret;
	}
	return std::wstring();
}

DWORD  GetPIDForProcess(wchar_t* process)
{
	HANDLE hSnapshot;
	DWORD dPid = 0;
	PROCESSENTRY32W pe32;
	int working;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!hSnapshot) {
		return 0;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	for (working = Process32FirstW(hSnapshot, &pe32); working; working = Process32NextW(hSnapshot, &pe32))
	{
		if (!wcscmp(pe32.szExeFile, process))
		{
			dPid = pe32.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);
	return dPid;
}

HMODULE GetDLLHandle(wchar_t* wDllName, DWORD dPid)
{
	HMODULE result;
	tagMODULEENTRY32W me32;
	void* snapMod;

	if (!dPid) {
		return 0;
	}

	snapMod = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dPid);
	me32.dwSize = sizeof(tagMODULEENTRY32W);
	if (Module32FirstW(snapMod, &me32))
	{
		while (wcscmp(wDllName, me32.szModule))
		{
			if (!Module32NextW(snapMod, &me32))
				goto error;
		}
		CloseHandle(snapMod);
		result = me32.hModule;
	}
	else
	{
	error:
		CloseHandle(snapMod);
		result = 0;
	}
	return result;
}



int  InjectDll(wchar_t* szPName, wchar_t* szDllPath)
{
	int result;
	HANDLE hRemoteThread;
	LPTHREAD_START_ROUTINE lpSysLibAddr;
	HINSTANCE__* hKernelModule;
	LPVOID lpRemoteDllBase;
	HANDLE hProcess;
	unsigned int dwPid;
	size_t ulDllLength;

	dwPid = GetPIDForProcess(szPName);
	ulDllLength = (wcslen(szDllPath) + 1) * sizeof(wchar_t);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
	if (!hProcess) {
		return 0;
	}

	lpRemoteDllBase = VirtualAllocEx(hProcess, NULL, ulDllLength, MEM_COMMIT, PAGE_READWRITE);
	if (lpRemoteDllBase)
	{
		if (WriteProcessMemory(hProcess, lpRemoteDllBase, szDllPath, ulDllLength, NULL)
			&& (hKernelModule = GetModuleHandleW(L"kernel32.dll")) != 0
			&& (lpSysLibAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernelModule, "LoadLibraryW")) != 0
			&& (hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, lpSysLibAddr, lpRemoteDllBase, 0, NULL)) != 0)
		{
			WaitForSingleObject(hRemoteThread, INFINITE);
			VirtualFreeEx(hProcess, lpRemoteDllBase, ulDllLength, MEM_DECOMMIT | MEM_RELEASE);
			CloseHandle(hRemoteThread);
			CloseHandle(hProcess);
			OutputDebugStringA("[DBG] dll inject success");
			result = 1;
		}
		else
		{
			VirtualFreeEx(hProcess, lpRemoteDllBase, ulDllLength, MEM_DECOMMIT | MEM_RELEASE);
			CloseHandle(hProcess);
			result = 0;
		}
	}
	else
	{
		CloseHandle(hProcess);
		result = 0;
	}
	return result;
}

int  UnInjectDll(wchar_t* szPName, wchar_t* szDName)
{
	HMODULE  hDll;
	HANDLE lpFreeLibAddr;
	HINSTANCE__* hK32;
	HANDLE hProcess;
	unsigned int dwPID;

	dwPID = GetPIDForProcess(szPName);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPID);
	if (!hProcess) {
		return 0;
	}

	hK32 = GetModuleHandleW(L"Kernel32.dll");
	if (!hK32) {
		return 0;
	}
	
	lpFreeLibAddr = GetProcAddress(hK32, "FreeLibraryAndExitThread");
	//lpFreeLibAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(hK32, "FreeLibrary");
	hDll = GetDLLHandle(szDName, dwPID);
	if (hDll) {
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpFreeLibAddr, hDll, NULL, NULL);
		if (hThread == NULL) {
			int errorCode = GetLastError();
			return 0;
		}
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		CloseHandle(hProcess);
		return 1;
	}

	CloseHandle(hProcess);
	return 0;
}
int main(int argc, char** argv)
{
	int param;
	char cInjectprogram[MAX_PATH] = { 0 };
	char cUnInjectprogram[MAX_PATH] = { 0 };
	char cDllPath[MAX_PATH] = { 0 };
	char cDllName[MAX_PATH] = { 0 };

	while ((param = getopt(argc, argv, "i:p:u:d:h")) != -1)
	{
		switch (param)
		{
		case 'i':
			strcpy(cInjectprogram, optarg);
			break;
		case 'p':
			strcpy(cDllPath, optarg);
			break;
		case 'u':
			strcpy(cUnInjectprogram, optarg);
		case 'd':
			strcpy(cDllName, optarg);
			break;
		case 'h':
			printf("Usage: %s [-i/u]  [-p/d] \n", argv[0]);
			printf("Options:\n");
			printf("  -h         Print this help message.\n");
			printf("  -i <program name>   Name of the running program to be injected.\n");
			printf("  -u <program name>   Name of the running program to be uninstalled.\n");
			printf("  -p <path>   Full path of injection file.\n");
			printf("  -d <file>   Name of injection file.\n");
			printf("\n");
			printf("Examples:\n");
			printf("  window>  %s -i test.exe -p c:/inject.dll \n", argv[0]);
			printf("  window>  %s -u test.exe -d inject.dll \n", argv[0]);
			exit(0);
			break;
		default:
			abort();
			break;
		}
	}
	if (cInjectprogram[0] != 0 && cDllPath[0] != 0)
	{
		if (cInjectprogram[0] != '\0' && cDllPath[0] != '\0')
		{
			std::wstring wsProgram = Utf8ToUnicode(cInjectprogram);
			std::wstring wsPath = Utf8ToUnicode(cDllPath);
			int ret = InjectDll((wchar_t*)wsProgram.c_str(), (wchar_t*)wsPath.c_str());
			printf(" 注入结果：%i \n", ret);
		}

	}

	if (cUnInjectprogram[0] != 0 && cDllName[0] != 0)
	{
		if (cUnInjectprogram[0] != '\0' && cDllName[0] != '\0')
		{
			std::wstring wsUnInjectProgram = Utf8ToUnicode(cUnInjectprogram);
			std::wstring wsName = Utf8ToUnicode(cDllName);
			int ret = UnInjectDll((wchar_t*)wsUnInjectProgram.c_str(), (wchar_t*)wsName.c_str());
			printf(" 卸载结果：%i \n", ret);
		}

	}

	return 0;
}
