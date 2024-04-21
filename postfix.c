/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10 
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

	printf("[----- [강수빈] [2023078011] -----]\n");

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		// command 문자 입력 받음
		printf("Command = ");
		scanf(" %c", &command);

		// 문자에 따른 함수실행
		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			// 그 외 문자 입력시 출력 문구
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); // 반복 조건

	return 1;
}

void postfixPush(char x)  //postfix 스택의 top 값을 증가하고 그 top 위치에 스택에 값 넣는 함수
{
    postfixStack[++postfixStackTop] = x;
}

char postfixPop()   // postfix 스택에서 값을 빼서 리턴하고 top은 값 하나 감소
{
    char x;
    if(postfixStackTop == -1)  // 스택이 비어있다면 \0반환
        return '\0';
    else { // 스택이 비어있는게 아니라면
    	x = postfixStack[postfixStackTop--];   //top이 있는곳에 있는값을 x에 저장하고 top의 값 1 감소
    }
    return x;   // x 반환
}

void evalPush(int x)  // 정수 값을 받아 eval스택에 넣음
{
    evalStack[++evalStackTop] = x;
}

int evalPop()  // 스택에서 요소 제거
{
    if(evalStackTop == -1)  // 스택 비어있다면 -1 반환
        return -1;
    else
        return evalStack[evalStackTop--];  // 아니라면 top이 가리키는 곳의 값을 반환하고 top의 값 1감소
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);    // infix 문자열을 입력받음
}

precedence getToken(char symbol)  // 문자를 받아 symbol을 우선순위 값으로 리턴
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)  // 우선순위 리턴 함수
{
	return getToken(x);   
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)   
{
	if (postfixExp == '\0')  // postfix 스택이 비어있다면
		strncpy(postfixExp, c, 1);  // c가 가리키는 문자를 복사해서 넣음
	else  // 비어있지않다면
		strncat(postfixExp, c, 1);  // c가 가리키는 문자를 뒤에 추가함
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char* exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */

		while (*exp != '\0') {
			if (getToken(*exp) == operand) {  // 가져온 문자가 연산자가 아닐경우
				charCat(exp);  // 문자를 postfixExp에 추가
			}

			// 연산자일 경우
			else {
				if (*exp == '(') { // 가져온 문자가 '('일 경우
					postfixPush(*exp);  // 그걸 postfixStack에 넣음
				}
				else if (*exp == ')') {  // 가져온 문자가 ')'일 경우
					while ((x = postfixPop()) != '(') {  // postifixStack에서 가져온 값이 '('아닐 때 까지
						charCat(&x);  // postfixExp에 가져온 값을 넣는다.
					}
				}
				// 괄호가 아닌 경우
				else {
					// postfixStack이 비어있지 않고 top이 가리키고 있는 연산자보다 가져온 연산자의 우선순위가 더 낮은 동안 실행
					while (postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) {
						x = postfixPop(); // postfixStack에서 연산자 가져와서
						charCat(&x);   // postfixExp에 추가한다.
					}
					postfixPush(*exp);   // postfixStack에 그 연산자를 넣는다.
				    }
		        }
                 exp++;  // exp의 값을 증가해 다음 문자로 넘어간다.
		}

        // 스택에 남은 연산자를 pop하여 postfixExp에 추가
        while(postfixStackTop != -1){
            x = postfixPop();
            charCat(&x);
        }

}

// 디버깅 함수
void debug()
{
	printf("\n---DEBUG\n");
	// 각각의 값 출력
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	// postfixStack 값 차례대로 출력
	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

// 초기화 함수
void reset()
{
	// 값 초기화
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	// postfixStack 모든 값 NULL로 초기화
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
    
	// top의값과 evalResult 값 초기화
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
	/* postfixExp, evalStack을 이용한 계산 */
	char* exp = postfixExp;  // postfixExp에서 문자를 하나씩 가져올 포인터
	int operand1, operand2;  // 숫자 담아둘 변수 2개 선언

	while (*exp != '\0') {  // exp가 가져올 문자가 있을때
		
		// exp로 가져온 문자가 연산자가 아니라면(숫자라면) 그 값에 '0'을 뺀다(문자이기때문)
		if (getToken(*exp) == operand) { 
			evalPush(*exp - '0');  // evalStack에 저장
		}

		//연산자라면
		else {
			//evalStack에 있는 값을 차례로 가져옴
			operand2 = evalPop();
			operand1 = evalPop();

			//exp가 가져온 연산자에 따라 가져온 두 숫자를 계산해서 evalStack에 넣는다
			switch (*exp) {
			case '+':
				evalPush(operand1 + operand2);
				break;
			case '-':
				evalPush(operand1 - operand2);
				break;
			case '*':
				evalPush(operand1 * operand2);
				break;
			case '/':
				evalPush(operand1 / operand2);
				break;
			}
		}
		exp++;  // 다음 문자를 가져오기위해 증가한다.
	}

	// evalStack에서 최종 계산 결과를 출력한다.
	evalResult = evalPop();
	printf("Evaluation result : %d\n", evalResult);
}
