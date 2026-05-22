/*******************************************************************************
 * PROJECT 10: HOTEL FOOD ORDERING SYSTEM (KEY-VALUE LOCAL LOGGING)
 * Course: Ethnotech C Training
 * Instructor: Jaswanth Narne
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MENU_ITEMS 50
#define MAX_ORDER_ITEMS 20
#define MENU_FILE "menu.txt"
#define ORDERS_FILE "orders.txt"

/* Structure Definitions */
typedef struct {
    int item_id;
    char name[40];
    char cuisine[25];
    float price;
    int prep_time;
    char category[20]; 
} MenuItem;

typedef struct {
    int item_id;
    int quantity;
} OrderItem;

typedef struct {
    int order_id;
    int room_number;
    OrderItem items[MAX_ORDER_ITEMS];
    int item_count;
    char status[15]; 
} RoomOrder;

/* Global Arrays and Counters */
MenuItem menu[MAX_MENU_ITEMS];
int menu_count = 0;

/* Function Prototypes */
void initialize_default_menu_if_missing(void);
void load_menu_from_file(void);
void load_orders_and_get_next_id(int *next_id);
void save_order_to_file(RoomOrder order);
void rewrite_all_orders_file(RoomOrder orders_list[], int total_orders);
int find_menu_item_idx(int id);

void display_left_aligned_main_menu(void);
void display_multi_cuisine_menu(void);
void print_menu_category_section(const char *cat_token, const char *display_title);
void place_new_order(int *next_id);
void add_items_to_existing_order(void);
void cancel_items_from_order(void);
void generate_consolidated_room_bill(void);
void view_kitchen_order_summary(void);
void update_order_status(void);

/* Helper Utility for Centered Sub-Menus and Invoices */
void print_centered(const char *str, int width) {
    int len = strlen(str);
    if (len >= width) {
        printf("%s\n", str);
    } else {
        int padding = (width - len) / 2;
        for (int i = 0; i < padding; i++) printf(" ");
        printf("%s", str);
        int extra = width - len - padding;
        for (int i = 0; i < extra; i++) printf(" ");
        printf("\n");
    }
}

int main(void) {
    int choice;
    int auto_generated_order_id = 5001;

    initialize_default_menu_if_missing();
    load_menu_from_file();
    load_orders_and_get_next_id(&auto_generated_order_id);

    while (1) {
        display_left_aligned_main_menu();
        printf("Enter your choice (1-8): ");
        if (scanf("%d", &choice) != 1) {
            printf("\n[ERROR] Invalid key input detected. Exiting program gracefully.\n");
            break;
        }

        switch (choice) {
            case 1:
                display_multi_cuisine_menu();
                break;
            case 2:
                place_new_order(&auto_generated_order_id);
                break;
            case 3:
                add_items_to_existing_order();
                break;
            case 4:
                cancel_items_from_order();
                break;
            case 5:
                generate_consolidated_room_bill();
                break;
            case 6:
                view_kitchen_order_summary();
                break;
            case 7:
                update_order_status();
                break;
            case 8:
                printf("\n==================================================\n");
                printf("Thank you for using our service. Goodbye!\n");
                printf("==================================================\n");
                exit(0);
            default:
                printf("\n[ERROR] Choice out of scope! Please input a digit from 1 to 8.\n");
        }
        printf("\nPress Enter to return to Main Menu...");
        getchar(); 
        getchar(); 
    }
    return 0;
}

void initialize_default_menu_if_missing(void) {
    FILE *file = fopen(MENU_FILE, "r");
    if (file != NULL) {
        fclose(file);
        return; 
    }

    file = fopen(MENU_FILE, "w");
    if (file == NULL) return;

    fprintf(file, "101;Paneer Tikka;Veg Starter;240.00;15;STARTER\n");
    fprintf(file, "102;Gobi Manchurian;Veg Starter;180.00;12;STARTER\n");
    fprintf(file, "103;Chicken Tikka;Non-Veg Starter;290.00;15;STARTER\n");
    fprintf(file, "201;Butter Paneer Masala;North Indian;260.00;18;NORTH\n");
    fprintf(file, "202;Dal Makhani;North Indian;210.00;20;NORTH\n");
    fprintf(file, "203;Kadhai Chicken;North Indian;320.00;22;NORTH\n");
    fprintf(file, "301;Masala Dosa;South Indian;90.00;10;SOUTH\n");
    fprintf(file, "302;Idli Vada Combo;South Indian;80.00;8;SOUTH\n");
    fprintf(file, "303;Hyderabadi Biryani;South Indian;280.00;20;SOUTH\n");
    fprintf(file, "401;Veg Hakka Noodles;Chinese;180.00;12;CHINESE\n");
    fprintf(file, "402;Schezwan Fried Rice;Chinese;195.00;12;CHINESE\n");
    fprintf(file, "501;Gulab Jamun (2 Pcs);Sweet;80.00;5;DESSERT\n");
    fprintf(file, "502;Sizzling Brownie;Ice Cream Mix;160.00;8;DESSERT\n");
    fprintf(file, "503;Vanilla Scoop;Ice Cream;60.00;4;DESSERT\n");

    fclose(file);
}

void load_menu_from_file(void) {
    FILE *file = fopen(MENU_FILE, "r");
    if (file == NULL) return;

    menu_count = 0;
    while (menu_count < MAX_MENU_ITEMS && 
           fscanf(file, "%d;%[^;];%[^;];%f;%d;%[^\n]\n", 
                  &menu[menu_count].item_id, 
                  menu[menu_count].name, 
                  menu[menu_count].cuisine, 
                  &menu[menu_count].price, 
                  &menu[menu_count].prep_time,
                  menu[menu_count].category) == 6) {
        menu_count++;
    }
    fclose(file);
}

void load_orders_and_get_next_id(int *next_id) {
    FILE *file = fopen(ORDERS_FILE, "r");
    if (file == NULL) return;

    int current_id;
    while (fscanf(file, "OrderID:%d,", &current_id) != EOF) {
        if (current_id >= *next_id) {
            *next_id = current_id + 1;
        }
        char line_discard[512];
        fgets(line_discard, sizeof(line_discard), file);
    }
    fclose(file);
}

int find_menu_item_idx(int id) {
    for (int i = 0; i < menu_count; i++) {
        if (menu[i].item_id == id) return i;
    }
    return -1;
}

void save_order_to_file(RoomOrder order) {
    FILE *file = fopen(ORDERS_FILE, "a");
    if (file == NULL) return;

    fprintf(file, "OrderID:%d,Room:%d,Status:%s,ItemsCount:%d", 
            order.order_id, order.room_number, order.status, order.item_count);
    for (int i = 0; i < order.item_count; i++) {
        fprintf(file, ",ItemID:%d,Qty:%d", order.items[i].item_id, order.items[i].quantity);
    }
    fprintf(file, "\n");
    fclose(file);
}

void rewrite_all_orders_file(RoomOrder orders_list[], int total_orders) {
    FILE *file = fopen(ORDERS_FILE, "w");
    if (file == NULL) return;

    for (int i = 0; i < total_orders; i++) {
        fprintf(file, "OrderID:%d,Room:%d,Status:%s,ItemsCount:%d", 
                orders_list[i].order_id, orders_list[i].room_number, orders_list[i].status, orders_list[i].item_count);
        for (int j = 0; j < orders_list[i].item_count; j++) {
            fprintf(file, ",ItemID:%d,Qty:%d", orders_list[i].items[j].item_id, orders_list[i].items[j].quantity);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void display_left_aligned_main_menu(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    printf("==================================================\n");
    printf("           HOTEL FOOD ORDERING SYSTEM             \n");
    printf("==================================================\n");
    printf(" 1. Display Multi-Cuisine Menu\n");
    printf(" 2. Place New Order\n");
    printf(" 3. Add Items to Existing Order\n");
    printf(" 4. Cancel Items from Order\n");
    printf(" 5. Generate Consolidated Room Bill\n");
    printf(" 6. View Kitchen Order Summary\n");
    printf(" 7. Update Order Status\n");
    printf(" 8. Exit\n");
    printf("==================================================\n");
}

void print_menu_category_section(const char *cat_token, const char *display_title) {
    int section_has_items = 0;
    for (int i = 0; i < menu_count; i++) {
        if (strcmp(menu[i].category, cat_token) == 0) {
            section_has_items = 1;
            break;
        }
    }
    if (!section_has_items) return;

    printf("\n---------------------------------------------------------------------------\n");
    print_centered(display_title, 75);
    printf("---------------------------------------------------------------------------\n");
    printf("  %-10s %-22s %-15s %-12s %-10s\n", "ID", "Item Name", "Cuisine", "Price(Rs)", "Prep(mins)");

    for (int i = 0; i < menu_count; i++) {
        if (strcmp(menu[i].category, cat_token) == 0) {
            printf(" %-10d %-22s %-15s %-12.2f %-10d\n", 
                   menu[i].item_id, menu[i].name, menu[i].cuisine, menu[i].price, menu[i].prep_time);
        }
    }
}

void display_multi_cuisine_menu(void) {
    printf("\n===========================================================================\n");
    print_centered("HOTEL ROOM SERVICE MENU", 75);
    printf("===========================================================================\n");
    
    print_menu_category_section("STARTER", "STARTERS");
    print_menu_category_section("NORTH", "NORTH INDIAN");
    print_menu_category_section("SOUTH", "SOUTH INDIAN");
    print_menu_category_section("CHINESE", "CHINESE");
    print_menu_category_section("DESSERT", "DESSERTS (SWEETS, ICE CREAMS & BEVERAGES)");
    
    printf("===========================================================================\n");
}

void place_new_order(int *next_id) {
    RoomOrder new_order;
    new_order.order_id = *next_id;
    new_order.item_count = 0;
    strcpy(new_order.status, "Received");

    printf("\n--- PLACING NEW ORDER ---\n");
    
    while (1) {
        printf("Enter Room Number (1-100): ");
        if (scanf("%d", &new_order.room_number) != 1) {
            printf("[ERROR] Numeric input only.\n");
            while (getchar() != '\n'); 
            continue;
        }
        if (new_order.room_number >= 1 && new_order.room_number <= 100) break;
        printf("[ERROR] Invalid Room Number! Value must be between 1 and 100.\n");
    }

    display_multi_cuisine_menu();
    while (new_order.item_count < MAX_ORDER_ITEMS) {
        int item_id, qty;
        printf("Enter Item ID to add (or 0 to complete selection): ");
        scanf("%d", &item_id);
        if (item_id == 0) break;

        int menu_idx = find_menu_item_idx(item_id);
        if (menu_idx == -1) {
            printf("[ERROR] Item ID missing from menu records. Try again.\n");
            continue;
        }

        printf("Enter Quantity: ");
        scanf("%d", &qty);
        if (qty <= 0) {
            printf("[ERROR] Quantity must be a positive integer.\n");
            continue;
        }

        int existing_idx = -1;
        for (int i = 0; i < new_order.item_count; i++) {
            if (new_order.items[i].item_id == item_id) {
                existing_idx = i;
                break;
            }
        }

        if (existing_idx != -1) {
            new_order.items[existing_idx].quantity += qty;
        } else {
            new_order.items[new_order.item_count].item_id = item_id;
            new_order.items[new_order.item_count].quantity = qty;
            new_order.item_count++;
        }
        printf("Added successfully: %s x %d\n\n", menu[menu_idx].name, qty);
    }

    if (new_order.item_count > 0) {
        save_order_to_file(new_order);
        printf("\n[SUCCESS] Order placed successfully! Forwarded to the kitchen.\n");
        printf("Your allocated Order ID tracking number is: %d\n", new_order.order_id);
        (*next_id)++;
    } else {
        printf("\n[CANCELLED] No items selected.\n");
    }
}

void add_items_to_existing_order(void) {
    RoomOrder orders[100];
    int total_orders = 0, search_id, found_idx = -1;

    printf("\n--- ADD ITEMS TO EXISTING ORDER ---\n");
    printf("Enter Order ID to modify: ");
    scanf("%d", &search_id);

    FILE *file = fopen(ORDERS_FILE, "r");
    if (file == NULL) {
        printf("[ERROR] No existing order records found.\n");
        return;
    }

    while (fscanf(file, "OrderID:%d,Room:%d,Status:%[^,],ItemsCount:%d", 
                  &orders[total_orders].order_id, &orders[total_orders].room_number, 
                  orders[total_orders].status, &orders[total_orders].item_count) != EOF) {
        for (int i = 0; i < orders[total_orders].item_count; i++) {
            fscanf(file, ",ItemID:%d,Qty:%d", &orders[total_orders].items[i].item_id, &orders[total_orders].items[i].quantity);
        }
        fscanf(file, "\n");
        if (orders[total_orders].order_id == search_id) found_idx = total_orders;
        total_orders++;
    }
    fclose(file);

    if (found_idx == -1) {
        printf("[ERROR] Reference Order ID not found.\n");
        return;
    }

    if (strcmp(orders[found_idx].status, "Served") == 0) {
        printf("[ERROR] Cannot modify. Order is already served.\n");
        return;
    }

    display_multi_cuisine_menu();
    int curr_count = orders[found_idx].item_count;
    
    while (curr_count < MAX_ORDER_ITEMS) {
        int item_id, qty;
        printf("Enter Item ID to append (or 0 to finish): ");
        scanf("%d", &item_id);
        if (item_id == 0) break;

        int menu_idx = find_menu_item_idx(item_id);
        if (menu_idx == -1) {
            printf("[ERROR] Item validation failed!\n");
            continue;
        }

        printf("Enter Quantity: ");
        scanf("%d", &qty);
        if (qty <= 0) continue;

        int matched = 0;
        for (int i = 0; i < curr_count; i++) {
            if (orders[found_idx].items[i].item_id == item_id) {
                orders[found_idx].items[i].quantity += qty;
                matched = 1;
                break;
            }
        }
        if (!matched) {
            orders[found_idx].items[curr_count].item_id = item_id;
            orders[found_idx].items[curr_count].quantity = qty;
            curr_count++;
        }
        printf("Appended: %s\n", menu[menu_idx].name);
    }

    orders[found_idx].item_count = curr_count;
    rewrite_all_orders_file(orders, total_orders);
    printf("[SUCCESS] Items added successfully to Order %d.\n", search_id);
}

void cancel_items_from_order(void) {
    RoomOrder orders[100];
    int total_orders = 0, search_id, found_idx = -1;

    printf("\n--- CANCEL ITEMS FROM EXISTING ORDER ---\n");
    printf("Enter Order ID to locate: ");
    scanf("%d", &search_id);

    FILE *file = fopen(ORDERS_FILE, "r");
    if (file == NULL) return;

    while (fscanf(file, "OrderID:%d,Room:%d,Status:%[^,],ItemsCount:%d", 
                  &orders[total_orders].order_id, &orders[total_orders].room_number, 
                  orders[total_orders].status, &orders[total_orders].item_count) != EOF) {
        for (int i = 0; i < orders[total_orders].item_count; i++) {
            fscanf(file, ",ItemID:%d,Qty:%d", &orders[total_orders].items[i].item_id, &orders[total_orders].items[i].quantity);
        }
        fscanf(file, "\n");
        if (orders[total_orders].order_id == search_id) found_idx = total_orders;
        total_orders++;
    }
    fclose(file);

    if (found_idx == -1) {
        printf("[ERROR] Requested Order sequence record does not exist.\n");
        return;
    }

    if (strcmp(orders[found_idx].status, "Served") == 0) {
        printf("[ERROR] Cannot cancel items. Meal has already been served.\n");
        return;
    }

    printf("\nCurrent active entries under Order %d:\n", search_id);
    for (int i = 0; i < orders[found_idx].item_count; i++) {
        int m_idx = find_menu_item_idx(orders[found_idx].items[i].item_id);
        if (m_idx != -1) {
            printf("  -> Item ID: %d | %-20s | Qty: %d\n", menu[m_idx].item_id, menu[m_idx].name, orders[found_idx].items[i].quantity);
        }
    }

    int cancel_id, cancel_qty;
    printf("\nEnter Item ID to reduce: ");
    scanf("%d", &cancel_id);

    int target_item_idx = -1;
    for (int i = 0; i < orders[found_idx].item_count; i++) {
        if (orders[found_idx].items[i].item_id == cancel_id) {
            target_item_idx = i;
            break;
        }
    }

    if (target_item_idx == -1) {
        printf("[ERROR] Item ID not inside this order.\n");
        return;
    }

    printf("Enter quantity to deduct: ");
    scanf("%d", &cancel_qty);

    if (cancel_qty >= orders[found_idx].items[target_item_idx].quantity) {
        for (int i = target_item_idx; i < orders[found_idx].item_count - 1; i++) {
            orders[found_idx].items[i] = orders[found_idx].items[i + 1];
        }
        orders[found_idx].item_count--;
    } else {
        orders[found_idx].items[target_item_idx].quantity -= cancel_qty;
    }

    rewrite_all_orders_file(orders, total_orders);
    printf("[SUCCESS] Order adjustments processed successfully.\n");
}

void generate_consolidated_room_bill(void) {
    int search_id, found = 0;
    RoomOrder order;

    printf("\nEnter Order ID for Invoice Generation: ");
    scanf("%d", &search_id);

    FILE *file = fopen(ORDERS_FILE, "r");
    if (file == NULL) return;

    while (fscanf(file, "OrderID:%d,Room:%d,Status:%[^,],ItemsCount:%d", 
                  &order.order_id, &order.room_number, order.status, &order.item_count) != EOF) {
        for (int i = 0; i < order.item_count; i++) {
            fscanf(file, ",ItemID:%d,Qty:%d", &order.items[i].item_id, &order.items[i].quantity);
        }
        fscanf(file, "\n");
        if (order.order_id == search_id) {
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("[ERROR] No matching Order ID tracked down.\n");
        return;
    }

    float subtotal = 0;
    char meta_header[60];

    printf("\n==================================================\n");
    print_centered("HOTEL INVOICE", 50);
    printf("==================================================\n");
    sprintf(meta_header, "Order ID: %-10d          Room: %d", order.order_id, order.room_number);
    print_centered(meta_header, 50);
    sprintf(meta_header, "Current Status: %s", order.status);
    print_centered(meta_header, 50);
    printf("--------------------------------------------------\n");
    printf("  %-18s %-5s %-10s %-10s\n", "Item Name", "Qty", "Unit Price", "Total");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < order.item_count; i++) {
        int m_idx = find_menu_item_idx(order.items[i].item_id);
        if (m_idx != -1) {
            float cost = menu[m_idx].price * order.items[i].quantity;
            subtotal += cost;
            printf("  %-18.18s %-5d %-10.2f %-10.2f\n", 
                   menu[m_idx].name, order.items[i].quantity, menu[m_idx].price, cost);
        }
    }

    float cgst = subtotal * 0.025f;  
    float sgst = subtotal * 0.025f;  
    float service_charge = subtotal * 0.05f; 
    float grand_total = subtotal + cgst + sgst + service_charge;

    printf("--------------------------------------------------\n");
    printf("  %-33s Rs. %7.2f\n", "Subtotal:", subtotal);
    printf("  %-33s Rs. %7.2f\n", "CGST (2.5%):", cgst);
    printf("  %-33s Rs. %7.2f\n", "SGST (2.5%):", sgst);
    printf("  %-33s Rs. %7.2f\n", "Service Charge (5%):", service_charge);
    printf("--------------------------------------------------\n");
    printf("  %-33s Rs. %7.2f\n", "GRAND TOTAL:", grand_total);
    printf("==================================================\n");
}

void view_kitchen_order_summary(void) {
    RoomOrder order;
    int pending_flag = 0;

    FILE *file = fopen(ORDERS_FILE, "r");
    if (file == NULL) {
        printf("\n[INFO] No active orders logged for kitchen review yet.\n");
        return;
    }

    printf("\n==================================================\n");
    print_centered("KITCHEN ORDER SUMMARY", 50);
    printf("==================================================\n");

    while (fscanf(file, "OrderID:%d,Room:%d,Status:%[^,],ItemsCount:%d", 
                  &order.order_id, &order.room_number, order.status, &order.item_count) != EOF) {
        for (int i = 0; i < order.item_count; i++) {
            fscanf(file, ",ItemID:%d,Qty:%d", &order.items[i].item_id, &order.items[i].quantity);
        }
        fscanf(file, "\n");

        if (strcmp(order.status, "Served") != 0) {
            pending_flag = 1;
            printf(" Order ID: %-5d | Room: %-3d | Status: [%s]\n", order.order_id, order.room_number, order.status);
            printf(" Items Queue:\n");
            int total_eta = 0;
            
            for (int i = 0; i < order.item_count; i++) {
                int m_idx = find_menu_item_idx(order.items[i].item_id);
                if (m_idx != -1) {
                    printf("    * %s (Qty: %d)\n", menu[m_idx].name, order.items[i].quantity);
                    if (menu[m_idx].prep_time > total_eta) total_eta = menu[m_idx].prep_time;
                }
            }
            printf(" Estimated Prep Block: %d mins\n", total_eta);
            printf("--------------------------------------------------\n");
        }
    }
    fclose(file);

    if (!pending_flag) {
        print_centered("All caught up! No pending food orders.", 50);
        printf("==================================================\n");
    }
}

void update_order_status(void) {
    RoomOrder orders[100];
    int total_orders = 0, search_id, found_idx = -1;

    printf("\n--- UPDATE ORDER STATUS INTERACTION ---\n");
    printf("Enter Target Order ID: ");
    scanf("%d", &search_id);

    FILE *file = fopen(ORDERS_FILE, "r");
    if (file == NULL) return;

    while (fscanf(file, "OrderID:%d,Room:%d,Status:%[^,],ItemsCount:%d", 
                  &orders[total_orders].order_id, &orders[total_orders].room_number, 
                  orders[total_orders].status, &orders[total_orders].item_count) != EOF) {
        for (int i = 0; i < orders[total_orders].item_count; i++) {
            fscanf(file, ",ItemID:%d,Qty:%d", &orders[total_orders].items[i].item_id, &orders[total_orders].items[i].quantity);
        }
        fscanf(file, "\n");
        if (orders[total_orders].order_id == search_id) found_idx = total_orders;
        total_orders++;
    }
    fclose(file);

    if (found_idx == -1) {
        printf("[ERROR] Order target record trace dropped.\n");
        return;
    }

    printf("Current Status: [%s]\n", orders[found_idx].status);
    printf("Select Target Pipeline Level:\n");
    printf("1. Preparing\n");
    printf("2. Served\n");
    printf("Enter choice selection (1-2): ");
    
    int opt;
    scanf("%d", &opt);
    if (opt == 1) {
        strcpy(orders[found_idx].status, "Preparing");
    } else if (opt == 2) {
        strcpy(orders[found_idx].status, "Served");
    } else {
        printf("[ERROR] Action cancelled. Invalid index.\n");
        return;
    }

    rewrite_all_orders_file(orders, total_orders);
    printf("[SUCCESS] Status shifted successfully to: %s\n", orders[found_idx].status);
}