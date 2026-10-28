#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <ctime>

using namespace std;

class UserException : public runtime_error {
public:
    explicit UserException(const string& pesan) : runtime_error(pesan) {}
};

class User {
public:
    static const int BATAS_PERCOBAAN_GAGAL = 3;

protected:
    string idUser;
    string username;
    string password;
    string role;

    string statusAkun;
    int jumlahPercobaanGagal;
    string tanggalDibuat;

private:
    static int totalUserTerdaftar;

    string dapatkanTanggalSistemSekarang() const {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        char buffer[12];
        strftime(buffer, sizeof(buffer), "%d-%m-%Y", now);
        return string(buffer);
    }

    bool isStatusAkunValid(const string& status) const {
        return status == "Aktif" || status == "Terkunci" || status == "Nonaktif";
    }

public:
    User() : idUser(""), username(""), password(""), role(""),
              statusAkun("Aktif"), jumlahPercobaanGagal(0) {
        tanggalDibuat = dapatkanTanggalSistemSekarang();
        totalUserTerdaftar++;
    }

    User(const string& id, const string& uname, const string& pass, const string& uRole)
        : idUser(id), username(uname), password(pass), role(uRole),
          statusAkun("Aktif"), jumlahPercobaanGagal(0) {
        tanggalDibuat = dapatkanTanggalSistemSekarang();
        totalUserTerdaftar++;
    }

    User(const User& lain)
        : idUser(lain.idUser), username(lain.username), password(lain.password), role(lain.role),
          statusAkun(lain.statusAkun), jumlahPercobaanGagal(lain.jumlahPercobaanGagal),
          tanggalDibuat(lain.tanggalDibuat) {
        totalUserTerdaftar++;
    }

    User& operator=(const User& lain) {
        if (this == &lain) {
            return *this;
        }
        idUser = lain.idUser;
        username = lain.username;
        password = lain.password;
        role = lain.role;
        statusAkun = lain.statusAkun;
        jumlahPercobaanGagal = lain.jumlahPercobaanGagal;
        tanggalDibuat = lain.tanggalDibuat;
        return *this;
    }

    virtual ~User() {
        totalUserTerdaftar--;
    }

    string getIdUser() const { return idUser; }
    void setIdUser(const string& id) { idUser = id; }

    string getUsername() const { return username; }
    void setUsername(const string& uname) { username = uname; }

    string getPassword() const { return password; }
    void setPassword(const string& pass) { password = pass; }

    string getRole() const { return role; }
    void setRole(const string& uRole) { role = uRole; }

    string getStatusAkun() const { return statusAkun; }
    void setStatusAkun(const string& status) {
        if (!isStatusAkunValid(status)) {
            throw UserException("Status akun tidak dikenali: " + status);
        }
        statusAkun = status;
    }

    string getTanggalDibuat() const { return tanggalDibuat; }
    int getJumlahPercobaanGagal() const { return jumlahPercobaanGagal; }

    static int getTotalUserTerdaftar() {
        return totalUserTerdaftar;
    }

    string getPasswordTersamar() const {
        if (password.empty()) {
            return "-";
        }
        return string(password.length(), '*');
    }

    bool apakahAktif() const { return statusAkun == "Aktif"; }
    bool apakahTerkunci() const { return statusAkun == "Terkunci"; }
    bool apakahNonaktif() const { return statusAkun == "Nonaktif"; }

    bool verifikasiPassword(const string& inputPassword) const {
        return inputPassword == password;
    }

    bool cobaLogin(const string& inputPassword) {
        if (apakahTerkunci()) {
            cout << "[Sistem Keamanan] Error: Akun '" << username << "' sedang terkunci akibat terlalu banyak percobaan gagal!\n";
            return false;
        }
        if (apakahNonaktif()) {
            cout << "[Sistem Keamanan] Error: Akun '" << username << "' berstatus nonaktif!\n";
            return false;
        }
        if (verifikasiPassword(inputPassword)) {
            jumlahPercobaanGagal = 0;
            return true;
        }

        jumlahPercobaanGagal++;
        cout << "[Sistem Keamanan] Error: Password salah! Percobaan ke-" << jumlahPercobaanGagal
             << " dari " << BATAS_PERCOBAAN_GAGAL << ".\n";

        if (jumlahPercobaanGagal >= BATAS_PERCOBAAN_GAGAL) {
            kunciAkun();
        }
        return false;
    }

    void kunciAkun() {
        statusAkun = "Terkunci";
        cout << "[Sistem Keamanan] Akun '" << username << "' otomatis dikunci karena melebihi batas percobaan gagal!\n";
    }

    void bukaKunciAkun() {
        statusAkun = "Aktif";
        jumlahPercobaanGagal = 0;
        cout << "[Sistem Keamanan] Akun '" << username << "' berhasil dibuka kembali oleh administrator.\n";
    }

    void gantiPassword(const string& passwordLama, const string& passwordBaru) {
        if (!verifikasiPassword(passwordLama)) {
            throw UserException("Password lama tidak sesuai, penggantian password dibatalkan.");
        }
        if (!validasiPassword(passwordBaru)) {
            throw UserException("Password baru tidak memenuhi standar keamanan sistem.");
        }
        password = passwordBaru;
    }

    static bool validasiId(const string& id) {
        if (id.empty()) {
            cout << "[Sistem Validasi] Error: ID Pengguna tidak boleh kosong!\n";
            return false;
        }
        if (id.length() < 3 || id.length() > 15) {
            cout << "[Sistem Validasi] Error: ID Pengguna harus berukuran antara 3 sampai 15 karakter!\n";
            return false;
        }
        for (char c : id) {
            if (!isalnum(static_cast<unsigned char>(c)) && c != '-' && c != '_') {
                cout << "[Sistem Validasi] Error: ID hanya diizinkan berisi kombinasi huruf, angka, tanda hubung (-), atau underscore (_).\n";
                return false;
            }
        }
        return true;
    }

    static bool validasiUsername(const string& uname) {
        if (uname.empty()) {
            cout << "[Sistem Validasi] Error: Username tidak boleh kosong!\n";
            return false;
        }
        if (uname.length() < 5) {
            cout << "[Sistem Validasi] Error: Username minimal harus terdiri dari 5 karakter!\n";
            return false;
        }
        for (char c : uname) {
            if (isspace(static_cast<unsigned char>(c))) {
                cout << "[Sistem Validasi] Error: Username dilarang keras mengandung karakter spasi!\n";
                return false;
            }
            if (!isalnum(static_cast<unsigned char>(c))) {
                cout << "[Sistem Validasi] Error: Username hanya diizinkan berupa kombinasi karakter huruf dan angka saja!\n";
                return false;
            }
        }
        return true;
    }

    static bool validasiPassword(const string& pass) {
        if (pass.length() < 6) {
            cout << "[Sistem Validasi] Error: Password terlalu lemah! Panjang minimal wajib 6 karakter.\n";
            return false;
        }

        bool adaHurufBesar = false;
        bool adaHurufKecil = false;
        bool adaAngka = false;

        for (char c : pass) {
            if (isupper(static_cast<unsigned char>(c))) adaHurufBesar = true;
            if (islower(static_cast<unsigned char>(c))) adaHurufKecil = true;
            if (isdigit(static_cast<unsigned char>(c))) adaAngka = true;
        }

        if (!adaHurufBesar) {
            cout << "[Sistem Validasi] Error: Password wajib mengandung minimal satu karakter huruf besar (A-Z)!\n";
            return false;
        }
        if (!adaHurufKecil) {
            cout << "[Sistem Validasi] Error: Password wajib mengandung minimal satu karakter huruf kecil (a-z)!\n";
            return false;
        }
        if (!adaAngka) {
            cout << "[Sistem Validasi] Error: Password wajib mengandung minimal satu karakter digit angka (0-9)!\n";
            return false;
        }

        return true;
    }

    virtual void inputFormulir() {
        string inputId = "";
        string inputUname = "";
        string inputPass = "";

        cout << "\n+-------------------------------------------------------+\n";
        cout << "|             FORMULIR UTAMA PENGISIAN USER             |\n";
        cout << "+-------------------------------------------------------+\n";

        do {
            cout << " -> Masukkan ID User Baru       : ";
            cin >> inputId;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
            }
        } while (!validasiId(inputId));
        this->idUser = inputId;

        do {
            cout << " -> Tentukan Username Akun      : ";
            cin >> inputUname;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
            }
        } while (!validasiUsername(inputUname));
        this->username = inputUname;

        do {
            cout << " -> Tentukan Password Keamanan  : ";
            cin >> inputPass;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
            }
        } while (!validasiPassword(inputPass));
        this->password = inputPass;

        int opsiRole = 0;
        do {
            cout << " -> Pilih Tingkatan Otoritas Role Pengguna:\n";
            cout << "    [1] Admin Utama System\n";
            cout << "    [2] Petugas Lapangan/Operasional\n";
            cout << "    [3] Pelanggan Reguler Travel\n";
            cout << "    Pilihan Anda (1-3): ";
            cin >> opsiRole;

            if (cin.fail() || opsiRole < 1 || opsiRole > 3) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Sistem Validasi] Error: Opsi salah! Silakan masukkan pilihan nomor 1 hingga 3.\n";
                opsiRole = 0;
            }
        } while (opsiRole == 0);

        if (opsiRole == 1) this->role = "Admin";
        else if (opsiRole == 2) this->role = "Petugas";
        else this->role = "Pelanggan";

        this->statusAkun = "Aktif";
        this->jumlahPercobaanGagal = 0;
        this->tanggalDibuat = dapatkanTanggalSistemSekarang();

        cout << "+-------------------------------------------------------+\n";
        cout << "| [Sukses] Data dasar entitas pengguna berhasil disimpan.|\n";
        cout << "+-------------------------------------------------------+\n";
    }

    virtual void tampilkanInformasi() const {
        cout << "\n=========================================================\n";
        cout << "                 DETAIL DATA ENTITAS USER                \n";
        cout << "=========================================================\n";
        cout << " * ID Registrasi Akun  : " << this->idUser << "\n";
        cout << " * Nama Akun / Uname   : " << this->username << "\n";
        cout << " * Hak Akses Otentikasi: " << this->role << "\n";
        cout << " * Status Akun         : " << this->statusAkun << "\n";
        cout << " * Tanggal Terdaftar   : " << this->tanggalDibuat << "\n";
        cout << "=========================================================\n";
    }

    bool operator==(const User& lain) const {
        return idUser == lain.idUser;
    }

    bool operator!=(const User& lain) const {
        return !(*this == lain);
    }

    friend ostream& operator<<(ostream& out, const User& user) {
        out << "User[" << user.idUser << ", " << user.username
            << ", role=" << user.role << ", status=" << user.statusAkun << "]";
        return out;
    }
};

#endif
