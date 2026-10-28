#ifndef LOGINSYSTEM_H
#define LOGINSYSTEM_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "Petugas.h"
#include "Pelanggan.h"
#include "Session.h"

using namespace std;

class LoginSystem {
private:
    static const int MAX_LOG = 300;
    static const int MAX_AKUN_PANTAU = 100;

    string adminUsernameAktif;
    string adminPasswordAktif;

    string logUsername[MAX_LOG];
    string logRole[MAX_LOG];
    string logKeterangan[MAX_LOG];
    int logWaktu[MAX_LOG];
    int jumlahLog;

    string usernamePantauan[MAX_AKUN_PANTAU];
    int jumlahPercobaanGagal[MAX_AKUN_PANTAU];
    bool statusTerkunci[MAX_AKUN_PANTAU];
    int jumlahAkunDipantau;

    int batasPercobaanGagal;
    int penghitungWaktuSimulasi;
    int kodeOtpTersimpan;

    static int totalLoginBerhasil;
    static int totalLoginGagal;
    static int totalLogoutTercatat;
    static int totalAkunTerkunci;

    int cariIndexPantauan(string user) const {
        for (int i = 0; i < jumlahAkunDipantau; i++) {
            if (usernamePantauan[i] == user) {
                return i;
            }
        }
        return -1;
    }

    void catatLog(string user, string role, string keterangan) {
        if (jumlahLog >= MAX_LOG) {
            for (int i = 0; i < MAX_LOG - 1; i++) {
                logUsername[i] = logUsername[i + 1];
                logRole[i] = logRole[i + 1];
                logKeterangan[i] = logKeterangan[i + 1];
                logWaktu[i] = logWaktu[i + 1];
            }
            jumlahLog--;
        }
        logUsername[jumlahLog] = user;
        logRole[jumlahLog] = role;
        logKeterangan[jumlahLog] = keterangan;
        logWaktu[jumlahLog] = penghitungWaktuSimulasi;
        jumlahLog++;
        penghitungWaktuSimulasi++;
    }

    void cetakGaris(char karakter, int panjang) const {
        for (int i = 0; i < panjang; i++) {
            cout << karakter;
        }
        cout << endl;
    }

public:
    LoginSystem() {
        adminUsernameAktif = "admin";
        adminPasswordAktif = "Admin123";
        jumlahLog = 0;
        jumlahAkunDipantau = 0;
        batasPercobaanGagal = 3;
        penghitungWaktuSimulasi = 0;
        kodeOtpTersimpan = 0;
        srand((unsigned int) time(0));
    }

    LoginSystem(string usernameAdmin, string passwordAdmin, int batasGagal) {
        adminUsernameAktif = usernameAdmin;
        adminPasswordAktif = passwordAdmin;
        jumlahLog = 0;
        jumlahAkunDipantau = 0;
        batasPercobaanGagal = (batasGagal > 0) ? batasGagal : 3;
        penghitungWaktuSimulasi = 0;
        kodeOtpTersimpan = 0;
        srand((unsigned int) time(0));
    }

    virtual ~LoginSystem() {}

    bool validasiFormatUsername(string user) const {
        if (user.length() < 4) {
            cout << "[Validasi Gagal] Username minimal 4 karakter" << endl;
            return false;
        }
        for (int i = 0; i < (int) user.length(); i++) {
            if (user[i] == ' ') {
                cout << "[Validasi Gagal] Username tidak boleh mengandung spasi" << endl;
                return false;
            }
        }
        return true;
    }

    bool validasiFormatPassword(string pass) const {
        if (pass.length() < 6) {
            cout << "[Validasi Gagal] Password minimal 6 karakter" << endl;
            return false;
        }
        bool adaHuruf = false;
        bool adaAngka = false;
        for (int i = 0; i < (int) pass.length(); i++) {
            if (isalpha(pass[i])) {
                adaHuruf = true;
            }
            if (isdigit(pass[i])) {
                adaAngka = true;
            }
        }
        if (!adaHuruf || !adaAngka) {
            cout << "[Validasi Gagal] Password harus mengandung huruf dan angka" << endl;
            return false;
        }
        return true;
    }

    bool cekApakahAkunTerkunci(string user) const {
        int index = cariIndexPantauan(user);
        if (index == -1) {
            return false;
        }
        return statusTerkunci[index];
    }

    void tambahPercobaanGagal(string user) {
        int index = cariIndexPantauan(user);
        if (index == -1) {
            if (jumlahAkunDipantau >= MAX_AKUN_PANTAU) {
                cout << "[Peringatan] Kapasitas pemantauan akun sudah penuh" << endl;
                return;
            }
            usernamePantauan[jumlahAkunDipantau] = user;
            jumlahPercobaanGagal[jumlahAkunDipantau] = 1;
            statusTerkunci[jumlahAkunDipantau] = false;
            jumlahAkunDipantau++;
            index = jumlahAkunDipantau - 1;
        } else {
            jumlahPercobaanGagal[index]++;
        }

        if (jumlahPercobaanGagal[index] >= batasPercobaanGagal && !statusTerkunci[index]) {
            statusTerkunci[index] = true;
            totalAkunTerkunci++;
            cout << "[Keamanan] Akun '" << user << "' dikunci sementara akibat "
                 << jumlahPercobaanGagal[index] << " kali percobaan gagal berturut-turut" << endl;
        }
    }

    void resetPercobaanGagal(string user) {
        int index = cariIndexPantauan(user);
        if (index != -1) {
            jumlahPercobaanGagal[index] = 0;
            statusTerkunci[index] = false;
        }
    }

    bool bukaKembaliAkunTerkunci(string user) {
        int index = cariIndexPantauan(user);
        if (index == -1) {
            cout << "[Info] Akun tidak ditemukan dalam daftar pantauan" << endl;
            return false;
        }
        if (!statusTerkunci[index]) {
            cout << "[Info] Akun '" << user << "' tidak dalam status terkunci" << endl;
            return false;
        }
        statusTerkunci[index] = false;
        jumlahPercobaanGagal[index] = 0;
        cout << "[Keamanan] Akun '" << user << "' berhasil dibuka kembali oleh Admin" << endl;
        return true;
    }

    bool loginAdmin(const string& user, const string& pass, Session& s) {
        if (cekApakahAkunTerkunci(user)) {
            cout << "[Otentikasi Ditolak] Akun '" << user << "' sedang terkunci sementara!" << endl;
            catatLog(user, "Admin", "Ditolak - Akun Terkunci");
            return false;
        }

        if (user == adminUsernameAktif && pass == adminPasswordAktif) {
            bool statusSesi = s.buatSesiBaru("ADM-001", user, "Admin");
            if (statusSesi) {
                cout << "[Otentikasi Sukses] Selamat datang Admin Utama. Sesi Anda telah aktif." << endl;
                resetPercobaanGagal(user);
                catatLog(user, "Admin", "Login Berhasil");
                totalLoginBerhasil++;
                return true;
            }
        }

        cout << "[Otentikasi Gagal] Error: Kredensial Username atau Password Admin salah!" << endl;
        tambahPercobaanGagal(user);
        catatLog(user, "Admin", "Login Gagal");
        totalLoginGagal++;
        return false;
    }

    bool loginPetugas(const string& user, const string& pass,
                      const Petugas* daftarPetugas, int jumlahPetugas, Session& s) {
        if (cekApakahAkunTerkunci(user)) {
            cout << "[Otentikasi Ditolak] Akun '" << user << "' sedang terkunci sementara!" << endl;
            catatLog(user, "Petugas", "Ditolak - Akun Terkunci");
            return false;
        }

        if (daftarPetugas == nullptr || jumlahPetugas == 0) {
            cout << "[Otentikasi Gagal] Error: Belum ada data petugas yang terdaftar di sistem!" << endl;
            catatLog(user, "Petugas", "Login Gagal - Repositori Kosong");
            totalLoginGagal++;
            return false;
        }

        for (int i = 0; i < jumlahPetugas; i++) {
            if (daftarPetugas[i].getUsername() == user && daftarPetugas[i].getPassword() == pass) {
                bool statusSesi = s.buatSesiBaru(daftarPetugas[i].getIdUser(), user, "Petugas");
                if (statusSesi) {
                    cout << "[Otentikasi Sukses] Login Berhasil! Selamat bertugas, "
                         << daftarPetugas[i].getUsername() << "." << endl;
                    resetPercobaanGagal(user);
                    catatLog(user, "Petugas", "Login Berhasil");
                    totalLoginBerhasil++;
                    return true;
                }
                catatLog(user, "Petugas", "Login Gagal - Sesi Tidak Terbentuk");
                totalLoginGagal++;
                return false;
            }
        }

        cout << "[Otentikasi Gagal] Error: Kredensial Username atau Password Petugas tidak cocok!" << endl;
        tambahPercobaanGagal(user);
        catatLog(user, "Petugas", "Login Gagal");
        totalLoginGagal++;
        return false;
    }

    bool loginPelanggan(const string& user, const string& pass,
                        const Pelanggan* daftarPelanggan, int jumlahPelanggan, Session& s) {
        if (cekApakahAkunTerkunci(user)) {
            cout << "[Otentikasi Ditolak] Akun '" << user << "' sedang terkunci sementara!" << endl;
            catatLog(user, "Pelanggan", "Ditolak - Akun Terkunci");
            return false;
        }

        if (daftarPelanggan == nullptr || jumlahPelanggan == 0) {
            cout << "[Otentikasi Gagal] Error: Repositori kosong! Silakan registrasi akun terlebih dahulu." << endl;
            catatLog(user, "Pelanggan", "Login Gagal - Repositori Kosong");
            totalLoginGagal++;
            return false;
        }

        for (int i = 0; i < jumlahPelanggan; i++) {
            if (daftarPelanggan[i].getUsername() == user && daftarPelanggan[i].getPassword() == pass) {
                bool statusSesi = s.buatSesiBaru(daftarPelanggan[i].getIdUser(), user, "Pelanggan");
                if (statusSesi) {
                    cout << "[Otentikasi Sukses] Login Berhasil! Selamat datang kembali, "
                         << daftarPelanggan[i].getUsername() << "." << endl;
                    resetPercobaanGagal(user);
                    catatLog(user, "Pelanggan", "Login Berhasil");
                    totalLoginBerhasil++;
                    return true;
                }
                catatLog(user, "Pelanggan", "Login Gagal - Sesi Tidak Terbentuk");
                totalLoginGagal++;
                return false;
            }
        }

        cout << "[Otentikasi Gagal] Error: Username atau Password Pelanggan salah / tidak terdaftar!" << endl;
        tambahPercobaanGagal(user);
        catatLog(user, "Pelanggan", "Login Gagal");
        totalLoginGagal++;
        return false;
    }

    void logoutUser(string user, string role) {
        cout << "[Info] Pengguna '" << user << "' dengan peran '" << role << "' telah keluar dari sistem." << endl;
        catatLog(user, role, "Logout");
        totalLogoutTercatat++;
    }

    bool gantiPasswordAdmin(string userLama, string passLama, string passBaru) {
        if (userLama != adminUsernameAktif || passLama != adminPasswordAktif) {
            cout << "[Gagal] Username atau password lama tidak cocok" << endl;
            return false;
        }
        if (!validasiFormatPassword(passBaru)) {
            return false;
        }
        adminPasswordAktif = passBaru;
        catatLog(userLama, "Admin", "Ganti Password");
        cout << "[Sukses] Password Admin berhasil diperbarui" << endl;
        return true;
    }

    bool gantiUsernameAdmin(string userLama, string passLama, string userBaru) {
        if (userLama != adminUsernameAktif || passLama != adminPasswordAktif) {
            cout << "[Gagal] Username atau password lama tidak cocok" << endl;
            return false;
        }
        if (!validasiFormatUsername(userBaru)) {
            return false;
        }
        adminUsernameAktif = userBaru;
        catatLog(userLama, "Admin", "Ganti Username");
        cout << "[Sukses] Username Admin berhasil diperbarui menjadi '" << userBaru << "'" << endl;
        return true;
    }

    int generateKodeOtp() {
        kodeOtpTersimpan = (rand() % 9000) + 1000;
        cout << "[OTP] Kode verifikasi telah dikirim: " << kodeOtpTersimpan << endl;
        return kodeOtpTersimpan;
    }

    bool verifikasiKodeOtp(int inputKode) {
        if (inputKode == kodeOtpTersimpan && kodeOtpTersimpan != 0) {
            cout << "[OTP] Verifikasi berhasil" << endl;
            kodeOtpTersimpan = 0;
            return true;
        }
        cout << "[OTP] Kode verifikasi salah atau sudah kedaluwarsa" << endl;
        return false;
    }

    void tampilkanRiwayatLog() const {
        cetakGaris('=', 65);
        cout << "RIWAYAT LOG AKTIVITAS OTENTIKASI SISTEM" << endl;
        cetakGaris('=', 65);
        if (jumlahLog == 0) {
            cout << "Belum ada aktivitas yang tercatat" << endl;
        }
        for (int i = 0; i < jumlahLog; i++) {
            cout << "[" << logWaktu[i] << "] " << logUsername[i]
                 << " (" << logRole[i] << ") -> " << logKeterangan[i] << endl;
        }
        cetakGaris('=', 65);
    }

    void tampilkanRiwayatLoginByUser(string user) const {
        cetakGaris('-', 65);
        cout << "RIWAYAT AKTIVITAS UNTUK USERNAME: " << user << endl;
        cetakGaris('-', 65);
        bool ditemukan = false;
        for (int i = 0; i < jumlahLog; i++) {
            if (logUsername[i] == user) {
                cout << "[" << logWaktu[i] << "] " << logRole[i] << " -> " << logKeterangan[i] << endl;
                ditemukan = true;
            }
        }
        if (!ditemukan) {
            cout << "Tidak ada riwayat aktivitas untuk username tersebut" << endl;
        }
        cetakGaris('-', 65);
    }

    void tampilkanDaftarAkunTerkunci() const {
        cetakGaris('-', 65);
        cout << "DAFTAR AKUN YANG SEDANG TERKUNCI" << endl;
        cetakGaris('-', 65);
        bool adaTerkunci = false;
        for (int i = 0; i < jumlahAkunDipantau; i++) {
            if (statusTerkunci[i]) {
                cout << "- " << usernamePantauan[i] << " (Percobaan Gagal: "
                     << jumlahPercobaanGagal[i] << ")" << endl;
                adaTerkunci = true;
            }
        }
        if (!adaTerkunci) {
            cout << "Tidak ada akun yang sedang terkunci" << endl;
        }
        cetakGaris('-', 65);
    }

    bool simpanLogKeFile(string namaFile) const {
        ofstream file(namaFile.c_str());
        if (!file.is_open()) {
            cout << "[Gagal] Tidak dapat membuka file untuk menyimpan log" << endl;
            return false;
        }
        file << jumlahLog << endl;
        for (int i = 0; i < jumlahLog; i++) {
            file << logUsername[i] << endl;
            file << logRole[i] << endl;
            file << logKeterangan[i] << endl;
            file << logWaktu[i] << endl;
        }
        file.close();
        cout << "[Sukses] Log berhasil disimpan ke file " << namaFile << endl;
        return true;
    }

    bool muatLogDariFile(string namaFile) {
        ifstream file(namaFile.c_str());
        if (!file.is_open()) {
            cout << "[Gagal] Tidak dapat membuka file log" << endl;
            return false;
        }
        file >> jumlahLog;
        file.ignore();
        for (int i = 0; i < jumlahLog && i < MAX_LOG; i++) {
            getline(file, logUsername[i]);
            getline(file, logRole[i]);
            getline(file, logKeterangan[i]);
            file >> logWaktu[i];
            file.ignore();
        }
        file.close();
        cout << "[Sukses] Log berhasil dimuat dari file " << namaFile << endl;
        return true;
    }

    void setBatasPercobaanGagal(int batas) {
        if (batas > 0) {
            batasPercobaanGagal = batas;
        }
    }

    int getBatasPercobaanGagal() const {
        return batasPercobaanGagal;
    }

    int getJumlahLog() const {
        return jumlahLog;
    }

    int getJumlahAkunDipantau() const {
        return jumlahAkunDipantau;
    }

    static int getTotalLoginBerhasil() {
        return totalLoginBerhasil;
    }

    static int getTotalLoginGagal() {
        return totalLoginGagal;
    }

    static int getTotalLogoutTercatat() {
        return totalLogoutTercatat;
    }

    static int getTotalAkunTerkunci() {
        return totalAkunTerkunci;
    }

    static double hitungPersentaseLoginBerhasil() {
        int total = totalLoginBerhasil + totalLoginGagal;
        if (total == 0) {
            return 0.0;
        }
        return ((double) totalLoginBerhasil / (double) total) * 100.0;
    }

    void tampilkanStatistikLogin() const {
        cetakGaris('=', 65);
        cout << "STATISTIK AKTIVITAS OTENTIKASI SISTEM" << endl;
        cetakGaris('=', 65);
        cout << "Total Login Berhasil   : " << totalLoginBerhasil << endl;
        cout << "Total Login Gagal      : " << totalLoginGagal << endl;
        cout << "Total Logout Tercatat  : " << totalLogoutTercatat << endl;
        cout << "Total Akun Terkunci    : " << totalAkunTerkunci << endl;
        cout << "Persentase Keberhasilan: " << hitungPersentaseLoginBerhasil() << "%" << endl;
        cetakGaris('=', 65);
    }

    void tampilkanFormulirLoginAplikasi() const {
        cout << endl;
        cetakGaris('+', 57);
        cout << "|             GERBANG OTENTIKASI UTAMA SYSTEM           |" << endl;
        cout << "|                 UTAMA TRAVEL SYSTEM v2.0              |" << endl;
        cetakGaris('+', 57);
        cout << "  Petunjuk Keamanan:" << endl;
        cout << "  - Jaga kerahasiaan kata sandi Anda dari pihak luar." << endl;
        cout << "  - Pastikan hak akses yang Anda pilih sesuai jabatan." << endl;
        cout << "  - Akun akan terkunci otomatis setelah " << batasPercobaanGagal
             << " kali percobaan gagal." << endl;
        cetakGaris('+', 57);
    }

    void tampilkanMenuOtentikasi() const {
        cout << endl;
        cetakGaris('-', 40);
        cout << "PILIH LEVEL HAK AKSES LOGIN" << endl;
        cetakGaris('-', 40);
        cout << "1. Admin" << endl;
        cout << "2. Petugas" << endl;
        cout << "3. Pelanggan" << endl;
        cout << "0. Batal" << endl;
        cetakGaris('-', 40);
    }
};

#endif
