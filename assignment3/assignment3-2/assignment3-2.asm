extern _printf         

section .data
    fmt db "%c", 0     

section .text
    global _main      

; 打印 ASCII 字符 
print_ascii:
    mov ecx, 128       
    mov bl, 0          

.print:
    push ecx;保护ecx
    push ebx          
    push fmt          
    call _printf      
    add esp, 8;不需要清除ecx

    pop ecx;取出数据，准备循环
    inc bl             
    loop .print        

    ret               

_main:
    call print_ascii  

    xor eax, eax      
    ret              
