#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#pragma warning(disable:4996)

struct stacknode {
	int data;
	struct stacknode * next;
};

stacknode * top;
int numofdata = 0;

void init();
void push(int);
int pop();
void stack_operator1(char*, char, int *);
void stack_operator2(char*, char, int*);
void stack_operator3(char);
void stack_num(char*, int*);
void stack_block(char*, int*);
void infixtopostfix(char *, char *, int *);

int main() {
	char input[100];
	char *postfix;
	int cri = 0;
	printf("╫д ют╥б : ");
	fgets(input, sizeof(input), stdin);
	fflush(stdin);
	postfix = (char*)malloc(sizeof(strlen(input) * 2));
	infixtopostfix(postfix, input, &cri);
	printf("%s\n", postfix);
	system("pause");
	return 0;
}

void init() {
	top = NULL;
}

void push(int num) {
	stacknode *temp = (stacknode*)malloc(sizeof(stacknode));
	if (top == NULL) {
		temp->data = num;
		top = temp;
		numofdata++;
	}
	else {
		temp->data = num;
		temp->next = top;
		top = temp;
		numofdata++;
	}
}

int pop() {
	int temp;
	stacknode * prev = top;
	if (top == NULL) { printf("Stack is empty\n"); return -1; }
	else if (numofdata == 1) {
		temp = top->data;
		top = NULL;
		numofdata--;
		return temp;
	}
	else {
		temp = top->data;
		top = prev->next;
		free(prev);
		numofdata--;
		return temp;
	}
}

void stack_operator1(char * postfix, char ch, int * cri) {
	char temp;
	while (1) {
		if (top == NULL) { break; }
		while (1) { temp = (char)pop(); if (temp != ' ') { break; } }
		if (temp=='*'||temp=='/'||temp=='%'||temp=='^') {
			postfix[(*cri)++] = temp;
			postfix[(*cri)++] = ' ';
			break;
		}
		else {
			push(temp);
			break;
		}
	}
	push(ch);
	return;
}

void stack_operator2(char*postfix, char ch, int *cri) {
	char temp;
	while (1) {
		if (top == NULL) { break; }
		while (1) {
			temp = (char)pop(); if (temp != ' ') { break; }
		}
		if (temp == '^') {
			postfix[(*cri)++] = temp;
			postfix[(*cri)++] = ' ';
			break;
		}
		else {
			push(temp);
			break;
		}
	}
	push(ch);
	return;
}

void stack_operator3(char ch) {
	push(ch);
	return;
}

void stack_block(char*postfix, int*cri) {
	char temp;
	while (1) {
		temp = (char)pop();
		if (temp != '(') {
			postfix[(*cri)++] = temp;
			postfix[(*cri)++] = ' ';
		}
		else {
			break;
		}
	}
	return;
}

void stack_num(char *postfix, char ch, int *cri) {

		postfix[(*cri)++] = ch;
		postfix[(*cri)++] = ' ';

}

void infixtopostfix(char * postfix, char *example, int * cri) {
	char ch;
	for (int i = 0; i < strlen(example); i++) {
		ch = example[i];
		switch (ch) {
		case '(':
			push(ch); break;
		case ')':
			stack_block(postfix, cri); break;
		case '+':
		case '-':
			stack_operator1(postfix, ch, cri); break;
		case '*':
		case '/':
		case '%':
			stack_operator2(postfix, ch, cri); break;
		case '^':
			stack_operator3(ch); break;
		case ' ':
		case '\0': 
		case '\n':
		case '?':
			break;
		default:
			stack_num(postfix, ch, cri); break;
		}
	}
	while (1) {
		postfix[(*cri)++] = (char)pop();
		postfix[(*cri)++] = ' ';
		if (top == NULL) { break; }
	}
	postfix[*cri] = '\0';
}