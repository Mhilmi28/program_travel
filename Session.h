#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <sstream>

using namespace std;

class SessionException : public runtime_error {
public:
    explicit SessionException(const string& pesan) : runtime_error(pesan) {}
};

class Session {
public:
    static const int MAKS_RIWAYAT = 30;

private:
    string usernameAktif;
    string roleAktif;
    string idUserAktif;
    string waktuLogin;
    bool statusLoginActive;

    time_t waktuLoginEpoch;
    time_t waktuAktivitasTerakhir;

    string riwayatSesi[MAKS_RIWAYAT];
    int jumlahRiwayat;
    int jumlahLoginBerhasil;

    static int totalSesiAktifSaatIni;
    static long totalLoginSepanjangWaktu;
    static long totalLogoutSepanjangWaktu;

    string dapatkanWaktuSistem() const {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        char buffer[25];
        strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", now);
        return string(buffer);
    }

    void catatRiwayatSesi(const string& keterangan) {
        ostringstream oss;
        oss << "[" << dapatkanWaktuSistem() << "] " << keterangan;
        if (jumlahRiwayat < MAKS_RIWAYAT) {
            riwayatSesi[jumlahRiwayat] = oss.str();
            jumlahRiwayat++;
        } else {
            for (int i = 1; i < MAKS_RIWAYAT; i++) {
                riwayatSesi[i - 1] = riwayatSesi[i];
            }
            riwayatSesi[MAKS_RIWAYAT - 1] = oss.str();
        }
    }

    string formatDurasiDetik(double detik) const {
        long total = static_cast<long>(detik);
        long jam = total / 3600;
        long menit = (total % 3600) / 60;
        long sisaDetik = total % 60;
        ostringstream oss;
        oss << jam << "j " << menit << "m " << sisaDetik << "d";
        return oss.str();
    }

public:
    Session() {
        usernameAktif = "";
        roleAktif = "GUEST";
        idUserAktif = "";
        waktuLogin = "-";
        statusLoginActive = false;
        waktuLoginEpoch = 0;
        waktuAktivitasTerakhir = 0;
        jumlahRiwayat = 0;
        jumlahLoginBerhasil = 0;
    }

    virtual ~Session() {
        if (statusLoginActive) {
            totalSesiAktifSaatIni--;
        }
    }

    string getUsernameAktif() const { return usernameAktif; }
    string getRoleAktif() const { return roleAktif; }
    string getIdUserAktif() const { return idUserAktif; }
    string getWaktuLogin() const { return waktuLogin; }
    bool isLoginActive() const { return statusLoginActive; }

    int getJumlahLoginBerhasil() const { return jumlahLoginBerhasil; }
    int getJumlahRiwayat() const { return jumlahRiwayat; }

    string getRiwayatKe(int index) const {
        if (index < 0 || index >= jumlahRiwayat) {
            throw SessionException("Index riwayat sesi di luar batas.");
        }
        return riwayatSesi[index];
    }

    static int getTotalSesiAktifSaatIni() {
        return totalSesiAktifSaatIni;
    }

    static long getTotalLoginSepanjangWaktu() {
        return totalLoginSepanjangWaktu;
    }

    static long getTotalLogoutSepanjangWaktu() {
        return totalLogoutSepanjangWaktu;
    }

    bool apakahRole(const string& role) const {
        return statusLoginActive && roleAktif == role;
    }

    bool apakahAdmin() const { return apakahRole("Admin"); }
    bool apakahPetugas() const { return apakahRole("Petugas"); }
    bool apakahPelanggan() const { return apakahRole("Pelanggan"); }

    void perpanjangSesi() {
        perbaruiAktivitas();
        catatRiwayatSesi("Sesi diperpanjang secara manual oleh pengguna.");
    }

    void gantiRoleSementara(const string& roleBaru) {
        if (!statusLoginActive) {
            throw SessionException("Tidak dapat mengubah role, tidak ada sesi yang aktif.");
        }
        if (roleBaru.empty()) {
            throw SessionException("Role baru tidak boleh kosong.");
        }
        catatRiwayatSesi("Role sesi diubah sementara dari '" + roleAktif + "' menjadi '" + roleBaru + "'.");
        roleAktif = roleBaru;
    }

    double hitungSisaWaktuTimeoutDetik(int batasMenit) const {
        if (batasMenit <= 0) {
            throw SessionException("Batas menit timeout harus bernilai positif.");
        }
        if (!statusLoginActive) {
            return 0.0;
        }
        double selisih = difftime(time(nullptr), waktuAktivitasTerakhir);
        return (batasMenit * 60) - selisih;
    }

    string getRingkasanSingkat() const {
        ostringstream oss;
        if (statusLoginActive) {
            oss << "Session[" << usernameAktif << ", role=" << roleAktif
                << ", login=" << waktuLogin << ", durasi=" << getDurasiSesiAktif() << "]";
        } else {
            oss << "Session[GUEST, tidak ada sesi aktif]";
        }
        return oss.str();
    }

    bool buatSesiBaru(const string& id, const string& uname, const string& role) {
        if (this->statusLoginActive) {
            cout << "[Sistem Sesi] Error: Sesi lain sedang aktif! Silakan logout terlebih dahulu.\n";
            return false;
        }

        if (id.empty() || uname.empty() || role.empty()) {
            cout << "[Sistem Sesi] Error: Parameter data pembentukan sesi tidak boleh kosong!\n";
            return false;
        }

        this->idUserAktif = id;
        this->usernameAktif = uname;
        this->roleAktif = role;
        this->waktuLogin = dapatkanWaktuSistem();
        this->statusLoginActive = true;
        this->waktuLoginEpoch = time(nullptr);
        this->waktuAktivitasTerakhir = waktuLoginEpoch;

        jumlahLoginBerhasil++;
        totalLoginSepanjangWaktu++;
        totalSesiAktifSaatIni++;
        catatRiwayatSesi("Login berhasil sebagai '" + uname + "' dengan hak akses '" + role + "'.");

        return true;
    }

    bool hancurkanSesiAktif() {
        if (!this->statusLoginActive) {
            cout << "[Sistem Sesi] Peringatan: Tidak ada sesi aktif yang dapat dihancurkan.\n";
            return false;
        }

        cout << "[Sistem Sesi] Mengakhiri sesi pengguna: " << this->usernameAktif << "...\n";

        string ringkasanDurasi = getDurasiSesiAktif();
        catatRiwayatSesi("Logout, total durasi sesi aktif: " + ringkasanDurasi);

        this->usernameAktif = "";
        this->roleAktif = "GUEST";
        this->idUserAktif = "";
        this->waktuLogin = "-";
        this->statusLoginActive = false;
        this->waktuLoginEpoch = 0;
        this->waktuAktivitasTerakhir = 0;
        totalSesiAktifSaatIni--;
        totalLogoutSepanjangWaktu++;

        cout << "[Sistem Sesi] Sesi berhasil dibersihkan dengan aman dari memori kerja.\n";
        return true;
    }

    void perbaruiAktivitas() {
        if (!statusLoginActive) {
            throw SessionException("Tidak dapat memperbarui aktivitas, tidak ada sesi yang aktif.");
        }
        waktuAktivitasTerakhir = time(nullptr);
    }

    bool apakahSesiTimeout(int batasMenit) const {
        if (batasMenit <= 0) {
            throw SessionException("Batas menit timeout harus bernilai positif.");
        }
        if (!statusLoginActive) {
            return false;
        }
        double selisihDetik = difftime(time(nullptr), waktuAktivitasTerakhir);
        return selisihDetik > (batasMenit * 60);
    }

    string getDurasiSesiAktif() const {
        if (!statusLoginActive) {
            return "0j 0m 0d";
        }
        double selisih = difftime(time(nullptr), waktuLoginEpoch);
        return formatDurasiDetik(selisih);
    }

    bool validasiOtoritasAkses(const string& roleTarget) const {
        if (!this->statusLoginActive) {
            cout << "[Security Policy] Akses Ditolak: Anda belum melakukan login ke dalam sistem!\n";
            return false;
        }
        if (this->roleAktif != roleTarget) {
            cout << "[Security Policy] Akses Ditolak: Hak akses '" << this->roleAktif
                 << "' tidak diizinkan menggunakan fitur khusus '" << roleTarget << "' ini!\n";
            return false;
        }
        return true;
    }

    bool validasiOtoritasAksesMultiRole(const string rolesDiizinkan[], int jumlahRole) const {
        if (!this->statusLoginActive) {
            cout << "[Security Policy] Akses Ditolak: Anda belum melakukan login ke dalam sistem!\n";
            return false;
        }
        for (int i = 0; i < jumlahRole; i++) {
            if (this->roleAktif == rolesDiizinkan[i]) {
                return true;
            }
        }
        cout << "[Security Policy] Akses Ditolak: Hak akses '" << this->roleAktif
             << "' tidak termasuk dalam daftar role yang diizinkan untuk fitur ini!\n";
        return false;
    }

    void tampilkanKartuSesi() const {
        cout << "\n+-------------------------------------------------------+\n";
        cout << "|              STATUS INFORMASI SESI PENGGUNA           |\n";
        cout << "+-------------------------------------------------------+\n";
        if (this->statusLoginActive) {
            cout << "  > Status Koneksi     : [ CONNECTED / ACTIVE ]\n";
            cout << "  > ID Akun Pengguna   : " << this->idUserAktif << "\n";
            cout << "  > Nama Akun (Uname)  : " << this->usernameAktif << "\n";
            cout << "  > Otoritas Hak Akses : " << this->roleAktif << "\n";
            cout << "  > Timestamp Mulai    : " << this->waktuLogin << "\n";
            cout << "  > Durasi Sesi Aktif  : " << getDurasiSesiAktif() << "\n";
        } else {
            cout << "  > Status Koneksi     : [ DISCONNECTED / ANONYMOUS ]\n";
            cout << "  > Otoritas Hak Akses : " << this->roleAktif << " (Akses Menu Terbatas)\n";
            cout << "  > Tindakan Disarankan: Silakan lakukan pendaftaran atau login.\n";
        }
        cout << "+-------------------------------------------------------+\n";
    }

    void tampilkanRiwayatSesi() const {
        cout << "=== Riwayat Aktivitas Sesi Objek Ini ===\n";
        for (int i = 0; i < jumlahRiwayat; i++) {
            cout << (i + 1) << ". " << riwayatSesi[i] << "\n";
        }
    }

    friend ostream& operator<<(ostream& out, const Session& sesi) {
        out << sesi.getRingkasanSingkat();
        return out;
    }
};

#endif
