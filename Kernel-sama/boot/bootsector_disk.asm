

disk_load:
	pusha
	push dx

	mov ah, 0x02  ; BIOS read call/function
	mov al, dh    ; number of sections to read
	mov cl, 0x02  ; the first available sector
	mov ch, 0x00  ; cylinder
	mov dh, 0x00  ; head
	
	; [es:bx] is set in the main. The start buffer
	; of our first readable data.
	int 0x13     ; call that bitch	
	jc disk_error

	pop dx
	cmp al, dh ; check nubmer of sectors read
	jne sectors_error
	popa
	ret

disk_error:
	; please don't ever call
	mov bx, DISK_ERROR
	call print
	call print_nl
	mov dh, ah
	call print_hex 
	jmp disk_loop

sectors_error:
	mov bx, SECTORS_ERROR
	call print

disk_loop:
	jmp $

DISK_ERROR: 
	db "Disk read error", 0
SECTORS_ERROR: 
	db "Incorrect number of sectors read", 0

