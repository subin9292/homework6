/**
 * circularQ.c
 *
 * School of Computer Science,
 * Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4   // 원형 큐 크기 정의

typedef char element;  // char 자료형을 element라는 이름으로 쓰겠다

typedef struct {						// 원형 큐 구조체 생성
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType; // 구조체 변수 선언

// 함수 선언
QueueType* createQueue();
int freeQueue(QueueType* cQ);
int isEmpty(QueueType* cQ);
int isFull(QueueType* cQ);
void enQueue(QueueType* cQ, element item);
void deQueue(QueueType* cQ, element* item);
void printQ(QueueType* cQ);
void debugQ(QueueType* cQ);
element getElement();

int main(void)
{
	QueueType* cQ = createQueue();  //구조체 포인터 cQ 생성 후 front, rear값 0으로 초기화
	element data;   // element 타입 data 변수 생성
	char command;  // char 자료형 command 변수 생성

	do {
		// 초기 프린트 내용
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		// command 종류 문자 하나 입력 받음
		printf("Command = ");
		scanf(" %c", &command);

		// 입련된 문자에 따라 다른 함수 실행
		switch (command) {
		case 'i': case 'I':
			data = getElement();   // 알파벳 입력받음
			enQueue(cQ, data);  // 큐에 추가
			break;
		case 'd': case 'D':
			deQueue(cQ, &data); // 큐에서 삭제
			break;
		case 'p': case 'P':
			printQ(cQ); // 큐 출력
			break;
		case 'b': case 'B':
			debugQ(cQ); // 디버깅
			break;
		case 'q': case 'Q':
			freeQueue(cQ);  // 큐 동적할당 해제
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); // 그 외 문자 입력시 출력 문구
			break;
		}

	} while (command != 'q' && command != 'Q');  // 종료 조건

	return 1;
}

// 함수 정의

// 큐 생성 함수
QueueType* createQueue()  
{
	QueueType* cQ;  // 구조체 포인터 cQ 선언
	cQ = (QueueType*)malloc(sizeof(QueueType));  // 구조체 하나 만큼 동적할당 받음
	// 그 구조체 안 front와 rear 값 0으로 초기화
	cQ->front = 0;  
	cQ->rear = 0;
	return cQ;  // 그 구조체의 주소값 리턴
}

// 큐 동적할당 해제 함수
int freeQueue(QueueType* cQ)
{
	if (cQ == NULL)  // cQ가 가리키는 곳이 비어있다면 종료
		return 1;

	free(cQ);  // 아니라면 cQ가 가리키는 곳의 동적할당해제
	return 1;
}

// 데이터 입력받는 함수
element getElement()
{
	element item;  
	printf("Input element = "); 
	scanf(" %c", &item);   // 원형큐에 넣을 값 입력받아서
	return item;   // 그 값을 리턴
}


/* complete the function */
// 원형 큐가 비었는지 확인하는 함수
int isEmpty(QueueType* cQ)  //비었는지 확인
{
	return(cQ->front == cQ->rear);  // 원형큐의 front값과 rear값이 같을 때 비어있다면 1리턴

}

/* complete the function */
// 원형 큐가 꽉 찼는지 확인하는 함수
int isFull(QueueType* cQ)  // 원형 큐가 꽉 찼는지 확인
{
	// 원형큐의 rear을 하나 증가한 값(원형큐를 한바퀴 돌고 돌아옴)에 큐 크기를 나눈 나머지가 front와 같다면 꽉 차있다면 1반환
	return((cQ->rear + 1) % MAX_QUEUE_SIZE == cQ->front);

}


/* complete the function */
// 큐에 데이터 추가하는 함수
void enQueue(QueueType* cQ, element item) 
{
	if (isFull(cQ)) {		// 원형 큐가 꽉 찼는지 확인하고 꽉 찼다면 문구 출력
		printf("Queue is full.\n");
        return;
	}
    cQ->queue[cQ->rear] = item;  // 큐의 rear 위치에 item 저장
	cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear를 다음 위치로 이동

}

/* complete the function */
//큐에서 데이터 삭제하는 함수
void deQueue(QueueType* cQ, element* item) 
{
	if (isEmpty(cQ)) {  // 큐가 비어있는지 확인하고 비어있으면 문구 출력
		printf("Queue is empty.\n");
        return;
	}
    *item = cQ->queue[cQ->front];    // 현재 front 위치의 요소를 item에 저장
     cQ->front = (cQ->front+1)%MAX_QUEUE_SIZE; // front 다음 위치로 이동
	
}

// 큐 출력 함수
void printQ(QueueType* cQ)
{
	int i, first, last; // 정수형 변수 선언

    // 나머지 대입. 각각 front와 rear의 다음 위치를 이용한다
	first = (cQ->front+1) % MAX_QUEUE_SIZE;  
	last = (cQ->rear+1) % MAX_QUEUE_SIZE;

	printf("Circular Queue : [");

	i = first;  // i에 first 값 대입

    // front부터 rear까지 출력          
	while (i != last) {
		printf("%3c", cQ->queue[i]);
		i = (i + 1) % MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}

// 디버깅 함수
void debugQ(QueueType* cQ)
{

	printf("\n---DEBUG\n");

    // 0~3까지 큐의 요소 출력
	for (int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
        // front위치 출력
		if (i == cQ->front) {
			printf("  [%d] = front\n", i);
			continue;
		}
        // 나머지 값들 출력
		printf("  [%d] = %c\n", i, cQ->queue[i]);

	}
    // front와 rear 출력
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);
}


