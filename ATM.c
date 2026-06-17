#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
struct Balance
{
	int bal;
	char dateNtime[];
};
struct Accounts
{
	int accNo[8];
	char name[50];
	int pin[4];//4 unique numbers
	long int balance;//only takes data to billion
	char accType[50];
	struct Balance transaction[5];
} AccData;
long int cash;
void  Authenticaton()
{
	FILE *readptr=fopen("acc_details.dat","rb");
	int accnum[8],pinnum[4],retry=0,pinflag=0,accflag=0,i,j;
Auth:
	printf("\nEnter your Account number(one at a time):");
	for (i=0; i<8; i++)
	{
		scanf("%d",&accnum[i]);
	}
	printf("\nEnter your PIN number(one at a time):");
	for (i=0; i<4; i++)
	{
		scanf("%d",&pinnum[i]);
	}
	while (fread(&AccData,sizeof(struct Accounts),1,readptr)!=EOF)//need to be checked if it accepts /0 or EOF or NULL
	{
		for (i=0; i>8 ; i++)//comparison logic is wrong
		{
			if (AccData.accNo[i]==accnum[i])
			{
			accflag++;
			}
		}
		if (accflag!=7)
		{
			accflag=0;
		}
		if (accflag==7)
		{
			while (fread(&AccData,sizeof(struct Accounts),1,readptr)!=EOF)//need to be checked if it accepts /0 or EOF or NULL
			{
				for(j=0;j<4;j++)//comparison logic is wrong
				{
					if (AccData.pin[i]==pinnum[i])
					{	
						pinflag++;
					}
				}
				pinflag=0;
			}
		}
	}
	//may need editing
	if ((pinflag<4||accflag<7) && retry<5)
	{
		printf("\nIncorrect PIN or account number please try again.");
		retry++;
		getch();
		system("cls");
		goto Auth;
	}
	else if ((pinflag<4||accflag<7) && retry==5)
	{
		printf("\nYour choice of input has failed to access the account multiple times. So we have proceeded to end the application.");
		fclose(readptr);
		exit(0);
	}
	else if (pinflag==4 && accflag==7)
	{
		printf("\nWelcome");
		fclose(readptr);
	}
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
		printf("Nrs %ld has been deposited in your acc %ld",cash,AccData.accNo);
		AccData.balance=AccData.balance+cash;//amend statements are missing
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
	Authenticaton();

}
void NewID()
{
	//file pointer needed to link the new accs to file
	char choice;
	printf("\nEnter you Name: ");
	gets(AccData.name);
	//unique generation for acc no. needs to be put in place of 8 digits
	printf("\nEnter a unique pin to your account: ");
	scanf("%d",AccData.pin);
	printf("\nEnter the account type: ");
	gets(AccData.accType);
	AccData.balance=1000;
NewIDChoice:
	printf("Is there any addition to opening balance than what is required?");
	fflush(stdin);
	gets(&choice);
	strupr(&choice);
	if (choice=='Y')
	{
		printf("\nEnter additional amount: ");
		scanf("%ld",&cash);
		AccData.balance+=cash;
		//time constraint needs to be added
		AccData.transaction[0].bal=cash;
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
	AccType:
	printf("\nChoose a account type:\na.Fixed Account\nb.Current Account\nc.Savings Account");	
	fflush(stdin);
	gets(&choice);
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
			printf("Account type is out of range please choose again.");
			goto AccType;
			break;
	}
	cash=0;
}


/*AVOID THIS!!
#include <stdio.h>
#include <time.h>

int main() {
    char date_string[100];
    time_t raw_time = time(NULL); // Get current epoch time
    struct tm *local_time = localtime(&raw_time); // Convert to local time components

    // Format: "YYYY-MM-DD HH:MM:SS"
    strftime(date_string, sizeof(date_string), "%Y-%m-%d %H:%M:%S", local_time);

    printf("Formatted Date/Time: %s\n", date_string);
    return 0;
}
*/
int main()
{
	FILE *ptr=fopen("C:\\codes\\nodeRepo.bin","wb");//need to change wb to rb+ later also create a folder in C drive named codes in your computer
	char choice;
Startup:
	printf("\n\t\t\t\t\t\tWelcome To KIST Sem1 Banking/ATM System");
	getch();
	system("cls");
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------------");
	printf("\na. Withdraw Cash");
	printf("\nb. Deposit Cash");
	printf("\nc. Balance Inquiry");
	printf("\nd. Balance Inquiry");
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------------");
	printf("\nProceed to select an option: ");
	fflush(stdin);
	scanf("%c",&choice);
	strlwr(&choice);
	/*AVOID THIS!!!
	time_t now = time(NULL);
	char *string =ctime(&now);
	printf("%s\n",string);*/
	switch(choice)
	{
		case 'a'://withdraw
			Withdraw();
			goto	Startup;
			break;
		case 'b'://deposit
			Deposit();
			goto	Startup;
			break;
		case 'c'://balance inquiry
			BalanceInquiry();
			goto	Startup;
			break;
		case 'd'://new acc creation
			NewID();
			goto	Startup;
			break;
		case 'e'://exit case
			exit(0);
		default:
			printf("\nGiven input is out of range, please select option from range (a to d).");
			getch();
			system("cls");
			goto Startup;
			break;
	}
	return 0;
}