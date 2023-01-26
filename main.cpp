#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;
void update_balance(string,string,float);

void check_balance(float balance){
    system("cls");
    cout<<endl<<endl;
    cout << "Your current balance is: Rs " << balance << endl;
    cout<<"Press any key to return to main menu...";
    _getch();
}

void write_transaction(string type, string name , string card_no , float amount, float balance){
    string file_name = card_no + ".txt";
    ofstream transaction_file;
    transaction_file.open(file_name , ios::app);
    time_t now = time(0);
    char* dt = ctime(&now);
    transaction_file << dt <<endl<<"          " <<card_no << " | " << name <<  " | " << type << " | " <<amount << " | " << balance << endl;
    transaction_file.close();
}
void withdraw_money(string card_no , string name ,  float balance){
    system("cls");
    float withdraw_amount;
    cout<<endl;
    cout<<"Enter the amount you would like to withdraw: ";    cin >> withdraw_amount;

    if(withdraw_amount > balance){
        cout <<endl<<endl<< "Insufficient funds. Please enter a lower amount." << endl<<endl;
    }
    else{
        balance -= withdraw_amount;
        cout <<endl<< "Withdraw Successful";
        cout <<endl<< "Your new balance is: RS" << balance << endl;
        update_balance(name , card_no , balance);
        write_transaction("Withdraw", name , card_no , withdraw_amount, balance);
        cout<<"Press any key to return to main menu";
        _getch();
    }
}

void transfer_money(string card_no , string name ,  float balance){
    float transfer_amount;
    string recipient_card_no;
    cout<<endl<<"Enter the amount you would like to transfer: Rs ";    cin >> transfer_amount;
    cout<<endl <<"Enter the card number of the recipient: ";   cin >>recipient_card_no;

    if(transfer_amount > balance){
        cout<< "Insufficient funds. Please enter a lower amount." << endl;
    }
    else{
        balance -= transfer_amount;
        cout<<endl<<"Transfer of Rs " << transfer_amount << " to card number " << recipient_card_no << " was successful." << endl;
        cout <<endl<< "Your new balance is: Rs " << balance << endl;
        update_balance(name , card_no , balance);
        write_transaction("Transfer", name , card_no , transfer_amount, balance);
    }
}
void update_balance( string name_arg , string card_no_arg , float amount){
        ifstream file("account.csv");
        ofstream temp("temp.csv");
        string line;
        while (getline(file, line))
        {
            string card_no;
            string pin;
            string name;
            int balance;

            stringstream ss(line);
            getline(ss, card_no, ',');
            getline(ss, pin, ',');
            getline(ss, name, ',');
            ss >> balance;

            if (card_no == card_no_arg)
            {
                balance = amount;
            }
            temp<<card_no << "," << pin << "," << name << "," << balance <<endl;
        }
        file.close();
        temp.close();
        ifstream tempa("temp.csv");
        ofstream filea("account.csv");
        string linea;
        while (getline(tempa, linea))
        {
            string card_no;
            string pin;
            string name;
            int balance;

            stringstream ss(linea);
            getline(ss, card_no, ',');
            getline(ss, pin, ',');
            getline(ss, name, ',');
            ss >> balance;
            filea<<card_no << "," << pin << "," << name << "," << balance <<endl;
        }
        file.close();
        temp.close();

        
}

void view_transactions(string card_no){
    system("cls");
    cout<<endl<<endl;
    string file_name = card_no +".txt";
    ifstream transaction_file(file_name);
    if(transaction_file.is_open()){
        cout << "          Card no. | Name | Type | Amount | Balance" << endl;
        string line;
        while(getline(transaction_file, line)){
            cout << line << endl;
        }
    }
    else{
        cout <<endl<< "No transactions found." << endl;
    }
    cout<<endl<<endl<<"Press any key to return to Main Menu...";
    _getch();
}
void menu(string card_no, string name ,float balance){
    while(1){
        char choise;
        char r;
        cout<<endl<<"=================Main Menu================="<<endl;
        cout<<"              1. Check Balance"<<endl;
        cout<<"              2. WithDraw Money"<<endl;
        cout<<"              3. Transfer Money"<<endl;
        cout<<"              4. View Previous Transactions"<<endl;
        cout<<"              5. Exit"<<endl;    
        cout<<endl<<"==========================================="<<endl;
        cout<<"Enter your Choise: ";
        cin>>choise;
        switch(choise){
            case '1':
                check_balance(balance);
                break;
            case '2':
                withdraw_money(card_no , name ,balance);
                break;
            case '3':
                transfer_money(card_no , name ,balance);
                break;
            case '4':
                view_transactions(card_no);
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
    string card_e;
    cout<<endl<<"==========================================="<<endl;
    cout<<"Enter your Card No.  = ";
    cin>>card_e;
    cout<<endl<<"==========================================="<<endl;
    return card_e;
}

string pin(){
    string pin_e;
    cout<<endl<<"==========================================="<<endl;
    cout<<"Enter your Pin  = ";
    cin>>pin_e;
    cout<<endl<<"==========================================="<<endl;
    return pin_e;
}

int main()
{
    system("Color 70");
    int login_attempts = 0;
    string card_no_val;
    string pin_val;
    string name;
    float balance = 0;
    string card_no_entered = card_no();
    string pin_entered = pin();
    while(login_attempts < 3){
        
        bool card_en = false;
        bool pin_en =false;
        bool matched=false;
        string line;
        ifstream afile ("account.csv");
        while (getline(afile, line)) {
            istringstream afield(line);
            string field;
            int field_count = 0;
            if(card_en && pin_en){
                break;
            }
            while (getline(afield, field, ',')){
                if(card_no_entered == field){
                    card_no_val = field;
                    card_en = true;
                    field_count++;
                    continue;
                }
                else if( pin_entered == field && field_count == 1 && card_en ){
                    pin_val = field;
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
                else if(!card_no || !pin){
                    break;
                }
            }   
        }
        if( card_en && pin_en){
            system("cls");
            cout <<endl<< "Welcome " << name << "!" << endl;
            write_transaction("Login",name,card_no_val , 0, 0);
            menu(card_no_val,name,balance);
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
        write_transaction("Failed Login", name,card_no_val  , 0, 0);
        cout << "Too many login attempts. Your card has been locked." << endl;
    }
    return 0;
}
