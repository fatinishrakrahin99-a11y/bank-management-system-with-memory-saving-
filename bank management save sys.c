#include <stdio.h>
#include <string.h>

struct Account {
    int accNumber;
    char name[50];
    int pin;
    float balance;
};

struct Log {
    int accNumber;
    char msg[200];
};

void saveAccount(struct Account user[], int total) {

    FILE *fp = fopen("accounts.txt", "w");

    for (int i = 0; i < total; i++) {
        fprintf(fp, "%d %s %d %.2f\n",
                user[i].accNumber,
                user[i].name,
                user[i].pin,
                user[i].balance);
    }

    fclose(fp);
}

int loadAccount(struct Account user[]) {

    FILE *fp = fopen("accounts.txt", "r");

    if (fp == NULL) return 0;

    int i = 0;

    while (fscanf(fp, "%d %s %d %f",
                  &user[i].accNumber,
                  user[i].name,
                  &user[i].pin,
                  &user[i].balance) != EOF) {
        i++;
    }

    fclose(fp);
    return i;
}

void addLog(int acc, char msg[]) {

    FILE *fp = fopen("history.txt", "a");
    fprintf(fp, "%d|%s\n", acc, msg);
    fclose(fp);
}

void showHistory(int acc) {

    FILE *fp = fopen("history.txt", "r");

    char line[300];
    int found = 0;

    printf("\n===== TRANSACTION HISTORY =====\n");

    while (fgets(line, sizeof(line), fp)) {

        int fileAcc;
        char msg[200];

        sscanf(line, "%d|%[^\n]", &fileAcc, msg);

        if (fileAcc == acc) {
            printf("%s\n", msg);
            found = 1;
        }
    }

    if (!found) {
        printf("No Transaction History\n");
    }

    fclose(fp);
}

int main() {

    struct Account user[100];

    int total = loadAccount(user);

    // 🔥 PRELOADED USERS (only first run)
    if (total == 0) {

        user[0] = (struct Account){1001, "Rahin", 1234, 5000};
        user[1] = (struct Account){1002, "Fatin", 5678, 7000};
        user[2] = (struct Account){1003, "Siam", 4321, 9000};

        total = 3;
        saveAccount(user, total);
    }

    int choice, acc, pin, found;
    float amount;

    while (1) {

        printf("\n===== X BANK =====\n");
        printf("1. Login\n");
        printf("2. Create Account\n");
        printf("3. Exit\n");

        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 3) {
            saveAccount(user, total);
            printf("Saved & Exiting...\n");
            break;
        }

        // CREATE ACCOUNT
        else if (choice == 2) {

            printf("\nEnter Account Number: ");
            scanf("%d", &user[total].accNumber);

            printf("Enter Name: ");
            scanf("%s", user[total].name);

            printf("Enter PIN: ");
            scanf("%d", &user[total].pin);

            printf("Enter Balance: ");
            scanf("%f", &user[total].balance);

            total++;

            saveAccount(user, total);

            printf("Account Created & Saved!\n");
        }

        // LOGIN
        else if (choice == 1) {

            printf("Enter Account Number: ");
            scanf("%d", &acc);

            printf("Enter PIN: ");
            scanf("%d", &pin);

            found = -1;

            for (int i = 0; i < total; i++) {
                if (user[i].accNumber == acc && user[i].pin == pin) {
                    found = i;
                    break;
                }
            }

            if (found == -1) {
                printf("Invalid Login!\n");
                continue;
            }

            printf("\nWelcome, %s to X BANK\n", user[found].name);

            int dash;

            while (1) {

                printf("\n===== DASHBOARD =====\n");
                printf("1. Check Balance\n");
                printf("2. Deposit\n");
                printf("3. Withdraw\n");
                printf("4. Transfer\n");
                printf("5. Transaction History\n");
                printf("6. Logout\n");

                printf("Choice: ");
                scanf("%d", &dash);

                if (dash == 6) break;

                // BALANCE
                if (dash == 1) {
                    printf("Balance: %.2f\n", user[found].balance);
                }

                // DEPOSIT
                else if (dash == 2) {

                    printf("Amount: ");
                    scanf("%f", &amount);

                    user[found].balance += amount;

                    char msg[200];
                    sprintf(msg, "Deposited %.2f", amount);
                    addLog(user[found].accNumber, msg);

                    saveAccount(user, total);

                    printf("Deposit Successful!\n");
                }

                // WITHDRAW
                else if (dash == 3) {

                    printf("Amount: ");
                    scanf("%f", &amount);

                    if (amount > user[found].balance) {
                        printf("Insufficient Balance!\n");
                    }
                    else {
                        user[found].balance -= amount;

                        char msg[200];
                        sprintf(msg, "Withdrawn %.2f", amount);
                        addLog(user[found].accNumber, msg);

                        saveAccount(user, total);

                        printf("Withdraw Successful!\n");
                    }
                }

                // TRANSFER
                else if (dash == 4) {

                    int recv;
                    printf("Receiver Acc: ");
                    scanf("%d", &recv);

                    printf("Amount: ");
                    scanf("%f", &amount);

                    int rIndex = -1;

                    for (int i = 0; i < total; i++) {
                        if (user[i].accNumber == recv) {
                            rIndex = i;
                            break;
                        }
                    }

                    if (rIndex == -1) {
                        printf("Receiver Not Found!\n");
                    }
                    else if (amount > user[found].balance) {
                        printf("Insufficient Balance!\n");
                    }
                    else {

                        user[found].balance -= amount;
                        user[rIndex].balance += amount;

                        char msg1[200], msg2[200];

                        sprintf(msg1, "Sent %.2f to %d", amount, recv);
                        sprintf(msg2, "Received %.2f from %d", amount, user[found].accNumber);

                        addLog(user[found].accNumber, msg1);
                        addLog(user[rIndex].accNumber, msg2);

                        saveAccount(user, total);

                        printf("Transfer Successful!\n");
                    }
                }

                // HISTORY
                else if (dash == 5) {
                    showHistory(user[found].accNumber);
                }
            }
        }
    }

    return 0;
}