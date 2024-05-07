// Jayden Sastradi
// p5x.cpp
// Purpose : This program is a inventory checking software where the user
// inputs a file to be read in order to print out one of two files. The
// first file is an overview of all the parts that are available while
// displaying the different parts available and the total value of all the
//parts.
// The second part of the program is another report which shows the user
// the items that need to be restocked because the amount of the items is less
// than the minimum quantity, the total amount of items needed to be purchased
// and the total price of the items.

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// Inventory Struct for the Inventory item
struct Inventory
{
  string itemName;
  string manufacturer;
  int quantity;
  int minimum;
  double price;
};
const int INVALID = -1;
const int PRNTINV = 1;
const int PRNTREORDER = 2;
const int ADDITEM = 3;
const int QUIT = 4;
const int MENUMAX = 4;
const int MENUMIN = 1;
const int MAX = 150;
const int SPACES = 25;
const int STRINGLEN = 20;
const int NUMLEN = 7;
const int EXTRANUMLEN = 9;
const int TITLESPACE = 15;
const char YES = 'y';
const int DECIMALP = 2;
const int SCREEN_HEIGHT = 20;


void clearScreen();

void centerTitle();

// function for a welcome message
void welcome();

// function for requesting file name
string fileInput();

// Function for reading the file that is given by the user
bool fileRead(Inventory list[], string fName, int& numE);

// Printing function to show all the items in the given file
void inventoryReport(Inventory list[], int numE);

// Prints out the items that are less than the 
void reorderReport(Inventory list[], int numE);

void goodbye();

// function that sorts the item name in alphabetic order
void selectionSort(Inventory list[], int numE);

// searching function for the addItem menu selection, it searches for the item the user wants to add
// if the item is already in the list it will allow the user to add it to the existing item and if it
// isn't then the program will allows the user to add a new item to the list
int binarySearch(Inventory list[], int numE, string target);

// swap function for the selectionSort function
void swapItem(Inventory& item1, Inventory& item2);

// function that takes a user input and writes it into a file
bool fileWrite(Inventory list[], int numE, Inventory nItem);

int main()
{
  Inventory items[MAX];
  Inventory newItem;
  newItem.price = -1;
  int count = 0;
  int choice = -1;
  string fileName;
  bool running = false;
  char repeat = YES;
  int location;
  char response;
  string itemToAdd;
  int addAmount;
  
  welcome();
  fileName = fileInput();
  running = fileRead(items, fileName, count);
  if(!running){
    cout << "\nThere was a problem with the file read. "
         << "\nPlease press enter to return to the menu . . . ";
    cin.get();
    return 0;
  }
  while (tolower(repeat) == YES){
    selectionSort(items, count);
    cout << "\nMenu\n" << "(1) Inventory report " << "\n(2) Reorder Report";
    cout << "\n(3) Adding listing \n(4) Exit Program";
    cout << "\nSelect an option ";
    cin >> choice;
    while(choice < MENUMIN || choice > MENUMAX){
      cout << "\nPlease select a choice between " << MENUMIN << " and "
           << MENUMAX << " ";
      cin >> choice;
    }
    if (choice == PRNTINV){
      clearScreen();
      centerTitle();
      inventoryReport(items, count);
      cin.get();
    }
    else if (choice == PRNTREORDER){
      clearScreen();
      centerTitle();
      reorderReport(items, count);
      cin.get();
      cin.get();
    }
    else if (choice == ADDITEM){
      clearScreen();
      
      cout << "\nWhat is the item name you want to add"
           << "(Capitalize the first letter)";
      cin >> itemToAdd;
      location = binarySearch(items, count, itemToAdd);
      
      if(location > -1){
        cout << "The Item exists in the database!";
        cout << "\nHow much of the item to you have? ";
        cin >> addAmount;
        items[location].quantity += addAmount;
      }
      else{
        cout << "\nThis item was not in the Inventory, would you like to add"
             << " it? (y/n) ";
        cin >> response;
        if (response == YES){
          newItem.itemName = itemToAdd;
          cout << "\nWhat is the manufacturer? ";
          cin >> newItem.manufacturer;
          cout << "\nWhat is the quantity of the item? ";
          cin >> newItem.quantity;
          cout << "\nWhat is the minimum quantity of the item? ";
          cin >> newItem.minimum;
          cout << "\nWhat is the cost of each item? ";
          cin >> newItem.price;
        }
        else
          cout << "\nThis item will not be written in";
      }
      
    }
    else{
      running = fileWrite(items, count, newItem);
      fileWrite(items, count, newItem);
      if(!running){
        cout << "\nThere was an error writing the file"
             << " please press enter to leave . . .";
        cin.get();
        return 0;
      }
      else{
        goodbye();
        return 0;
      }
    }
  }
}

void reorderReport(Inventory list[], int numE)
{
  int count = 0;
  int itemTotal = 0;
  double totalCost = 0;
  cout << "Reorder Report\n\n";
  cout << left << setw(STRINGLEN) << "Item" << left
       << setw(STRINGLEN) << "Manufacturer"
       << right << setw(NUMLEN) << " Quantity "
       << right << setw(NUMLEN) << " Minimum "
       << right << setw(NUMLEN) << " Order  "
       << right << setw(NUMLEN) << "  Order Cost" << "\n";

  for (int i = 0; i < numE;){
    if (list[i].quantity < list[i].minimum){
      int reorderAmount = 0;
      reorderAmount = list[i].minimum - list[i].quantity;
      cout << left << setw(STRINGLEN) << list[i].itemName << left
           << setw(STRINGLEN) << list[i].manufacturer
           << right << setw(NUMLEN) << list[i].quantity
           << right << setw(EXTRANUMLEN) << list[i].minimum
           << right << setw(EXTRANUMLEN) << reorderAmount
           << right << setw(NUMLEN) << "$"
           << fixed << showpoint << setw(NUMLEN) << setprecision(DECIMALP)
           << reorderAmount * list[i].price << "\n";
      count++;
      itemTotal += reorderAmount;
      totalCost += reorderAmount * list[i].price;
      i++;
    }
    else
      i++;
  }
  cout << "\nNumber of different items to order: " << count;
  cout << "\nTotal number to order: " << itemTotal;
  cout << "\nTotal cost: $" << totalCost;
  cout << "\n\nPress enter to continue . . .";
  return;
}
void inventoryReport(Inventory list[], int numE)
{
  int totalQuantity = 0;
  double totalValue = 0;
  cout << "Inventory Report\n\n";  
  cout << left << setw(STRINGLEN) << "Item" << left
       << setw(STRINGLEN) << "Manufacturer"
       << right << setw(NUMLEN) << " Quantity "
       << right << setw(NUMLEN) << " Cost "
       << setw(NUMLEN) << " "
       << right << setw(NUMLEN) << " Total Value" << "\n";

  for (int i = 0 ; i < numE; i++){
    cout << left << setw(STRINGLEN) << list[i].itemName << left
         << setw(STRINGLEN) << list[i].manufacturer
         << right << setw(NUMLEN) << list[i].quantity
         << right << setw(NUMLEN) << "$"
         << fixed << showpoint << setprecision(DECIMALP) << setw(NUMLEN)
         << list[i].price
         << right << setw(NUMLEN) << "$"
         << fixed << showpoint << setw(NUMLEN) << setprecision(DECIMALP)
         << list[i].quantity * list[i].price << "\n";

    totalQuantity += list[i].quantity;  
    totalValue += list[i].quantity * list[i].price;
    if (i > 0 && i% SCREEN_HEIGHT == 0){
      cout << "\nPlease press enter to continue . . .";
      cin.get();
      cin.get();
    }
  }
  cout << "\n\nNumber of different items: " << numE;
  cout << "\nTotal quantity: " << totalQuantity;
  cout << "\nTotal Value of all items: $" << totalValue;
  cout << "\nPlease press Enter to continue . . . ";

}
void welcome()
{
  cout << "\n\n\n\n"
       << "Welcome to Everything Electronic's Inventory page!";
  cout << "\nHere you can view the total invetory of a file,"
       << "\nOr you can view the items that need to be reordered!";
  return;
}

string fileInput()
{
  string file;
  cout << "\nWhat is the file you would like to scan? ";
  cin >> file;

  return file;

}
bool fileRead(Inventory list[], string fName, int& numE)
{
  ifstream infile;
  infile.open(fName);
  if (infile.fail()){
    return false;
  }
  else
    while (infile >> list[numE].itemName){
      infile >> list[numE].manufacturer >> list[numE].quantity
             >> list[numE].minimum >> list[numE].price;
      numE++;
    }
  infile.close();
  return true;
}
void clearScreen()
{
  for (int i = 0; i < SPACES; i++){
    cout << "\n";
  }
  return;
}


void centerTitle()
{
  for(int i = 0; i < TITLESPACE; i++){
    cout << "  ";
  }
  return;
}

void goodbye()
{
  cout << "\nThank you for using the Inventory management program!"
       << "\n\n\n\n";
  return;
}
void selectionSort(Inventory list[], int numE)
{
  int min = 0;
  for(int i = 0; i < numE; i++){
    min = i;
    for(int j = i+1; j < numE; j++){
      if (list[j].itemName < list[min].itemName)
        min = j;
    }
    if (min !=i){
      swapItem(list[min],list[i]);
    }
  }
  return;
}

void swapItem(Inventory& item1, Inventory& item2)
{
  Inventory temp;
  temp = item1;
  item1 = item2;
  item2 = temp;
  
  return;
}


int binarySearch(Inventory list[], int numE, string target)
{
  string guess;
  int high = numE;
  int low = 0;
  while (low <= high){
    int mid =  (low + high)/2;

    if (list[mid].itemName == target)
      return mid;
    else if (list[mid].itemName < target)
      low = mid + 1;
    else
      high = mid - 1;
  }
  return -1;

}
bool fileWrite(Inventory list[], int numE, Inventory nItem)
{
  ofstream outfile;
  outfile.open("p5xdatabase.dat");
  if(outfile.fail()){
    return false;
  }
  else{
    
    for(int i = 0; i < numE; i++){
      outfile << list[i].itemName << " " << list[i].manufacturer << " "
              << list[i].quantity << " " << list[i].minimum << " "
              << list[i].price << "\n" ;
    }
    if (nItem.price == INVALID){
    }
    
    else{
      outfile << nItem.itemName << " " << nItem.manufacturer << " "
              << nItem.quantity << " " << nItem.minimum << " "
              << nItem.price << "\n" ;
    }
  }
  outfile.close();
  return true;
}
