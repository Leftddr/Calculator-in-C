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
void stack_operator1(char*,char *, char, int *, int *, int);
void stack_operator2(char*, char, int*, int *);
void stack_operator3(char);
void stack_num(char*, char*, char, int *, int *, int);
void stack_block(char*, int*);
void infixtopostfix(char *, char *, int *, int *);
int calcpostfix(char *);
int syntaxsearch(char *);

int main() {
	char input[100];
	char *postfix;
	int cri = 0;
	int cri2;
	int search;
	int result;
	printf("시작하려면 Enter, 종료할려면 q를 누르세요\n");
	while (getchar() != 'q') {
		printf("식 입력 : ");
		fgets(input, sizeof(input), stdin);
		search = syntaxsearch(input);
		if (search == -1) {
			printf("잘못 입력 하셨습니다\n"); printf("\n시작하려면 Enter, 종료할려면 q를 누르세요\n");}
		else {
			postfix = (char*)malloc((strlen(input) * 2));
			infixtopostfix(postfix, input, &cri, &cri2);
			printf("%s\n", postfix);
			result = calcpostfix(postfix);
			printf("결과값 :  %d\n", result);
			printf("\n시작하려면 Enter, 종료할려면 q를 누르세요\n");
		}
		cri = 0;
	}
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

void stack_operator1(char * postfix, char *example,char ch, int * cri, int*cri2, int i) {
	char temp;
	while (1) {
		if (example[0] == '-' && i==0 ) { *cri2 = 1; break; }
		else if (i >= 2) {
			if ((example[i - 1] == '+' || example[i - 1] == '-' || example[i - 1] == '*') && ch == '-') { *cri2 = 1; break; }
		}
		if (top == NULL) { break; }
		while (1) { temp = (char)pop(); if (temp != ' ') { break; } }
		if (temp == '*' || temp == '/' || temp == '%' || temp == '^') {
			postfix[(*cri)++] = temp;
			postfix[(*cri)++] = ' ';
			break;
		}
		else {
			push(temp);
			break;
		}
	}
	if (*cri2 != 1) {
		push(ch);
	}

	return;
}

void stack_operator2(char*postfix, char ch, int *cri, int *cri2) {
	char temp;
	while (1) {
		if (top == NULL) { break; }
		while (1) { temp = (char)pop(); if (temp != ' ') { break; } }
		if (temp == '^') {
			postfix[(*cri)++] = temp;
			postfix[(*cri)++] = ' ';
			break;
		}
		else if (temp == '-') { *cri = 1; }
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

void stack_num(char *postfix, char *example, char ch, int *cri, int * cri2, int i) {

	if (*cri2 == 1) { postfix[(*cri)++] = '-'; *cri2 = 0; }
	postfix[(*cri)++] = ch;
	if (example[i + 1] == '+' || example[i + 1] == '-' || example[i + 1] == '*' || example[i + 1] == '/' || example[i + 1] == '%' || example[i + 1] == '^' || example[i + 1] == '\n' || example[i + 1] == ')') {
		postfix[(*cri)++] = ' ';
	}

}

void infixtopostfix(char * postfix, char *example, int * cri, int *cri2) {
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
			stack_operator1(postfix,example,ch, cri, cri2, i); break;
		case '*':
		case '/':
		case '%':
			stack_operator2(postfix, ch, cri, cri2); break;
		case '^':
			stack_operator3(ch); break;
		case ' ':
		case '\0':
		case '\n':
		case '?':
			break;
		default:
			stack_num(postfix, example, ch, cri, cri2, i); break;
		}
	}
	while (1) {
		if (top == NULL) { break; }
		postfix[(*cri)++] = (char)pop();
		postfix[(*cri)++] = ' ';
	}
	postfix[*cri] = '\0';
}

int calcpostfix(char *postfix) {
	int num1, num2, num3;
	int cri = 0, cri2 = 0;
	int result;
	char temp, temp2;

	for (int i = 0; i < strlen(postfix); i++) {
		temp = postfix[i];
		if ((temp >= '0' && temp <= '9')) {
			while (postfix[i + cri] != ' ') {// 숫자구별 코드
				cri++;
			}
			num1 = atoi(&postfix[i]);
			i += cri;
			cri = 0;
			if (postfix[0] == '-') { num1 = -num1; }
			if (i >= 2) {
				if ((postfix[i - 2] == '+' || postfix[i - 2] == '-' || postfix[i - 2] == '*') && postfix[i - 1] == '-') {
					num1 = -num1; cri2 = 1;
				}
			}
			printf("%d\n", num1);
			push(num1);
		}
		else {
			if (temp != ' ') {
				if (i != 0 && cri2 != 1) {
					if (cri2!=1 && !(postfix[i - 1] >= '0' && postfix[i - 1] <= '9')) {
						num1 = pop(); num2 = pop();
						switch (temp) {
						case '+': (result = num1 + num2); push(result); break; // push를 해줘서 그 값을 기억하게 해야한다
						case '-': (result = num1 - num2); push(result); break;
						case '*':(result = num1 * num2);  push(result); break;
						case '/':(result = num2 / num1);  push(result); break;
						case '%':(result = num2 % num1);  push(result); break;
						case '^': {num3 = num2;
							for (int j = 0; j < (num1 - 1); j++) {
								num2 *= num3;
							}
							result = num2;  push(result); } break;
						default: printf("잘못된 수식 입력으로 계산 불가 \n"); break;
						}
					}
				}
			}
		}
		cri2 = 0;
	}
	return pop();
}

int syntaxsearch(char *exp)
{
	int length = strlen(exp);
	int i = 0;
	int ncount = 0;
	char symbol, opr;

	for (i = 0; i < length; i++)
	{
		symbol = exp[i];

		switch (symbol)
		{
		case '+':
		case '-':
		case '*':
		case '/':
			opr = exp[i + 1];
			if (opr == '+' || opr == '-' || opr == '*' || opr == '/')
			{
				printf("Operator error");
				return -1;
			}
			break;

		case '(':
		case '{':
		case '[':
			push(symbol);
			break;

		case ')':
			if (top == NULL)
			{
				printf("' ) ' Syntax error!!\n");
				return -1;
			}
			else
			{
				if (pop() != '(')
				{
					printf("' ( ' Syntax error!!\n");
					return -1;
				}
			}
			break;

		case '}':
			if (top == NULL)
			{
				printf("' } ' Syntax error!!\n");
				return -1;
			}
			else
			{
				if (pop() != '{')
				{
					printf("' { ' Syntax error!!\n");
					return -1;
				}
			}
			break;

		case ']':
			if (top == NULL)
			{
				printf("' ] ' Syntax error!!\n");
				return -1;
			}
			else
			{
				if (pop() != '[')
				{
					printf("' [ ' Syntax error!!\n");
					return -1;
				}
			}
			break;
		case '\n': break;
		default:
			if (symbol < '0' || symbol > '9')
			{
				printf("All not number!!\n");
				return -1;
			}
			else
			{
				ncount++;
			}
			break;
		}
	}
	if (!ncount || ncount == 1)
	{
		printf("Nmber is empty or unavailable!!");
		return -1;
	}
	while (top)
	{
		opr = (char)pop();
		if (opr == '(' || opr == '{' || opr == '[' ||
			opr == '+' || opr == '-' || opr == '*' || opr == '/')
		{
			printf("` %c ' syntax error!!\n", opr);
			return -1;
		}
	}
	return 0;
}