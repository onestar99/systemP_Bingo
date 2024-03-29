#include <iostream>
#include <cstdlib>
#include <time.h>
#include <iomanip>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include<stdio.h>
#include <stdlib.h>
#include<string.h>//stdio랑 string은 strlen쓰기위함.
#include<sys/wait.h>
#include <stdio_ext.h> //버퍼비우기
#define WORD_MAX 1024
#define WORD_SIZE 31 //제한글자개수*3+1 (한글자당 3바이트에 마지막 '\0')
using namespace std;

// 건들지마시오
char word[WORD_MAX][WORD_SIZE]; //사용자 단어 저장 WORD_MAX: 단어 개수 제한 WORD_SIZE: 단어 바이트 제한
char c_word[WORD_MAX][WORD_SIZE]; //컴퓨터 단어 저장 WORD_MAX: 단어 개수 제한 WORD_SIZE: 단어 바이트 제한
char words[WORD_MAX][WORD_SIZE];
// 건들지마시오


class Bingo
{
private:

public:
	int nStatus[5][5]; // bingoNum[5][5]의 숫자가 불렸는지를 체크하는 배열
	int WBingoCount[5]; // 가로줄 빙고 확인
	int HBingoCount[5]; // 세로줄 빙고 확인
	int RBingoCount; // 오른쪽 대각선 확인
	int LBingoCount; // 왼쪽 대각선 확인
	Bingo() {// 이 생성자에서는 bingoNum[5][5] 및 그 이외의 변수를 초기화한다.

		int i, j;
		int x;

		for (i = 0; i < 5; i++) { // 1차배열을 2차 배열로 만들기
			for (j = 0; j < 5; j++) {
				nStatus[i][j] = 1;
			}
		}
		// -------------------------------초기화
		for (i = 0; i < 5; i++) { // 배열 초기화
			WBingoCount[i] = 0;
			HBingoCount[i] = 0;
		}
		RBingoCount = 0;
		LBingoCount = 0;

	}

	int checkBingo() // 이 함수는 nStatus[5][5]를 이용하여 빙고를 부를 수 있는지 검사한다.
					 // 빙고이면 return 1, 아니면 return 0
	{
		int i, j;
		int Wcount = 0;
		int Hcount = 0;
		int RCcount = 0;
		int LCoount = 0;
		int bingo = 0;

		for (i = 0; i < 5; i++) { // 가로,세로줄 빙고 체크
			Wcount = 0;
			Hcount = 0;
			for (j = 0; j < 5; j++) {
				if (nStatus[i][j] == 0) // 가로 체크
				{
					Wcount++;
				}
				if (nStatus[j][i] == 0) // 세로 체크
				{
					Hcount++;
				}
				if (Wcount == 5)
				{
					bingo++;
				}
				if (Hcount == 5)
				{
					bingo++;
				}
			}
		}

		for (j = 4, i = 0; j >= 0, i < 5; j--, i++) {
			if (nStatus[j][j] == 0) // 왼쪽 대각선 체크
			{
				LCoount++;
			}
			if (nStatus[i][j] == 0) // 오른쪽 대각선 체크
			{
				RCcount++;
			}
			if (LCoount == 5)
			{
				bingo++;
			}
			if (RCcount == 5)
				bingo++;
		}
		if (bingo >= 3)
			return 1; // 빙고
		else
			return 0; // 게임 진행중

	}

	void printBingoGrid() { // 이 함수는 빙고판을 출력한다.

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				int k = strlen(word[(i * 5) + j]);
				if (nStatus[i][j] == 0) { //빙고이면

										  /*for (int t = 0; t < 31-k; t=t+3) {
										  if (t > 31-k)break;
										  cout << "  ";
										  }*/
					cout.width(23);
					cout.fill(' ');
					cout << " ★";
				}
				else {

					//cout << setw(3) << nStatus[i][j];
					for (int t = 0; t < 31 - k; t = t + 3) {
						if (t > 31 - k)break;
						cout << "  ";
					}
					if (k == 1)cout << " " << word[(i * 5) + j];
					else cout << word[(i * 5) + j];
				}
			}
			cout << "\n";
		}
		cout << "\n";

	}


	//컴퓨터의 빙고판 출력
	void printBingoGrid_com() { // 이 함수는 컴퓨터의 빙고판을 출력한다.

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				int k = strlen(c_word[(i * 5) + j]);
				if (nStatus[i][j] == 0) { //빙고이면

					for (int t = 0; t < 31 - k; t = t + 3) {
						if (t > 31 - k)break;
						cout << "  ";
					}
					if (k == 1)cout << " " << c_word[(i * 5) + j];
					else cout << c_word[(i * 5) + j];
				}
				else {

					/*for (int t = 0; t < 31-k; t=t+3) {
					if (t > 31-k)break;
					cout << "   ";
					}*/
					cout.width(22);
					cout.fill(' ');
					cout << "x";
				}
			}
			cout << "\n";
		}
		cout << "\n";
	}

	int c_callNumber(char aaa[WORD_SIZE]) {

		int i, j;

		for (i = 0; i < 5; i++) {
			for (j = 0; j < 5; j++) {
				if (nStatus[i][j] == 0 && strcmp(c_word[i * 5 + j], aaa) == 0) { // 중복 체크
					cout << "오류!! 중복됩니다" << endl;
					return 0;
				}
				if (strcmp(c_word[i * 5 + j], aaa) == 0) { // 문제 없음
					nStatus[i][j] = 0;
					return 1;
				}
			}
		}
		return 0;
	}
	int u_callNumber(char number[WORD_SIZE]) {

		int i, j;

		for (i = 0; i < 5; i++) {
			for (j = 0; j < 5; j++) {
				if (nStatus[i][j] == 0 && strcmp(word[i * 5 + j], number) == 0) { // 중복 체크
					cout << "오류!! 중복됩니다" << endl;
					return 0;
				}
				if (strcmp(word[5 * i + j], number) == 0) { // 문제 없음
					nStatus[i][j] = 0;
					return 1;
				}
			}
		}

		cout << "사용자의 빙고판에 없는 단어/숫자입니다!" << endl;
		return 0;
	}

	int now_sero = -1;
	int now_garo = -1;
	int AI() // 컴퓨터의 [5][5]배열을 통해서 가로, 세로, 대각선의
			 // 0개수를 파악하여 0이 가장 많은 줄에 0을 넣게하는 알고리즘.
	{
		int i, j;
		int zerocount[4] = { 0 }; // 가장 큰 가로 세로 외쪽 오른쪽 배열에 넣기
		int Wzerocount[5] = { 0 }; // 가로줄 0 개수 저장
		int Hzerocount[5] = { 0 }; // 가로줄 0 개수 저장
		int Wbigline = 0; // 가장 0이 많은 줄 저장
		int Hbigline = 0; // 가장 0이 많은 줄 저장
		int temp = 0;
		int bignum = -1; // 가로 세로 대각선중 가장 큰 쪽을 저장
		int result;
		int Wbigcount = 0; // 가로줄 중 0이 가장 많은 줄의 0갯수 저장
		int Hbigcount = 0; // 세로줄 중 0이 가장 많은 줄의 0갯수 저장
		int Rbigcount = 0; // 오른쪽 0 개수 저장
		int Lbigcount = 0; // 왼쪽 0 개수 저장
		int centercount = 0; // 중앙에 {11,12,13 21,22,23 31,32,33} 배열 0 숫자 카운트

		if (nStatus[2][2] == 0) {


			for (i = 1; i < 4; i++) // 중앙에 {11,12,13 21,22,23 31,32,33} 배열 0 숫자 카운트
			{
				for (j = 1; j < 4; j++) {
					if (nStatus[i][j] == 0) {
						centercount++;
					}


				}
			}
			if (centercount < 2) {
				for (i = 1; i < 4; i++) // 중앙 {111,12,13 21,22,23 31,32,33} 배열에 숫자를 넣는다
				{
					for (j = 1; j < 4; j++) {
						if (nStatus[i][j] != 0) {
							result = nStatus[i][j];
							now_sero = i;
							now_garo = j;
							nStatus[i][j] = 0;
							return result;
						}
					}
				}
			}

			for (i = 0; i < 5; i++)//가로 세로 0 체크
			{
				for (j = 0; j < 5; j++) {
					if (nStatus[i][j] == 0 && WBingoCount[i] == 0) {
						Wzerocount[i]++;
						if (Wzerocount[i] == 5)
						{
							WBingoCount[i]++; // 만약 가로줄 중에 빙고가 된다면 다시 못들어오게한다.
							Wzerocount[i] = 0; // 그리고 가로줄 라인은 갯수를 0으로 만든다.
						}
					}
				}
				if (nStatus[j][i] == 0 && HBingoCount[i] == 0) {
					Hzerocount[i]++;
					if (Hzerocount[i] == 5) {
						HBingoCount[i]++; // 만약 세로줄 줄에 빙고가 된다면 다시 못들어오게한다.
						Hzerocount[i] = 0; // 그리고 세로줄라인은 갯수를 0으로 만든다.
					}
				}
			}

			Wbigcount = Wzerocount[0]; // 처음 가로줄을 가장큰수로 잡는다.
			Hbigcount = Hzerocount[0]; // 처음 세로줄을 가장큰수로 잡는다.

			for (i = 0; i < 5; i++) // 가장 많이 0이된 줄을 찾는다.
			{
				if (Wbigcount < Wzerocount[i]) {
					Wbigcount = Wzerocount[i];
					Wbigline = i;
				}
				if (Hbigcount < Hzerocount[i]) {
					Hbigcount = Hzerocount[i];
					Hbigline = i;
				}
			}

			zerocount[0] = Wbigcount; // 가로 세로 대각선을 비교하기 위해서
			zerocount[1] = Hbigcount; // 가로 세로를 저장한다.

									  /*---------------------------여기서 부터 대각선--------*/

			for (i = 4, j = 0; i >= 0, j < 5; i--, j++) {
				if (nStatus[j][j] == 0 && LBingoCount == 0) // 왼쪽 대각선
				{
					Lbigcount++;
					if (Lbigcount == 5) {
						LBingoCount++;
						Lbigcount = 0;
					}
				}
				if (nStatus[i][j] == 0 && RBingoCount == 0) // 오른쪽 대각선
				{
					Rbigcount++;
					if (Rbigcount == 5)
					{
						RBingoCount++;
						Rbigcount = 0;
					}
				}
			}

			zerocount[2] = Lbigcount; // 왼쪽 대각선의 0개 개수를 저장한다.
			zerocount[3] = Rbigcount; // 오른쪽 대각선의 0개 개수를 저장한다.

									  //---------------------------------------------------------------
									  //zerocount[0]는 가로줄의 0개수
									  //zerocount[1]는 세로줄의 0개수
									  //zerocount[2]는 왼쪽 대각선의 0개수
									  //zerocount[3]는 오른쪽 대각선의 0개수
			temp = zerocount[0];
			for (i = 0; i < 4; i++) {
				if (zerocount[i] >= temp) {
					temp = zerocount[i]; // 가로 세로 왼쪽 오른쪽 중에 가장 0이 많은곳을 찾는다.
					bignum = i; // 이때 가장 큰곳을 bignum에 저장한다.
				}
			}

			if (bignum == 0) // 가로에 0이 없는 곳에 넣는다.
			{
				for (j = 0; j < 5; j++) {
					if (nStatus[Wbigline][j] != 0) {
						result = nStatus[Wbigline][j];
						now_sero = Wbigline;
						now_garo = j;
						nStatus[Wbigline][j] = 0;
						return result;
					}
				}
			}
			else if (bignum == 1) // 세로에 0이 없는 곳에 넣는다.
			{
				for (i = 0; i < 5; i++) {
					if (nStatus[i][Hbigline] != 0) {
						result = nStatus[i][Hbigline];
						nStatus[i][Hbigline] = 0;
						now_sero = i;
						now_garo = Hbigline;
						return result;
					}
				}
			}
			else if (bignum == 2) // 왼쪽 대각선에 0이 없는 곳에 넣는다.
			{
				for (i = 0; i < 5; i++) {
					if (nStatus[i][i] != 0) {
						result = nStatus[i][i];
						nStatus[i][i] = 0;
						now_sero = i;
						now_garo = i;
						return result;
					}
				}
			}
			else if (bignum == 3) // 오른쪽 대각선에 0이 없는곳에 넣는다.
			{
				for (i = 0, j = 4; i < 5, j >= 0; i++, j--) {
					if (nStatus[i][j] != 0) {
						result = nStatus[i][j];
						nStatus[i][j] = 0;
						now_sero = i;
						now_garo = j;
						return result;
					}
				}
			}

		}
		else
		{
			result = nStatus[2][2];
			nStatus[2][2] = 0; // 첫번째로 nStatus[2][2]에 0이 없으면 0을 넣는다.
			now_sero = 2;
			now_garo = 2;
			return result;
		}

	}
};


//----------------------------------- 여기까지 Class Bingo  ------------------------------------------------------------


//한 줄 씩 읽기
int readline(int fd, char* buf, int nbytes) {
	int numread = 0;
	int returnval;

	while (numread < nbytes - 1) {
		returnval = read(fd, buf + numread, 1);
		if ((returnval == -1) && (errno == EINTR))
			continue;
		if ((returnval == 0) && (numread == 0))
			return 0;
		if (returnval == 0)
			break;
		if (returnval == -1)
			return -1;
		numread++;
		if (buf[numread - 1] == '\n') { //! -1 없애보기
			buf[numread - 1] = '\0';
			return numread;
		}
	}
	errno = EINVAL;
	return -1;
}

//단어 쓰기
int write_word(int fd, const char* buf, size_t nbytes) {
	size_t write_b = nbytes; //써야할 바이트 수
	ssize_t written = 0; //write호출마다 쓴 바이트 수
	size_t numwrite = 0; //실제로 쓴 바이트 수

	while (write_b > 0) {
		written = write(fd, buf, write_b);
		if (written == -1) {
			if (errno != EINTR) {
				return -1;
			}
			else {
				written = 0;
				continue;
			}
		}
		buf += written;
		numwrite += written;
		write_b -= written;
	}
	//마지막에 \n추가
	char nc[1] = { '\n' };
	ssize_t plus_null = write(fd, nc, 1);
	if (plus_null == -1) {
		perror("newline 추가 오류.");
		return -1;
	}
	else
		return numwrite;
}

//테마 선택 함수

int fd; //file descriptor
int word_count = 0;//단어 몇 개인지 카운트


int Select_Thema() {
	srand((unsigned)time(NULL));
	//word초기화
	for (int i = 0; i < WORD_MAX; i++) {
		words[i][0] = '\0';
	}
	//테마 번호 입력
	int thema = 0;
	while (1) {
		cout << "테마를 선택하세요 (1. 음식 2. 동물 3. 숫자 4. 나라이름) : ";
		cin >> thema;
		if (thema >= 1 && thema <= 4)
			break;
		else
			cout << "1에서 4의 숫자를 입력해주세요." << endl;

	}
	//테마 별로 오픈
	switch (thema) {
	case 1:
		//r_food();
		fd = open("./food.txt", O_RDONLY);
		break;
	case 2:
		//r_animal();
		fd = open("./animal.txt", O_RDONLY);
		break;
	case 3:
		//r_number();
		fd = open("./number.txt", O_RDONLY);
		break;
	case 4:
		//r_country();
		fd = open("./country.txt", O_RDONLY);
		break;
	}

	int numread;
	//char words[1024][31];
	word_count = 0;
	//한 줄 씩 저장된 단어들을 word 배열에 하나씩 넣기
	while (1) {
		numread = readline(fd, words[word_count], WORD_SIZE); //일단 words배열에 모든 단어들 다 넣어두기
		if (numread == -1) {
			perror("읽는데 에러 발생");
		}
		if (numread == 0)
			break;
		word_count++;
	}

	int r_arr1[25] = { -1 }, r_arr2[25] = { -1 }; //중복 제거용 배열
	for (int i = 0; i < 25; i++) {
		int ran1 = rand() % (word_count);
		int ran2 = rand() % (word_count);

		if (r_arr1[0] > -1 && r_arr2[0] > -1)
		{
			while (1) {
				ran1 = rand() % (word_count);
				bool checker = 1;

				for (int j = 0; j < i; j++) {
					if (r_arr1[j] == ran1) {

						checker = 0;
						break;
					}
				}
				if (checker == 1)break;

			}
			while (1) {
				ran2 = rand() % (word_count);
				bool checker2 = 1;
				for (int j = 0; j < i; j++) {
					if (r_arr2[j] == ran2)
					{
						checker2 = 0;
						break;
					}
				}
				if (checker2 == 1)break;
			}
		}
		r_arr1[i] = ran1;
		r_arr2[i] = ran2;
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			strcpy(word[(i * 5) + j], words[r_arr1[(i * 5) + j]]);
			strcpy(c_word[(i * 5) + j], words[r_arr2[(i * 5) + j]]);
		}
	}

	//close file
	int retval;
	while (retval = close(fd), retval == -1 && errno == EINTR);
	if (retval == -1)
		perror("close에러 발생");
	fd = -1;//파일 닫았으니까

	return thema;

}

int is_alarmed = 1; //제한시간이 다 되면 0으로 

void myalarm(int sig) {
	cout << endl;
	cout << "Time out!!  컴퓨터의 턴" << endl;
	is_alarmed = 0;
};

//실제 게임 진행 함수

void vscomputer() {
	srand((unsigned)time(NULL));

	Bingo user;
	Bingo com;

	int x;
	string ch;
	char num[WORD_SIZE];
	int a;
	int y;

	while (1) {

		__fpurge(stdin);//버퍼 비우기
						//fflush(stdin); // 버퍼를 비워준다.//리눅스에서는 안됩니당...
		cin.clear();

		printf("사용자 빙고판\n");
		user.printBingoGrid();
		printf("컴퓨터 빙고판\n");
		com.printBingoGrid_com();

		//nStatus를 출력하고 확인해서 빙고확인용 배열과 화면에 뜨는 출력이 같은지/빙고상황인지 확인.
		//for (int i = 0; i < 5; i++) {
		//   for (int j = 0; j < 5; j++) {
		//      cout.width(2);
		//      cout.fill(' ');
		//      cout << user.nStatus[i][j] << " ";
		//   }cout << endl;
		//}cout << endl;
		//for (int i = 0; i < 5; i++) {
		//   for (int j = 0; j < 5; j++) {
		//      cout.width(2);
		//      cout.fill(' ');
		//      cout << com.nStatus[i][j] << " ";
		//   }cout << endl;
		//}cout << endl;

		cout << "빙고판의 문자를 입력하세요 : ";


		struct sigaction alarmed;//알람 시그널 핸들러 달기
		alarmed.sa_handler = myalarm;
		alarmed.sa_flags = 0;
		if (sigaction(SIGALRM, &alarmed, NULL) == -1)
			perror("Failed to install SIGALRM signal handler");
		int a = 100;
		is_alarmed = 1; //초기화

		a = alarm(7); //제한시간 후 컴퓨터 턴으로 //제한 시간 바꿀꺼면 여기 바꾸기

		while (1) {
			if (is_alarmed != 0) {
				cin >> ch; //입력 받기
				break;
			}
		}

		strcpy(num, ch.c_str());

		if (is_alarmed == 0) {
			strcpy(num, "time out");
		}

		cin.clear();
		//cout << "입력 확인용 출력 : " << num << endl;
		cout << "---------------------------------" << endl;

		if (is_alarmed != 0) { // time out X인 경우 
			if (user.u_callNumber(num) == 0) {
				continue;
			}
			com.c_callNumber(num);
		}

		is_alarmed == 1;//다 썼으니까 초기화



		if (user.checkBingo() == 1) // 사용자가 먼저 빙고를 부르면 승리!!를 체크
		{
			alarm(0); //alarm 제거해주기
			cout << "-------------------------------" << endl;
			cout << "빙고!! 승리하셨습니다!" << endl;
			user.printBingoGrid();
			cout << "-------------------------------" << endl;
			break;
		}
		if (com.checkBingo() == 1)
		{
			alarm(0);
			cout << "-------------------------------" << endl;
			cout << "빙고!! 패배하셨습니다!" << endl;
			com.printBingoGrid_com();
			cout << "-------------------------------" << endl;
			break;
		}

		////---------------------------------컴퓨터--------------------------------

		y = com.AI(); // 컴퓨터 실행;
		cout << "컴퓨터가 부른 문자는? : " << c_word[com.now_sero * 5 + com.now_garo] << endl;
		user.u_callNumber(c_word[com.now_sero * 5 + com.now_garo]);

		if (com.checkBingo() == 1) // 컴퓨터가 먼저 빙고를 부르면 승리!!를 체크
		{
			alarm(0);
			cout << "-------------------------------" << endl;
			cout << "컴퓨터 빙고!! 패배하셨습니다!" << endl;
			com.printBingoGrid_com();
			cout << "-------------------------------" << endl;
			break;
		}
		if (user.checkBingo() == 1)
		{
			alarm(0);
			cout << "-------------------------------" << endl;
			cout << "유저 빙고!! 승리하셨습니다!" << endl;
			user.printBingoGrid();
			cout << "-------------------------------" << endl;
			break;
		}

	}
}

//1번 메뉴(게임시작) 미완성
void num1() {
	int thema = Select_Thema();
	vscomputer();
}

//단어 추가(write)
void Add_word(int thema) {
	//@@@@@@@@@@@한글 3바이트@@@@@@@@@@
	//사용자에게 현재 저장 단어 출력하기
	cout << "현재 저장된 단어를 출력합니다." << endl;
	for (int i = 0; i < WORD_MAX; i++) {
		if (words[i][0] == '\0') {
			break;
		}
		else
			cout << i + 1 << ") " << words[i] << endl;
	}

	//테마 별로 오픈
	switch (thema) {
	case 1:
		//r_food();
		fd = open("./food.txt", O_WRONLY | O_APPEND);
		break;
	case 2:
		//r_animal();
		fd = open("./animal.txt", O_WRONLY | O_APPEND);
		break;
	case 3:
		//r_number();
		fd = open("./number.txt", O_WRONLY | O_APPEND);
		break;
	case 4:
		//r_country();
		fd = open("./country.txt", O_WRONLY | O_APPEND);
		break;
	}

	//단어 입력받기
	char add[WORD_SIZE] = { '\0' };
	cout << "해당 테마에 추가하고 싶은 단어를 입력하세요 : ";
	//버퍼 비우기
	cin.clear();
	while (cin.get() != '\n');
	//입력받기
	cin.getline(add, WORD_SIZE);
	size_t size = strlen(add);
	if (size > 0) {
		int iswrite = write_word(fd, add, size);
		if (iswrite == -1)
			perror("write 오류 발생");
		cout << "입력한 단어가 추가되었습니다." << endl;
	}
	else
		cout << "아무것도 입력하지 않아, 단어를 추가하지 않고 종료합니다." << endl;

	//close file
	int retval;
	while (retval = close(fd), retval == -1 && errno == EINTR);
	if (retval == -1)
		perror("close에러 발생");
	fd = -1;//파일 닫았으니까

}


pid_t childpid = 0;
int status;
int pid;//자식 프로세스 아이디

		//메뉴 선택
void Game_Start() {
	cout << "메뉴를 선택하세요. " << endl << endl;
	cout << "1. 게임 시작" << endl;
	cout << "2. 보기 추가 " << endl;
	cout << "3. 게임 종료 " << endl;
	cout << endl;
	int menu = 0;
	int i = 0;
	while (menu < 1 || menu >3) {
		if (i > 0) {
			cout << "1~3을 입력해주세요." << endl;
		}
		cin >> menu;
		i++;
		cin.clear();
		while (cin.get() != '\n');
	}
	cout << endl;
	switch (menu) {
	case 1:
		//cout << 1<< endl; fork
		childpid = fork();
		if (childpid < 0) { //fork 오류
			perror("Failed to fork");
			exit(0);
		}
		if (childpid == 0) { //자식 프로세스
			pid = getpid();//pid 에 현재 프로세스 아이디 저장
						   //cout << "자식 프로세스 시작" << endl;

			num1();

			while (1) {//exit시키기
				if (pid == getpid()) {
					exit(0);
				}
				else {
					perror("Failed to exit");
				}
			}

		}
		else { //부모 프로세스
			   //cout << "부모 프로세스 시작" << endl;
			waitpid(childpid, &status, 0);//자식이 끝날때까지 기다림
										  //cout << "부모 프로세스 종료" << endl;
		}
		break;
	case 2: { //보기 추가
		int thema = Select_Thema();
		Add_word(thema);
		break;
	}
	case 3:
		exit(0);
		break;
	}

}

int main()
{
	while (1) {
		Game_Start();
	}
}