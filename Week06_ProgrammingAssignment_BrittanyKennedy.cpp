/*
======================================================================
Program: Vegetarian Nutrition for Weightlifting
Author : Brittany Kennedy
Date   : 11/22/2025
Section: ITCS 2530 H0801 2025FA - C++ Programming 1
======================================================================
Purpose:
    Program to help users figure out their suggested daily and per-meal
    macros (protein, carbs, and fats). Percentages are geared toward
    vegetarian weightlifters. The program:
      - Collects and validates user input
      - Calculates daily and weekly macro targets
      - Displays a menu using a switch statement
      - Writes a formatted report to a file
      - Uses colors in the Windows console
      - Demonstrates selection, repetition, and user-defined functions
======================================================================
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <windows.h>   // Used to change console text color (Windows only)

using namespace std;


// ========================== CONSTANTS =====================================

// Macro ratios as percentages of daily calories
const double PROTEIN_PERCENT = 0.30;
const double CARB_PERCENT = 0.40;
const double FAT_PERCENT = 0.30;

// Calories per gram for each macro
const int CALORIES_PER_GRAM_PROTEIN = 4;
const int CALORIES_PER_GRAM_CARBS = 4;
const int CALORIES_PER_GRAM_FAT = 9;

// Days per week
const int NUMBER_OF_DAYS = 7;

// Activity thresholds (hours per week)
const double ACTIVITY_HIGH = 5.0;
const double ACTIVITY_MEDIUM = 3.0;

// Calorie thresholds (per day)
const int CAL_EXCESS = 2500;
const int CAL_HIGH = 2200;
const int CAL_MEDIUM = 2000;
const int CAL_LOW = 1800;

// Meal frequency thresholds (per day)
const int MEALS_PER_DAY_HIGH = 6;
const int MEALS_PER_DAY_MEDIUM = 4;
const int MEALS_PER_DAY_LOW = 2;

// Weekly meal thresholds (derived from daily thresholds)
const int MEALS_PER_WEEK_HIGH = MEALS_PER_DAY_HIGH * NUMBER_OF_DAYS;
const int MEALS_PER_WEEK_MEDIUM = MEALS_PER_DAY_MEDIUM * NUMBER_OF_DAYS;
const int MEALS_PER_WEEK_LOW = MEALS_PER_DAY_LOW * NUMBER_OF_DAYS;

// Console and formatting constants
const int CONSOLE_WIDTH = 75;
const int INPUT_IGNORE_LIMIT = 1000;

// Windows color codes
const int COLOR_DEFAULT = 15;  // White
const int COLOR_HEADER = 3;   // Aqua
const int COLOR_SUB_HEADING = 2;   // Green
const int COLOR_RECIPE = 14;  // Yellow
const int COLOR_ERROR = 12;  // Red


// ============================ ENUM TYPE ===================================
// Goal type for the lifter
enum GoalType {FAT_LOSS, MAINTENANCE, MUSCLE_GAIN};


// ======================== FUNCTION PROTOTYPES =============================

// Utility
void setConsoleColor(int color);
void printSectionBreakLine();
void clearInputStream();

// Enum input
GoalType getUserGoal();

// Arrays / weekly calorie log
void fillDailyCaloriesArray(int dailyCaloriesLog[], int size);
void displayWeeklyCalorieLog(const int dailyCaloriesLog[], int size);
double calculateAverageCalories(const int dailyCaloriesLog[], int size);
int findHighestCalories(const int dailyCaloriesLog[], int size);

// Display Menu Options
void displayIntroBanner();
void displayMenu();

void displayDailyMacros(string name, string favoriteProtein,
    int dailyCalories, int mealsPerDay, double weeklyWorkoutHours,
    int dailyProteinGrams, int dailyCarbGrams, int dailyFatGrams,
    int proteinPerMeal, int carbsPerMeal, int fatsPerMeal);

void processReportGeneration(string name, string favoriteProtein,
    int dailyCalories, int weeklyCalories,
    int dailyProteinGrams, int dailyCarbGrams, int dailyFatGrams,
    int totalProteinGrams, int totalCarbGrams, int totalFatGrams,
    int mealsPerDay, double weeklyWorkoutHours,
    int proteinPerMeal, int carbsPerMeal, int fatsPerMeal);

void displayNutritionCheckAndRecipe(int dailyCalories,
    int mealsPerDay, double weeklyWorkoutHours, GoalType goal);

// Supporting Functions for Menu Options
void generateReport(string name, string favoriteProtein,
    int dailyCalories, int weeklyCalories,
    int dailyProteinGrams, int dailyCarbGrams, int dailyFatGrams,
    int totalProteinGrams, int totalCarbGrams, int totalFatGrams,
    int mealsPerDay, double weeklyWorkoutHours,
    int proteinPerMeal, int carbsPerMeal, int fatsPerMeal);

void evaluateNutrition(int dailyCalories, double weeklyWorkoutHours,
    GoalType goal);
void evaluateMealFrequency(int mealsPerWeek, double weeklyWorkoutHours);
void displaySmoothieRecipe();

// Input Validation
string getValidatedStringInput(string requestInput);
int    getValidatedIntInput(string requestInput);
double getValidatedDoubleInput(string requestInput);

// Calculations
void calculateMacros(int dailyCalories, int mealsPerDay,
    int& dailyProteinGrams, int& dailyCarbGrams, int& dailyFatGrams,
    int& proteinPerMeal, int& carbsPerMeal, int& fatsPerMeal,
    int& weeklyCalories,
    int& totalProteinGrams, int& totalCarbGrams, int& totalFatGrams);


// ============================= MAIN FUNCTION ==============================
// Main function controlling program flow and user interaction
int main()
{
    // Setting variables outside loop to use throughout program  
    string name;
    int menuChoice = 0;
    char startNewSession = 'n';

    // Use fixed point formatting with two decimal places for doubles
    cout << fixed << showpoint << setprecision(2);

    // Display introductory banner using color and formatting to greet the user
    displayIntroBanner();
    name = getValidatedStringInput("What is your name? ");


    // Main session loop so user can restart with new inputs
    do
    {
        // ------------------------ Variable Setup ------------------------
        // These variables are reset each time the user starts a new session.
        string favoriteProtein;
        int    dailyCalories = 0;
        int    weeklyCalories = 0;
        int    mealsPerDay = 0;
        double weeklyWorkoutHours = 0.0;

        int dailyProteinGrams = 0;
        int dailyCarbGrams = 0;
        int dailyFatGrams = 0;

        int totalProteinGrams = 0;
        int totalCarbGrams = 0;
        int totalFatGrams = 0;

        int proteinPerMeal = 0;
        int carbsPerMeal = 0;
        int fatsPerMeal = 0;

        // Weekly calorie log array (one entry per day)
        int dailyCaloriesLog[NUMBER_OF_DAYS];


        // ------------------------ User Input ----------------------------
        // Call functions to get and validate user's input
        cout << "Hi " << name << "!\n\n";
        favoriteProtein = getValidatedStringInput("What is your favorite vegetarian protein source? ");
        dailyCalories = getValidatedIntInput("How many calories are you targeting each day? ");
        mealsPerDay = getValidatedIntInput("How many meals do you eat per day? ");
        weeklyWorkoutHours = getValidatedDoubleInput("How many hours per week do you weightlift? ");

        // Ask for user goal using enum
        GoalType goal = getUserGoal();

        // Create weekly calorie log array (array + validation)
        fillDailyCaloriesArray(dailyCaloriesLog, NUMBER_OF_DAYS);


        // ------------------------ Calculations --------------------------
        // Compute daily grams, per meal targets, and weekly totals.
        calculateMacros(dailyCalories, mealsPerDay,
            dailyProteinGrams, dailyCarbGrams, dailyFatGrams,
            proteinPerMeal, carbsPerMeal, fatsPerMeal,
            weeklyCalories,
            totalProteinGrams, totalCarbGrams, totalFatGrams);


        // ------------------------ Menu Loop -----------------------------
        // User menu implemented with switch
        // Use do...while loop to let user return to menu until they choose to exit
        do
        {
            displayMenu();
            cin >> menuChoice;
            cout << endl;

            // If menu input is invalid, clear the stream and continue
            if (cin.fail())
            {
                clearInputStream();
                setConsoleColor(COLOR_ERROR);
                cout << "Invalid choice. Please enter a number from 1 to 5.\n\n";
                setConsoleColor(COLOR_DEFAULT);
                continue;   // Re-display the menu
            }


            switch (menuChoice)
            {
            case 1:
                // Show suggested daily macros and per meal breakdown
                displayDailyMacros(name, favoriteProtein,
                    dailyCalories, mealsPerDay, weeklyWorkoutHours,
                    dailyProteinGrams, dailyCarbGrams, dailyFatGrams,
                    proteinPerMeal, carbsPerMeal, fatsPerMeal);
                break;

            case 2:
                // Write a formatted report to report.txt
                processReportGeneration(name, favoriteProtein,
                    dailyCalories, weeklyCalories,
                    dailyProteinGrams, dailyCarbGrams, dailyFatGrams,
                    totalProteinGrams, totalCarbGrams, totalFatGrams,
                    mealsPerDay, weeklyWorkoutHours,
                    proteinPerMeal, carbsPerMeal, fatsPerMeal);
                break;

            case 3:
                // Check calorie intake vs activity amd show smoothie recipe
                displayNutritionCheckAndRecipe(dailyCalories,
                    mealsPerDay, weeklyWorkoutHours, goal);
                break;

            case 4:
                // View weekly calorie log (array)
                displayWeeklyCalorieLog(dailyCaloriesLog, NUMBER_OF_DAYS);
                break;

            case 5:
                // Start a new session with fresh inputs
                setConsoleColor(COLOR_SUB_HEADING);
                cout << "Starting a new session...\n\n";
                setConsoleColor(COLOR_DEFAULT);
                break;

            case 6:
                // Exit the program
                setConsoleColor(COLOR_SUB_HEADING);
                cout << "Thanks for using the program, " << name
                    << "! Keep lifting strong!\n";
                printSectionBreakLine();
                setConsoleColor(COLOR_DEFAULT);
                break;

            default:
                setConsoleColor(COLOR_ERROR);
                cout << "Invalid choice. Please select 1–6.\n\n";
                setConsoleColor(COLOR_DEFAULT);
                break;
            }

        } while (menuChoice != 5 && menuChoice != 6);

        // Clear leftover newline from menu input
        clearInputStream();

        // Check if user decided to start a new session (4 = new session)
        startNewSession = (menuChoice == 5) ? 'y' : 'n';

    } while (startNewSession == 'y');

    cout << "\nProgram ended. Have a great day!\n";

    return 0;
}


// ======================== FUNCTION DEFINITIONS =============================

// ---------------------------------------------------------------------------
// Function to allow changing the text color used in the Windows console.
// ---------------------------------------------------------------------------
void setConsoleColor(int color)
{
    // Handle is a Windows data type that references the console
    // GetSTDHandle is a Windows function that retrieves the console/output device
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // SetConsoleTextAttribute is a Windows function to allow changing the text color in the console
    SetConsoleTextAttribute(hConsole, color);
}


// ---------------------------------------------------------------------------
// Function to print section lines
// ---------------------------------------------------------------------------
void printSectionBreakLine()
{
    cout << setfill('*') << setw(CONSOLE_WIDTH) << '*' << "\n";
    cout << setfill(' ');
}


// ---------------------------------------------------------------------------
// Function that clears errors on cin and removes any remaining characters
//     from the input buffer up to a newline.
// ---------------------------------------------------------------------------
void clearInputStream()
{
    cin.clear();
    cin.ignore(INPUT_IGNORE_LIMIT, '\n');
}


// ---------------------------------------------------------------------------
// Function to display the introduction banner and greeting.
// ---------------------------------------------------------------------------
void displayIntroBanner()
{
    setConsoleColor(COLOR_HEADER);
    printSectionBreakLine();

    cout << left << setw(5) << " "
        << " Welcome to the Vegetarian Nutrition for Weightlifters Program! "
        << right << setw(5) << " " << "\n";

    printSectionBreakLine();
    cout << "\nLet's learn more about you!\n\n";

    setConsoleColor(COLOR_DEFAULT);
}


// ---------------------------------------------------------------------------
// Ask the user for their goal using the GoalType enum
// ---------------------------------------------------------------------------
GoalType getUserGoal()
{
    int goalChoice = 0;

    cout << "Select your primary goal:\n"
        << "  1. Fat loss\n"
        << "  2. Maintenance\n"
        << "  3. Muscle gain\n\n";
    cout << "Enter 1, 2, or 3: ";
    cin >> goalChoice;
    cout << endl;

    while (cin.fail() || goalChoice < 1 || goalChoice > 3)
    {
        clearInputStream();
        setConsoleColor(COLOR_ERROR);
        cout << "That is not a valid choice. Please enter 1, 2, or 3: ";
        setConsoleColor(COLOR_DEFAULT);

        cin >> goalChoice;
        cout << endl;
    }

    clearInputStream();

    // Convert user's goal choice to the enum value (0-2)
    return static_cast<GoalType>(goalChoice - 1);
}


// ---------------------------------------------------------------------------
// Function to fill an array with daily calorie values for each day of the week
// ---------------------------------------------------------------------------
void fillDailyCaloriesArray(int dailyCaloriesLog[], int size)
{
    cout << "Now let's log your actual calories for each day this week.\n";
    cout << "(Enter a positive number for each day.)\n\n";

    int caloriesDayTotal = 0;

    for (int day = 0; day < size; ++day)
    {
        cout << "Enter your total calories for day " << (day + 1) << ": ";
        cin >> caloriesDayTotal;

        // Repeat until the user enters a valid answer
        while (cin.fail() || caloriesDayTotal < 0)
        {
            clearInputStream();
            setConsoleColor(COLOR_ERROR);
            cout << "Please enter a positive number: " << (day + 1) << ": ";;
            setConsoleColor(COLOR_DEFAULT);

            cin >> caloriesDayTotal;
        }

        clearInputStream();
        dailyCaloriesLog[day] = caloriesDayTotal;
    }

    cout << "Thank you! Your weekly calorie log has been recorded.\n\n";
}


// ---------------------------------------------------------------------------
// Function to calculate average calories from the log array.
// ---------------------------------------------------------------------------
double calculateAverageCalories(const int dailyCaloriesLog[], int size)
{
    int sum = 0;

    for (int day = 0; day < size; ++day)
        sum = sum + dailyCaloriesLog[day];

    return static_cast<double>(sum) / size;
}


// ---------------------------------------------------------------------------
// Function to find the highest calories in the log array.
// ---------------------------------------------------------------------------
int findHighestCalories(const int dailyCaloriesLog[], int size)
{
    int highest = dailyCaloriesLog[0];

    for (int day = 0; day < size; ++day)
    {
        if (dailyCaloriesLog[day] > highest)
            highest = dailyCaloriesLog[day];
    }
    return highest;
}


// ---------------------------------------------------------------------------
// Function to show the menu on the console.
// ---------------------------------------------------------------------------
void displayMenu()
{
    setConsoleColor(COLOR_HEADER);
    printSectionBreakLine();
    cout << "MAIN MENU:\n"
        << "1. View Suggested Daily Macros\n"
        << "2. Generate Report\n"
        << "3. Check Nutrition and Unlock Recipe\n"
        << "4. View Weekly Calorie Log\n"
        << "5. Start a New Session\n"
        << "6. Exit Program\n\n";
    setConsoleColor(COLOR_DEFAULT);
}


// ---------------------------------------------------------------------------
// Function to display menu option 1.
// ---------------------------------------------------------------------------
void displayDailyMacros(string name, string favoriteProtein,
    int dailyCalories, int mealsPerDay, double weeklyWorkoutHours,
    int dailyProteinGrams, int dailyCarbGrams, int dailyFatGrams,
    int proteinPerMeal, int carbsPerMeal, int fatsPerMeal)
{
    setConsoleColor(COLOR_SUB_HEADING);
    printSectionBreakLine();
    cout << "You chose to view your suggested daily macros!\n\n";
    setConsoleColor(COLOR_DEFAULT);

    cout << "Here is the information you provided:\n";
    cout << left << setw(50) << setfill('.') << "Favorite protein source: " << right << favoriteProtein << "\n";
    cout << left << setw(50) << setfill('.') << "Daily calorie target: " << right << dailyCalories << "\n";
    cout << left << setw(50) << setfill('.') << "Meals per day: " << right << mealsPerDay << "\n";
    cout << left << setw(50) << setfill('.') << "Weekly workout hours: " << right << weeklyWorkoutHours << "\n\n";

    cout << "Suggested daily macros:\n";
    cout << left << setw(50) << setfill('.') << "Protein: " << right << dailyProteinGrams << " grams\n";
    cout << left << setw(50) << setfill('.') << "Carbs: " << right << dailyCarbGrams << " grams\n";
    cout << left << setw(50) << setfill('.') << "Fats: " << right << dailyFatGrams << " grams\n\n";

    cout << "Per meal macro targets:\n";
    cout << left << setw(50) << setfill('.') << "Protein grams per meal: " << right << proteinPerMeal << "\n";
    cout << left << setw(50) << setfill('.') << "Carbs grams per meal: " << right << carbsPerMeal << "\n";
    cout << left << setw(50) << setfill('.') << "Fats grams per meal: " << right << fatsPerMeal << "\n";

    setConsoleColor(COLOR_SUB_HEADING);
    printSectionBreakLine();
    cout << "Keep fueling with " << favoriteProtein << " to hit " << dailyProteinGrams << " grams of protein daily!\n";
    cout << "Remember your " << weeklyWorkoutHours << " hours of weightlifting per week...\n";
    cout << "Keep lifting heavy, " << name << "!\n\n";
    setConsoleColor(COLOR_DEFAULT);
}


// ---------------------------------------------------------------------------
// Function to display menu option 2.
// ---------------------------------------------------------------------------
void processReportGeneration(string name, string favoriteProtein,
    int dailyCalories, int weeklyCalories,
    int dailyProteinGrams, int dailyCarbGrams, int dailyFatGrams,
    int totalProteinGrams, int totalCarbGrams, int totalFatGrams,
    int mealsPerDay, double weeklyWorkoutHours,
    int proteinPerMeal, int carbsPerMeal, int fatsPerMeal)
{

    setConsoleColor(COLOR_SUB_HEADING);
    printSectionBreakLine();
    cout << "You chose to generate a report of your suggested " << "daily and weekly macros!\n";
    cout << "Open 'report.txt' to view, save, or print it.\n\n";
    printSectionBreakLine();
    setConsoleColor(COLOR_DEFAULT);

    generateReport(name, favoriteProtein,
        dailyCalories, weeklyCalories,
        dailyProteinGrams, dailyCarbGrams, dailyFatGrams,
        totalProteinGrams, totalCarbGrams, totalFatGrams,
        mealsPerDay, weeklyWorkoutHours,
        proteinPerMeal, carbsPerMeal, fatsPerMeal);
}


// ---------------------------------------------------------------------------
// Function to display menu option 3.
// ---------------------------------------------------------------------------
void displayNutritionCheckAndRecipe(int dailyCalories,
    int mealsPerDay, double weeklyWorkoutHours, GoalType goal)
{
    setConsoleColor(COLOR_SUB_HEADING);
    printSectionBreakLine();
    cout << "Checking your calorie intake vs activity level...\n\n";
    setConsoleColor(COLOR_DEFAULT);

    // Calories vs. activity and goal guidanec
    evaluateNutrition(dailyCalories, weeklyWorkoutHours, goal);

    // Calculate meals per week before evaluating meal frequency
    int mealsPerWeek = mealsPerDay * NUMBER_OF_DAYS;
    evaluateMealFrequency(mealsPerWeek, weeklyWorkoutHours);

    // Display recipe
    displaySmoothieRecipe();
}


// ---------------------------------------------------------------------------
// Function to display menu option 4.
// Uses arrays, loops, and array calculations.
// ---------------------------------------------------------------------------
void displayWeeklyCalorieLog(const int dailyCaloriesLog[], int size)
{
    setConsoleColor(COLOR_SUB_HEADING);
    printSectionBreakLine();
    cout << "Your Weekly Calorie Log:\n\n";
    setConsoleColor(COLOR_DEFAULT);

    cout << left << setw(10) << "Day" << right << setw(19) << "Calories\n";
    printSectionBreakLine();

    for (int day = 0; day < size; ++day)
    { 
        cout << left << setw(10) << (day + 1) << right << setw(15) << dailyCaloriesLog[day] << endl;
    }

    cout << endl << endl;

    double averageCalories = calculateAverageCalories(dailyCaloriesLog, size);
    int highestCalories = findHighestCalories(dailyCaloriesLog, size);

    cout << "Average daily calories this week: " << static_cast<int>(averageCalories) << "\n";
    cout << "Highest daily calories this week: " << highestCalories << "\n\n";

    setConsoleColor(COLOR_SUB_HEADING);
    printSectionBreakLine();
    setConsoleColor(COLOR_DEFAULT);
    cout << endl;
}



// ---------------------------------------------------------------------------
// Function to write the report for menu option 2 
//     (used in the processReportGeneration function)
// ---------------------------------------------------------------------------
void generateReport(string name, string favoriteProtein,
    int dailyCalories, int weeklyCalories,
    int dailyProteinGrams, int dailyCarbGrams, int dailyFatGrams,
    int totalProteinGrams, int totalCarbGrams, int totalFatGrams,
    int mealsPerDay, double weeklyWorkoutHours,
    int proteinPerMeal, int carbsPerMeal, int fatsPerMeal)
{
    ofstream outFile("report.txt");

    if (!outFile)
    {
        setConsoleColor(COLOR_ERROR);
        cout << "Error: Unable to open report.txt\n";
        setConsoleColor(COLOR_DEFAULT);
        return;
    }

    // Calculate macro calories per day for the report
    int dailyProteinCalories = dailyProteinGrams * CALORIES_PER_GRAM_PROTEIN;
    int dailyCarbCalories = dailyCarbGrams * CALORIES_PER_GRAM_CARBS;
    int dailyFatCalories = dailyFatGrams * CALORIES_PER_GRAM_FAT;

    // Report header
    outFile << setfill('*') << setw(CONSOLE_WIDTH) << '*' << "\n";
    outFile << "Vegetarian Nutrition for Weightlifters Report\n";
    outFile << setfill('*') << setw(CONSOLE_WIDTH) << '*' << "\n\n";

    outFile << "Information you provided:\n";
    outFile << left << setw(50) << setfill('.') << "Favorite protein source: " << right << favoriteProtein << "\n";
    outFile << left << setw(50) << setfill('.') << "Daily calorie target: " << right << dailyCalories << "\n";
    outFile << left << setw(50) << setfill('.') << "Weekly calorie target: " << right << weeklyCalories << "\n";
    outFile << left << setw(50) << setfill('.') << "Meals per day: " << right << mealsPerDay << "\n";
    outFile << fixed << showpoint << setprecision(2);
    outFile << left << setw(50) << setfill('.') << "Weekly workout hours: " << right << weeklyWorkoutHours << "\n\n";

    // Weekly totals
    outFile << "Suggested weekly macros:\n";
    outFile << left << setw(50) << setfill('.') << "Protein: " << right << totalProteinGrams << " grams\n";
    outFile << left << setw(50) << setfill('.') << "Carbs: " << right << totalCarbGrams << " grams\n";
    outFile << left << setw(50) << setfill('.') << "Fats: " << right << totalFatGrams << " grams\n\n";

    // Daily macros
    outFile << "Suggested total daily macros:\n";
    outFile << left << setw(50) << setfill('.') << "Protein: " << right << dailyProteinGrams << " grams (" << dailyProteinCalories << " cal)\n";
    outFile << left << setw(50) << setfill('.') << "Carbs: " << right << dailyCarbGrams << " grams (" << dailyCarbCalories << " cal)\n";
    outFile << left << setw(50) << setfill('.') << "Fats: " << right << dailyFatGrams << " grams (" << dailyFatCalories << " cal)\n\n";

    // Per-meal macros
    outFile << "Per meal macro targets:\n";
    outFile << left << setw(50) << setfill('.') << "Protein grams per meal: " << right << proteinPerMeal << "\n";
    outFile << left << setw(50) << setfill('.') << "Carbs grams per meal: " << right << carbsPerMeal << "\n";
    outFile << left << setw(50) << setfill('.') << "Fats grams per meal:  " << right << fatsPerMeal << "\n\n";

    outFile << setfill('*') << setw(CONSOLE_WIDTH) << '*' << "\n";
    outFile << "Keep fueling with " << favoriteProtein << " to hit " << dailyProteinGrams << " grams of protein daily!\n";
    outFile << "Remember your " << weeklyWorkoutHours << " hours of weightlifting per week...\n";
    outFile << "Keep lifting heavy, " << name << "!\n\n";

    outFile.close();

    // Confirm report generation in console using green success message
    setConsoleColor(COLOR_SUB_HEADING);
    cout << "Report successfully generated: report.txt\n\n";
    setConsoleColor(COLOR_DEFAULT);
}


// ---------------------------------------------------------------------------
// Function that uses calorie and activity thresholds to give feedback to
//     the user for menu option 3
// ---------------------------------------------------------------------------
void evaluateNutrition(int dailyCalories, double weeklyWorkoutHours, GoalType goal)
{
    if (weeklyWorkoutHours >= ACTIVITY_HIGH && dailyCalories >= CAL_HIGH)
        cout << "Your intake is sufficient for a high activity week. Keep it up!\n\n";
    else if (weeklyWorkoutHours >= ACTIVITY_MEDIUM && dailyCalories >= CAL_MEDIUM)
        cout << "Your intake is decent for your activity level, but you could increase protein slightly.\n\n";
    else if (weeklyWorkoutHours < ACTIVITY_MEDIUM && dailyCalories < CAL_LOW)
        cout << "You might be under fueling. Consider adding extra calories per day.\n\n";
    else if (weeklyWorkoutHours < ACTIVITY_MEDIUM && dailyCalories > CAL_EXCESS)
        cout << "You may be eating more than your activity requires. Consider slightly reducing calories per day.\n\n";
    else
        cout << "Your calorie intake seems balanced for your activity level.\n\n";

    // Additional guidance based on goal(enum used in switch)
    switch (goal)
    {
    case FAT_LOSS:
        cout << "Goal: Fat loss\n";
        cout << "Aim for a small, sustainable calorie deficit and prioritize protein.\n\n";
        break;
    case MAINTENANCE:
        cout << "Goal: Maintenance\n"; 
        cout << "Keep your calorie intake steady and focus on consistency.\n\n";
        break;
    case MUSCLE_GAIN:
        cout << "Goal: Muscle gain\n"; 
        cout << "Make sure you're in a slight calorie surplus and hitting your protein target.\n\n";
        break;
    default:
        break;
    }  
}


// ---------------------------------------------------------------------------
// Function that uses weekly meal count and activity level to give feedback to
//     the user for menu option 3
// ---------------------------------------------------------------------------
void evaluateMealFrequency(int mealsPerWeek, double weeklyWorkoutHours)
{
    if (mealsPerWeek >= MEALS_PER_WEEK_MEDIUM &&
        mealsPerWeek <= MEALS_PER_WEEK_HIGH &&
        weeklyWorkoutHours >= ACTIVITY_HIGH)
    {
        cout << "Excellent meal frequency for muscle recovery! Try the smoothie below!\n\n";
    }
    else if (mealsPerWeek < MEALS_PER_WEEK_MEDIUM &&
        weeklyWorkoutHours >= ACTIVITY_MEDIUM)
    {
        cout << "Consider eating more often to support your training. Try the smoothie below!\n\n";
    }
    else if (mealsPerWeek > MEALS_PER_WEEK_HIGH)
    {
        cout << "You might be eating more than necessary — ensure portion sizes are balanced. Try the smoothie below!\n\n";
    }
    else
    {
        cout << "Your meal frequency seems appropriate for your activity level. Try the smoothie below!\n\n";
    }
}


// ---------------------------------------------------------------------------
// Function to print a recipe for menu option 3
// ---------------------------------------------------------------------------
void displaySmoothieRecipe()
{
    setConsoleColor(COLOR_RECIPE);
    cout << "Oatmeal Smoothie Recipe:\n"
        << "- 1/4 cup rolled oats\n"
        << "- 2 tbsp cocoa powder\n"
        << "- 1/4 cup protein powder\n"
        << "- 1 tbsp maple syrup\n"
        << "- 1 tbsp chia seeds\n"
        << "- 1 tbsp peanut butter\n"
        << "- 3/4 cup almond milk\n\n"
        << "Blend all ingredients until smooth.\n"
        << "Protein: 30g | Carbs: 37g | Fat: 13g\n\n";
    setConsoleColor(COLOR_SUB_HEADING);
    printSectionBreakLine();
    setConsoleColor(COLOR_DEFAULT);
    cout << endl;
}


// ---------------------------------------------------------------------------
// Function to prompt the user for a string and ensures that the input is not
//     empty. If the input is invalid, the user is re-prompted.
// ---------------------------------------------------------------------------
string getValidatedStringInput(string requestInput)
{
    string value;

    cout << requestInput;
    getline(cin, value);
    cout << endl;

    // Repeat until the user enters a non-empty string
    while (value.length() == 0)
    {
        setConsoleColor(COLOR_ERROR);
        cout << "That is not a valid answer. Please try again: ";
        setConsoleColor(COLOR_DEFAULT);

        getline(cin, value);
        cout << endl;
    }

    return value;
}


// ---------------------------------------------------------------------------
// Function to prompt the user for an integer and validates that it is a
//     positive number. If input is invalid, the user is re-prompted.
// ---------------------------------------------------------------------------
int getValidatedIntInput(string requestInput)
{
    int value;

    cout << requestInput;
    cin >> value;
    cout << endl;

    // Loop continues until the user enters a valid positive integer
    while (cin.fail() || value <= 0)
    {
        clearInputStream();
        setConsoleColor(COLOR_ERROR);
        cout << "That is not a valid answer. Please enter a positive number: ";
        setConsoleColor(COLOR_DEFAULT);

        cin >> value;
        cout << endl;
    }

    clearInputStream();   // Clear newline left in the buffer
    return value;
}


// ---------------------------------------------------------------------------
// Function to prompt the user for a double and validates that it is a
//     nonnegative number. If input is invalid, the user is re-prompted.
// ---------------------------------------------------------------------------
double getValidatedDoubleInput(string requestInput)
{
    double value;

    cout << requestInput;
    cin >> value;
    cout << endl;

    // Loop continues until the user enters a valid nonnegative double
    while (cin.fail() || value < 0.0)
    {
        clearInputStream();
        setConsoleColor(COLOR_ERROR);
        cout << "That is not a valid answer. Please enter a nonnegative number: ";
        setConsoleColor(COLOR_DEFAULT);

        cin >> value;
        cout << endl;
    }

    clearInputStream();   // Clear newline left in the buffer
    return value;
}


// ---------------------------------------------------------------------------
// Function that calculates daily macro grams based on calorie ratios, per meal
//     macro targets, and weekly totals for calories and macros.
// It uses the dailyCalories and mealsPerDay inputs from the user.
// Reference parameters are used to return multiple values to usein main().
// ---------------------------------------------------------------------------
void calculateMacros(int  dailyCalories, int  mealsPerDay,
    int& dailyProteinGrams, int& dailyCarbGrams, int& dailyFatGrams,
    int& proteinPerMeal, int& carbsPerMeal, int& fatsPerMeal,
    int& weeklyCalories,
    int& totalProteinGrams, int& totalCarbGrams, int& totalFatGrams)
{
    // Daily macro grams based on percentage of calories
    dailyProteinGrams = static_cast<int>((dailyCalories * PROTEIN_PERCENT) /
        CALORIES_PER_GRAM_PROTEIN);
    dailyCarbGrams = static_cast<int>((dailyCalories * CARB_PERCENT) /
        CALORIES_PER_GRAM_CARBS);
    dailyFatGrams = static_cast<int>((dailyCalories * FAT_PERCENT) /
        CALORIES_PER_GRAM_FAT);

    // Per meal targets.
    proteinPerMeal = dailyProteinGrams / mealsPerDay;
    carbsPerMeal = dailyCarbGrams / mealsPerDay;
    fatsPerMeal = dailyFatGrams / mealsPerDay;

    // Initialize weekly totals
    weeklyCalories = 0;
    totalProteinGrams = 0;
    totalCarbGrams = 0;
    totalFatGrams = 0;

    // for loop to accumulate totals across all days of the week.
    for (int day = 1; day <= NUMBER_OF_DAYS; ++day)
    {
        weeklyCalories += dailyCalories;
        totalProteinGrams += dailyProteinGrams;
        totalCarbGrams += dailyCarbGrams;
        totalFatGrams += dailyFatGrams;
    }
}
