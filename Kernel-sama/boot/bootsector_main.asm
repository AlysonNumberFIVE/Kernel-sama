

[org 0x7c00]

mov bp, 0x8000
mov sp, bp

mov bx, 0x9000
mov dh, 2

call disk_load

mov dx, [0x9000]
call print_hex
call print_nl

mov dx, [0x9000 + 512]
call print_hex

jmp $

%include "bootsector_disk.asm"
%include "print_hex/bootsec_print.asm"
%include "print_hex/print_hex.asm"

times 510-($-$$) db 0
dw 0xaa55


times 256 dw 0xdada 
times 256 dw 0xface
