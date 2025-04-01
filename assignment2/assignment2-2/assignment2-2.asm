extern _printf, _scanf    

section .data
    prompt1 db "Enter the first number: ", 0
    prompt2 db "Enter the second number: ", 0

    r_less db "-1", 0
    r_greater db "1", 0
    r_equal db "0", 0
    fmt_scanf db "%d", 0           

section .bss
    num1 resd 1                    
    num2 resd 1                   

section .text
    global _main                 

_main:
    ; 输入第一个数字
    push prompt1
    call _printf
    add esp, 4

    push num1
    push fmt_scanf
    call _scanf
    add esp, 8

    ; 输入第二个数字
    push prompt2
    call _printf
    add esp, 4

    push num2
    push fmt_scanf
    call _scanf
    add esp, 8

    mov eax, [num1]                
    cmp eax, [num2]             
    je equal                      
    jl less                       
    jg greater                   

equal:
    ; 输出 "0"
    push r_equal
    call _printf
    add esp, 4
    jmp exit

less:
    ; 输出 "-1"
    push r_less
    call _printf
    add esp, 4
    jmp exit

greater:
    ; 输出 "1"
    push r_greater
    call _printf
    add esp, 4

exit:
    xor eax, eax                
    ret                         
