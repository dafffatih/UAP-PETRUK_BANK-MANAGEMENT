#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// Struktur untuk menyimpan transaksi
struct Transaction {
    string type;      // "Transfer" atau "Top-up"
    string sender;    // Nama pengirim (untuk top-up bisa dibiarkan kosong)
    string recipient; // Nama penerima (untuk top-up adalah nama pengguna)
    double amount;         // Jumlah transaksi
};

// Fungsi untuk melakukan merge sort pada histori transaksi
void merge(vector<Transaction>& transactions, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Transaction> L(n1);
    vector<Transaction> R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = transactions[left + i];
    }
    for (int i = 0; i < n2; i++) {
        R[i] = transactions[mid + 1 + i];
    }

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
    unordered_map<string, double> saldo;
    unordered_map<string, vector<Transaction>> transactionHistory;

public:
    void registerUser() {
        string username, password;

        cout << "\nMasukkan nama pengguna: ";
        cin >> username;
        if (users.find(username) != users.end()) {
            cout << "Nama pengguna sudah terdaftar. Silakan pilih nama pengguna lain.\n";
            return;
        }

        cout << "Masukkan kata sandi: ";
        cin >> password;

        users[username] = password;
        saldo[username] = 0.0;
        cout << "Registrasi berhasil. Silakan login.\n";
    }

    bool loginUser(string& username) {
        string password;

        cout << "\nMasukkan nama pengguna: ";
        cin >> username;
        cout << "Masukkan kata sandi: ";
        cin >> password;

        if (users.find(username) != users.end() && users[username] == password) {
            cout << "Login berhasil. Selamat datang, " << username << "!\n";
            return true;
        } else {
            cout << "Login gagal. Nama pengguna atau kata sandi salah.\n";
            return false;
        }
    }

    void showAccount(string& username) {
        cout << "\nSaldo Anda: Rp" << saldo[username] << endl;
    }

    void transfer(string& username) {
        string recipient;
        double amount;
        char confirm;

        cout << "\nMasukkan nama penerima: ";
        cin >> recipient;
        if (saldo.find(recipient) == saldo.end()) {
            cout << "Penerima tidak ditemukan.\n";
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

            // Menyimpan transaksi ke histori
            Transaction transaksi;
            transaksi.type = "Transfer";
            transaksi.sender = username;
            transaksi.recipient = recipient;
            transaksi.amount = amount;
            transactionHistory[username].push_back(transaksi);
            transactionHistory[recipient].push_back(transaksi);

            printReceipt(username, recipient, amount);
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

        // Menyimpan transaksi top-up ke histori
        Transaction topup;
        topup.type = "Top-up";
        topup.sender = "";
        topup.recipient = username;
        topup.amount = amount;
        transactionHistory[username].push_back(topup);
    }

    void printReceipt(string sender, string recipient, double amount) {
        cout << "\n========== STRUK PEMBAYARAN ==========\n";
        cout << "Pengirim: " << sender << endl;
        cout << "Penerima: " << recipient << endl;
        cout << "Jumlah yang ditransfer: Rp" << amount << endl;
        cout << "Terima kasih telah menggunakan layanan kami.\n";
        cout << "======================================\n";
    }

    void showTransactionHistory(string& username) {
        cout << "\n===== RIWAYAT TRANSAKSI =====\n";
        vector<Transaction> history = transactionHistory[username];
        for (const auto& transaction : history) {
            std::cout << "Jenis Transaksi: " << transaction.type << endl;
            if (transaction.type == "Transfer") {
                cout << "Pengirim: " << transaction.sender << endl;
                cout << "Penerima: " << transaction.recipient << endl;
            } else if (transaction.type == "Top-up") {
                cout << "Penerima: " << transaction.recipient << endl;
            }
            cout << "Jumlah: Rp" << transaction.amount << endl;
            cout << "-----------------------------\n";
        }
    }
};

void clearScreen() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

void printWelcomeMessage() {
    cout << "\n\n";
    cout << "=========================================\n";
    cout << "      SELAMAT DATANG DI BANK BNI\n";
    cout << "=========================================\n";
    cout << "\n\n";
}

int main() {
    BankSystem bankSystem;
    string username;
    int choice;

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
                    while (true) {
                        clearScreen();
                        cout << "Selamat datang, " << username << "!\n";
                        cout << "1. Lihat saldo dan riwayat transaksi\n";
                        cout << "2. Transfer dana\n";
                        cout << "3. Top up\n";
                        cout << "4. Lihat histori transaksi\n";
                        cout << "5. Keluar\n";
                        cout << "Pilih opsi: ";
                        cin >> choice;

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
                                bankSystem.showTransactionHistory(username);
                                break;
                            case 5:
                                clearScreen();
                                std::cout << "Terima kasih telah menggunakan sistem kami. Sampai jumpa lagi!\n";
                                choice = -1;
                                break;
                            default:
                                clearScreen();
                                std::cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                        }

                        if (choice == -1)
                            break;

                        cout << "Tekan enter untuk melanjutkan...";
                        cin.ignore();
                        cin.get();
                    }
                }
                break;
            case 3:
                clearScreen();
                cout << "Terima kasih telah menggunakan sistem kami. Sampai jumpa lagi!\n";
                return 0;
            default:
                clearScreen();
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }

        cout << "Tekan enter untuk melanjutkan...";
    	cin.ignore();
        cin.get();
    }

    return 0;
}
