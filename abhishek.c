#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct items {
    char item[50];
    float price;
    int qty;
};

struct orders {
    char customer[50];
    char date[50];
    int numOfItems;
    struct items *itm;
};

// Function to generate bill header
void generateBillHeader(char name[50], char date[50]) {
    printf("\n\n");
    printf("\t    ADV. Restaurant");
    printf("\n\t   -----------------");
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n---------------------------------------");
    printf("\n\n");
}

// Function to generate bill body
void generateBillBody(char item[50], int qty, float price) {
    printf("%-16s", item);
    printf("%-8d", qty);
    printf("%.2f\n", qty * price);
}

// Function to generate bill footer
void generateBillFooter(float total) {
    float dis = 0.1 * total;
    float netTotal = total - dis;
    float cgst = 0.09 * netTotal, grandTotal = netTotal + 2 * cgst; // netTotal + cgst + sgst

    printf("\n---------------------------------------");
    printf("\nSub Total\t\t\t%.2f", total);
    printf("\nDiscount @10%%\t\t\t%.2f", dis);
    printf("\n\t\t\t\t-------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nCGST @9%%\t\t\t%.2f", cgst);
    printf("\nSGST @9%%\t\t\t%.2f", cgst);
    printf("\n---------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n---------------------------------------\n");
}

int main() {
    int opt, n;
    struct orders ord;
    struct orders order;
    char saveBill = 'y', contFlag = 'y';
    char name[50];
    FILE *fp;

    while (contFlag == 'y') {
        // Clear screen cross-platform
        printf("\033[H\033[J");
        float total = 0;
        int invoiceFound = 0;

        printf("\t============ADV. RESTAURANT============");
        printf("\n\nPlease select your preferred operation");
        printf("\n1. Generate Invoice");
        printf("\n2. Show all Invoices");
        printf("\n3. Search Invoice");
        printf("\n4. Exit");

        printf("\n\nYour choice: ");
        scanf("%d", &opt);
        getchar(); // To consume the newline character left in the buffer

        switch (opt) {
            case 1:
                // Clear screen
                printf("\033[H\033[J");
                printf("\nPlease enter the name of the customer: ");
                fgets(ord.customer, 50, stdin);
                ord.customer[strcspn(ord.customer, "\n")] = 0; // Remove trailing newline
                strcpy(ord.date, __DATE__);

                printf("\nPlease enter the number of items: ");
                scanf("%d", &ord.numOfItems);
                getchar(); // Consume newline

                ord.itm = (struct items *)malloc(ord.numOfItems * sizeof(struct items));

                for (int i = 0; i < ord.numOfItems; i++) {
                    printf("\nPlease enter item %d:\t", i + 1);
                    fgets(ord.itm[i].item, 50, stdin);
                    ord.itm[i].item[strcspn(ord.itm[i].item, "\n")] = 0; // Remove trailing newline

                    printf("Please enter the quantity:\t");
                    scanf("%d", &ord.itm[i].qty);
                    printf("Please enter the unit price:\t");
                    scanf("%f", &ord.itm[i].price);
                    getchar(); // Consume newline

                    total += ord.itm[i].qty * ord.itm[i].price;
                }

                generateBillHeader(ord.customer, ord.date);
                for (int i = 0; i < ord.numOfItems; i++) {
                    generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                }
                generateBillFooter(total);

                printf("\nDo you want to save the invoice [y/n]: ");
                scanf("%c", &saveBill);

                if (saveBill == 'y') {
                    fp = fopen("RestaurantBill.dat", "a+");
                    if (fp == NULL) {
                        printf("\nError opening file!");
                        break;
                    }
                    fwrite(&ord, sizeof(struct orders) + ord.numOfItems * sizeof(struct items), 1, fp);
                    if (fwrite != 0)
                        printf("\nSuccessfully saved\n");
                    else
                        printf("\nError saving\n");
                    fclose(fp);
                }
                free(ord.itm); // Free the allocated memory
                break;

            case 2:
                // Clear screen
                printf("\033[H\033[J");
                fp = fopen("RestaurantBill.dat", "r");
                if (fp == NULL) {
                    printf("\nError opening file!");
                    break;
                }
                printf("\n  *****Your Previous Invoices*****\n");
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    fread(order.itm, sizeof(struct items), order.numOfItems, fp);
                    float tot = 0;
                    generateBillHeader(order.customer, order.date);
                    for (int i = 0; i < order.numOfItems; i++) {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        tot += order.itm[i].qty * order.itm[i].price;
                    }
                    generateBillFooter(tot);
                }
                fclose(fp);
                break;

            case 3:
                getchar(); // Consume any trailing newline character
                printf("Enter the name of the customer: ");
                fgets(name, 50, stdin);
                name[strcspn(name, "\n")] = 0; // Remove trailing newline

                // Clear screen
                printf("\033[H\033[J");
                fp = fopen("RestaurantBill.dat", "r");
                if (fp == NULL) {
                    printf("\nError opening file!");
                    break;
                }
                printf("\t*****Invoice of %s*****", name);
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    fread(order.itm, sizeof(struct items), order.numOfItems, fp);
                    float tot = 0;
                    if (strcmp(order.customer, name) == 0) {
                        generateBillHeader(order.customer, order.date);
                        for (int i = 0; i < order.numOfItems; i++) {
                            generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                            tot += order.itm[i].qty * order.itm[i].price;
                        }
                        generateBillFooter(tot);
                        invoiceFound = 1;
                    }
                }
                if (!invoiceFound) {
                    printf("\nSorry, the invoice for %s does not exist\n", name);
                }
                fclose(fp);
                break;

            case 4:
                printf("\n\t\tBye Bye :)\n\n");
                exit(0);
                break;

            default:
                printf("Sorry, invalid option\n");
                break;
        }
        printf("\nDo you want to perform another operation? [y/n]: ");
        scanf(" %c", &contFlag); // Added a space before %c to consume any leftover newline character
    }
    printf("\n\t\tBye Bye :)\n\n");

    return 0;
}
