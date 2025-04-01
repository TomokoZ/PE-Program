extern _printf
;定义数据
section .data
    real_number1 dd 1.234
    real_number2 dq 123.456
    real_number3 dt 1.234567e20
;定义字符串
    fmt_float db "real_number1: %.3f", 10, 0
    fmt_double db "real_number2: %.3lf", 10, 0
    fmt_extended db "real_number3: %.6e", 10, 0

section .text
    global _main

_main:
;打印float
    fld dword [real_number1]  
    sub esp, 8               
    fstp qword [esp]         
    push fmt_float
    call _printf
    add esp, 12

;打印double
    fld qword [real_number2] 
    sub esp, 8               
    fstp qword [esp]    
    push fmt_double
    call _printf
    add esp, 12

;打印longdouble
    fld tword [real_number3] 
    sub esp, 8               
    fstp qword [esp]    
    push fmt_extended
    call _printf
    add esp, 12

    xor eax, eax
    ret
