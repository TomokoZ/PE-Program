extern _printf, _scanf

section .data
;定义数据
    fmt_prompt db "Enter the first number: ", 0
    fmt_prompt2 db "Enter the second number: ", 0
    fmt_prompt3 db "Enter the third number: ", 0
    fmt_prompt4 db "Enter the last number: ", 0
;定义输入
    fmt_scanf_short db "%hd", 0     
    fmt_scanf_float db "%f", 0      
    fmt_scanf_double db "%lf", 0    
    fmt_scanf_extended db "%Lf", 0 

    fmt_short db "Data1 (short): %hd", 10, 0
    fmt_float db "Data2 (float): %.3f", 10, 0
    fmt_double db "Data3 (double): %.3lf", 10, 0
    fmt_extended db "Data4 (long double): %.6Le", 10, 0

section .bss
    Data1 resw 1    
    Data2 resd 1    
    Data3 resq 1    
    Data4 rest 1   

section .text
    global _main

_main:
    ; 输入处理
    push fmt_prompt
    call _printf
    add esp, 4

    push Data1
    push fmt_scanf_short
    call _scanf
    add esp, 8

    push fmt_prompt2
    call _printf
    add esp, 4

    push Data2
    push fmt_scanf_float
    call _scanf
    add esp, 8

    push fmt_prompt3
    call _printf
    add esp, 4

    push Data3
    push fmt_scanf_double
    call _scanf
    add esp, 8

    push fmt_prompt4
    call _printf
    add esp, 4

    push Data4
    push fmt_scanf_extended
    call _scanf
    add esp, 8  

    ; 输出处理

    movsx eax, word [Data1]
    push eax
    push fmt_short
    call _printf
    add esp, 8

    fld dword [Data2]   
    sub esp, 8             
    fstp qword [esp]      
    push fmt_float
    call _printf
    add esp, 12

    sub esp, 8
    fld qword [Data3]     
    fstp qword [esp]      
    push fmt_double
    call _printf
    add esp, 12

    sub esp, 12           
    mov eax, [Data4]
    mov [esp], eax
    mov eax, [Data4+4]
    mov [esp+4], eax
    mov eax, [Data4+8]
    mov [esp+8], eax       
    push fmt_extended
    call _printf
    add esp, 16

    xor eax, eax
    ret