#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct Node
{
  int data;
  struct Node *next;
} Node;

Node *createNode(int data);
void insertAtEnd(Node **head, int data);
int length(Node *head);
void printList(Node *head);

#endif
