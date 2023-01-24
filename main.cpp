#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;

void check_balance(float balance){
    cout << "Your current balance is: Rs " << balance << endl;
}

void write_transaction(string type, float amount, float balance){
    ofstream transaction_file;
    transaction_file.open("transactions.txt", ios::app);
    time_t now = time(0);
    char* dt = ctime(&now);
    transaction_file << dt << " | " << type << " | " << amount << " | " << balance << endl;
    transaction_file.close();
}

void withdraw_money(float &balance){
    float withdraw_amount;
    cout<<"Enter the amount you would like to withdraw:";    cin >> withdraw_amount;

    if(withdraw_amount > balance){
        cout << "Insufficient funds. Please enter a lower amount." << endl;
    }
    else{
        balance -= withdraw_amount;
        cout << "Your new balance is: RS" << balance << endl;
        write_transaction("Withdraw", withdraw_amount, balance);
    }
}

void transfer_money(float balance){
    float transfer_amount;
    string recipient_card_no;
    cout<<"Enter the amount you would like to transfer: $\";\n";    cin >> transfer_amount;
    cout <<"Enter the card number of the recipient:";   cin >>recipient_card_no;

    if(transfer_amount > balance){
        cout<< "Insufficient funds. Please enter a lower amount." << endl;
    }
    else{
        balance -= transfer_amount;
        cout << "Transfer of $" << transfer_amount << " to card number " << recipient_card_no << " was successful." << endl;
        cout << "Your new balance is: Rs " << balance << endl;
        write_transaction("Transfer", transfer_amount, balance);
    }
}

void view_transactions(){
    ifstream transaction_file("transactions.txt");
    if(transaction_file.is_open()){
        cout << "Time | Type | Amount | Balance" << endl;
        string line;
        int count = 0;
        while(getline(transaction_file, line) && count < 10){
            cout << line << endl;
            count++;
        }
    }
    else{
        cout << "No transactions found." << endl;
    }
}
void menu(float balance){
    while(1){
        char choise;
        char r;
    cout<<"1. Check Balance"<<endl;
    cout<<"2. WithDraw Money"<<endl;
    cout<<"3. Transfer Money"<<endl;
    cout<<"4. View Last 10 Transactions"<<endl;
    cout<<"5. Exit"<<endl;    cin>>choise;
        switch(choise){
            case '1':
                check_balance(balance);
                break;
            case '2':
                withdraw_money(balance);
                break;
            case '3':
                transfer_money(balance);
                break;
            case '4':
                view_transactions();
                break;
            case '5':
                r ='e';
                break;
        }
        if(r =='e'){
            break;
        }
    }
}
string card_no(){
    string card;
    cout<<"Enter your Card No.:";
    cin>>card;
    return card;
}

string pin(){
    string pin;
    cout<<"Enter your Pin:";
    cin>>pin;
    return pin;
}

int main()
{
    int login_attempts = 0;
    string card_no_entered = card_no();
    string pin_entered = pin();
    while(login_attempts < 3){
        float balance = 0;
        string name;
        ifstream afile ("account.csv");
        string card;
        bool card_en = false;
        bool pin_en =true;
        bool matched=false;
        while (getline(afile, card)) {
            istringstream iss(card);
            string field;
            int field_count = 0;
            while (getline(iss, field, ',')){
                if(card_no_entered == field){
                    card_en = true;
                    matched=true;
                    field_count++;
                    continue;
                }
                else if( pin_entered == field && field_count == 1 && card_en ){
                    pin_en= true;
                    field_count++;
                    continue;
                }
                else if(field_count== 2 && card_en && pin_en){
                    name = field;
                    field_count++;
                    continue;
                }
                else if(field_count == 3 && card_en && pin_en){
                    balance = stof(field);
                    field_count++;
                }
                else if(!matched){
                    break;
                }
            }   
        }
        if(balance > 0 ){
            cout << "Welcome " << name << "!" << endl;
            write_transaction("Login", 0, 0);
            menu(balance);
            break;
        }
        else{
            login_attempts++;
            cout << "Invalid card number or pin. Please try again. Attempts remaining: " << (3 - login_attempts) << endl;
            card_no_entered = card_no();
            pin_entered = pin();
        }
    }
    if(login_attempts >= 3){
        write_transaction("Failed Login", 0, 0);
        cout << "Too many login attempts. Your card has been locked." << endl;
    }
    return 0;
}
