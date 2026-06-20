#include<stdio.h>
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
	char accNo[9];
	char name[50];
	char pin[5];//4 unique numbers
	long int balance;//only takes data to billion
	char accType[50];
	struct Balance transaction[10];
	int DataPoint; //stores pointer to overwrite data
} AccData;

long int cash;

void  Authenticaton()
{
	FILE* ReadPtr=fopen("AccountRecords.txt","rb");
	if (ReadPtr==NULL)
	{
		printf("\nFile not created");
	}
	int retry=0;
	char accnum[8],pinnum[4];
Auth:
	printf("\nEnter your Account number: ");
	fflush(stdin);
	gets(pinnum);
	printf("\nEnter your PIN number: ");
	fgets(pinnum,sizeof(pinnum),stdin);
	while (fread(&AccData,sizeof(struct Accounts),1,ReadPtr)==1)//need to be checked if it accepts /0 or EOF or NULL
	{
		if (strcmp(AccData.accNo,accnum)!=0||strcmp(AccData.pin,pinnum)!=0)
		{
			retry++;
			if (retry<5)
			{
				printf("\nInvalid account number or PIN number please try again");
				getch();
				system("cls");
				goto Auth;
			}
			else if (retry==5)
			{
			printf("\nYour choice of input has failed to access the account multiple times. So we have proceeded to end the application.");
			fclose(ReadPtr);
			exit(0);
			}
		}
		else if(strcmp(AccData.accNo,accnum)==0 && strcmp(AccData.pin,pinnum)==0)
		{
			printf("\nWelcome");
			fclose(ReadPtr);
			break;
		}
	}
}

void Amend(int Inflow,int Outflow)
{
	FILE *AmendPtr=fopen("AccountRecords.txt","rb+");
	struct Accounts Amd;
	while (fread(&Amd,sizeof(struct Accounts),1,AmendPtr)==1)
	{
		if (strcmp(AccData.accNo,Amd.accNo)==0)
		{
			Amd=AccData;
			Amd.transaction[AccData.DataPoint].bal[0]=Inflow;
			Amd.transaction[AccData.DataPoint].bal[1]=Outflow;
			strcpy(Amd.transaction[AccData.DataPoint].date,__DATE__);
			strcpy(Amd.transaction[AccData.DataPoint].time,__TIME__);
			Amd.DataPoint++;
			if (Amd.DataPoint==10)
			{
				Amd.DataPoint=0;
			}
		}
		fwrite(&Amd,sizeof(Amd),1,AmendPtr);
	}
	fclose(AmendPtr);
}

void Withdraw()
{
	Authenticaton();
WD:
	printf("\nEnter amount in Nrs that you want to withdraw:");
	scanf("%ld",&cash);
	if (cash<=50000 && cash>0)
	{
		if (AccData.balance>=cash)
		{
			AccData.balance=AccData.balance-cash;//amend statements are missing
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
		printf("\nYou are trying to withdraw more money than what is permitted by the bank please reconsider a valid amount.");
		getch();
		system("cls");
		goto WD;
	}
	cash=0;
}

void Deposit()
{
	Authenticaton();
DP:
	printf("\nEnter the amount you want to deposit to your account:");
	scanf("%ld",&cash);
	if (cash<=50000 && cash>0)
	{
		printf("\nNrs %ld has been deposited in your acc %ld",cash,AccData.accNo);
		AccData.balance=AccData.balance+cash;//amend statements are missing
		Amend(cash,0);
	}
	else
	{
		printf("\nYou are trying to deposit more money than what is permitted by the bank please reconsider a valid amount.");
		system("cls");
		goto DP;
	}
	cash=0;
}

void BalanceInquiry()
{
	int i=0;
	Authenticaton();
	printf("\n                                   BALANCE INQUIRY");
	printf("\n_____________________________________________________________________________________________");
	printf("\nDate                     |Time                        |Credit                 |Debit         ");
	for (i=0;i<10;i++)
	{
		if (AccData.transaction[i].bal[1]==0 && AccData.transaction[i].bal[0]==0)
		{
			break;
		}
		else if(AccData.transaction[i].bal[1]==0 && AccData.transaction[i].bal[0]!=0)
		{
			printf("%s     %s     %d             -",AccData.transaction[i].date,AccData.transaction[i].time,AccData.transaction[i].bal[0]);
		}
		else if(AccData.transaction[i].bal[0]==0 && AccData.transaction[i].bal[1]!=0)
		{
			printf("%s     %s        -             %d",AccData.transaction[i].date,AccData.transaction[i].time,AccData.transaction[i].bal[1]);
		}
	}
	printf("\n                                                     Total balance is:%ld",AccData.balance);
	getch();
}

void NewID()
{
	
	int i,j,min=10000000,max=99999999,num=0;
	srand(time(NULL));
	FILE *NewIDPtr=fopen("AccountRecords.bin","ab");
	if (NewIDPtr==NULL)
	{
		printf("file not created");
	}
	char choice;
	printf("\nEnter you Name: ");
	fflush(stdin);
	gets(AccData.name);
	PIN:
	printf("\nEnter a unique pin to your account: ");
	fflush(stdin);
	gets(AccData.pin);
    for (i=0;i<4;i++)
    {
    	for (j=i+1;j<4;j++)
    	{
    		if (AccData.pin[i]==AccData.pin[j])
			{
				printf("\nThe pin is not unique enough please try again.");
				goto PIN;
			}		
		}
	}
	AccData.balance=1000;
NewIDChoice:
	printf("\nIs there any addition to opening balance than what is required?(y/n)");
	fflush(stdin);
	gets(&choice);
	strupr(&choice);
	if (choice=='Y')
	{
		printf("\nEnter additional amount: ");
		scanf("%ld",&cash);
		AccData.balance+=cash;
	}
	else if (choice=='N')
	{
		printf("\nYour opening balance has been set");
	}
	else
	{
		printf("\nInvalid choice please try again.");
		goto NewIDChoice;
	}
	Amend(AccData.balance,0);
	num=(rand()%(max-min+1))+min;
	snprintf(AccData.accNo,sizeof(AccData.accNo),"%d",num);//unique generation for acc no. needs to be put in place of 8 digits
	printf("\nYour Account Number is: %s with PIN number: %s",AccData.accNo,AccData.pin);
	printf("\n%s",AccData.name);
	AccType:
	printf("\nChoose a account type:\na.Fixed Account\nb.Current Account\nc.Savings Account\n");	
	fflush(stdin);
	gets(&choice);
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
	AccData.DataPoint=1;
	fwrite(&AccData,sizeof(AccData),1,NewIDPtr);
	cash=0;
	fclose(NewIDPtr);
}

int main()
{
	char choice;
Startup:
	fflush(stdin);
	printf("\n\t\t\t\t\t\tWelcome To KIST Sem1 Banking/ATM System");
	getch();
	system("cls");
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
			break;
		case 'b'://deposit
			Deposit();
			system("cls");
			break;
		case 'c'://balance inquiry
			BalanceInquiry();
			system("cls");
			break;
		case 'd'://new acc creation
			NewID();
			system("cls");
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