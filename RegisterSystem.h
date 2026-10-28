#ifndef REGISTERSYSTEM_H
#define REGISTERSYSTEM_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <ctime>
#include "Petugas.h"
#include "Pelanggan.h"

using namespace std;

class RegisterSystemException : public runtime_error {
public:
    explicit RegisterSystemException(const string& pesan) : runtime_error(pesan) {}
};

class RegisterSystem {
public:
    static const int MAKS_RIWAYAT = 50;
    static const int PANJANG_MINIMUM_USERNAME = 4;
    static const int PANJANG_MINIMUM_PASSWORD = 6;

private:
    string riwayatRegistrasi[MAKS_RIWAYAT];
    int jumlahRiwayat;

    int totalRegistrasiPelangganBerhasil;
    int totalRegistrasiPetugasBerhasil;
    int totalRegistrasiGagal;

    string ambilWaktuSekarang() const {
        time_t sekarang = time(nullptr);
        string waktu = ctime(&sekarang);
        if (!waktu.empty() && waktu.back() == '\n') {
            waktu.pop_back();
        }
        return waktu;
    }

    void catatRiwayatRegistrasi(const string& keterangan) {
        ostringstream oss;
        oss << "[" << ambilWaktuSekarang() << "] " << keterangan;
        if (jumlahRiwayat < MAKS_RIWAYAT) {
            riwayatRegistrasi[jumlahRiwayat] = oss.str();
            jumlahRiwayat++;
        } else {
            for (int i = 1; i < MAKS_RIWAYAT; i++) {
                riwayatRegistrasi[i - 1] = riwayatRegistrasi[i];
            }
            riwayatRegistrasi[MAKS_RIWAYAT - 1] = oss.str();
        }
    }

public:
    RegisterSystem() : jumlahRiwayat(0), totalRegistrasiPelangganBerhasil(0),
                        totalRegistrasiPetugasBerhasil(0), totalRegistrasiGagal(0) {
        catatRiwayatRegistrasi("Subsistem registrasi diinisialisasi.");
    }

    virtual ~RegisterSystem() {}

    int getTotalRegistrasiPelangganBerhasil() const { return totalRegistrasiPelangganBerhasil; }
    int getTotalRegistrasiPetugasBerhasil() const { return totalRegistrasiPetugasBerhasil; }
    int getTotalRegistrasiGagal() const { return totalRegistrasiGagal; }
    int getTotalSeluruhRegistrasi() const {
        return totalRegistrasiPelangganBerhasil + totalRegistrasiPetugasBerhasil + totalRegistrasiGagal;
    }

    int getJumlahRiwayat() const { return jumlahRiwayat; }

    string getRiwayatKe(int index) const {
        if (index < 0 || index >= jumlahRiwayat) {
            throw RegisterSystemException("Index riwayat registrasi di luar batas.");
        }
        return riwayatRegistrasi[index];
    }

    static bool validasiFormatUsername(const string& username) {
        if (username.empty()) {
            cout << "[Sistem Registrasi] Error: Username tidak boleh kosong!\n";
            return false;
        }
        if (static_cast<int>(username.length()) < PANJANG_MINIMUM_USERNAME) {
            cout << "[Sistem Registrasi] Error: Username minimal " << PANJANG_MINIMUM_USERNAME << " karakter!\n";
            return false;
        }
        for (char c : username) {
            if (isspace(static_cast<unsigned char>(c))) {
                cout << "[Sistem Registrasi] Error: Username tidak boleh mengandung spasi!\n";
                return false;
            }
        }
        return true;
    }

    static bool validasiKekuatanPassword(const string& password) {
        if (static_cast<int>(password.length()) < PANJANG_MINIMUM_PASSWORD) {
            cout << "[Sistem Registrasi] Error: Password minimal " << PANJANG_MINIMUM_PASSWORD << " karakter!\n";
            return false;
        }
        return true;
    }

    int cariIndexPelangganByUsername(const string& username, const Pelanggan* daftarPelanggan, int jumlahPelanggan) const {
        if (daftarPelanggan == nullptr) {
            return -1;
        }
        for (int i = 0; i < jumlahPelanggan; i++) {
            if (daftarPelanggan[i].getUsername() == username) {
                return i;
            }
        }
        return -1;
    }

    int cariIndexPetugasByUsername(const string& username, const Petugas* daftarPetugas, int jumlahPetugas) const {
        if (daftarPetugas == nullptr) {
            return -1;
        }
        for (int i = 0; i < jumlahPetugas; i++) {
            if (daftarPetugas[i].getUsername() == username) {
                return i;
            }
        }
        return -1;
    }

    bool cekUnikUsernamePelanggan(const string& username, const Pelanggan* daftarPelanggan, int jumlahPelanggan) const {
        if (daftarPelanggan == nullptr) {
            return true;
        }
        if (cariIndexPelangganByUsername(username, daftarPelanggan, jumlahPelanggan) != -1) {
            cout << "[Sistem Registrasi] Error: Username '" << username << "' sudah terpakai oleh pengguna lain!\n";
            return false;
        }
        return true;
    }

    bool cekUnikUsernamePetugas(const string& username, const Petugas* daftarPetugas, int jumlahPetugas) const {
        if (daftarPetugas == nullptr) {
            return true;
        }
        if (cariIndexPetugasByUsername(username, daftarPetugas, jumlahPetugas) != -1) {
            cout << "[Sistem Registrasi] Error: Username '" << username << "' sudah terdaftar sebagai staf!\n";
            return false;
        }
        return true;
    }

    bool cekUnikUsernameGlobal(const string& username,
                               const Pelanggan* daftarPelanggan, int jumlahPelanggan,
                               const Petugas* daftarPetugas, int jumlahPetugas) const {
        return cekUnikUsernamePelanggan(username, daftarPelanggan, jumlahPelanggan) &&
               cekUnikUsernamePetugas(username, daftarPetugas, jumlahPetugas);
    }

    void prosesRegistrasiPelanggan(Pelanggan*& daftarPelanggan, int& jumlahPelanggan, int& kapasitasPelanggan) {
        if (kapasitasPelanggan <= 0) {
            throw RegisterSystemException("Kapasitas awal daftar pelanggan tidak valid.");
        }

        if (jumlahPelanggan >= kapasitasPelanggan) {
            int kapasitasBaru = kapasitasPelanggan * 2;
            Pelanggan* temp = new Pelanggan[kapasitasBaru];
            for (int i = 0; i < jumlahPelanggan; i++) {
                temp[i] = daftarPelanggan[i];
            }
            delete[] daftarPelanggan;
            daftarPelanggan = temp;
            kapasitasPelanggan = kapasitasBaru;
        }

        Pelanggan pelangganBaru;
        string targetUname;

        cout << "\n+-------------------------------------------------------+\n";
        cout << "|             FORMULIR REGISTRASI MEMBER BARU          |\n";
        cout << "|                 UTAMA TRAVEL INDONESIA               |\n";
        cout << "+-------------------------------------------------------+\n";

        pelangganBaru.inputFormulirPelanggan();
        targetUname = pelangganBaru.getUsername();

        if (!validasiFormatUsername(targetUname)) {
            totalRegistrasiGagal++;
            catatRiwayatRegistrasi("Registrasi pelanggan gagal, format username tidak valid: " + targetUname);
            cout << "\n[Registrasi Gagal] Format username tidak memenuhi standar keamanan sistem.\n";
            cout << "+-------------------------------------------------------+\n";
            return;
        }

        if (cekUnikUsernamePelanggan(targetUname, daftarPelanggan, jumlahPelanggan)) {
            daftarPelanggan[jumlahPelanggan] = pelangganBaru;
            jumlahPelanggan++;
            totalRegistrasiPelangganBerhasil++;
            catatRiwayatRegistrasi("Registrasi pelanggan berhasil: " + targetUname);
            cout << "\n[Registrasi Berhasil] Akun Anda terdaftar. Silakan lakukan Login.\n";
        } else {
            totalRegistrasiGagal++;
            catatRiwayatRegistrasi("Registrasi pelanggan gagal, username duplikat: " + targetUname);
            cout << "\n[Registrasi Gagal] Mohon ulangi proses pendaftaran menggunakan username unik.\n";
        }
        cout << "+-------------------------------------------------------+\n";
    }

    void prosesRegistrasiPetugas(Petugas*& daftarPetugas, int& jumlahPetugas, int& kapasitasPetugas) {
        if (kapasitasPetugas <= 0) {
            throw RegisterSystemException("Kapasitas awal daftar petugas tidak valid.");
        }

        if (jumlahPetugas >= kapasitasPetugas) {
            int kapasitasBaru = kapasitasPetugas * 2;
            Petugas* temp = new Petugas[kapasitasBaru];
            for (int i = 0; i < jumlahPetugas; i++) {
                temp[i] = daftarPetugas[i];
            }
            delete[] daftarPetugas;
            daftarPetugas = temp;
            kapasitasPetugas = kapasitasBaru;
        }

        Petugas petugasBaru;
        string targetUname;

        cout << "\n+-------------------------------------------------------+\n";
        cout << "|             FORMULIR PENDAFTARAN STAF OPERASIONAL     |\n";
        cout << "|                 OTORITAS SUPER ADMIN PANEL            |\n";
        cout << "+-------------------------------------------------------+\n";

        petugasBaru.inputFormulirPetugas();
        targetUname = petugasBaru.getUsername();

        if (!validasiFormatUsername(targetUname)) {
            totalRegistrasiGagal++;
            catatRiwayatRegistrasi("Registrasi petugas gagal, format username tidak valid: " + targetUname);
            cout << "\n[Gagal] Format username staf tidak memenuhi standar keamanan sistem.\n";
            cout << "+-------------------------------------------------------+\n";
            return;
        }

        if (cekUnikUsernamePetugas(targetUname, daftarPetugas, jumlahPetugas)) {
            daftarPetugas[jumlahPetugas] = petugasBaru;
            jumlahPetugas++;
            totalRegistrasiPetugasBerhasil++;
            catatRiwayatRegistrasi("Registrasi petugas berhasil: " + targetUname);
            cout << "\n[Sukses] Akun Petugas baru berhasil diaktivasi ke dalam sistem database.\n";
        } else {
            totalRegistrasiGagal++;
            catatRiwayatRegistrasi("Registrasi petugas gagal, username duplikat: " + targetUname);
            cout << "\n[Gagal] Akun Petugas ditolak karena indikasi duplikasi username.\n";
        }
        cout << "+-------------------------------------------------------+\n";
    }

    bool hapusAkunPelanggan(Pelanggan* daftarPelanggan, int& jumlahPelanggan, const string& username) {
        int index = cariIndexPelangganByUsername(username, daftarPelanggan, jumlahPelanggan);
        if (index == -1) {
            cout << "[Sistem Registrasi] Error: Username pelanggan '" << username << "' tidak ditemukan!\n";
            return false;
        }
        for (int i = index; i < jumlahPelanggan - 1; i++) {
            daftarPelanggan[i] = daftarPelanggan[i + 1];
        }
        jumlahPelanggan--;
        catatRiwayatRegistrasi("Akun pelanggan dihapus: " + username);
        cout << "[Sukses] Akun pelanggan '" << username << "' berhasil dihapus dari sistem.\n";
        return true;
    }

    bool hapusAkunPetugas(Petugas* daftarPetugas, int& jumlahPetugas, const string& username) {
        int index = cariIndexPetugasByUsername(username, daftarPetugas, jumlahPetugas);
        if (index == -1) {
            cout << "[Sistem Registrasi] Error: Username petugas '" << username << "' tidak ditemukan!\n";
            return false;
        }
        for (int i = index; i < jumlahPetugas - 1; i++) {
            daftarPetugas[i] = daftarPetugas[i + 1];
        }
        jumlahPetugas--;
        catatRiwayatRegistrasi("Akun petugas dihapus: " + username);
        cout << "[Sukses] Akun petugas '" << username << "' berhasil dihapus dari sistem.\n";
        return true;
    }

    void tampilkanStatistikRegistrasi() const {
        cout << "\n+=======================================================+\n";
        cout << "|             STATISTIK SUBSISTEM REGISTRASI            |\n";
        cout << "+=======================================================+\n";
        cout << "  Total Registrasi Pelanggan Berhasil : " << totalRegistrasiPelangganBerhasil << "\n";
        cout << "  Total Registrasi Petugas Berhasil   : " << totalRegistrasiPetugasBerhasil << "\n";
        cout << "  Total Registrasi Gagal              : " << totalRegistrasiGagal << "\n";
        cout << "  Total Keseluruhan Transaksi         : " << getTotalSeluruhRegistrasi() << "\n";
        cout << "+=======================================================+\n";
    }

    void tampilkanRiwayatRegistrasi() const {
        cout << "=== Riwayat Aktivitas Subsistem Registrasi ===\n";
        for (int i = 0; i < jumlahRiwayat; i++) {
            cout << (i + 1) << ". " << riwayatRegistrasi[i] << "\n";
        }
    }
};

#endif
