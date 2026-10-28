#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <ctime>
#include "User.h"

using namespace std;

class AdminException : public runtime_error {
public:
    explicit AdminException(const string& pesan) : runtime_error(pesan) {}
};

class Admin : public User {
public:
    enum class TingkatOtoritas {
        MODERATOR = 1,
        REGULAR_ADMIN = 2,
        SUPER_ADMIN = 3,
        SYSTEM_OWNER = 5
    };

private:
    static const int MAKS_RIWAYAT = 100;
    static const int MAKS_PERSETUJUAN = 50;
    static const int MAKS_PENOLAKAN = 50;
    static const int MAKS_DESTINASI = 50;

    string hakAkses;
    string tokenKeamanan;
    int levelOtoritas;

    string riwayatAktivitas[MAKS_RIWAYAT];
    int jumlahRiwayat;

    string daftarPersetujuanPemesanan[MAKS_PERSETUJUAN];
    int jumlahPersetujuan;

    string daftarPenolakanPemesanan[MAKS_PENOLAKAN];
    int jumlahPenolakan;

    string destinasiDikelola[MAKS_DESTINASI];
    int jumlahDestinasi;

    int jumlahAksiDilakukan;
    bool statusAktif;
    string tanggalBergabung;

    static int totalAdminTerdaftar;
    static const int LEVEL_MINIMUM = 1;
    static const int LEVEL_MAKSIMUM = 5;

    bool isHakAksesValid(const string& hak) const {
        return hak == "SUPER_ADMIN" || hak == "REGULAR_ADMIN" ||
               hak == "MODERATOR" || hak == "SYSTEM_OWNER";
    }

    bool isLevelValid(int level) const {
        return level >= LEVEL_MINIMUM && level <= LEVEL_MAKSIMUM;
    }

    string ambilWaktuSekarang() const {
        time_t sekarang = time(nullptr);
        string waktu = ctime(&sekarang);
        if (!waktu.empty() && waktu.back() == '\n') {
            waktu.pop_back();
        }
        return waktu;
    }

    void catatAktivitas(const string& aksi) {
        ostringstream oss;
        oss << "[" << ambilWaktuSekarang() << "] " << aksi;
        if (jumlahRiwayat < MAKS_RIWAYAT) {
            riwayatAktivitas[jumlahRiwayat] = oss.str();
            jumlahRiwayat++;
        }
        jumlahAksiDilakukan++;
    }

    void pastikanAkunAktif() const {
        if (!statusAktif) {
            throw AdminException("Operasi ditolak, akun admin sedang nonaktif.");
        }
    }

    void pastikanOtorisasiMinimal(int levelDibutuhkan) const {
        if (levelOtoritas < levelDibutuhkan) {
            throw AdminException("Level otoritas tidak mencukupi untuk aksi ini.");
        }
    }

public:
    Admin()
        : User(), hakAkses("REGULAR_ADMIN"), tokenKeamanan("ADM-999"),
          levelOtoritas(1), jumlahRiwayat(0), jumlahPersetujuan(0),
          jumlahPenolakan(0), jumlahDestinasi(0), jumlahAksiDilakukan(0),
          statusAktif(true) {
        this->role = "Admin";
        tanggalBergabung = ambilWaktuSekarang();
        totalAdminTerdaftar++;
        catatAktivitas("Akun admin dibuat menggunakan konstruktor default.");
    }

    Admin(const string& idU, const string& uname, const string& pass,
          const string& hak, const string& token, int level)
        : User(idU, uname, pass, "Admin"), hakAkses(hak), tokenKeamanan(token),
          levelOtoritas(level), jumlahRiwayat(0), jumlahPersetujuan(0),
          jumlahPenolakan(0), jumlahDestinasi(0), jumlahAksiDilakukan(0),
          statusAktif(true) {
        if (!isHakAksesValid(hak)) {
            throw AdminException("Hak akses tidak valid: " + hak);
        }
        if (!isLevelValid(level)) {
            throw AdminException("Level otoritas harus berada pada rentang 1 sampai 5.");
        }
        tanggalBergabung = ambilWaktuSekarang();
        totalAdminTerdaftar++;
        catatAktivitas("Akun admin dibuat untuk pengguna: " + uname);
    }

    Admin(const Admin& lain)
        : User(lain), hakAkses(lain.hakAkses), tokenKeamanan(lain.tokenKeamanan),
          levelOtoritas(lain.levelOtoritas), jumlahRiwayat(lain.jumlahRiwayat),
          jumlahPersetujuan(lain.jumlahPersetujuan), jumlahPenolakan(lain.jumlahPenolakan),
          jumlahDestinasi(lain.jumlahDestinasi), jumlahAksiDilakukan(lain.jumlahAksiDilakukan),
          statusAktif(lain.statusAktif), tanggalBergabung(lain.tanggalBergabung) {
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatAktivitas[i] = lain.riwayatAktivitas[i];
        }
        for (int i = 0; i < jumlahPersetujuan; i++) {
            daftarPersetujuanPemesanan[i] = lain.daftarPersetujuanPemesanan[i];
        }
        for (int i = 0; i < jumlahPenolakan; i++) {
            daftarPenolakanPemesanan[i] = lain.daftarPenolakanPemesanan[i];
        }
        for (int i = 0; i < jumlahDestinasi; i++) {
            destinasiDikelola[i] = lain.destinasiDikelola[i];
        }
        totalAdminTerdaftar++;
    }

    Admin& operator=(const Admin& lain) {
        if (this == &lain) {
            return *this;
        }
        User::operator=(lain);
        hakAkses = lain.hakAkses;
        tokenKeamanan = lain.tokenKeamanan;
        levelOtoritas = lain.levelOtoritas;
        jumlahAksiDilakukan = lain.jumlahAksiDilakukan;
        statusAktif = lain.statusAktif;
        tanggalBergabung = lain.tanggalBergabung;

        jumlahRiwayat = lain.jumlahRiwayat;
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatAktivitas[i] = lain.riwayatAktivitas[i];
        }

        jumlahPersetujuan = lain.jumlahPersetujuan;
        for (int i = 0; i < jumlahPersetujuan; i++) {
            daftarPersetujuanPemesanan[i] = lain.daftarPersetujuanPemesanan[i];
        }

        jumlahPenolakan = lain.jumlahPenolakan;
        for (int i = 0; i < jumlahPenolakan; i++) {
            daftarPenolakanPemesanan[i] = lain.daftarPenolakanPemesanan[i];
        }

        jumlahDestinasi = lain.jumlahDestinasi;
        for (int i = 0; i < jumlahDestinasi; i++) {
            destinasiDikelola[i] = lain.destinasiDikelola[i];
        }

        return *this;
    }

    virtual ~Admin() {
        totalAdminTerdaftar--;
    }

    string getHakAkses() const {
        return hakAkses;
    }

    string getTokenKeamanan() const {
        return tokenKeamanan;
    }

    int getLevelOtoritas() const {
        return levelOtoritas;
    }

    int getJumlahAksiDilakukan() const {
        return jumlahAksiDilakukan;
    }

    bool getStatusAktif() const {
        return statusAktif;
    }

    string getTanggalBergabung() const {
        return tanggalBergabung;
    }

    int getJumlahRiwayat() const {
        return jumlahRiwayat;
    }

    string getRiwayatKe(int index) const {
        if (index < 0 || index >= jumlahRiwayat) {
            throw AdminException("Index riwayat aktivitas di luar batas.");
        }
        return riwayatAktivitas[index];
    }

    int getJumlahPersetujuan() const {
        return jumlahPersetujuan;
    }

    string getPersetujuanKe(int index) const {
        if (index < 0 || index >= jumlahPersetujuan) {
            throw AdminException("Index persetujuan pemesanan di luar batas.");
        }
        return daftarPersetujuanPemesanan[index];
    }

    int getJumlahPenolakan() const {
        return jumlahPenolakan;
    }

    string getPenolakanKe(int index) const {
        if (index < 0 || index >= jumlahPenolakan) {
            throw AdminException("Index penolakan pemesanan di luar batas.");
        }
        return daftarPenolakanPemesanan[index];
    }

    int getJumlahDestinasi() const {
        return jumlahDestinasi;
    }

    string getDestinasiKe(int index) const {
        if (index < 0 || index >= jumlahDestinasi) {
            throw AdminException("Index destinasi wisata di luar batas.");
        }
        return destinasiDikelola[index];
    }

    void setHakAkses(const string& hakBaru) {
        if (!isHakAksesValid(hakBaru)) {
            throw AdminException("Hak akses tidak valid: " + hakBaru);
        }
        hakAkses = hakBaru;
        catatAktivitas("Hak akses diperbarui menjadi: " + hakBaru);
    }

    void setTokenKeamanan(const string& tokenBaru) {
        if (tokenBaru.size() < 4) {
            throw AdminException("Token keamanan minimal terdiri dari 4 karakter.");
        }
        tokenKeamanan = tokenBaru;
        catatAktivitas("Token keamanan diperbarui.");
    }

    void setLevelOtoritas(int levelBaru) {
        if (!isLevelValid(levelBaru)) {
            throw AdminException("Level otoritas harus berada pada rentang 1 sampai 5.");
        }
        levelOtoritas = levelBaru;
        ostringstream oss;
        oss << "Level otoritas diperbarui menjadi: " << levelBaru;
        catatAktivitas(oss.str());
    }

    void setStatusAktif(bool status) {
        statusAktif = status;
        catatAktivitas(status ? "Akun diaktifkan kembali." : "Akun dinonaktifkan.");
    }

    bool verifikasiToken(const string& tokenInput) const {
        return tokenInput == tokenKeamanan;
    }

    void perbaruiTokenKeamanan(const string& tokenBaru, const string& tokenLama) {
        pastikanAkunAktif();
        if (!verifikasiToken(tokenLama)) {
            throw AdminException("Token lama tidak sesuai, pembaruan token dibatalkan.");
        }
        setTokenKeamanan(tokenBaru);
    }

    bool cekOtorisasi(int levelDibutuhkan) const {
        return levelOtoritas >= levelDibutuhkan;
    }

    bool isSuperAdmin() const {
        return hakAkses == "SUPER_ADMIN" || hakAkses == "SYSTEM_OWNER";
    }

    bool isRegularAdmin() const {
        return hakAkses == "REGULAR_ADMIN";
    }

    bool isModerator() const {
        return hakAkses == "MODERATOR";
    }

    void tingkatkanLevelOtoritas() {
        pastikanAkunAktif();
        if (levelOtoritas >= LEVEL_MAKSIMUM) {
            throw AdminException("Level otoritas sudah berada pada batas maksimum.");
        }
        levelOtoritas++;
        catatAktivitas("Level otoritas dinaikkan satu tingkat.");
    }

    void turunkanLevelOtoritas() {
        pastikanAkunAktif();
        if (levelOtoritas <= LEVEL_MINIMUM) {
            throw AdminException("Level otoritas sudah berada pada batas minimum.");
        }
        levelOtoritas--;
        catatAktivitas("Level otoritas diturunkan satu tingkat.");
    }

    void tambahDestinasiWisata(const string& namaDestinasi) {
        pastikanAkunAktif();
        pastikanOtorisasiMinimal(2);
        if (jumlahDestinasi >= MAKS_DESTINASI) {
            throw AdminException("Kapasitas daftar destinasi wisata sudah penuh.");
        }
        destinasiDikelola[jumlahDestinasi] = namaDestinasi;
        jumlahDestinasi++;
        catatAktivitas("Menambahkan destinasi wisata baru: " + namaDestinasi);
    }

    void hapusDestinasiWisata(const string& namaDestinasi) {
        pastikanAkunAktif();
        pastikanOtorisasiMinimal(3);
        int indexDitemukan = -1;
        for (int i = 0; i < jumlahDestinasi; i++) {
            if (destinasiDikelola[i] == namaDestinasi) {
                indexDitemukan = i;
                break;
            }
        }
        if (indexDitemukan == -1) {
            throw AdminException("Destinasi wisata tidak ditemukan: " + namaDestinasi);
        }
        for (int i = indexDitemukan; i < jumlahDestinasi - 1; i++) {
            destinasiDikelola[i] = destinasiDikelola[i + 1];
        }
        jumlahDestinasi--;
        catatAktivitas("Menghapus destinasi wisata: " + namaDestinasi);
    }

    void approvePemesanan(const string& idPemesanan) {
        pastikanAkunAktif();
        pastikanOtorisasiMinimal(2);
        if (jumlahPersetujuan >= MAKS_PERSETUJUAN) {
            throw AdminException("Kapasitas daftar persetujuan pemesanan sudah penuh.");
        }
        daftarPersetujuanPemesanan[jumlahPersetujuan] = idPemesanan;
        jumlahPersetujuan++;
        catatAktivitas("Menyetujui pemesanan dengan ID: " + idPemesanan);
    }

    void tolakPemesanan(const string& idPemesanan, const string& alasan) {
        pastikanAkunAktif();
        pastikanOtorisasiMinimal(2);
        if (jumlahPenolakan >= MAKS_PENOLAKAN) {
            throw AdminException("Kapasitas daftar penolakan pemesanan sudah penuh.");
        }
        ostringstream oss;
        oss << idPemesanan << " (alasan: " << alasan << ")";
        daftarPenolakanPemesanan[jumlahPenolakan] = oss.str();
        jumlahPenolakan++;
        catatAktivitas("Menolak pemesanan dengan ID: " + idPemesanan + ", alasan: " + alasan);
    }

    void aturDiskonPaketWisata(const string& kodePaket, double persentaseDiskon) {
        pastikanAkunAktif();
        pastikanOtorisasiMinimal(3);
        if (persentaseDiskon < 0.0 || persentaseDiskon > 100.0) {
            throw AdminException("Persentase diskon harus berada pada rentang 0 sampai 100.");
        }
        ostringstream oss;
        oss << "Mengatur diskon " << persentaseDiskon << "% untuk paket wisata: " << kodePaket;
        catatAktivitas(oss.str());
    }

    void kelolaDataPengguna(const string& idUser, const string& aksi) {
        pastikanAkunAktif();
        pastikanOtorisasiMinimal(2);
        catatAktivitas("Mengelola data pengguna [" + idUser + "] dengan aksi: " + aksi);
    }

    void resetPasswordPengguna(const string& idUser, const string& passwordBaru) {
        pastikanAkunAktif();
        pastikanOtorisasiMinimal(3);
        if (passwordBaru.size() < 6) {
            throw AdminException("Password baru minimal terdiri dari 6 karakter.");
        }
        catatAktivitas("Mereset kata sandi pengguna dengan ID: " + idUser);
    }

    void nonaktifkanAkunPengguna(const string& idUser) {
        pastikanAkunAktif();
        pastikanOtorisasiMinimal(4);
        catatAktivitas("Menonaktifkan akun pengguna dengan ID: " + idUser);
    }

    void aktifkanKembaliAkunPengguna(const string& idUser) {
        pastikanAkunAktif();
        pastikanOtorisasiMinimal(3);
        catatAktivitas("Mengaktifkan kembali akun pengguna dengan ID: " + idUser);
    }

    void generateLaporanTransaksi(int bulan, int tahun) const {
        if (bulan < 1 || bulan > 12) {
            throw AdminException("Bulan tidak valid, harus antara 1 dan 12.");
        }
        cout << "=== Laporan Transaksi Periode " << bulan << "/" << tahun << " ===\n";
        cout << "Disusun oleh admin: " << this->username << " (" << hakAkses << ")\n";
        cout << "Total persetujuan pemesanan: " << jumlahPersetujuan << "\n";
        cout << "Total penolakan pemesanan: " << jumlahPenolakan << "\n";
        cout << "Total destinasi dikelola: " << jumlahDestinasi << "\n";
    }

    void lihatSeluruhLogAktivitas() const {
        cout << "=== Log Aktivitas Admin: " << this->username << " ===\n";
        for (int i = 0; i < jumlahRiwayat; i++) {
            cout << (i + 1) << ". " << riwayatAktivitas[i] << "\n";
        }
    }

    void hapusSeluruhLogAktivitas() {
        pastikanOtorisasiMinimal(4);
        jumlahRiwayat = 0;
        catatAktivitas("Seluruh log aktivitas sebelumnya telah dihapus.");
    }

    int hitungTotalPersetujuan() const {
        return jumlahPersetujuan;
    }

    int hitungTotalPenolakan() const {
        return jumlahPenolakan;
    }

    void tampilkanDaftarPersetujuan() const {
        cout << "Daftar pemesanan yang disetujui oleh " << this->username << ":\n";
        for (int i = 0; i < jumlahPersetujuan; i++) {
            cout << "- " << daftarPersetujuanPemesanan[i] << "\n";
        }
    }

    void tampilkanDaftarPenolakan() const {
        cout << "Daftar pemesanan yang ditolak oleh " << this->username << ":\n";
        for (int i = 0; i < jumlahPenolakan; i++) {
            cout << "- " << daftarPenolakanPemesanan[i] << "\n";
        }
    }

    virtual void tampilkanInfo() const {
        cout << "==============================================\n";
        cout << "ID Pengguna       : " << this->idUser << "\n";
        cout << "Username          : " << this->username << "\n";
        cout << "Peran             : " << this->role << "\n";
        cout << "Hak Akses         : " << hakAkses << "\n";
        cout << "Level Otoritas    : " << levelOtoritas << "\n";
        cout << "Status Akun       : " << (statusAktif ? "Aktif" : "Nonaktif") << "\n";
        cout << "Tanggal Bergabung : " << tanggalBergabung << "\n";
        cout << "Jumlah Aksi       : " << jumlahAksiDilakukan << "\n";
        cout << "==============================================\n";
    }

    virtual string ringkasanAdmin() const {
        ostringstream oss;
        oss << "Admin[" << this->username << ", hakAkses=" << hakAkses
            << ", level=" << levelOtoritas << ", aktif=" << (statusAktif ? "ya" : "tidak") << "]";
        return oss.str();
    }

    static int getTotalAdminTerdaftar() {
        return totalAdminTerdaftar;
    }

    static string generateAdminID(int urutan) {
        ostringstream oss;
        oss << "ADM-" << (urutan < 1000 ? "0" : "") << (urutan < 100 ? "0" : "")
            << (urutan < 10 ? "0" : "") << urutan;
        return oss.str();
    }

    static bool validasiFormatToken(const string& token) {
        if (token.size() < 4) {
            return false;
        }
        return token.find("ADM-") == 0 || token.find("SEC-") == 0;
    }

    bool operator==(const Admin& lain) const {
        return this->idUser == lain.idUser && hakAkses == lain.hakAkses;
    }

    bool operator!=(const Admin& lain) const {
        return !(*this == lain);
    }

    bool operator<(const Admin& lain) const {
        return levelOtoritas < lain.levelOtoritas;
    }

    bool operator>(const Admin& lain) const {
        return levelOtoritas > lain.levelOtoritas;
    }

    friend ostream& operator<<(ostream& out, const Admin& admin) {
        out << admin.ringkasanAdmin();
        return out;
    }
};

#endif
