;Johannes Obenaus und Christoph Husemann

section .data

section .text

global strToInt

strToInt: 
	mov rax,0
	mov r9,0
	mov r9b,dl		;die Basis wird in r9b gespeichert, da RDX (und damit dl) bei Multiplikationen überschrieben werden können
	cmp r9b,36
	jg endCalc		;Falls r9b (letztes BYTE von RDX) also b > 36, springe zu endCalc
	cmp r9b,0
	jle endCalc		;Falls rax also b <= 0, springe zu endCalc
	mov r8,0
	mov rcx,[rsi]			
	loop:			;Schleife zum Auslesen der einzelnen Stellen					
		mov r8b, [rdi]	;Lese das nächste BYTE aus dem Speicher aus
		cmp r8b,cl
		je endCalc	;Ende "Zeichen" wurde gelesen
		cmp r8b,0
		je endCalc	;Falls die nächste Stelle eine 0 (ASCII NULL) ist, springe zum endCalc
			
		number:
			cmp r8b,48	;48-57 (0-9)
			jl endCalc	;Falls das BYTE < 48 ist, also keine Ziffer oder Buchstabe, dann springe zu "endCalc"
			cmp r8b,57
			jg letterG	;Falls das BYTE > 57 ist, ist es keine Ziffer -> Springe zur Prüfung auf Großbuchstabe
			sub r8b,48	;Es liegt eine Ziffer zwischen 0 und 9 (48 und 57) vor. Um sie zu erhalten, wird 48 subtrahiert
			jmp addByte

		letterG:
			cmp r8b,65
			jl endCalc	;Falls 57 < BYTE < 65 ist, also kein Buchstabe, dann springe zu "endCalc"
			cmp r8b,90
			jg letterS	;Falls das BYTE > 90 ist, ist es kein Großbuchstabe -> Springe zur Prüfung auf Kleinbuchstabe
			sub r8b,55	;Es liegt ein Großbuchstabe vor. Mit Subtraktion von 55 wird z.B. A als 10 gewertet usw.
			jmp addByte
		
		letterS:
			cmp r8b,97
			jl endCalc	;Falls 65 < BYTE < 97 ist, liegt kein Buchstabe vor, dann springe zu "endCalc"
			cmp r8b,122
			jg endCalc	;Falls das BYTE > 122 ist, liegt kein Buchstabe vor, springe zu "endCalc"
			sub r8b,87	;Es liegt ein Kleinbuchstabe vor. Mit Subtraktion von 87 wird z.B. a als 10 gewertet usw.
			
		addByte:
			cmp r8b,r9b
			jge endCalc	;Falls das gelesene Zeichen r8b >= die Basis r9b ist, springe zu "endCalc"
			mul r9		;Multipliziere Ergebnis in rax mit Basis r9 (r9b erzeugt überlauf)
			add rax,r8	;Neue Stelle wird zum Ergebnis rax hinzugefügt
			mov [rsi],rdi	;Die letzte gelesene Stelle wird neu gesetzt
		nextByte:		
			inc rdi
			jmp loop			
	endCalc:
		mov [rsi],rdi	;Die letzte gelesene Stelle wird neu gesetzt. Falls das Ergebnis korrekt ist, zeigt rdi auf das 0-BYTE
	ret
