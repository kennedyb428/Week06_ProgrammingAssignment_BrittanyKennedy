# Week06_ProgrammingAssignment_BrittanyKennedy

This C++ program helps vegetarian weightlifters plan and evaluate their daily and weekly nutrition. It provides personalized macro recommendations based on the user’s calorie goals, meal frequency, workout hours, and weekly calorie log. It also guides users through validated input prompts, generates formatted reports, and evaluates nutrition based on fitness goals using an enum.

Key Features
- Structured Menu Navigation
  + Switch-based menu with options to view macros, generate reports, check nutrition and recipes, view weekly calorie logs, or start a new session.

- Input Validation
  + Expanded validation for strings, integers, and doubles with proper buffer clearing to prevent infinite loops and ensure clean user input.

- Console Color Output
  + Uses Windows console colors to highlight headers, errors, and recipe sections, making the program easier to read and visually appealing.

- Macro & Nutrition Calculations
  + Computes daily macro grams, per-meal targets, weekly totals, and calorie/activity feedback using constants and reference parameters.

- Formatted Report Generation
  + Creates a clean, aligned report.txt file summarizing user inputs, macro calculations, and weekly totals.

- Enum Integration (New This Week)
  + A GoalType enum (Fat Loss, Maintenance, Muscle Gain) is used to personalize nutrition guidance.

- Array Integration (New This Week)
  + Stores 7 days of calorie entries in an int array, validates each input, and performs array-based calculations such as:
    - Weekly average calories
    - Highest calorie day
    - Viewing all entries in a formatted display
    - Includes multiple array-based functions:
      + fillDailyCaloriesArray, displayWeeklyCalorieLog, calculateAverageCalories, findHighestCalories.
