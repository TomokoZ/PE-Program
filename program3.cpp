#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winnt.h>

void xor_encrypt(unsigned char *data, size_t len, unsigned char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key; 
    }
}

int main() {
    FILE *fp_program1 = fopen("program1.exe", "rb");  
    fseek(fp_program1, 0, SEEK_END);  
    long program1_size = ftell(fp_program1);  
    fseek(fp_program1, 0, SEEK_SET);  

    unsigned char *program1_data = (unsigned char*)malloc(program1_size);
    fread(program1_data, 1, program1_size, fp_program1); 
    fclose(fp_program1); 

    xor_encrypt(program1_data, program1_size, 0x40);

    FILE *fp_program2 = fopen("program2.exe", "rb");

    // 读取DOS头
    IMAGE_DOS_HEADER dos_header;
    fread(&dos_header, sizeof(IMAGE_DOS_HEADER), 1, fp_program2);

    // 读取NT头 
    fseek(fp_program2, dos_header.e_lfanew, SEEK_SET);
    IMAGE_NT_HEADERS64 nt_headers;
    fread(&nt_headers, sizeof(IMAGE_NT_HEADERS64), 1, fp_program2);

    // 获取节区数量
    int num_sections = nt_headers.FileHeader.NumberOfSections;
    IMAGE_SECTION_HEADER *sections = (IMAGE_SECTION_HEADER*)malloc((num_sections + 1) * sizeof(IMAGE_SECTION_HEADER));

    // 读取节区头
    fseek(fp_program2, dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS64), SEEK_SET);
    fread(sections, sizeof(IMAGE_SECTION_HEADER), num_sections, fp_program2);

    // 创建新节区 ".program"
    IMAGE_SECTION_HEADER new_section = {0};
    memcpy(new_section.Name, ".program", 8);  

    // 获取节区对齐和文件对齐值
    DWORD section_alignment = nt_headers.OptionalHeader.SectionAlignment;
    DWORD file_alignment = nt_headers.OptionalHeader.FileAlignment;

    // 通过最后一个节区的地址计算新节区的地址
    IMAGE_SECTION_HEADER* last_section = &sections[num_sections - 1];
    new_section.VirtualAddress = (last_section->VirtualAddress + last_section->Misc.VirtualSize + section_alignment - 1) & ~(section_alignment - 1);
    new_section.PointerToRawData = (last_section->PointerToRawData + last_section->SizeOfRawData + file_alignment - 1) & ~(file_alignment - 1);

    // 设置新节区的大小
    new_section.Misc.VirtualSize = (program1_size + section_alignment - 1) & ~(section_alignment - 1);
    new_section.SizeOfRawData = program1_size;

    // 设置新节区的特性
    new_section.Characteristics = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE;

    // 更新 NT 头信息
    nt_headers.FileHeader.NumberOfSections += 1;
    nt_headers.OptionalHeader.SizeOfImage = new_section.VirtualAddress + new_section.Misc.VirtualSize;

    FILE *fp_output = fopen("retrofitting.exe", "wb");

    fseek(fp_program2, 0, SEEK_END);
    long program2_size = ftell(fp_program2);
    fseek(fp_program2, 0, SEEK_SET);
    unsigned char *program2_data = (unsigned char*)malloc(program2_size);
    fread(program2_data, 1, program2_size, fp_program2);
    fwrite(program2_data, 1, program2_size, fp_output);  
    free(program2_data);
    // 写入新的 NT 头
    fseek(fp_output, dos_header.e_lfanew, SEEK_SET);
    fwrite(&nt_headers, sizeof(IMAGE_NT_HEADERS64), 1, fp_output);

    // 写入新的节区头信息
    fseek(fp_output, dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS64), SEEK_SET);
    fwrite(sections, sizeof(IMAGE_SECTION_HEADER), num_sections, fp_output);

    // 写入 .program 节区头信息
    fwrite(&new_section, sizeof(IMAGE_SECTION_HEADER), 1, fp_output);

    fseek(fp_output, new_section.PointerToRawData, SEEK_SET);
    fwrite(program1_data, 1, program1_size, fp_output);

    fclose(fp_program2);
    fclose(fp_output);
    free(program1_data);
    free(sections);

    return 0;
}

