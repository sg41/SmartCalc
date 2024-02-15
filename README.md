# SmartCalc v2.0 Program Description

## Overview
SmartCalc v2.0 is a comprehensive C++ program designed to provide advanced mathematical functionality, including expression calculation, graph plotting, and specialized financial calculators. The program follows the C++17 standard, adheres to the Google coding style, and employs the Model-View-Controller (MVC) pattern for clear separation of concerns.

## Key Features

### 1. Expression Calculation
- Full coverage of arithmetic expressions in infix notation.
- Support for integers, real numbers (with a dot), and exponential notation.
- Verifiable accuracy of the fractional part up to 7 decimal places.
- Bracketed arithmetic expressions with a wide range of operators and mathematical functions.

### 2. Graph Plotting
- Visualization of mathematical functions given by infix expressions with the variable 'x.'
- Implementation of MVC pattern ensures separation of business logic from the view.
- Display of coordinate axes, scale markings, and an adaptive grid.
- Limited domain and codomain to numbers from -1000000 to 1000000.

### 3. User Interface
- Console-based and GUI-based interfaces using Qt libraries compatible with C++17.
- User-friendly input with support for up to 255 characters.
- Calculation triggered by pressing the '=' symbol.

### 4. MVC Pattern
- Strict adherence to the MVC pattern to maintain clean separation of concerns.
- Thin controllers with no interface code.
- No business logic in the view code.

### 5. Credit Calculator (Bonus Feature)
- Specialized mode for calculating credit-related parameters.
- Inputs: total credit amount, term, interest rate, and type (annuity or differentiated).
- Outputs: monthly payment, overpayment on credit, total payment.

### 6. Deposit Calculator (Bonus Feature)
- Specialized mode for calculating deposit profitability.
- Inputs: deposit amount, term, interest rate, tax rate, payment periodicity, interest capitalization, replenishments list, partial withdrawals list.
- Outputs: accrued interest, tax amount, deposit amount by the end of the term.

## Build System
- Utilizes a Makefile with standard GNU targets (all, install, uninstall, clean, dvi, dist, tests).
- Provides flexibility in choosing the installation directory.

## Feedback
Users are encouraged to provide feedback on their experience with the Pedago Team, aiming to improve the educational experience.
