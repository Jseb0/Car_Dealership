
# Car Dealership Management System

## Project Overview

This project implements a **Car Dealership Management System** that allows users to:
- View available cars and their details.
- Purchase cars by selecting a model and applying a discount code.
- Store sales and feedback data.
- View sales data and feedback for each car.

The system is built in **C** and uses **file handling** to store sales and customer feedback persistently. This project allows users to interact with the system through a **command-line interface** (CLI).

---

## Features

### 1. **View Cars**
Users can view a list of available cars, including their brand, model, year, price, and stock availability.

### 2. **Buy Cars**
Users can:
- Select a car brand and model.
- Enter customer details (name, age).
- Apply a discount code (`CAR20`) for 20% off the car price.
- Purchase the car, which updates the stock and saves the transaction.

### 3. **Sales Data**
Users can view the sales data by brand, including the total cars sold and the total price of sold cars.

### 4. **Feedback System**
After purchasing a car, users can provide feedback and rate their purchase on a scale of 1-5. This feedback is saved for later viewing.

### 5. **Persistent Storage**
- **Sales data** is saved in the `sales_data.txt` file.
- **Feedback data** is stored in the `feedback_data.txt` file.

---

## Requirements

- **C Compiler** (e.g., GCC)
- **CMake** (for building the project)

---

## How to Compile and Run

### 1. Clone the Repository

```bash
git clone https://github.com/your-username/Car-Dealership-System.git
cd Car-Dealership-System
```

### 2. Build the Project with CMake

```bash
mkdir build
cd build
cmake ..
make
```

### 3. Run the Program

```bash
./car_dealership
```

---

## Project Structure

```bash
/Car-Dealership-System
├── main.c                    # Main program code
├── CMakeLists.txt            # CMake build configuration
├── sales_data.txt            # Sales data (persisted after car purchase)
├── feedback_data.txt         # Customer feedback (persisted after car purchase)
├── README.md                 # Project description and instructions
└── /src                       # Optional: Source files if modularized
```

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## Contributing

If you would like to contribute to this project, please fork the repository, create a new branch, make your changes, and submit a pull request.

---
