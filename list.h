typedef struct mnode {
	char file[100];
	struct mnode *next;
}mnode;
typedef struct list {
	mnode *front;
	mnode *end;
}list;
void init (list *);
void add (list *, char[]);
char* del (list*);
int isempty (list *l);

