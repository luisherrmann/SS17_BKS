/*
 * Autor: Thomas Tegethoff
 * Idee & Vorlage: Heiko Will
 * Hinweise:
 *  - die Prozessliste ist doppelt verkettet, d.h. jedes Element kennt seinen
 *    Vorgänger und Nachfolger
 *  - das erste Element / der Kopf (head) der Prozessliste ist ein Dummy, d.h. es
 *    enthält keine Daten
 *  - die Prozessliste ist im Kreis verkettet, d.h. das erste Element (head)
 *    zeigt auf das zweite usw, und das letzte Element zeigt wieder auf das
 *    erste (head)
 *  - wenn kein Prozess der bereit ist gefunden werden kann, soll nichts gemacht werden
 *  - die Funktionen sollen den state des Prozesses, der als nächstes laufen
 *    soll, zu RUNNING ändern
 *  - der state des Prozesses der bisher gelaufen ist soll (abhängig von
 *    Scheduling-Verfahren) auch angepasst werden, entweder zu READY wenn 
 *    cycles_todo ungleich 0 ist bzw. zu DEAD wenn cycles_todo gleich 0 ist
 *  - Sie sollen keine anderen Werte der Prozesse verändern!
 */

#include <stdlib.h>
#include <stdio.h>
#include "scheduler.h"

void rr(struct Process* head) {
	if(head->next != NULL){
		struct Process* current = head->next;
		struct Process* rrProcess = head; //head wird mit state = DEAD erzeugt.
		do{
			if(current->state == RUNNING){
				if(current->cycles_todo == 0) current->state = DEAD;
				//Falls schon ein aelterer Prozess wartet, aktiviere
				else if(rrProcess != head){
					current->state = READY;
					break;
				}
				//Ansonsten versuche juengeren wartenden Prozess zu finden
				else{
				//Falls kein juengerer Prozess gefunden wird, wird state zum Schluss auf RUNNING gesetzt.
					current->state = READY;
					rrProcess = current;
				}
			}
			else if(current->state == READY){
				rrProcess = current;
			}
			current = current->next;
		} while(current != head);
		if(rrProcess != head) rrProcess->state = RUNNING;
	}
}

void fcfs(struct Process* head) {
	if(head->next != NULL){
		struct Process* current = head->next;
		if(current->cycles_done == 0) current->state = RUNNING;
		else{
			while(current->next != head && current->state != RUNNING){
				current = current->next;
			}
			if(current->cycles_todo == 0){
				current->state = DEAD;
				current->next->state = RUNNING;
			}
		}
	}
}

void spn(struct Process* head) {
	if(head->next != NULL){
		struct Process* sptProcess = head;
		struct Process* current = head->next;
		do{
			if(current->state == RUNNING){
				if(current->cycles_todo == 0) current->state = DEAD;
				else{
					sptProcess = current;
					break;
				}
			}
			else if(current->state == READY && (sptProcess->cycles_todo == 0 || current->cycles_todo < sptProcess->cycles_todo)){
				sptProcess = current;
			}
			current = current->next;
		} while(current != head);
		if(sptProcess != head) sptProcess->state = RUNNING;
	}
}

void hrrn(struct Process* head) {
	if(head->next != NULL){
		struct Process* hrrProcess = head;
		double hrrRatio = -1.0;
		struct Process* current = head->next;
		do{
			if(current->state == RUNNING){
				if(current->cycles_todo == 0) current->state = DEAD;
				else{
					hrrProcess = current;
					break;
				}
			}
			else if(current->state == READY){
				const uint64_t currRatio = (double)(current->cycles_waited + current->cycles_todo)/(double)current->cycles_todo;
				if(currRatio > hrrRatio){
					hrrProcess = current;
					hrrRatio = currRatio;
				}
			}
			current = current->next;
		} while(current != head);
		if(hrrProcess != head) hrrProcess->state = RUNNING;
	}
}

