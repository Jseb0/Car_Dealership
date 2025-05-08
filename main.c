#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h> // added for character validation

#define MAX_CARS 15
#define MAX_CUSTOMERS 15
#define MAX_BRANDS 15
#define MAX_MODELS_PER_BRAND 3
#define DISCOUNT_CODE "CAR20"
#define DISCOUNT_PERCENTAGE 0.2

struct FeedbackInfo {
    char customerName[50];
    int customerAge;
    char carBrand[50];
    char carModel[50];
    char dateOfPurchase[20];
    char feedback[100];
    int score;
};

// Function to validate and filter integer input
int getValidatedIntegerInput() {
    int input;
    char buffer[64];
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &input) == 1) {
                return input;
            }
        }
        printf("Invalid input. Please enter a valid integer: ");
    }
}

// Function to validate and filter string input
void getValidatedStringInput(char *input, int maxLength) {
    char buffer[256];
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0; // remove trailing newline
            if (strlen(buffer) <= maxLength) {
                strcpy(input, buffer);
                return;
            }
        }
        printf("Invalid input. Please enter a valid string: ");
    }
}

void viewCars(const char brands[][50], const char models[][50], const int prices[], const int years[], const int sold[], const int stockRemaining[]) {
    printf("%-15s%-15s%-10s%-10s%-15s\n", "Brand", "Model", "Year", "Price", "Stock");
    for (int i = 0; i < MAX_CARS; i++) {
        printf("%-15s%-15s%-10d£%-10d%-15d\n", brands[i], models[i], years[i], prices[i], stockRemaining[i]);
    }
    printf("Enter 'x' to return to the main menu: ");
    char returnOption;
    scanf(" %c", &returnOption);
    if (returnOption == 'x' || returnOption == 'X') {
        printf("Returning to the main menu.\n");
    } else {
        printf("Invalid option. Returning to the main menu.\n");
    }
}

void buyCars(char brands[][50], char models[][50], int prices[], int years[], int sold[], int stockRemaining[],
             char customers[MAX_CUSTOMERS][50], int ages[MAX_CUSTOMERS],
             char dates[MAX_CARS][20], struct FeedbackInfo feedbackArray[MAX_CARS], int *feedbackCount) {

    printf("Available Brands:\n");
    int brandsPerRow = 3;
    for (int i = 0; i < MAX_CARS; i++) {
        printf("%-3d. %-15s", i + 1, brands[i]);
        if ((i + 1) % brandsPerRow == 0 || i == MAX_CARS - 1) {
            printf("\n");
        }
    }
    printf("Enter the number of the brand you want to explore: ");
    int brandChoice = getValidatedIntegerInput();
    if (brandChoice < 1 || brandChoice > MAX_CARS) {
        printf("Invalid brand choice.\n");
        return;
    }
    printf("Models for %s:\n", brands[brandChoice - 1]);
    for (int i = 0; i < MAX_CARS; i++) {
        if (!sold[i] && strcmp(brands[i], brands[brandChoice - 1]) == 0) {
            printf("%d. %s\n", i + 1, models[i]);
        }
    }
    printf("Enter the number of the model you want to buy: ");
    int modelChoice = getValidatedIntegerInput();
    if (modelChoice < 1 || modelChoice > MAX_CARS) {
        printf("Invalid model choice.\n");
        return;
    }

    char customerName[50];
    printf("Enter your name: ");
    getValidatedStringInput(customerName, sizeof(customerName));

    printf("Enter your age: ");
    int customerAge = getValidatedIntegerInput();

    printf("Selected Car Details:\n");
    printf("Brand: %s\n", brands[modelChoice - 1]);
    printf("Model: %s\n", models[modelChoice - 1]);
    printf("Year: %d\n", years[modelChoice - 1]);
    printf("Price: £%d\n", prices[modelChoice - 1]);
    printf("Stock Remaining: %d\n", stockRemaining[modelChoice - 1]);

    printf("Do you have a discount code? (Y/N): ");
    char hasDiscountCode;
    scanf(" %c", &hasDiscountCode);
    if (toupper(hasDiscountCode) == 'Y') {
        char discountCode[10];
        printf("Enter your discount code: ");
        scanf("%s", discountCode);
        if (strcmp(discountCode, DISCOUNT_CODE) == 0) {
            prices[modelChoice - 1] -= prices[modelChoice - 1] * DISCOUNT_PERCENTAGE;
            printf("Discount Applied! New Car Price: £%d\n", prices[modelChoice - 1]);
        } else {
            printf("Invalid discount code. No discount applied.\n");
        }
    }
    printf("Do you want to buy this car? (Y/N): ");
    char confirmation;
    scanf(" %c", &confirmation);
    if (toupper(confirmation) == 'Y') {
        time_t t;
        struct tm *current_time;
        char date_str[20];
        time(&t);
        current_time = localtime(&t);
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", current_time);
        strcpy(dates[modelChoice - 1], date_str);
        printf("\n\nReceipt:\n");
        printf("Date: %s\n", date_str);
        printf("Customer Name: %s\n", customerName);
        printf("Customer Age: %d\n", customerAge);
        printf("Car Details:\n");
        printf("Brand: %s\n", brands[modelChoice - 1]);
        printf("Model: %s\n", models[modelChoice - 1]);
        printf("Year: %d\n", years[modelChoice - 1]);
        printf("Price: £%d\n", prices[modelChoice - 1]);
        printf("Stock Remaining: %d\n", stockRemaining[modelChoice - 1]);
        sold[modelChoice - 1] = 1;
        stockRemaining[modelChoice - 1]--;
        printf("\nCar purchased successfully!\n");
        printf("Thank you for your purchase!\n");
        strcpy(customers[modelChoice - 1], customerName);
        ages[modelChoice - 1] = customerAge;
        char feedbackText[100];
        int score;
        printf("Provide feedback (max 100 characters):\n");
        scanf(" %[^\n]", feedbackText);
        printf("Provide a score out of 5: ");
        scanf("%d", &score);
        strcpy(feedbackArray[*feedbackCount].customerName, customerName);
        feedbackArray[*feedbackCount].customerAge = customerAge;
        strcpy(feedbackArray[*feedbackCount].carBrand, brands[modelChoice - 1]);
        strcpy(feedbackArray[*feedbackCount].carModel, models[modelChoice - 1]);
        strcpy(feedbackArray[*feedbackCount].dateOfPurchase, dates[modelChoice - 1]);
        strcpy(feedbackArray[*feedbackCount].feedback, feedbackText);
        feedbackArray[*feedbackCount].score = score;
        (*feedbackCount)++;

        // Store sales data in a text file
        FILE *salesFile = fopen("sales_data.txt", "a");
        if (salesFile != NULL) {
            fprintf(salesFile, "Date: %s\n", date_str);
            fprintf(salesFile, "Customer Name: %s\n", customerName);
            fprintf(salesFile, "Customer Age: %d\n", customerAge);
            fprintf(salesFile, "Car Details:\n");
            fprintf(salesFile, "Brand: %s\n", brands[modelChoice - 1]);
            fprintf(salesFile, "Model: %s\n", models[modelChoice - 1]);
            fprintf(salesFile, "Year: %d\n", years[modelChoice - 1]);
            fprintf(salesFile, "Price: £%d\n", prices[modelChoice - 1]);
            fprintf(salesFile, "Stock Remaining: %d\n\n", stockRemaining[modelChoice - 1]);
            fclose(salesFile);
        } else {
            printf("Error: Unable to open sales data file.\n");
        }

        // Store feedback in a text file
        FILE *feedbackFile = fopen("feedback_data.txt", "a");
        if (feedbackFile != NULL) {
            fprintf(feedbackFile, "Customer: %s, Age: %d, Car: %s %s, Date: %s, Score: %d\n",
                    customerName, customerAge,
                    brands[modelChoice - 1], models[modelChoice - 1],
                    dates[modelChoice - 1], score);
            fprintf(feedbackFile, "Feedback: %s\n\n", feedbackText);
            fclose(feedbackFile);
        } else {
            printf("Error: Unable to open feedback data file.\n");
        }
    } else {
        printf("Car not purchased.\n");
    }
}

void salesDataByBrand(const char brands[][50], const int sold[], const int prices[], const int stockRemaining[]) {
    printf("Sales Data by Brand:\n");
    printf("%-15s%-15s%-15s\n", "Brand", "Stock Sold", "Total Price");

    // Array to keep track of brands already processed
    int processed[MAX_BRANDS] = {0};

    for (int i = 0; i < MAX_CARS; i++) {
        // Check if the brand for this car is already processed
        if (!processed[i] && sold[i] == 1) {
            int totalSold = 0;
            int totalPrice = 0;
            for (int j = 0; j < MAX_CARS; j++) {
                if (strcmp(brands[j], brands[i]) == 0 && sold[j] == 1) {
                    totalSold++;
                    totalPrice += prices[j];
                    processed[j] = 1; // Mark this brand as processed
                }
            }
            printf("%-15s%-15d£%-15d\n", brands[i], totalSold, totalPrice);
        }
    }
}

void viewFeedback(const struct FeedbackInfo feedbackArray[MAX_CARS], int feedbackCount) {
    printf("Feedback:\n");
    if (feedbackCount == 0) {
        printf("No feedback available.\n");
        return;
    }
    for (int i = 0; i < feedbackCount; i++) {
        printf("Customer: %s, Age: %d, Car: %s %s, Date: %s, Score: %d\n",
               feedbackArray[i].customerName, feedbackArray[i].customerAge,
               feedbackArray[i].carBrand, feedbackArray[i].carModel,
               feedbackArray[i].dateOfPurchase, feedbackArray[i].score);
        printf("Feedback: %s\n", feedbackArray[i].feedback);
        printf("\n");
    }
}

int main() {
    char brands[MAX_CARS][50] = {
            "Ford", "Vauxhall", "Volkswagen", "BMW", "Mercedes-Benz", "Smart",
            "Renault", "Peugeot", "MINI", "Audi", "Jaguar", "Toyota", "Kia",
            "Vauxhall", "Volkswagen"
    };

    char models[MAX_CARS][50] = {"Focus", "Zafira", "Tiguan", "X5", "E-Class", "For-Four",
                                 "Clio", "308", "Countryman", "A3", "XE", "RAV4", "Sportage",
                                 "Astra", "Polo"
    };

    int prices[MAX_CARS] = {
            10275, 20865, 30678, 15078, 10540, 20078,
            30659, 15086, 10889, 20567, 30000, 15000, 17000,
            13500, 12677
    };

    int years[MAX_CARS] = {
            2023, 2021, 2020, 2020, 2020, 2019, 2019, 2018, 2018, 2017, 2017, 2016, 2016, 2015, 2015
    };

    int sold[MAX_CARS] = {0};

    int StockRemaining[MAX_BRANDS * MAX_MODELS_PER_BRAND] = {10, 15, 8, 12, 20, 5, 10, 18, 7, 9, 11, 6, 14, 4, 13};

    char customers[MAX_CUSTOMERS][50];
    int ages[MAX_CUSTOMERS];
    char dates[MAX_CARS][20] = {""};
    struct FeedbackInfo feedbackArray[MAX_CARS];
    int feedbackCount = 0;

    int choice;

    printf("Welcome to the Car Dealership!\n");
    printf("We hope you find the car of your dreams today!\n");
    printf("USE CAR20 TO GET 20%% OFF\n");

    do {
        printf("1. View Cars\n");
        printf("2. Buy Cars\n");
        printf("3. Sales Data\n");
        printf("4. View Feedback\n");
        printf("5. Exit\n");

        printf("\nEnter your choice: ");
        choice = getValidatedIntegerInput();

        switch (choice) {
            case 1:
                viewCars(brands, models, prices, years, sold, StockRemaining);
                break;

            case 2:
                buyCars(brands, models, prices, years, sold, StockRemaining, customers, ages, dates,
                        feedbackArray, &feedbackCount);
                break;

            case 3:
                salesDataByBrand(brands, sold, prices, StockRemaining);
                break;

            case 4:
                viewFeedback(feedbackArray, feedbackCount);
                break;

            case 5:
                printf("Exiting the program. Goodbye!\n");
                break;

            default:
                printf("Invalid input. Returning to the main menu.\n");
                break;
        }

        printf("\nPress Enter to return to the main menu...");
        while (getchar() != '\n');
        printf("\n");

    } while (choice != 5);

    return 0;
}