;Johannes Obenaus und Christoph Husemann
;Quicksort

section .data

section .text

global sort

sort: 
	;rdi: len (r)
	;rsi: array (A)
	mov rdx,1	;p
	push rsi
	push rdi
	call quicksort
	pop rdi
	pop rax
	ret
quicksort:
	cmp rdx,rdi	;Abbruchbedingung Rekursion
	jge endQuickSort	
	push rdi	
	push rsi
	push rdx
	call partition
	mov rcx,rax
	pop rdx		;Wiederherstellen der Paramenter
	pop rsi
	pop rdi

	push rdi	;Sichern von len r
	push rsi	;sichern von array
	push rdx	;sichern von p
	push rcx	;sichern von q
	dec rcx
	mov rdi,rcx	;q-1 wird als len übergeben
	call quicksort
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	push rdi
	push rsi
	push rdx
	push rcx	
	inc rcx
	mov rdx,rcx	;q+1 wird als p übergeben
	call quicksort
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	endQuickSort:
	mov rax,rsi
	ret
partition:
	;rdi: len r
	;rsi: array A
	;rdx; p
	push r12 	;sichern von r12
	mov r8,[rsi+rdi*8-8]	;lade Vergleichselement x
	mov r9,rdx	;Initialisiere r9 wie i als Pointer auf den Trenner zwischen den kleineren und größeren Elementen
	dec r9
	mov rcx,rdx	;Initialisiere rcx wie j als counter
	mov r10,rdi	;Initialisiere r10 als len-1
	dec r10	
	part_loop:
		cmp rcx,r10	;Abbruchbedingung Schleife
		jg endPart
		mov r11,[rsi+rcx*8-8]	;lade nächsten Element
		cmp r11,r8	;Vergleich der Elemente
		jg nextPartLoop	;Springe zum nächsten Schleifendurchlauf
		inc r9	;Trennung zwischen kleineren und größeren Elementen wird verschoben, da array(rcx)<array(r8)
		mov r12,[rsi+r9*8-8]	;laden des r9-ten Elements in r12
		mov [rsi+r9*8-8],r11	;Vertauschen von den Elementen von r11 und r12 im Array
		mov [rsi+rcx*8-8],r12
		nextPartLoop:	
			inc rcx
			jmp part_loop
	endPart:
	inc r9
	mov r11,[rsi+r9*8-8]	;Lese r9-tes Element des Arrays
	mov [rsi+r9*8-8],r8	;Vertausche das r8-te und r9-te Element im Array
	mov [rsi+rdi*8-8],r11
	pop r12		;Wiederherstellen von r12	
	mov rax,r9
	ret
