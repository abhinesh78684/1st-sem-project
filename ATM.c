#include<stdio.h>
#include<stdlib.h>
#include<time.h>
struct Balance
{
	int bal;
	char dateNtime[];
};
struct Accounts
{
	int accNo;
	char name[50];
	int pin[4];//4 unique numbers
	long int balance;//only takes data to billion
	char accType[50];
	struct Balance transaction[5];

} cmp;
int flag=0,cash=0;
void  Authenticaton()
{
	FILE *ptr=fopen("acc_details.dat","r");
	int accnum,pinnum[4],retry=0,i;
Auth:
	printf("\nEnter your Account number:");
	scanf("%d",&accnum);
	printf("\nEnter your PIN number:");
	for (i=0; i<4; i++)
	{
		scanf("%d",&pinnum);
	}
	while (ptr!=EOF)
	{
		fread(&cmp,sizeof(struct Accounts),1,ptr);
		if (cmp.accNo==accnum)
		{
			for (i=0; i<4; i++)
			{
				if(pinnum[i]==cmp.pin[i])
				{
					flag=flag+1;
				}
			}
		}
	}
	if (flag<4 && retry<5)
	{
		printf("\nIncorrect PIN or account number please try again.");
		retry++;
		goto Auth;
	}
	else if (flag<4 && retry==5)
	{
		printf("\nYour choice of input has failed to access the account multiple times. So we have proceeded to end the application.");
		fclose(ptr);
		exit(0);
	}
	else if (flag==4)
	{
		printf("\nWelcome");
	}
	flag=0;
	fclose(ptr);
}
void Withdraw()
{
	Authenticaton();
WD:
	printf("\nEnter amount in Nrs that you want to withdraw:");
	scanf("%ld",&cash);
	if (cash<=50000 && cash>0)
	{
		if (cmp.balance>=cash)
		{
			cmp.balance=cmp.balance-cash;//amend statements are missing
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
}
void Deposit()
{
	Authenticaton();
DP:
	printf("\nEnter the amount you want to deposit to your account:");
	scanf("%ld",&cash);
	if (cash<=50000 && cash>0)
	{
		cmp.balance=cmp.balance+cash;//amend statements are missing
	}
	else
	{
		
		printf("\nYou are trying to deposit more money than what is permitted by the bank please reconsider a valid amount.");
		getch();
		system("cls");
		goto DP;
	}
}
void BalanceInquiry()
{
	Authenticaton();

}
void NewID()
{

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
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------------");
	printf("\na. Withdraw Cash");
	printf("\nb. Deposit Cash");
	printf("\nc. Balance Inquiry");
	printf("\nd. Balance Inquiry");
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------------");
	printf("\nProceed to select an option: ");
	fflush(stdin);
	scanf("%c",&choice);
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