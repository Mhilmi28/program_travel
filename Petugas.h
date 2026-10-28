#ifndef PETUGAS_H
#define PETUGAS_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <ctime>
#include "User.h"

using namespace std;

class PetugasException : public runtime_error {
public:
    explicit PetugasException(const string& pesan) : runtime_error(pesan) {}
};

class Petugas : public User {
public:
    static const int MAKS_RIWAYAT_TUGAS = 50;

private:
    string idPetugas;
    string namaPetugas;
    string nomorTelepon;
    string lokasiTugas;
    bool statusAktif;
    string tanggalBergabung;

    string riwayatTugas[MAKS_RIWAYAT_TUGAS];
    int jumlahRiwayatTugas;
    int jumlahTugasDiselesaikan;

    static int totalPetugasTerdaftar;

    string ambilWaktuSekarang() const {
        time_t sekarang = time(nullptr);
        string waktu = ctime(&sekarang);
        if (!waktu.empty() && waktu.back() == '\n') {
            waktu.pop_back();
        }
        return waktu;
    }

    void catatTugas(const string& keterangan) {
        ostringstream oss;
        oss << "[" << ambilWaktuSekarang() << "] " << keterangan;
        if (jumlahRiwayatTugas < MAKS_RIWAYAT_TUGAS) {
            riwayatTugas[jumlahRiwayatTugas] = oss.str();
            jumlahRiwayatTugas++;
        } else {
            for (int i = 1; i < MAKS_RIWAYAT_TUGAS; i++) {
                riwayatTugas[i - 1] = riwayatTugas[i];
            }
            riwayatTugas[MAKS_RIWAYAT_TUGAS - 1] = oss.str();
        }
    }

    void pastikanPetugasAktif() const {
        if (!statusAktif) {
            throw PetugasException("Operasi ditolak, akun petugas sedang nonaktif.");
        }
    }

    bool validasiTeksTidakKosong(const string& teks) const {
        return !teks.empty();
    }

public:
    Petugas() : User(), idPetugas(""), namaPetugas(""), nomorTelepon("-"),
                lokasiTugas("-"), statusAktif(true), jumlahRiwayatTugas(0),
                jumlahTugasDiselesaikan(0) {
        this->setRole("Petugas");
        tanggalBergabung = ambilWaktuSekarang();
        totalPetugasTerdaftar++;
        catatTugas("Akun petugas dibuat menggunakan konstruktor default.");
    }

    Petugas(const string& idU, const string& uname, const string& pass,
            const string& idPet, const string& namaPet)
        : User(idU, uname, pass, "Petugas"), idPetugas(idPet), namaPetugas(namaPet),
          nomorTelepon("-"), lokasiTugas("-"), statusAktif(true), jumlahRiwayatTugas(0),
          jumlahTugasDiselesaikan(0) {
        if (!validasiTeksTidakKosong(idPet)) {
            throw PetugasException("ID Petugas tidak boleh kosong.");
        }
        if (!validasiTeksTidakKosong(namaPet)) {
            throw PetugasException("Nama petugas tidak boleh kosong.");
        }
        tanggalBergabung = ambilWaktuSekarang();
        totalPetugasTerdaftar++;
        catatTugas("Akun petugas dibuat untuk: " + namaPet);
    }

    Petugas(const string& idU, const string& uname, const string& pass,
            const string& idPet, const string& namaPet,
            const string& telepon, const string& lokasi)
        : User(idU, uname, pass, "Petugas"), idPetugas(idPet), namaPetugas(namaPet),
          nomorTelepon(telepon), lokasiTugas(lokasi), statusAktif(true), jumlahRiwayatTugas(0),
          jumlahTugasDiselesaikan(0) {
        if (!validasiTeksTidakKosong(idPet)) {
            throw PetugasException("ID Petugas tidak boleh kosong.");
        }
        if (!validasiTeksTidakKosong(namaPet)) {
            throw PetugasException("Nama petugas tidak boleh kosong.");
        }
        tanggalBergabung = ambilWaktuSekarang();
        totalPetugasTerdaftar++;
        catatTugas("Akun petugas dibuat untuk: " + namaPet + ", lokasi tugas: " + lokasi);
    }

    Petugas(const Petugas& lain)
        : User(lain), idPetugas(lain.idPetugas), namaPetugas(lain.namaPetugas),
          nomorTelepon(lain.nomorTelepon), lokasiTugas(lain.lokasiTugas),
          statusAktif(lain.statusAktif), tanggalBergabung(lain.tanggalBergabung),
          jumlahRiwayatTugas(lain.jumlahRiwayatTugas),
          jumlahTugasDiselesaikan(lain.jumlahTugasDiselesaikan) {
        for (int i = 0; i < jumlahRiwayatTugas; i++) {
            riwayatTugas[i] = lain.riwayatTugas[i];
        }
        totalPetugasTerdaftar++;
    }

    Petugas& operator=(const Petugas& lain) {
        if (this == &lain) {
            return *this;
        }
        User::operator=(lain);
        idPetugas = lain.idPetugas;
        namaPetugas = lain.namaPetugas;
        nomorTelepon = lain.nomorTelepon;
        lokasiTugas = lain.lokasiTugas;
        statusAktif = lain.statusAktif;
        tanggalBergabung = lain.tanggalBergabung;
        jumlahRiwayatTugas = lain.jumlahRiwayatTugas;
        jumlahTugasDiselesaikan = lain.jumlahTugasDiselesaikan;
        for (int i = 0; i < jumlahRiwayatTugas; i++) {
            riwayatTugas[i] = lain.riwayatTugas[i];
        }
        return *this;
    }

    virtual ~Petugas() {
        totalPetugasTerdaftar--;
    }

    string getIdPetugas() const { return idPetugas; }
    void setIdPetugas(const string& id) {
        if (!validasiTeksTidakKosong(id)) {
            throw PetugasException("ID Petugas tidak boleh kosong.");
        }
        idPetugas = id;
    }

    string getNamaPetugas() const { return namaPetugas; }
    void setNamaPetugas(const string& nama) {
        if (!validasiTeksTidakKosong(nama)) {
            throw PetugasException("Nama petugas tidak boleh kosong.");
        }
        namaPetugas = nama;
    }

    string getNomorTelepon() const { return nomorTelepon; }
    void setNomorTelepon(const string& telepon) { nomorTelepon = telepon; }

    string getLokasiTugas() const { return lokasiTugas; }
    void setLokasiTugas(const string& lokasi) {
        lokasiTugas = lokasi;
        catatTugas("Lokasi tugas diperbarui menjadi: " + lokasi);
    }

    bool getStatusAktif() const { return statusAktif; }
    void setStatusAktif(bool status) {
        statusAktif = status;
        catatTugas(status ? "Akun petugas diaktifkan kembali." : "Akun petugas dinonaktifkan.");
    }

    string getTanggalBergabung() const { return tanggalBergabung; }

    int getJumlahRiwayatTugas() const { return jumlahRiwayatTugas; }

    string getRiwayatTugasKe(int index) const {
        if (index < 0 || index >= jumlahRiwayatTugas) {
            throw PetugasException("Index riwayat tugas di luar batas.");
        }
        return riwayatTugas[index];
    }

    int getJumlahTugasDiselesaikan() const { return jumlahTugasDiselesaikan; }

    static int getTotalPetugasTerdaftar() {
        return totalPetugasTerdaftar;
    }

    static string generateIdPetugas(int urutan) {
        ostringstream oss;
        oss << "PTG-" << (urutan < 1000 ? "0" : "") << (urutan < 100 ? "0" : "")
            << (urutan < 10 ? "0" : "") << urutan;
        return oss.str();
    }

    bool verifikasiTiketPenumpang(const string& idTiket) {
        pastikanPetugasAktif();
        if (!validasiTeksTidakKosong(idTiket)) {
            throw PetugasException("ID Tiket tidak boleh kosong saat verifikasi.");
        }
        catatTugas("Memverifikasi tiket dengan ID: " + idTiket);
        jumlahTugasDiselesaikan++;
        return true;
    }

    bool checkInPenumpang(const string& idPelanggan, const string& kodeKursi) {
        pastikanPetugasAktif();
        if (!validasiTeksTidakKosong(idPelanggan)) {
            throw PetugasException("ID Pelanggan tidak boleh kosong saat check-in.");
        }
        catatTugas("Check-in penumpang " + idPelanggan + " pada kursi " + kodeKursi);
        jumlahTugasDiselesaikan++;
        return true;
    }

    void catatKeberangkatanBus(const string& idBus, const string& jadwal) {
        pastikanPetugasAktif();
        catatTugas("Mencatat keberangkatan bus " + idBus + " pada jadwal " + jadwal);
        jumlahTugasDiselesaikan++;
    }

    void catatKedatanganBus(const string& idBus) {
        pastikanPetugasAktif();
        catatTugas("Mencatat kedatangan bus " + idBus + " di tujuan.");
        jumlahTugasDiselesaikan++;
    }

    void laporkanKendalaLapangan(const string& keterangan) {
        pastikanPetugasAktif();
        if (!validasiTeksTidakKosong(keterangan)) {
            throw PetugasException("Keterangan kendala tidak boleh kosong.");
        }
        catatTugas("Melaporkan kendala lapangan: " + keterangan);
    }

    void tambahCatatanTugasManual(const string& catatan) {
        pastikanPetugasAktif();
        catatTugas(catatan);
    }

    void inputFormulirPetugas() {
        string uname, pass;
        string idInput, namaInput, teleponInput, lokasiInput;

        cout << "\n+-------------------------------------------------------+\n";
        cout << "|             FORMULIR PENDAFTARAN PETUGAS BARU         |\n";
        cout << "+-------------------------------------------------------+\n";

        do {
            cout << " -> Input ID Petugas Lapangan  : ";
            cin >> idInput;
            if (!validasiTeksTidakKosong(idInput)) {
                cout << "[Sistem Validasi] Error: ID Petugas tidak boleh kosong!\n";
            }
        } while (!validasiTeksTidakKosong(idInput));
        this->idPetugas = idInput;

        cin.ignore(10000, '\n');
        do {
            cout << " -> Input Nama Lengkap Petugas : ";
            getline(cin, namaInput);
            if (!validasiTeksTidakKosong(namaInput)) {
                cout << "[Sistem Validasi] Error: Nama petugas tidak boleh kosong!\n";
            }
        } while (!validasiTeksTidakKosong(namaInput));
        this->namaPetugas = namaInput;

        cout << " -> Input Nomor Telepon Aktif  : ";
        cin >> teleponInput;
        this->nomorTelepon = teleponInput;

        cin.ignore(10000, '\n');
        cout << " -> Input Lokasi Penugasan     : ";
        getline(cin, lokasiInput);
        this->lokasiTugas = validasiTeksTidakKosong(lokasiInput) ? lokasiInput : "-";

        cout << " -> Buat Username Akun         : ";
        cin >> uname;
        cout << " -> Buat Password Akun         : ";
        cin >> pass;

        this->setIdUser(idPetugas);
        this->setUsername(uname);
        this->setPassword(pass);

        catatTugas("Formulir pendaftaran diisi secara manual melalui CLI.");

        cout << "+-------------------------------------------------------+\n";
        cout << "| [Sukses] Data petugas baru berhasil disimpan!         |\n";
        cout << "+-------------------------------------------------------+\n";
    }

    virtual void tampilkanInfoPetugas() const {
        cout << "==============================================\n";
        cout << "ID Petugas        : " << idPetugas << "\n";
        cout << "Nama Petugas      : " << namaPetugas << "\n";
        cout << "Nomor Telepon     : " << nomorTelepon << "\n";
        cout << "Lokasi Tugas      : " << lokasiTugas << "\n";
        cout << "Status Akun       : " << (statusAktif ? "Aktif" : "Nonaktif") << "\n";
        cout << "Tanggal Bergabung : " << tanggalBergabung << "\n";
        cout << "Tugas Diselesaikan: " << jumlahTugasDiselesaikan << "\n";
        cout << "==============================================\n";
    }

    void tampilkanRiwayatTugas() const {
        cout << "=== Riwayat Tugas Petugas: " << namaPetugas << " ===\n";
        for (int i = 0; i < jumlahRiwayatTugas; i++) {
            cout << (i + 1) << ". " << riwayatTugas[i] << "\n";
        }
    }

    string getRingkasanSingkat() const {
        ostringstream oss;
        oss << "Petugas[" << idPetugas << ", " << namaPetugas
            << ", lokasi=" << lokasiTugas
            << ", aktif=" << (statusAktif ? "ya" : "tidak") << "]";
        return oss.str();
    }

    bool operator==(const Petugas& lain) const {
        return idPetugas == lain.idPetugas;
    }

    bool operator!=(const Petugas& lain) const {
        return !(*this == lain);
    }

    bool operator<(const Petugas& lain) const {
        return jumlahTugasDiselesaikan < lain.jumlahTugasDiselesaikan;
    }

    bool operator>(const Petugas& lain) const {
        return jumlahTugasDiselesaikan > lain.jumlahTugasDiselesaikan;
    }

    friend ostream& operator<<(ostream& out, const Petugas& petugas) {
        out << petugas.getRingkasanSingkat();
        return out;
    }
};

#endif
