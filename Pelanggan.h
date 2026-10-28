#ifndef PELANGGAN_H
#define PELANGGAN_H

#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include "User.h"

using namespace std;

class Pelanggan : public User {
private:
    static const int MAX_RIWAYAT = 50;

    string idPelanggan;
    string nama;
    string email;
    string noHP;
    string alamat;
    string tanggalLahir;
    char jenisKelamin;

    int poinLoyalitas;
    int totalTransaksi;
    double totalPengeluaran;

    string riwayatIdPaket[MAX_RIWAYAT];
    double riwayatNominal[MAX_RIWAYAT];
    int jumlahRiwayat;

public:
    Pelanggan() : User() {
        idPelanggan = "";
        nama = "-";
        email = "-";
        noHP = "-";
        alamat = "-";
        tanggalLahir = "-";
        jenisKelamin = 'L';
        poinLoyalitas = 0;
        totalTransaksi = 0;
        totalPengeluaran = 0.0;
        jumlahRiwayat = 0;
        setRole("Pelanggan");
    }

    Pelanggan(const string& idU, const string& uname, const string& pass,
              const string& idP, const string& nm, const string& em, const string& hp)
        : User(idU, uname, pass, "Pelanggan") {
        idPelanggan = idP;
        nama = nm;
        email = em;
        noHP = hp;
        alamat = "-";
        tanggalLahir = "-";
        jenisKelamin = 'L';
        poinLoyalitas = 0;
        totalTransaksi = 0;
        totalPengeluaran = 0.0;
        jumlahRiwayat = 0;
    }

    Pelanggan(const Pelanggan& sumber) : User(sumber) {
        idPelanggan = sumber.idPelanggan;
        nama = sumber.nama;
        email = sumber.email;
        noHP = sumber.noHP;
        alamat = sumber.alamat;
        tanggalLahir = sumber.tanggalLahir;
        jenisKelamin = sumber.jenisKelamin;
        poinLoyalitas = sumber.poinLoyalitas;
        totalTransaksi = sumber.totalTransaksi;
        totalPengeluaran = sumber.totalPengeluaran;
        jumlahRiwayat = sumber.jumlahRiwayat;
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatIdPaket[i] = sumber.riwayatIdPaket[i];
            riwayatNominal[i] = sumber.riwayatNominal[i];
        }
    }

    virtual ~Pelanggan() {}

    Pelanggan& operator=(const Pelanggan& sumber) {
        if (this == &sumber) {
            return *this;
        }
        User::operator=(sumber);
        idPelanggan = sumber.idPelanggan;
        nama = sumber.nama;
        email = sumber.email;
        noHP = sumber.noHP;
        alamat = sumber.alamat;
        tanggalLahir = sumber.tanggalLahir;
        jenisKelamin = sumber.jenisKelamin;
        poinLoyalitas = sumber.poinLoyalitas;
        totalTransaksi = sumber.totalTransaksi;
        totalPengeluaran = sumber.totalPengeluaran;
        jumlahRiwayat = sumber.jumlahRiwayat;
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatIdPaket[i] = sumber.riwayatIdPaket[i];
            riwayatNominal[i] = sumber.riwayatNominal[i];
        }
        return *this;
    }

    string getIdPelanggan() const { return idPelanggan; }
    void setIdPelanggan(const string& idP) { idPelanggan = idP; }

    string getNama() const { return nama; }
    void setNama(const string& nm) { nama = nm; }

    string getEmail() const { return email; }
    void setEmail(const string& em) { email = em; }

    string getNoHP() const { return noHP; }
    void setNoHP(const string& hp) { noHP = hp; }

    string getAlamat() const { return alamat; }
    void setAlamat(const string& al) { alamat = al; }

    string getTanggalLahir() const { return tanggalLahir; }
    void setTanggalLahir(const string& tgl) {
        if (validasiFormatTanggal(tgl)) {
            tanggalLahir = tgl;
        } else {
            cout << "[Validasi Gagal] Format tanggal lahir harus DD-MM-YYYY" << endl;
        }
    }

    char getJenisKelamin() const { return jenisKelamin; }
    void setJenisKelamin(char jk) {
        if (jk == 'L' || jk == 'P') {
            jenisKelamin = jk;
        } else {
            cout << "[Validasi Gagal] Jenis kelamin harus 'L' atau 'P'" << endl;
        }
    }

    int getPoinLoyalitas() const { return poinLoyalitas; }
    int getTotalTransaksi() const { return totalTransaksi; }
    double getTotalPengeluaran() const { return totalPengeluaran; }
    int getJumlahRiwayat() const { return jumlahRiwayat; }

    static bool validasiFormatEmail(const string& em) {
        size_t posisiAt = em.find('@');
        size_t posisiTitik = em.find('.', posisiAt);

        if (posisiAt == string::npos || posisiTitik == string::npos) {
            cout << "[Sistem Validasi] Error: Format alamat email tidak valid (Wajib mengandung '@' dan '.')!" << endl;
            return false;
        }
        if (posisiAt == 0 || posisiTitik == posisiAt + 1 || posisiTitik == em.length() - 1) {
            cout << "[Sistem Validasi] Error: Penempatan karakter '@' atau '.' pada email tidak logis!" << endl;
            return false;
        }
        return true;
    }

    static bool validasiFormatNoHP(const string& hp) {
        if (hp.length() < 10 || hp.length() > 13) {
            cout << "[Sistem Validasi] Error: Panjang nomor HP harus berkisar antara 10 sampai 13 digit!" << endl;
            return false;
        }
        for (size_t i = 0; i < hp.length(); i++) {
            if (!isdigit(static_cast<unsigned char>(hp[i]))) {
                cout << "[Sistem Validasi] Error: Nomor HP hanya boleh diisi oleh karakter angka [0-9]!" << endl;
                return false;
            }
        }
        return true;
    }

    static bool validasiFormatTanggal(const string& tgl) {
        if (tgl.length() != 10) {
            return false;
        }
        if (tgl[2] != '-' || tgl[5] != '-') {
            return false;
        }
        for (int i = 0; i < 10; i++) {
            if (i == 2 || i == 5) {
                continue;
            }
            if (!isdigit(static_cast<unsigned char>(tgl[i]))) {
                return false;
            }
        }
        return true;
    }

    bool tambahRiwayatTransaksi(string idPaket, double nominal) {
        if (nominal <= 0.0) {
            cout << "[Gagal] Nominal transaksi tidak valid" << endl;
            return false;
        }
        if (jumlahRiwayat >= MAX_RIWAYAT) {
            cout << "[Gagal] Kapasitas riwayat transaksi pelanggan sudah penuh" << endl;
            return false;
        }
        riwayatIdPaket[jumlahRiwayat] = idPaket;
        riwayatNominal[jumlahRiwayat] = nominal;
        jumlahRiwayat++;
        totalTransaksi++;
        totalPengeluaran += nominal;
        poinLoyalitas += (int) (nominal / 100000.0);
        return true;
    }

    string hitungTierMembership() const {
        if (totalPengeluaran >= 50000000.0) {
            return "Platinum";
        } else if (totalPengeluaran >= 20000000.0) {
            return "Gold";
        } else if (totalPengeluaran >= 5000000.0) {
            return "Silver";
        }
        return "Bronze";
    }

    double hitungDiskonMember() const {
        string tier = hitungTierMembership();
        if (tier == "Platinum") {
            return 0.15;
        } else if (tier == "Gold") {
            return 0.10;
        } else if (tier == "Silver") {
            return 0.05;
        }
        return 0.0;
    }

    bool isMemberVIP() const {
        string tier = hitungTierMembership();
        return (tier == "Platinum" || tier == "Gold");
    }

    // Dipakai khusus oleh modul persistensi (PersistensiData) untuk memulihkan
    // statistik pelanggan (poin, jumlah transaksi, total pengeluaran) saat data
    // dimuat kembali dari file .txt. Tidak divalidasi karena sumber data terpercaya (file internal).
    void muatDataStatistik(int poin, int transaksi, double pengeluaran) {
        poinLoyalitas = (poin >= 0) ? poin : 0;
        totalTransaksi = (transaksi >= 0) ? transaksi : 0;
        totalPengeluaran = (pengeluaran >= 0.0) ? pengeluaran : 0.0;
    }

    bool tukarPoin(int poinDitukar) {
        if (poinDitukar <= 0) {
            cout << "[Gagal] Jumlah poin yang ditukar tidak valid" << endl;
            return false;
        }
        if (poinDitukar > poinLoyalitas) {
            cout << "[Gagal] Poin loyalitas tidak mencukupi untuk penukaran ini" << endl;
            return false;
        }
        poinLoyalitas -= poinDitukar;
        cout << "[Sukses] Berhasil menukar " << poinDitukar << " poin. Sisa poin: " << poinLoyalitas << endl;
        return true;
    }

    bool validasiKelengkapanProfil() const {
        if (idPelanggan.empty() || nama == "-" || email == "-" || noHP == "-") {
            return false;
        }
        return true;
    }

    bool operator==(const Pelanggan& lain) const {
        return idPelanggan == lain.idPelanggan;
    }

    bool operator<(const Pelanggan& lain) const {
        return totalPengeluaran < lain.totalPengeluaran;
    }

    bool operator>(const Pelanggan& lain) const {
        return totalPengeluaran > lain.totalPengeluaran;
    }

    friend ostream& operator<<(ostream& out, const Pelanggan& p) {
        out << "[" << p.idPelanggan << "] " << p.nama << " (" << p.hitungTierMembership() << ")";
        return out;
    }

    void inputFormulirPelanggan() {
        string inputUname, inputPass, inputNama, inputEmail, inputHP, inputAlamat, inputTanggal;
        char inputJenisKelamin;

        cout << endl;
        cout << "+-------------------------------------------------------+" << endl;
        cout << "|               FORMULIR PENGISIAN DATA PELANGGAN       |" << endl;
        cout << "+-------------------------------------------------------+" << endl;

        cout << " -> Masukkan ID Pelanggan Baru : ";
        cin >> idPelanggan;
        setIdUser("USR-" + idPelanggan);

        cout << " -> Buat Username Akun         : ";
        cin >> inputUname;
        setUsername(inputUname);

        cout << " -> Buat Password Akun         : ";
        cin >> inputPass;
        setPassword(inputPass);

        cin.ignore(10000, '\n');
        cout << " -> Nama Lengkap Sesuai KTP    : ";
        getline(cin, inputNama);
        nama = inputNama;

        do {
            cout << " -> Alamat Email Aktif         : ";
            cin >> inputEmail;
        } while (!validasiFormatEmail(inputEmail));
        email = inputEmail;

        do {
            cout << " -> Nomor HP / WhatsApp        : ";
            cin >> inputHP;
        } while (!validasiFormatNoHP(inputHP));
        noHP = inputHP;

        cin.ignore(10000, '\n');
        cout << " -> Alamat Domisili Lengkap    : ";
        getline(cin, inputAlamat);
        alamat = inputAlamat;

        do {
            cout << " -> Tanggal Lahir (DD-MM-YYYY) : ";
            cin >> inputTanggal;
        } while (!validasiFormatTanggal(inputTanggal));
        tanggalLahir = inputTanggal;

        do {
            cout << " -> Jenis Kelamin (L/P)        : ";
            cin >> inputJenisKelamin;
        } while (inputJenisKelamin != 'L' && inputJenisKelamin != 'P');
        jenisKelamin = inputJenisKelamin;

        cout << "+-------------------------------------------------------+" << endl;
        cout << "| [Sukses] Data keanggotaan pelanggan berhasil direkam. |" << endl;
        cout << "+-------------------------------------------------------+" << endl;
    }

    void tampilkanKartuMemberPelanggan() const {
        cout << endl;
        cout << "+=======================================================+" << endl;
        cout << "|               KARTU DIGITAL MEMBER UTAMA TRAVEL       |" << endl;
        cout << "+=======================================================+" << endl;
        cout << "  > ID Pelanggan   : " << idPelanggan << endl;
        cout << "  > Nama Member    : " << nama << endl;
        cout << "  > Kontak HP      : " << noHP << endl;
        cout << "  > Email Resmi    : " << email << endl;
        cout << "  > Alamat         : " << alamat << endl;
        cout << "  > Tanggal Lahir  : " << tanggalLahir << endl;
        cout << "  > Jenis Kelamin  : " << (jenisKelamin == 'L' ? "Laki-laki" : "Perempuan") << endl;
        cout << "  > Tier Membership: " << hitungTierMembership() << endl;
        cout << "  > Poin Loyalitas : " << poinLoyalitas << endl;
        cout << "  > Total Transaksi: " << totalTransaksi << " kali" << endl;
        cout << "  > Total Belanja  : Rp " << totalPengeluaran << endl;
        cout << "  > Status Akun    : " << getRole() << " Terverifikasi" << endl;
        cout << "+=======================================================+" << endl;
    }

    void tampilkanRiwayatTransaksi() const {
        cout << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "RIWAYAT TRANSAKSI PELANGGAN: " << nama << endl;
        cout << "---------------------------------------------------------" << endl;
        if (jumlahRiwayat == 0) {
            cout << "Belum ada riwayat transaksi" << endl;
        }
        for (int i = 0; i < jumlahRiwayat; i++) {
            cout << (i + 1) << ". Paket: " << riwayatIdPaket[i]
                 << " | Nominal: Rp " << riwayatNominal[i] << endl;
        }
        cout << "---------------------------------------------------------" << endl;
    }

    bool simpanProfilKeFile(string namaFile) const {
        ofstream file(namaFile.c_str());
        if (!file.is_open()) {
            cout << "[Gagal] Tidak dapat membuka file untuk menyimpan profil" << endl;
            return false;
        }
        file << idPelanggan << endl;
        file << nama << endl;
        file << email << endl;
        file << noHP << endl;
        file << alamat << endl;
        file << tanggalLahir << endl;
        file << jenisKelamin << endl;
        file << poinLoyalitas << endl;
        file << totalTransaksi << endl;
        file << totalPengeluaran << endl;
        file.close();
        return true;
    }

    static int cariIndexById(const Pelanggan* daftarPelanggan, int jumlahPelanggan, string id) {
        for (int i = 0; i < jumlahPelanggan; i++) {
            if (daftarPelanggan[i].getIdPelanggan() == id) {
                return i;
            }
        }
        return -1;
    }

    static int cariIndexByUsername(const Pelanggan* daftarPelanggan, int jumlahPelanggan, string username) {
        for (int i = 0; i < jumlahPelanggan; i++) {
            if (daftarPelanggan[i].getUsername() == username) {
                return i;
            }
        }
        return -1;
    }

    static void urutkanBerdasarkanPengeluaran(Pelanggan* daftarPelanggan, int jumlahPelanggan, bool naik) {
        for (int i = 0; i < jumlahPelanggan - 1; i++) {
            for (int j = 0; j < jumlahPelanggan - 1 - i; j++) {
                bool tukar;
                if (naik) {
                    tukar = daftarPelanggan[j].getTotalPengeluaran() > daftarPelanggan[j + 1].getTotalPengeluaran();
                } else {
                    tukar = daftarPelanggan[j].getTotalPengeluaran() < daftarPelanggan[j + 1].getTotalPengeluaran();
                }
                if (tukar) {
                    Pelanggan temp = daftarPelanggan[j];
                    daftarPelanggan[j] = daftarPelanggan[j + 1];
                    daftarPelanggan[j + 1] = temp;
                }
            }
        }
    }

    static int cariPelangganVIP(const Pelanggan* daftarPelanggan, int jumlahPelanggan, int hasilIndex[]) {
        int jumlahHasil = 0;
        for (int i = 0; i < jumlahPelanggan; i++) {
            if (daftarPelanggan[i].isMemberVIP()) {
                hasilIndex[jumlahHasil] = i;
                jumlahHasil++;
            }
        }
        return jumlahHasil;
    }

    static double hitungTotalPengeluaranSemuaPelanggan(const Pelanggan* daftarPelanggan, int jumlahPelanggan) {
        double total = 0.0;
        for (int i = 0; i < jumlahPelanggan; i++) {
            total += daftarPelanggan[i].getTotalPengeluaran();
        }
        return total;
    }

    static double hitungRataRataPengeluaran(const Pelanggan* daftarPelanggan, int jumlahPelanggan) {
        if (jumlahPelanggan <= 0) {
            return 0.0;
        }
        return hitungTotalPengeluaranSemuaPelanggan(daftarPelanggan, jumlahPelanggan) / jumlahPelanggan;
    }
};

#endif
