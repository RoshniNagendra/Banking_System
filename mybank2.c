#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

//creating structure

#define ENCRYPT_KEY 26

struct BANK
{
	char name[100];
	char bank_name[100];
	int acc_no;
	float balance;
	char password[500];
	char type[3];
	//int loan;
	
};

typedef struct extract //to extract stuff.
{
	char ACC_NO[50];
	char BANK_NAME[100];
	char NAME[100];
	char PASSWORD[500];
	char BALANCE[500];
	char TYPE[3];
}ex;

/* functions for encrypt and decrypt */

void encrypt_string(char *source, char *target);
void decrypt_string(char *source, char *target);


void acc_create();
int EXIT();
int checkacc(int);
int checkpassword(char pass[],int );
int deleteacc(int);
void details(int);
void changepass(int,char pass[]);
void transactions(int,int);
void create_file(int);
void NEFT_transaction(int);

//main running

int main() //maybe u cud be like hey r u new here, and then ask for account cretaion, else proceed to other funcs after entering password.
{
	int cond = 1;
	while(cond)	
	{
		printf("\n\nHi there! This is a banking system.\n\n*******************************************************************************\n");
		printf("\nAre you new here?[Y/N]\n");
	
		char ch1,ch2;
		//ch1 = getch();
		scanf("%c",&ch1);
	
		if(ch1 == 'y'||ch1 == 'Y')
		{
			fflush(stdin);
			printf("\nWould you like to create an account? [Y/N]\n");
			scanf("%c",&ch2);
			if(ch2 == 'y'||ch2 == 'Y')
				acc_create();
			
			else if(ch2 == 'n'||ch2 == 'N')
				cond = EXIT();
			else{
				printf("\nInvalid choice, Enter again\n");printf("%d",cond);} //I need to loop it back to this.
		}
		else if(ch1 == 'n'||ch1 == 'N')	//first you gotta ask for password and then show options!
		{
			char pass[50];
			int acc;
			printf("\nEnter your account number\n");
			scanf("%d",&acc);
			if(checkacc(acc)==1)
			{
				printf("\nEnter your password:\n");
				scanf("%s",pass);
				if(checkpassword(pass,acc)==1)
				{
				
					printf("\nChoose an option:\n\n1)Delete account\n2)Withdraw money\n3)Deposit money\n4)Check balance and other deatils\n5)Calculate interest\n");
					printf("6)NEFT\n7)Change password\n8)Check loan plans");
					printf("\n9)Check loan status\n10)Exit\n");
					int choice;
					scanf("%d",&choice);
					printf("\n*******************************************************************************\n");
					
					switch(choice)
					{
						case 1:
							deleteacc(acc);
							break;
						case 2:
							transactions(acc,2);
							break;
						case 3:
							transactions(acc,1);
							break;
						case 4:
							details(acc);
							break;
						case 5:
							//interest
							break;
						case 6:
							NEFT_transaction(int acc);
							break;
						case 7:
							changepass(acc,pass);
							break;
						case 8:
							//check loan plans;
							break;
						case 9:
							//check loan status
							break;
						case 10:
							EXIT();
							break;
						default:
							printf("\nInvalid choice entered, please try again\n");
							fflush(stdin);
							break;
					}
				}
				else
					printf("\nWrong password, Enter again\n");
			}
			else
				printf("\nAccount number doesn't exist, Enter again\n");
		}
		else
			printf("\nInvalid choice, Enter again\n");
		
	}
	return 0;
}

//account creation

void acc_create()
{
	struct BANK b1;
	int ch3,ch4;
	fflush(stdin);
	char *p1;
	char *type;
	char banks[6][30] = {"HDFC bank","ICICI bank","SBI bank","Canara bank","Axis bank","Yes bank"};
	char account_str[500], encrypted_str[500];
	//int ifsc[6] = {};
	printf("Which bank do you wish to create an account in?\n1)HDFC bank\n2)ICICI bank \n3)SBI bank\n4)Canara bank\n5)Axis bank\n6)Yes bank\n");
	scanf("%d",&ch3);
	for(int i = 1;i<=6;i++)
	{
		if(ch3 == i)
			{
			p1 = *(banks+(i-1));
			}			
	}
	b1.bank_name[0] = '\0';
	strcat(b1.bank_name, p1);

	printf("\nChoose a type of account:\n1)Savings account\n2)Loan account\n");
	scanf("%d",&ch4);
	while(1)
	{
		if(ch4 == 1)
		{
			type = "sa";
			break;
		}
		else if(ch4 == 2)
		{
			type = "la";
			break;
		}
		else
			printf("\nInvalid choice, Enter again\n");
	}
		
	srand(time(NULL));
	int k= (int)rand() + 10000;  
	while(checkacc(k))
		k = (int)rand() + 10000; 
	b1.acc_no = k;
	printf("\nEnter your name:\n");
	fflush(stdin);
	scanf("%[^\n]s",b1.name);
	while(1)
	{
		printf("\nPlease set up a password:\n");
		scanf("%s",&b1.password);
		char s[100];
		printf("\nPlease confirm password:\n");
		scanf("%s",&s);
		if(strcmp(b1.password,s)==0)
		{
			printf("\nPassword set up!\n");
			break;
		}
		else
			printf("\nRe-enter, please.");
	}
	//loan??
	printf("\nYou need to deposit a minimum of 2000 rupees to maintain a bank account. Please enter amount to be deposited into bank\n");
	//b1.balance = deposit();
	b1.balance = 2000.0;
	
	FILE* fp;
	fp = fopen("bankingdatabase.csv","r");
	if(fp == NULL) {
		fclose(fp);
		fp = fopen("bankingdatabase.csv","w");
		fprintf(fp, "%s\n", "Account_number,Bank_name,Name,Balance,Password,Account_type");
		fclose(fp);
	}
	fp = fopen("bankingdatabase.csv","a");
	if(fp == NULL)
		printf("\ncouldn't open file\n");
	else
	{
		fflush(stdin);
		sprintf(account_str,"%d,%s,%s,%f,%s,%s",b1.acc_no,p1,b1.name,b1.balance,b1.password,type);
		encrypt_string(account_str, encrypted_str);
		fprintf(fp,"%s\n",encrypted_str);
		account_str[0] = '\0';
		decrypt_string(encrypted_str, account_str);
	}
	fclose(fp);
	printf("\nYour account is set up! Account Number is %d\n", b1.acc_no);
}

int EXIT()
{
	printf("\nThanks for visiting :)\n");
	return 0;
}

int checkacc(int k) // to check if acc no already exists in code.
{
	FILE* fp1;
	fp1 = fopen("bankingdatabase.csv","r");
	ex values[999];
	
	if(fp1 == NULL)
		printf("\ncouldn't open file\n");
	else
	{
		fflush(stdin);
		char buff[1024], encrypted_str[1024];
		int row_count = 0;
		int field_count = 0;
		
		int i = 0;
		while(fgets(encrypted_str,1024,fp1))
		{
			decrypt_string(encrypted_str, buff);
			field_count = 0;
			row_count++;
			if(row_count == 1)
				continue;
			char *field = strtok(buff,","); 
			while(field)
			{
				if(field_count==0)
					strcpy(values[i].ACC_NO,field);
				field = strtok(NULL,",");
				field_count++;
			}
	
			i++;
		}
	}
	fclose(fp1);

	for(int j = 0;j< 999;j++)
			{
				//printf("\n%d",atoi(values[j].ACC_NO));
				if(atoi(values[j].ACC_NO) == k)
				{
					return 1;	
					
				}
				
			}
	return 0;

}

int checkpassword(char pass[],int acc) // to check if password entered matches the one in database
{
	FILE* fp1;
	fp1 = fopen("bankingdatabase.csv","r");
	ex values[999];
	if(fp1 == NULL)
		printf("\ncouldn't open file\n");
	else
	{
		fflush(stdin);
		char buff[1024], encrypted_str[1024];
		int row_count = 0;
		int field_count = 0;
		
		int i = 0;
		while(fgets(encrypted_str,1024,fp1))
		{
			decrypt_string(encrypted_str, buff);
			field_count = 0;
			row_count++;
			if(row_count == 1)
				continue;
			char *field = strtok(buff,","); 
			while(field)
			{
				if(field_count==4)
					strcpy(values[i].PASSWORD,field);
				if(field_count==0)
					strcpy(values[i].ACC_NO,field);
				field = strtok(NULL,",");
				field_count++;
			}
			i++;
		}
	}
	fclose(fp1);
	
	for(int j = 0;j<999;j++)
	{
		if(atoi(values[j].ACC_NO)==acc)
		{
			int d = strcmp(pass,values[j].PASSWORD);
			
			if(d ==0)
			{
				
				return 1;
			}
		}
	}
	return 0;

}

int deleteacc(int acc) // to delete account present in database
{
	FILE* fp1;
	fp1 = fopen("bankingdatabase.csv","r");
	ex values[999];
	int r, first_row;
	char buff[1024], encrypted_str[1024];
	if(fp1 == NULL) {
		printf("\ncouldn't open file\n");
		return -1;
	}
	else
	{
		fflush(stdin);
		int row_count = 0;
		int field_count = 0;
		
		int i = 0;
		while(fgets(encrypted_str,1024,fp1))
		{
			decrypt_string(encrypted_str, buff);
			field_count = 0;
			row_count++;
			if(row_count == 1)
				continue;
			char *field = strtok(buff,","); 
			while(field)
			{
				if(field_count==4)
					strcpy(values[i].PASSWORD,field);
				if(field_count==1)
					strcpy(values[i].BANK_NAME,field);
				if(field_count==2)
					strcpy(values[i].NAME,field);
				if(field_count==3)
					strcpy(values[i].BALANCE,field);
				if(field_count==0)
					strcpy(values[i].ACC_NO,field);
				if(field_count==5)
					strcpy(values[i].TYPE,field);
				field = strtok(NULL,",");
				field_count++;
			}
	
			i++;
			r = row_count;
		}
	}
	fclose(fp1);
	/*
	char* p;
	for(int i = 0;i<(r-1);i++)
	{
		p = strtok(values[i].TYPE,"\n");
		strcpy(values[i].TYPE,p);
	}
	*/
	FILE* fp;
	fp = fopen("bankingdatabase.csv","w");
	fflush(stdin);
	fprintf(fp,"Account_number,Bank_name,Name,Balance,Password,Account_type");
	fclose(fp);
	
	FILE* fp2;
	fp2 = fopen("bankingdatabase.csv","a");

	first_row=0;
	for(int i =0;i<(r-1);i++)
	{
		if(atoi(values[i].ACC_NO)==acc)
		{
			continue;
		}
		else 
		{
			sprintf(buff,"%s,%s,%s,%s,%s,%s",values[i].ACC_NO,values[i].BANK_NAME,values[i].NAME,values[i].BALANCE,values[i].PASSWORD,values[i].TYPE);
			encrypt_string(buff, encrypted_str);
			fflush(stdin);
			if (first_row == 0)
				fprintf(fp2,"\n%s\n",encrypted_str);
			else
				fprintf(fp2,"%s\n",encrypted_str);
			fflush(stdin);
			first_row = 1;
		}
	}
	fclose(fp2);
	printf("\nYour account has been deleted.");
}

void details(int acc) // to display details of existing record in database
{
	FILE* fp1;
	fp1 = fopen("bankingdatabase.csv","r");
	ex values[999];
	int r;
	if(fp1 == NULL)
		printf("\ncouldn't open file\n");
	else
	{
		fflush(stdin);
		char buff[1024], encrypted_str[1024];
		int row_count = 0;
		int field_count = 0;
		
		int i = 0;
		while(fgets(encrypted_str,1024,fp1))
		{
			row_count++;
			if(row_count == 1)
				continue;
			decrypt_string(encrypted_str, buff);
			field_count = 0;
			char *field = strtok(buff,","); 
			while(field)
			{
				if(field_count==4)
					strcpy(values[i].PASSWORD,field);
				if(field_count==1)
					strcpy(values[i].BANK_NAME,field);
				if(field_count==2)
					strcpy(values[i].NAME,field);
				if(field_count==3)
					strcpy(values[i].BALANCE,field);
				if(field_count==0)
					strcpy(values[i].ACC_NO,field);
				if(field_count==5)
					strcpy(values[i].TYPE,field);
				field = strtok(NULL,",");
				field_count++;
			}
	
			i++;
			r = row_count;
		}
	}
	fclose(fp1);
	
	/*
	char* p;
	for(int i = 0;i<(r-1);i++)
	{
		p = strtok(values[i].TYPE,"\n");
		strcpy(values[i].TYPE,p);
		
	}
	*/
	printf("\n\nHere are your details:\n");
	for(int i = 0;i<r;i++)
	{
		if(atoi(values[i].ACC_NO) == acc)
			printf("\nAccount number : %s\nBank : %s\nName : %s\nBalance : %s\nPassword : %s\nAccount type : %s\n",values[i].ACC_NO,values[i].BANK_NAME,values[i].NAME,values[i].BALANCE,values[i].PASSWORD,values[i].TYPE);
	}
}

void changepass(int acc, char pass[]) // to change password
{
	char newpass[100];
	char newpass2[100];
	char buff[1024], encrypted_str[1024];
	while(1)
	{
		printf("\nPlease enter the new password:\n");
		scanf("%s",newpass);
		printf("\nPlease confirm password:\n");
		scanf("%s",newpass2);
		if(strcmp(newpass,newpass2)==0)
		{
			printf("\nPassword has been changed!\n");
			break;
		}
		else
			printf("\nRe-enter, please.");
	
	}
	FILE* fp1;
	fp1 = fopen("bankingdatabase.csv","r");
	ex values[999];
	int r;
	if(fp1 == NULL)
	{
		printf("\ncouldn't open file\n");
		return;
	}
	else
	{
		fflush(stdin);
		int row_count = 0;
		int field_count = 0;
		
		int i = 0;
		while(fgets(encrypted_str,1024,fp1))
		{
			decrypt_string(encrypted_str, buff);
			field_count = 0;
			row_count++;
			if(row_count == 1)
				continue;
			char *field = strtok(buff,","); 
			while(field)
			{
				if(field_count==4)
					strcpy(values[i].PASSWORD,field);
				if(field_count==1)
					strcpy(values[i].BANK_NAME,field);
				if(field_count==2)
					strcpy(values[i].NAME,field);
				if(field_count==3)
					strcpy(values[i].BALANCE,field);
				if(field_count==0)
					strcpy(values[i].ACC_NO,field);
				if(field_count==5)
				{
					//field = strtok(buff,"\n");
					strcpy(values[i].TYPE,field);
				}
				//field = strtok(NULL,"\r");
				field = strtok(NULL,",");
				field_count++;
			}
	
			i++;
			r = row_count;
		}
	}
	fclose(fp1);
	
	/*
	char* p;
	for(int i = 0;i<(r-1);i++)
	{
		p = strtok(values[i].TYPE,"\n");
		strcpy(values[i].TYPE,p);
	}
	*/
	
	FILE* fp;
	fp = fopen("bankingdatabase.csv","w");
	fflush(stdin);
	fprintf(fp,"Account_number,Bank_name,Name,Balance,Password,Account_type");
	fclose(fp);
	
	FILE* fp2;
	fp2 = fopen("bankingdatabase.csv","a");
	
	for(int i =0;i<(r-1);i++)
	{
		if (atoi(values[i].ACC_NO)==acc)
		{
			sprintf(buff,"%s,%s,%s,%s,%s,%s",values[i].ACC_NO,values[i].BANK_NAME,values[i].NAME,values[i].BALANCE,newpass,values[i].TYPE);
		}
		else
		{
			sprintf(buff,"%s,%s,%s,%s,%s,%s",values[i].ACC_NO,values[i].BANK_NAME,values[i].NAME,values[i].BALANCE,values[i].PASSWORD,values[i].TYPE);
		}
		encrypt_string(buff, encrypted_str);
		if(i==0 && (atoi(values[i].ACC_NO)==acc))
		{
			//fprintf(fp2,"\n%s,%s,%s,%s,%s,%s\n",values[i].ACC_NO,values[i].BANK_NAME,values[i].NAME,values[i].BALANCE,newpass,values[i].TYPE);
			fprintf(fp2,"\n%s\n",encrypted_str);
			fflush(stdin);
		}	
		else if(i==0)
		{
			//fprintf(fp2,"\n%s,%s,%s,%s,%s,%s\n",values[i].ACC_NO,values[i].BANK_NAME,values[i].NAME,values[i].BALANCE,values[i].PASSWORD,values[i].TYPE);
			fprintf(fp2,"\n%s\n",encrypted_str);
		}
		else if(atoi(values[i].ACC_NO)==acc)
		{
			fflush(stdin);
			//fprintf(fp2,"%s,%s,%s,%s,%s,%s\n",values[i].ACC_NO,values[i].BANK_NAME,values[i].NAME,values[i].BALANCE,newpass,values[i].TYPE);
			fprintf(fp2,"%s\n",encrypted_str);
			fflush(stdin);
		}	
		else
		{
			fflush(stdin);
			//fprintf(fp2,"%s,%s,%s,%s,%s,%s\n",values[i].ACC_NO,values[i].BANK_NAME,values[i].NAME,values[i].BALANCE,values[i].PASSWORD,values[i].TYPE);
			fprintf(fp2,"%s\n",encrypted_str);
			fflush(stdin);
		}
	}
	fclose(fp2);
	
}	

void create_file(int acc) // to create file to hold transactions of each account
{
	FILE* fp;
	char snum[10];
	fp = fopen(strcat(itoa(acc,snum,10),".dat"),"a");
	fprintf(fp,"Account_number,Bank_name,Credit,Debit,Balance\n");
	fclose(fp);
}
	
void transactions(int acc,int choice) // to deposit 
{
	float credit=0.0,debit=0.0,balance=0.0;
	if(choice == 1)
	{
		printf("\nEnter the amount you wish to deposit:\n");
		scanf("%f",&credit);
	}
	else if(choice == 2)
	{
		printf("\nEnter the amount you wish to withdraw:\n");
		scanf("%f",&debit);
	}
	else
	{
		printf("invalid choice");
		return;
	}
		
	if(credit<0.0 || debit<0.0)
		return;
	
	FILE* fp1;
	fp1 = fopen("bankingdatabase.csv","r");
	ex values[999];
	char buff[1024], encrypted_str[1024];
	int r;
	//char bankname[50];
	if(fp1 == NULL)
		printf("\ncouldn't open file\n");
	else
	{
		fflush(stdin);
		int row_count = 0;
		int field_count = 0;
		
		int i = 0;
		while(fgets(encrypted_str,1024,fp1))
		{
			row_count++;
			if(row_count == 1)
				continue;
			decrypt_string(encrypted_str, buff);
			field_count = 0;
			char *field = strtok(buff,","); 
			while(field)
			{
				
				if(field_count==4)
					strcpy(values[i].PASSWORD,field);
				if(field_count==1)
					strcpy(values[i].BANK_NAME,field);
				if(field_count==2)
					strcpy(values[i].NAME,field);
				if(field_count==3)
					strcpy(values[i].BALANCE,field);
				if(field_count==0)
					strcpy(values[i].ACC_NO,field);
				if(field_count==5)
					strcpy(values[i].TYPE,field);
				
				field = strtok(NULL,",");
				field_count++;
			}
	
			i++;
			r = row_count;
			//printf("\nRows = %d\n",r);
		}
	}
	fclose(fp1);
	
	/*
	char* p;
	for(int k = 0;k<(r-1);k++)
	{
		p = strtok(values[k].TYPE,"\n");
		strcpy(values[k].TYPE,p);
		
	}
	*/
	
	char* bankname;
	for(int j =0;j<(r-1);j++)
	{
		if(atoi(values[j].ACC_NO)==acc)
		{
			balance = atof(values[j].BALANCE);
			bankname = values[j].BANK_NAME;
		}
	}
	
	if(choice == 1)
		balance = balance + credit;
	else if((choice == 2) && (debit<balance))
		balance = balance - debit;
	else
	{
		printf("The amount you wish to withdraw is greater than your bank balance");
		return;
	}

	FILE* fp2;
	fp2 = fopen("bankingdatabase.csv","w");
	fflush(stdin);
	fprintf(fp2,"Account_number,Bank_name,Name,Balance,Password,Account_type");
	fclose(fp2);
		
	FILE* fp3;
	fp3 = fopen("bankingdatabase.csv","a");
	if(fp3==NULL)
	{
		printf("\nFile not found\n");
	}
	else
	{
		for(int l=0;l<(r-1);l++)
		{
			
			if (atoi(values[l].ACC_NO)==acc)
			{
				sprintf(buff,"%s,%s,%s,%f,%s,%s",values[l].ACC_NO,values[l].BANK_NAME,values[l].NAME,balance,values[l].PASSWORD,values[l].TYPE);
			}
			else
			{
				sprintf(buff,"%s,%s,%s,%s,%s,%s",values[l].ACC_NO,values[l].BANK_NAME,values[l].NAME,values[l].BALANCE,values[l].PASSWORD,values[l].TYPE);
			}
			encrypt_string(buff, encrypted_str);
			
			if((l==0) && (atoi(values[l].ACC_NO)==acc))
			{
				//fprintf(fp3,"\n%s,%s,%s,%f,%s,%s\n",values[l].ACC_NO,values[l].BANK_NAME,values[l].NAME,balance,values[l].PASSWORD,values[l].TYPE);
				fprintf(fp3,"\n%s\n",encrypted_str);
				fflush(stdin);
				
			}	
			else if(l==0)
			{
				//fprintf(fp3,"\n%s,%s,%s,%s,%s,%s\n",values[l].ACC_NO,values[l].BANK_NAME,values[l].NAME,values[l].BALANCE,values[l].PASSWORD,values[l].TYPE);
				fprintf(fp3,"\n%s\n",encrypted_str);
			}
			else if(atoi(values[l].ACC_NO)==acc)
			{
				fflush(stdin);
				//fprintf(fp3,"%s,%s,%s,%f,%s,%s\n",values[l].ACC_NO,values[l].BANK_NAME,values[l].NAME,balance,values[l].PASSWORD,values[l].TYPE);
				fprintf(fp3,"%s\n",encrypted_str);
				fflush(stdin);
			}
			else
			{
				fflush(stdin);
				//fprintf(fp3,"%s,%s,%s,%s,%s,%s\n",values[l].ACC_NO,values[l].BANK_NAME,values[l].NAME,values[l].BALANCE,values[l].PASSWORD,values[l].TYPE);
				fprintf(fp3,"%s\n",encrypted_str);
				fflush(stdin);
			}

		}
	}
	fclose(fp3);	
	
	FILE* fp4;
	char snum[10];
	fp4 = fopen(strcat(itoa(acc,snum,10),".dat"),"r");
	if(fp4 == NULL)
	{
		create_file(acc);
	}
	fclose(fp4);
	
	FILE* fp5;
	char snum1[10];
	fp5 = fopen(strcat(itoa(acc,snum1,10),".dat"),"a");
	fprintf(fp5,"%d,%s,%f,%f,%f\n",acc,bankname,credit,debit,balance);
	fclose(fp5);
	
	printf("\nTransaction successful\n");
	
	if(choice == 1)	
		printf("\n%f has been deposited into your account\n",credit);
	else
		printf("\n%f has been withrawn from your account\n",debit);
}

void NEFT_transaction(int acc)
{
	int acc2,ifsc,amt;//ask saanch if we've to check against ifsc also
	printf("\nEnter the account number of the bank you wish to transfer money to:\n");
	scanf("%d",&acc2);
	if(checkacc(acc2))
	{
		printf("\nEnter the IFSC code:\n");
		scanf("%d",&ifsc);
		printf("\nEnter the amount you wish to transfer:\n");
		scanf("%d",&amt);
		
	}
	else
		printf("This bank account doesn't exist");
}

void encrypt_string(char *source, char *target) 
{
   int i;
   for(i = 0; i < strlen(source); i++)
      target[i] = source[i] + ENCRYPT_KEY;
   target[i] = '\0';
}
void decrypt_string(char *source, char *target) 
{
	int i, t;
	for(t = i = 0; i < strlen(source); i++)
	{
		if (source[i] != '\n')
			target[t++] = source[i] - ENCRYPT_KEY;
	}
	target[t] = '\0';
}
