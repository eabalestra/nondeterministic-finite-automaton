#include <stdio.h>
#include <stdlib.h>
//#include "linked_list.h"

typedef struct Node
{
  int data;
  struct Node *next;
} Node;

Node *createNode(int data);
void insertAtEnd(Node **head, int data);
int length(Node *head);
void printList(Node *head);

Node *createNode(int data)
{
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

void insertAtEnd(Node **head, int data)
{
    Node *newNode = createNode(data);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Node *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

void printList(Node *head)
{
  Node *current = head;
  while (current != NULL)
  {
    printf("%d -> ", current->data);
    current = current->next;
  }
  printf("NULL\n");
}

int length(Node *head)
{
  Node *current = head;
  int cant = 0;
  while (current != NULL)
  {
    cant++;
  }
  return cant;
}
