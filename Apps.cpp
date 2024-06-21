#include <iostream>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <ctime>
#include <conio.h>
using namespace std;


struct Transaction {
    string type;
    string sender;
    string recipient;
    string senderAccount;
    string recipientAccount;
    double amount;
};

template <typename T>
void merge(vector<Transaction>& transactions, T left, T mid, T right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Transaction> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = transactions[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = transactions[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].amount <= R[j].amount) {
            transactions[k] = L[i];
            i++;
        } else {
            transactions[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        transactions[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        transactions[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<Transaction>& transactions, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(transactions, left, mid);
        mergeSort(transactions, mid + 1, right);

        merge(transactions, left, mid, right);
    }
}

class BankSystem {
private:
    unordered_map<string, string> users;
    unordered_map<string, string> accountNumbers;
    unordered_map<string, double> saldo;
    unordered_map<string, vector<Transaction>> transactionHistory;

public:
    void registerUser() {
        char username[50], accountNumber[50];
        stack<char> password;

        cout << "\nMasukkan nama pengguna: ";
        cin >> username;
        if (users.find(username) != users.end()) {
            cout << "Nama pengguna sudah terdaftar. Silakan pilih nama pengguna lain.\n";
            cout << "Tekan Enter untuk melanjutkan...";
            cin.ignore();
            cin.get();
            return;
        }

        cout << "Masukkan kata sandi: ";
        while (char ch = _getch()) {
            if (ch == 13) { 
                break;
            } else if (ch == 8) { 
                if (!password.empty()) {
                    password.pop();
                    cout << "\b \b"; 
                }
            } else {
                password.push(ch);
                cout << '*'; 
            }
        }
        cout << endl;

        cout << "Masukkan nomor rekening: ";
        cin >> accountNumber;
        string strpassword;
        while (!password.empty()){
            strpassword += password.top();
            password.pop();
        }

        users[string(username)] = strpassword;
        accountNumbers[string(username)] = string(accountNumber);
        saldo[string(username)] = 0.0;
        cout << "Registrasi berhasil. Silakan login.\n";
        cout << "Tekan Enter untuk melanjutkan...";
        cin.ignore();
        cin.get();
    }

    bool loginUser(string& username) {
        char inputUsername[50], accountNumber[50];
        stack<char> password;

        cout << "\nMasukkan nama pengguna: ";
        cin >> inputUsername;
        cout << "Masukkan kata sandi: ";
        while (char ch = _getch()) {
            if (ch == 13) { 
                break;
            } else if (ch == 8) { 
                if (!password.empty()) {
                    password.pop();
                    cout << "\b \b"; 
                }
            } else {
                password.push(ch);
                cout << '*'; 
            }
        }
        cout << "\nMasukkan nomor rekening: ";
        cin >> accountNumber;

        string strpassword;
        while (!password.empty()){
            strpassword += password.top();
            password.pop();
        }
        if (users.find(inputUsername) != users.end() && users[inputUsername] == strpassword && accountNumbers[inputUsername] == accountNumber) {
            cout << "Login berhasil. Selamat datang, " << inputUsername << "!\n";
            username = string(inputUsername);
            return true;
        } else {
            cout << "Login gagal. Nama pengguna, kata sandi, atau nomor rekening salah.\n";
            cout << "Tekan Enter untuk melanjutkan...";
            cin.ignore();
            cin.get();
            return false;
        }
    }

    void showAccount(string& username) {
        cout << "\nSaldo Anda: Rp" << saldo[username] << endl;
    }

    void transfer(string& username) {
        string recipient, recipientAccount;
        double amount;
        char confirm;

        cout << "\nMasukkan nama penerima: ";
        cin >> recipient;
        if (saldo.find(recipient) == saldo.end()) {
            cout << "Penerima tidak ditemukan.\n";
            return;
        }

        cout << "Masukkan nomor rekening penerima: ";
        cin >> recipientAccount;
        if (accountNumbers[recipient] != recipientAccount) {
            cout << "Nomor rekening penerima tidak cocok.\n";
            return;
        }

        cout << "Masukkan jumlah yang akan ditransfer: Rp";
        cin >> amount;
        if (amount > saldo[username]) {
            cout << "Saldo Anda tidak mencukupi.\n";
            return;
        }

        cout << "Konfirmasi transfer? (y/n): ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            saldo[username] -= amount;
            saldo[recipient] += amount;
            cout << "Transfer berhasil.\n";

            Transaction transaksi;
            transaksi.type = "Transfer";
            transaksi.sender = username;
            transaksi.recipient = recipient;
            transaksi.senderAccount = accountNumbers[username];
            transaksi.recipientAccount = recipientAccount;
            transaksi.amount = amount;
            transactionHistory[username].push_back(transaksi);
            transactionHistory[recipient].push_back(transaksi);

            printReceipt(username, recipient, accountNumbers[username], recipientAccount, amount);
        } else {
            cout << "Transfer dibatalkan.\n";
        }
    }

    void topUp(string& username) {
        double amount;
        cout << "\nMasukkan jumlah yang akan ditambahkan: Rp";
        cin >> amount;
        saldo[username] += amount;
        cout << "Top up berhasil. Saldo Anda sekarang: Rp" << saldo[username] << endl;
	    
        Transaction topup;
        topup.type = "Top-up";
        topup.sender = "";
        topup.recipient = username;
        topup.senderAccount = "";
        topup.recipientAccount = accountNumbers[username];
        topup.amount = amount;
        transactionHistory[username].push_back(topup);
    }

    void printReceipt(string sender, string recipient, string senderAccount, string recipientAccount, double amount) {
        cout << "\n========== STRUK PEMBAYARAN ==========\n";
        cout << "Pengirim: " << sender << " (" << senderAccount << ")" << endl;
        cout << "Penerima: " << recipient << " (" << recipientAccount << ")" << endl;
        cout << "Jumlah yang ditransfer: Rp" << amount << endl;
        cout << "Terima kasih telah menggunakan layanan kami.\n";
        cout << "======================================\n";
    }

    void withdraw(string& username) {
        double amount;
        cout << "\nMasukkan jumlah yang akan ditarik: Rp";
        cin >> amount;
        if (amount > saldo[username]) {
            cout << "Saldo Anda tidak mencukupi.\n";
            return;
        }

        saldo[username] -= amount;
        cout << "Penarikan berhasil. Saldo Anda sekarang: Rp" << saldo[username] << endl;

        Transaction withdrawal;
        withdrawal.type = "Penarikan";
        withdrawal.sender = username;
        withdrawal.recipient = "";
        withdrawal.senderAccount = accountNumbers[username];
        withdrawal.recipientAccount = "";
        withdrawal.amount = amount;
        transactionHistory[username].push_back(withdrawal);

        printWithdrawalReceipt(username, amount);
    }

    void printWithdrawalReceipt(string username, double amount) {
        cout << "\n========== STRUK PENARIKAN ==========\n";
        cout << "Nama Pengguna: " << username << " (" << accountNumbers[username] << ")" << endl;
        cout << "Jumlah yang ditarik: Rp" << amount << endl;
        cout << "Terima kasih telah menggunakan layanan kami.\n";
        cout << "======================================\n";
    }

    void showTransactionHistory(string& username) {
        cout << "\n===== RIWAYAT TRANSAKSI =====\n";
        vector<Transaction> history = transactionHistory[username];
        for (const auto& transaction : history) {
            cout << "Jenis Transaksi: " << transaction.type << endl;
            if (transaction.type == "Transfer") {
                cout << "Pengirim: " << transaction.sender << " (" << transaction.senderAccount << ")" << endl;
                cout << "Penerima: " << transaction.recipient << " (" << transaction.recipientAccount << ")" << endl;
            } else if (transaction.type == "Top-up") {
                cout << "Penerima: " << transaction.recipient << " (" << transaction.recipientAccount << ")" << endl;
            } else if (transaction.type == "Penarikan") {
                cout << "Nama Pengguna: " << transaction.sender << " (" << transaction.senderAccount << ")" << endl;
            }
            cout << "Jumlah: Rp" << transaction.amount << endl;
            cout << "-----------------------------\n";
        }
    }
};
void clearScreen() {
    system("cls");
}

void startScreen(tm* tPtr) {
    clearScreen();
    cout << "\n\n\n\t\t\t Date: " << (tPtr->tm_mday) << "/" << (tPtr->tm_mon + 1) << "/" << (tPtr->tm_year + 1900) << endl;
    cout << "\t\t\t Time: " << (tPtr->tm_hour) << ":" << (tPtr->tm_min) << endl;

  cout << "\t\t\t ##########################################################################################################################" << endl;
	
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t      Selamat datang\t\t\t\t\t\t          #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t            di\t\t\t\t\t\t\t          #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t         Bank C++\t\t\t\t\t\t          #" << endl;

	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\tDibuat oleh akmal,rofiq,daffa     #" << endl;
	cout << "\t\t\t ##########################################################################################################################" << endl;
	cout << "\t\t\t ",system("pause");
}

void endScreen(tm* tPtr)
{
	cout << "\n\n\n\t\t\t Date: " << (tPtr->tm_mday) << "/" << (tPtr->tm_mon) + 1 << "/" << (tPtr->tm_year) + 1900 << endl;
	cout << "\t\t\t Time: " << (tPtr->tm_hour) << ":" << (tPtr->tm_min) << endl;

	cout << "\t\t\t ##########################################################################################################################" << endl;
	
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;

	cout << "\t\t\t #" << "\t\t\t\t\t\t\tTerima kasih\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t     sudah menggunakan\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t   Bank C++\t\t\t\t\t\t          #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t  #" << endl;
	
	cout << "\t\t\t #" << "\t\t\t\t\t\t\t\t\t\t\tDibuat oleh akmal,rofiq,daffa     #" << endl;
	cout << "\t\t\t ##########################################################################################################################" << endl;
    cout << "\t\t\t Tekan Enter untuk melanjutkan...";
    cin.ignore();
    cin.get();
    clearScreen();
}

void printWelcomeMessage() {
    cout << "\n\n";
    cout << "=========================================\n";
    cout << "      SELAMAT DATANG DI BANK C++\n";
    cout << "=========================================\n";
    cout << "\n\n";
}

int main() {
    BankSystem bankSystem;
    string username;
    int choice;
    time_t now = time(0);
    tm *tPtr = localtime(&now);

    startScreen(tPtr);
    clearScreen();

    while (true) {
        clearScreen();
        printWelcomeMessage();
        cout << "1. Registrasi\n";
        cout << "2. Login\n";
        cout << "3. Keluar\n";
        cout << "Pilih opsi: ";
        cin >> choice;

        switch (choice) {
            case 1:
             clearScreen();
                bankSystem.registerUser();
                break;
            case 2:
            clearScreen();
            if (bankSystem.loginUser(username)) {
                    bool out = false;
                    while (!out) {
                        clearScreen();
                        cout << "Selamat datang, " << username << "!\n";
                        cout << "1. Lihat saldo dan riwayat transaksi\n";
                        cout << "2. Transfer dana\n";
                        cout << "3. Top up\n";
                        cout << "4. Penarikan dana\n";
                        cout << "5. Lihat histori transaksi\n";
                        cout << "6. Keluar\n";
                        cout << "Pilih opsi: ";
                        cin >> choice;

                        clearScreen();

                        switch (choice) {
                            case 1:
                                 clearScreen();
                                bankSystem.showAccount(username);
                                break;
                            case 2:
                                 clearScreen();
                                bankSystem.transfer(username);
                                break;
                            case 3:
                                 clearScreen();
                                bankSystem.topUp(username);
                                break;
                            case 4:
                                 clearScreen();
                                bankSystem.withdraw(username);
                                break;
                            case 5:
                                 clearScreen();
                                bankSystem.showTransactionHistory(username);
                                break;
                            case 6:
                                 clearScreen();
                                cout << "Terima kasih telah menggunakan sistem kami.\n";
                                out = true;
                                break;
                            default:
                                cout << "Opsi tidak valid. Silakan coba lagi.\n";
                                break;
                        }

                        cout << "Tekan Enter untuk melanjutkan...";
                        cin.ignore();
                        cin.get();
                    }
                }
                break;
            case 3:
             clearScreen();
                endScreen(tPtr); 
                return 0;
            default:
                cout << "Opsi tidak valid. Silakan coba lagi.\n";
                break;
        }
    }

    return 0;
}
