# Project 10: Hotel Food Ordering System
An automated, menu-driven console application built in standard C for managing hotel room food orders and kitchen tracking.

---

## 👥 Team Members & Contributions
*The project rules require each member to document their specific contributions here and make a matching commit to the repository.*

1. **Prince Munnelli (Team Leader)** * **Roll Number:** [46]
   * **Contribution:** Project setup, main menu loop, and core file handling systems (`hotelfoodsys.c`).
2. **[Puttaraj]**
   * **Roll Number:** [51]
   * **Contribution:** Implemented the multi-cuisine menu display and order placement functions.
3. **[Amrut]**
   * **Roll Number:** [03]
   * **Contribution:** Built the consolidated room bill generation and price calculation logic.
4. **[Nimit]**
   * **Roll Number:** [37]
   * **Contribution:** Developed the kitchen order summary tracking and order status transition logic.
5. **[Md.Hashmi]**
   * **Roll Number:** [31]
   * **Contribution:** Handled input validations (room numbers, order IDs) and error handling routines.

---

## 🚀 Features Implemented
This system fully satisfies all the specific constraints outlined in the project problem statement:
* **Menu-Driven Interface:** A clean, user-friendly numeric console interface.
* **Multi-Cuisine Menu:** Displays item IDs, names, prices, cuisine types, and preparation times.
* **Order Management:** Supports placing new orders with unique auto-generated Order IDs, as well as adding/canceling items from existing orders.
* **Consolidated Room Billing:** Generates comprehensive room receipts tracking all item breakdowns.
* **Kitchen Tracking:** Displays pending orders alongside preparation times and transitions status through `Received -> Preparing -> Served`.
* **Data Persistence:** Automatically stores and reads record data locally using persistent `.txt` files to preserve state across application restarts.
* **Robust Input Validation:** Restricts room numbers safely between 1–100, checks for valid existing order/item IDs, and ensures quantities remain positive.

---

## 📂 File Structure
* `hotelfoodsys.c`: The main source code containing all modular C functions.
* `orders.txt`: Local plain text data file storing active order and transaction histories.
* `HotelProject_MenuData.txt`: Sample text file maintaining the persistent food menu catalog.
* `HotelProject_BillData.txt`: Sample text file recording generated room receipts.

---

## 💻 How to Compile and Run Locally
This project is written strictly in standard C (no C++ features) and is meant to be run in a local compilation environment.

### Prerequisites
Ensure you have an approved local compiler environment set up (such as GCC, VS Code, CodeBlocks, or Dev C++). **Online compilers are strictly prohibited.**

### Compilation Steps (Using GCC)
1. Open your terminal or command prompt and navigate to the project directory.
2. Compile the source code using the following command:
   ```bash
   gcc hotelfoodsys.c -o hotelfoodsys
