#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
void init (list *l) {
	l->front = NULL;
	l->end = NULL;
}
void add (list *l, char temp[]) {
	mnode *str = (mnode*)malloc(sizeof(mnode));
	strcpy(str->file, temp);
	str->next = NULL;
	if (l->front == NULL) {
		l->front = str;
		l->end = str;
		return;
	}
	mnode *ptr = l->end;
	ptr->next = str;
	l->end = str;
	return;
}
char* del (list *l) {
	//char *str;
	char *str = (char*)malloc(sizeof(char)*100);
	mnode *ptr = l->front;
	l->front = ptr->next;
	strcpy (str, ptr->file);
	free (ptr);
	return str;
}
int isempty (list *l) {
	return l->front == NULL;
}
