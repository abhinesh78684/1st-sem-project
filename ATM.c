#include<stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<string.h>

struct Balance
{
	long int bal[2];//0 stores deposit, 1 stores withdraw
	char date[50];
	char time[50];
};

struct Accounts
{
	char accNo[17];//16 unique numbers
	char name[50];
	char pin[5];//4 unique numbers
	long int balance;//only takes data to billion
	char accType[50];
	struct Balance transaction[10];
	int DataPoint; //stores pointer to overwrite data
	int Active;//1 means account is active, 0 means account is closed or frozen
} AccData;

long int cash;

void Authenticaton()
{
	FILE *ReadPtr=fopen("AccountRecords.bin","rb");
	if (ReadPtr==NULL)
	{
		printf("\nFile not readable");
		exit(0);
	}
	int retry=0,i,found;
	char accnum[17],pinnum[5];
Auth:
	printf("\nEnter your Account number: ");
	fflush(stdin);
	fgets(accnum,17,stdin);
	accnum[strcspn(accnum, "\n")] = '\0';
	if(strlen(accnum) != 16)
	{
		printf("\nAccount number must be exactly 16 digits.\n");
		goto Auth;
	}
	for(i = 0; i < (int)strlen(accnum); i++)
	{
		if(!(accnum[i] >= '0' && accnum[i] <= '9'))
		{
			printf("\nAccount number must contain only digits (0-9).\n");
			goto Auth;
		}
	}
PINAuth:
	printf("\nEnter your PIN number: ");
	fflush(stdin);
	fgets(pinnum,5,stdin);
	pinnum[strcspn(pinnum, "\n")] = '\0';
	if(strlen(pinnum) != 4)
	{
		printf("\nPIN must be exactly 4 digits.\n");
		goto PINAuth;
	}
	for(i = 0; i < (int)strlen(pinnum); i++)
	{
		if(!(pinnum[i] >= '0' && pinnum[i] <= '9'))
		{
			printf("\nPIN must contain only digits (0-9).\n");
			goto PINAuth;
		}
	}
	found=0;
	fseek(ReadPtr,0, SEEK_SET);
	while (fread(&AccData,sizeof(struct Accounts),1,ReadPtr)==1)
	{
		if (strcmp(AccData.accNo,accnum)==0 && strcmp(AccData.pin,pinnum)==0)
		{
			found=1;
			break;
		}
	}
	if (found)
	{
		if (AccData.Active==0)
		{
			printf("Your account is inaccessible at this point of time. Please contact the bank for further information or help.");
			getch();
			fclose(ReadPtr);
			exit(0);
		}
		printf("\nWelcome");
		fclose(ReadPtr);
	}
	else
	{
		retry=retry+1;
		if (retry<4)
		{
			printf("\nInvalid account number or PIN number please try again");
			printf("\ntries remaining:%d",5-retry);
			getch();
			system("cls");
			goto Auth;
		}
		else
		{
			printf("\nYour choice of input has failed to access the account multiple times. So we have proceeded to end the application.");
			fclose(ReadPtr);
			exit(0);
		}
	}
}
void Amend(long int Inflow,long int Outflow)
{
	struct Accounts Temp;
	FILE *AmendPtr=fopen("AccountRecords.bin","rb+");
	if (AmendPtr==NULL)
	{
		printf("\nFile not opened.");
	}
	while (fread(&Temp,sizeof(struct Accounts),1,AmendPtr)==1)
	{
		if (strcmp(AccData.accNo,Temp.accNo)==0)
		{
			AccData.transaction[AccData.DataPoint].bal[0]=Inflow;
			AccData.transaction[AccData.DataPoint].bal[1]=Outflow;
			strcpy(AccData.transaction[AccData.DataPoint].date,__DATE__);
			strcpy(AccData.transaction[AccData.DataPoint].time,__TIME__);
			AccData.DataPoint++;
			if (AccData.DataPoint==10)
			{
				AccData.DataPoint=0;
			}
			fseek(AmendPtr,-(long)sizeof(struct Accounts), SEEK_CUR);//changing negative unsigned number to signed number using long
			fwrite(&AccData,sizeof(struct Accounts),1,AmendPtr);
			break;
		}
	}
	fclose(AmendPtr);
}

void Withdraw()
{
	system("cls");
	Authenticaton();
WD:
	printf("\nEnter amount in Nrs that you want to withdraw:");
	scanf("%ld",&cash);
	if (cash<=50000 && cash>0)
	{
		if (AccData.balance>=cash)
		{
			printf("\nNrs %ld has been withdrawn from your acc %s",cash,AccData.accNo);
			AccData.balance=AccData.balance-cash;
			Amend(0,cash);
		}
		else
		{
			printf("\nInsufficient balance please try again after confirming your balance.");
			getch();
			system("cls");
			goto WD;
		}
	}
	else
	{
		printf("\nYou are trying to withdraw more money than what is permitted by the bank  or entering value 0 ornegative balance please reconsider a valid amount.");
		getch();
		system("cls");
		goto WD;
	}
	cash=0;
	getch();
}

void Deposit()
{
	system("cls");
	Authenticaton();
DP:
	printf("\nEnter the amount you want to deposit to your account:");
	scanf("%ld",&cash);
	if (cash<=50000 && cash>0)
	{
		printf("\nNrs %ld has been deposited in your acc %s",cash,AccData.accNo);
		AccData.balance=AccData.balance+cash;
		Amend(cash,0);
	}
	else
	{
		printf("\nYou are trying to deposit more money than what is permitted by the bank please reconsider a valid amount.");
		system("cls");
		goto DP;
	}
	cash=0;
	getch();
}

void BalanceInquiry()
{
	system("cls");
	int i=0;
	Authenticaton();
	printf("\n                                   BALANCE INQUIRY");
	printf("\n_____________________________________________________________________________________________");
	printf("\nDate                     |Time                        |Credit                 |Debit         ");
	for (i=0; i<10; i++)
	{
		if (AccData.transaction[i].bal[1]==0 && AccData.transaction[i].bal[0]==0)
		{
			break;
		}
		else
		{
			if(AccData.transaction[i].bal[1]==0 && AccData.transaction[i].bal[0]!=0)//deposit
			{
				printf("\n%s\t%s\t%ld\t\t-",AccData.transaction[i].date,AccData.transaction[i].time,AccData.transaction[i].bal[0]);
			}
			else if(AccData.transaction[i].bal[0]==0 && AccData.transaction[i].bal[1]!=0)//withdraw
			{
				printf("\n%s\t%s\t-\t\t%ld",AccData.transaction[i].date,AccData.transaction[i].time,AccData.transaction[i].bal[1]);
			}
		}
	}
	printf("\n                                                     Total balance is:%ld",AccData.balance);
	getch();
}

void NewID()
{
	system("cls");
	char ch;
	memset(&AccData, 0, sizeof(AccData));
	struct Accounts AccChk;
	long long int min=1111111111111111,max=9999999999999999,accnum;
	int i,j,flag;
	char choice;
	FILE *NewIDPtr=fopen("AccountRecords.bin","ab");
	FILE *AccNo=fopen("AccountRecords.bin","rb");
	if (NewIDPtr==NULL)
	{
		printf("file not opened");
		exit(0);
	}
AccNoLoop:
	accnum = min + (((long long)rand() * RAND_MAX + rand()) % (max - min + 1));//unique generation for acc no.
	snprintf(AccData.accNo,sizeof(AccData.accNo),"%lld",accnum);//acc no. generated above is integer in nature so we change it into string
	fseek(AccNo,0, SEEK_SET);
	while(fread(&AccChk,sizeof(struct Accounts),1,AccNo)==1)
	{
		if (strcmp(AccData.accNo,AccChk.accNo)==0)
		{
			goto AccNoLoop;
		}
	}
	do
	{
		flag = 1;
		printf("\nEnter your Name: ");
		fflush(stdin);
		fgets(AccData.name, sizeof(AccData.name), stdin);
		AccData.name[strcspn(AccData.name, "\n")] = '\0';
		if(strlen(AccData.name) == 0)
		{
			flag = 0;
		}
		// Validate characters
		for(i = 0; AccData.name[i] != '\0'; i++)
		{
			ch = AccData.name[i];
			if(!((ch >= 'A' && ch <= 'Z') ||
			        (ch >= 'a' && ch <= 'z') ||
			        (ch == ' ')))
			{
				flag = 0;
				break;
			}
		}
		if(flag == 0)
		{
			printf("\nInvalid Name!");
			printf("\nOnly alphabets and spaces are allowed.\n");
		}
	}
	while(flag == 0);
PIN:
	printf("\nEnter a 4-digit PIN: ");
	fflush(stdin);
	fgets(AccData.pin, sizeof(AccData.pin), stdin);
	AccData.pin[strcspn(AccData.pin, "\n")] = '\0';
	if(strlen(AccData.pin) != 4)
	{
		printf("\nPIN must be exactly 4 digits.\n");
		goto PIN;
	}
	for(i = 0; i < 4; i++)
	{
		if(AccData.pin[i] < '0' || AccData.pin[i] > '9')
		{
			printf("\nPIN must contain only digits (0-9).\n");
			goto PIN;
		}
	}
	// Check for duplicate digits (optional)
	for(i = 0; i < 4; i++)
	{
		for(j = i + 1; j < 4; j++)
		{
			if(AccData.pin[i] == AccData.pin[j])
			{
				printf("\nAll PIN digits must be unique.\n");
				goto PIN;
			}
		}
	}
AccType:
	printf("\nChoose a account type:\na.Fixed Account\nb.Current Account\nc.Savings Account\n");
	fflush(stdin);
	scanf("%c",&choice);
	strlwr(&choice);
	switch(choice)
	{
		case 'a':
			strcpy(AccData.accType,"Fixed Account");
			break;
		case 'b':
			strcpy(AccData.accType,"Current Account");
			break;
		case 'c':
			strcpy(AccData.accType,"Savings Account");
			break;
		default:
			printf("\nAccount type is out of range please choose again.");
			goto AccType;
			break;
	}
BalanceChoice:
	AccData.balance=1000;
	printf("\nIs there any addition to opening balance than what is required?(y/n)");
	fflush(stdin);
	scanf("%c",&choice);
	strlwr(&choice);
	if (choice=='y')
	{
		printf("\nEnter additional amount: ");
		scanf("%ld",&cash);
		strcpy(AccData.transaction[0].date,__DATE__);
		strcpy(AccData.transaction[0].time,__TIME__);
		AccData.transaction[0].bal[1]=0;
		AccData.transaction[0].bal[0]+=cash;
		AccData.balance+=cash;
		printf("\nYour opening balance with additional credits has been set");
	}
	else if (choice=='n')
	{
		printf("\nYour opening balance has been set");
		strcpy(AccData.transaction[0].date,__DATE__);
		strcpy(AccData.transaction[0].time,__TIME__);
		AccData.transaction[0].bal[1]=0;
		AccData.transaction[0].bal[0]+=cash;
	}
	else
	{
		printf("\nInvalid choice please try again.");
		goto BalanceChoice;
	}
	AccData.Active=1;
	printf("\n%s your Account Number is: %s with PIN number: %s",AccData.name,AccData.accNo,AccData.pin);
	fwrite(&AccData,sizeof(struct Accounts),1,NewIDPtr);
	cash=0;
	getch();
	fclose(NewIDPtr);
	fclose(AccNo);
}

int main()
{
	char choice;
	srand(time(NULL)^getpid());
	fflush(stdin);
	printf("\n\t\t\t\t\t\tWelcome To KIST Sem1 Banking/ATM System");
	getch();
Startup:
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------------");
	printf("\na. Withdraw Cash");
	printf("\nb. Deposit Cash");
	printf("\nc. Balance Inquiry");
	printf("\nd. New ID");
	printf("\ne. Exit");
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------------");
	printf("\nProceed to select an option\n");
	fflush(stdin);
	scanf("%c",&choice);
	strlwr(&choice);
	switch(choice)
	{
		case 'a'://withdraw
			Withdraw();
			system("cls");
			goto Startup;
			break;
		case 'b'://deposit
			Deposit();
			system("cls");
			goto Startup;
			break;
		case 'c'://balance inquiry
			BalanceInquiry();
			system("cls");
			goto Startup;
			break;
		case 'd'://new acc creation
			NewID();
			system("cls");
			goto Startup;
			break;
		case 'e'://exit case
			exit(0);
			break;
		default:
			printf("\nGiven input is out of range, please select option from range (a to e).");
			getch();
			system("cls");
			goto Startup;
			break;
	}
	return 0;
}