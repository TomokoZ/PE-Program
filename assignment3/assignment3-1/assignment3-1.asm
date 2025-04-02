extern _printf          

section .data
    msg db "Hello, world", 0        
    fmt db "String length: %d", 10, 0 

section .text
    global _main          

; 计算字符串长度
string_length:
    xor ecx, ecx              
.count:
    mov al, byte [edx + ecx]   
    cmp al, 0                  
    je .done                    
    inc ecx         ;加一          
    jmp .count      ;递归           
.done:
    ret                         

_main:
;存入字符串地址
    lea edx, [msg]  
    call string_length

    ; 打印输出
    push ecx
    push fmt
    call _printf
    add esp, 8               
    
    xor eax, eax               
    ret
