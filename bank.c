#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account{
    int AccountNumber;
    char name[50];
    float balance;
}; 

void MainMenu(); 

void CreateAccount();
void SaveAccount(struct Account a);
void DisplayAccount();
void DepositMoney();
void WithdrawMoney();

int main(){
    MainMenu();
    return 0;
}

void MainMenu(){
    int choice;

    do {
        printf("\n\t\tBank Management System\t\t\n");
        printf("1. Create Account\n");
        printf("2. View Account\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Exit.\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
            CreateAccount();
            break;

            case 2:
            DisplayAccount();
            break;

            case 3:
            DepositMoney();
            break;

            case 4:
            WithdrawMoney();
            break;

            case 5:
            printf("\nExiting program...");
            break;

            default:
            printf("\nInvalid choice. Please try again!");
        }
    }
    while (choice!=5);
}

void CreateAccount(){
    struct Account a;
    
    printf("\n\tCreate New Account\t");

    printf("\nEnter Account Number: ");
    scanf("%d", &a.AccountNumber);

    printf("\nEnter Account Holder's name:");
    scanf(" %[^\n]", a.name); 

    printf("\nEnter current balance: ");
    scanf("%f", &a.balance);

    printf("\nAccount created succefully!!!\n");
    SaveAccount(a);
}

void SaveAccount(struct Account a){
    FILE *fp;
    fp=fopen("bank.dat", "ab"); //open in append mode (binary file- copies data exactly as it is)
    if(fp==NULL){
        printf("\nFile not found!\n");
        return; //if no file found it exits this function
    }
    fwrite(&a, sizeof(struct Account), 1, fp); //writes the record stored in 'a' covering 1 account's worth of data and puts it into file pointed by fp
    fclose(fp);
}

void DisplayAccount(){
    FILE *fp;
    struct Account a; //freads puts data in this variable
    int aNo;
    int found=0;
    printf("\nEnter account number: \n");
    scanf("%d", &aNo);
    fp=fopen("bank.dat", "rb"); //open in read mode (binary file)
    if (fp==NULL){
        printf("File does not exist!\n");
        return;
    }
    while(fread(&a, sizeof(struct Account), 1, fp)){ //while used at it will check all the accounts one by one if if was used it would only check the first account
        if(aNo==a.AccountNumber){
            printf("\n\tAccount Details\t\n");
            printf("\nAccount Number: %d", a.AccountNumber);
            printf("\nName: %s", a.name);
            printf("\nBalance: %.2f", a.balance);
            found=1;
            break;
        }
    }
    fclose(fp);

    if(!found){
        printf("\nAccount does not exist!");
    }
}

void DepositMoney() {
   FILE *fp;
   struct Account a;
   int aNo;
   float amount;
   int found=0;

   printf("\nEnter account number: ");
   scanf("%d", &aNo);
   printf("\nEnter amount you want to deposit: ");
   scanf("%f", &amount);

   if (amount <= 0) {
    printf("Invalid amount!\n");
    return;
}

   fp=fopen("bank.dat", "rb+"); //opens the file in read and write mode, only edits existing files does not create new ones (binary file)
   if(fp==NULL){
    printf("\nFile not found!\n");
    return;
   }
   while(fread(&a, sizeof(struct Account), 1, fp)){
    if(aNo==a.AccountNumber){
        a.balance=a.balance+amount;
        fseek(fp, -sizeof(struct Account), SEEK_CUR); //fseek moves file pointer back by one account so the updated record overwrites the same account, SEEK_CUR moves the pointer from its current position to 1 account backward using -sizeof

        fwrite(&a, sizeof(struct Account), 1, fp);
        printf("\nAmount deposited successfully!");
        printf("\nUpdated balance: %.2f\n", a.balance);
        found=1;
        break;
    }
   }
   fclose(fp);
   if(!found){
    printf("\nAccount does not exist!\n");
   }
}

void WithdrawMoney() {
    FILE *fp;
    struct Account a;
    float amount;
    int aNo;
    int found=0;

    printf("\nEnter Account number: ");
    scanf("%d", &aNo);
    printf("\nEnter amount you want to withdraw: ");
    scanf("%f", &amount);

    if (amount <= 0) {
    printf("Invalid amount!\n");
    return;
}

    fp=fopen("bank.dat", "rb+");
    if(fp==NULL){
        printf("\nFile not found!\n");
        return;
    }

    while(fread(&a, sizeof(struct Account), 1, fp)){
        if(aNo==a.AccountNumber){
            if(amount<=a.balance){ //withdraw is same as deposit with this one extra step
                a.balance=a.balance-amount;
                fseek(fp, -sizeof(struct Account), SEEK_CUR);
                fwrite(&a, sizeof(struct Account), 1, fp);
                printf("\nAmount withdrawn successfully!");
                printf("\nUpdated balance: %.2f\n", a.balance);
            }
            else{
                printf("\nInsufficient balance!\n");
            }
            found=1;
            break;
        }
    }
    fclose(fp);
    if(!found){
        printf("\nAccount does not exist!\n");
        return;
    }
}

