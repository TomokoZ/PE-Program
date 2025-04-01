extern _printf, _scanf 


section .data
    fmt_prompt1 db "Enter the first number: ", 0
    fmt_prompt2 db "Enter the second number: ", 0
    fmt_sum db "sum is: %d", 10, 0
    fmt_sub db "subtraction is: %d", 10, 0
    fmt_scanf db "%d", 0          

section .bss
    Data1 resd 1      
    Data2 resq 1     

section .text
    global _main                

_main:
    push fmt_prompt1
    call _printf
    add esp, 4

    ; 读取第一个数字
    push Data1
    push fmt_scanf
    call _scanf
    add esp, 8

    push fmt_prompt2
    call _printf
    add esp, 4

    ; 读取第二个数字
    push Data2
    push fmt_scanf
    call _scanf
    add esp, 8

    ; 计算和
    mov eax, [Data1]  
    add eax, [Data2]  
    push eax                      
    push fmt_sum                   
    call _printf
    add esp, 8                     

    ; 计算差
    mov eax, [Data1]  
    sub eax, [Data2]   
    push eax                       
    push fmt_sub                  
    call _printf
    add esp, 8                    

    xor eax, eax
    ret