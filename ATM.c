#include<stdio.h>
#include<stdlib.h>
#include<time.h>
struct Balance
{
	int bal;
	char dateNtime[][5];
};
struct Accounts
{
	int accNo;
	char name[50];
	int pin;//4 unique numbers
	long long int balance;
	char accType[50];
	struct Balance transaction[];
} cmp;
int flag=0;
void  Authenticaton()
{
	FILE *ptr=fopen("acc_details.dat","r");
	int accnum,pinnum,retry=0;
Auth:
	printf("\nEnter your Account number:");
	scanf("%d",&accnum);
	printf("\nEnter your PIN number:");
	scanf("%d",&pinnum);
	while (ptr!=EOF)
	{
		fread(&cmp,sizeof(struct Accounts),1,ptr);
		if (cmp.accNo==accnum && pinnum==cmp.pin)
		{
			flag=1;
			break;
		}
	}
	if (flag==0 && retry>5)
	{
		printf("\nIncorrect PIN or account number please try again.");
		retry++;
		goto Auth;
	}
	else if (flag==0 && retry>5)
	{
		printf("\nYour choice of input has failed to access the account multiple times. So we have proceeded to end the application.");
	}
	fclose(ptr);
}
void Withdraw()
{
	Authenticaton();

}
void Deposit()
{
	Authenticaton();

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
	printf("\t\t\t\t\t\tWelcome To KIST Sem1 Banking/ATM System");
	printf("----------------------------------------------------------------------------------------------------------------------------------------------");
	printf("\na. Withdraw Cash");
	printf("\nb. Deposit Cash");
	printf("\nc. Balance Inquiry");
	printf("\nd. Balance Inquiry");
	printf("----------------------------------------------------------------------------------------------------------------------------------------------");
	printf("\nProceed to select an option: ");
	fflush(stdin);
	gets(choice);
	/*AVOID THIS!!!
	time_t now = time(NULL);
	char *string =ctime(&now);
	printf("%s\n",string);*/
	switch(choice)
	{

		case 'a'://withdraw

			goto	STARTUP;
			break;
		case 'b'://deposit

			goto	STARTUP;
			break;
		case 'c'://balance inquiry

			goto	STARTUP;
			break;
		case 'd'://new acc creation

			goto	STARTUP;
			break;
		default:
			printf("\nGiven input is out of range, please select option from range (a to d).");
	}
	return 0;
}