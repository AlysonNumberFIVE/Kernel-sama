

print:
	pusha
	mov ah, 0x0e
start:
	mov al, [bx]

	int 0x10
	cmp al, 0
	je done

	add bx, 1
	jmp start
done:
	popa
	ret	
	

print_nl:
	pusha 

	mov ah, 0x0e
	mov al, 0x0a
	int 0x10
	mov al, 0x0d
	int 0x10
	popa 
	ret
