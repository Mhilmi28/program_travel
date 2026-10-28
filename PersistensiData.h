#ifndef PERSISTENSIDATA_H
#define PERSISTENSIDATA_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Pelanggan.h"
#include "Petugas.h"
#include "Reservasi.h"

using namespace std;

/**
 * Modul utilitas untuk menyimpan & memuat data inti aplikasi ke/dari file teks (.txt).
 *
 * Menangani 3 kategori data sesuai kebutuhan sistem:
 *   1. Data Registrasi -> simpanDataPelanggan/muatDataPelanggan, simpanDataPetugas/muatDataPetugas
 *   2. Data Booking Tiket -> simpanDataReservasi/muatDataReservasi
 *
 * Data Login/Logout tidak diduplikasi di sini karena LoginSystem sudah memiliki
 * method simpanLogKeFile() / muatLogDariFile() bawaan; modul ini hanya melengkapi
 * bagian yang belum ada (Pelanggan, Petugas, Reservasi).
 *
 * Format file: 1 baris pertama berisi jumlah record, lalu setiap baris berikutnya
 * berisi 1 record dengan field dipisah karakter '|' (pipe).
 */
class PersistensiData {
private:
    static string kolomAman(const string& teks) {
        // Mencegah karakter pemisah '|' merusak format file.
        string hasil = teks;
        for (char& c : hasil) {
            if (c == '|') c = ';';
        }
        return hasil;
    }

    static void tumbuhkanKapasitasJikaPenuh(int jumlah, int& kapasitas) {
        if (jumlah >= kapasitas) {
            kapasitas = (kapasitas <= 0) ? 4 : kapasitas * 2;
        }
    }

public:
    // ============================= PELANGGAN =============================

    static bool simpanDataPelanggan(const Pelanggan* daftar, int jumlah, const string& namaFile = "data_pelanggan.txt") {
        ofstream file(namaFile.c_str());
        if (!file.is_open()) {
            cout << "[Persistensi] Gagal membuka file '" << namaFile << "' untuk menyimpan data pelanggan.\n";
            return false;
        }
        file << jumlah << "\n";
        for (int i = 0; i < jumlah; i++) {
            const Pelanggan& p = daftar[i];
            file << kolomAman(p.getIdUser()) << "|" << kolomAman(p.getUsername()) << "|" << kolomAman(p.getPassword()) << "|"
                 << kolomAman(p.getStatusAkun()) << "|" << kolomAman(p.getIdPelanggan()) << "|" << kolomAman(p.getNama()) << "|"
                 << kolomAman(p.getEmail()) << "|" << kolomAman(p.getNoHP()) << "|" << kolomAman(p.getAlamat()) << "|"
                 << kolomAman(p.getTanggalLahir()) << "|" << p.getJenisKelamin() << "|" << p.getPoinLoyalitas() << "|"
                 << p.getTotalTransaksi() << "|" << fixed << setprecision(2) << p.getTotalPengeluaran() << "\n";
        }
        file.close();
        cout << "[Persistensi] Data " << jumlah << " pelanggan berhasil disimpan ke '" << namaFile << "'.\n";
        return true;
    }

    static bool muatDataPelanggan(Pelanggan*& daftar, int& jumlah, int& kapasitas, const string& namaFile = "data_pelanggan.txt") {
        ifstream file(namaFile.c_str());
        if (!file.is_open()) {
            return false; // Wajar terjadi saat aplikasi pertama kali dijalankan (file belum ada).
        }

        int jumlahDariFile = 0;
        file >> jumlahDariFile;
        file.ignore(10000, '\n');

        for (int i = 0; i < jumlahDariFile; i++) {
            string baris;
            if (!getline(file, baris)) break;

            stringstream ss(baris);
            string idUser, uname, pass, statusAkun, idPelanggan, nama, email, noHP, alamat, tglLahir, jkStr, poinStr, transaksiStr, pengeluaranStr;

            getline(ss, idUser, '|');
            getline(ss, uname, '|');
            getline(ss, pass, '|');
            getline(ss, statusAkun, '|');
            getline(ss, idPelanggan, '|');
            getline(ss, nama, '|');
            getline(ss, email, '|');
            getline(ss, noHP, '|');
            getline(ss, alamat, '|');
            getline(ss, tglLahir, '|');
            getline(ss, jkStr, '|');
            getline(ss, poinStr, '|');
            getline(ss, transaksiStr, '|');
            getline(ss, pengeluaranStr, '|');

            tumbuhkanKapasitasJikaPenuh(jumlah, kapasitas);
            if (jumlah >= kapasitas) {
                Pelanggan* temp = new Pelanggan[kapasitas];
                for (int k = 0; k < jumlah; k++) temp[k] = daftar[k];
                delete[] daftar;
                daftar = temp;
            }

            Pelanggan p(idUser, uname, pass, idPelanggan, nama, email, noHP);
            p.setAlamat(alamat);
            if (Pelanggan::validasiFormatTanggal(tglLahir)) {
                p.setTanggalLahir(tglLahir);
            }
            if (!jkStr.empty()) {
                p.setJenisKelamin(jkStr[0]);
            }
            try {
                if (!statusAkun.empty()) p.setStatusAkun(statusAkun);
            } catch (...) { /* abaikan status tidak valid, gunakan default Aktif */ }

            int poin = poinStr.empty() ? 0 : stoi(poinStr);
            int transaksi = transaksiStr.empty() ? 0 : stoi(transaksiStr);
            double pengeluaran = pengeluaranStr.empty() ? 0.0 : stod(pengeluaranStr);
            p.muatDataStatistik(poin, transaksi, pengeluaran);

            daftar[jumlah] = p;
            jumlah++;
        }
        file.close();
        cout << "[Persistensi] Data " << jumlah << " pelanggan berhasil dimuat dari '" << namaFile << "'.\n";
        return true;
    }

    // ============================== PETUGAS ===============================

    static bool simpanDataPetugas(const Petugas* daftar, int jumlah, const string& namaFile = "data_petugas.txt") {
        ofstream file(namaFile.c_str());
        if (!file.is_open()) {
            cout << "[Persistensi] Gagal membuka file '" << namaFile << "' untuk menyimpan data petugas.\n";
            return false;
        }
        file << jumlah << "\n";
        for (int i = 0; i < jumlah; i++) {
            const Petugas& pt = daftar[i];
            file << kolomAman(pt.getIdUser()) << "|" << kolomAman(pt.getUsername()) << "|" << kolomAman(pt.getPassword()) << "|"
                 << kolomAman(pt.getStatusAkun()) << "|" << kolomAman(pt.getIdPetugas()) << "|" << kolomAman(pt.getNamaPetugas()) << "|"
                 << kolomAman(pt.getNomorTelepon()) << "|" << kolomAman(pt.getLokasiTugas()) << "|" << (pt.getStatusAktif() ? 1 : 0) << "\n";
        }
        file.close();
        cout << "[Persistensi] Data " << jumlah << " petugas berhasil disimpan ke '" << namaFile << "'.\n";
        return true;
    }

    static bool muatDataPetugas(Petugas*& daftar, int& jumlah, int& kapasitas, const string& namaFile = "data_petugas.txt") {
        ifstream file(namaFile.c_str());
        if (!file.is_open()) {
            return false;
        }

        int jumlahDariFile = 0;
        file >> jumlahDariFile;
        file.ignore(10000, '\n');

        for (int i = 0; i < jumlahDariFile; i++) {
            string baris;
            if (!getline(file, baris)) break;

            stringstream ss(baris);
            string idUser, uname, pass, statusAkun, idPetugas, namaPetugas, telepon, lokasi, aktifStr;

            getline(ss, idUser, '|');
            getline(ss, uname, '|');
            getline(ss, pass, '|');
            getline(ss, statusAkun, '|');
            getline(ss, idPetugas, '|');
            getline(ss, namaPetugas, '|');
            getline(ss, telepon, '|');
            getline(ss, lokasi, '|');
            getline(ss, aktifStr, '|');

            tumbuhkanKapasitasJikaPenuh(jumlah, kapasitas);
            if (jumlah >= kapasitas) {
                Petugas* temp = new Petugas[kapasitas];
                for (int k = 0; k < jumlah; k++) temp[k] = daftar[k];
                delete[] daftar;
                daftar = temp;
            }

            Petugas pt(idUser, uname, pass, idPetugas, namaPetugas, telepon, lokasi);
            try {
                if (!statusAkun.empty()) pt.setStatusAkun(statusAkun);
            } catch (...) { /* abaikan status tidak valid */ }
            pt.setStatusAktif(!aktifStr.empty() && aktifStr == "1");

            daftar[jumlah] = pt;
            jumlah++;
        }
        file.close();
        cout << "[Persistensi] Data " << jumlah << " petugas berhasil dimuat dari '" << namaFile << "'.\n";
        return true;
    }

    // ============================= RESERVASI ==============================

    static bool simpanDataReservasi(const Reservasi* daftar, int jumlah, const string& namaFile = "data_booking_tiket.txt") {
        ofstream file(namaFile.c_str());
        if (!file.is_open()) {
            cout << "[Persistensi] Gagal membuka file '" << namaFile << "' untuk menyimpan data booking tiket.\n";
            return false;
        }
        file << jumlah << "\n";
        for (int i = 0; i < jumlah; i++) {
            const Reservasi& r = daftar[i];
            file << kolomAman(r.getIdReservasi()) << "|" << kolomAman(r.getIdPelanggan()) << "|" << kolomAman(r.getIdPaket()) << "|"
                 << r.getJumlahKursi() << "|" << fixed << setprecision(2) << r.getTotalHarga() << "|" << kolomAman(r.getTanggal()) << "|"
                 << kolomAman(r.getStatusReservasi()) << "|" << kolomAman(r.getTanggalPemesanan()) << "\n";
        }
        file.close();
        cout << "[Persistensi] Data " << jumlah << " booking tiket berhasil disimpan ke '" << namaFile << "'.\n";
        return true;
    }

    static bool muatDataReservasi(Reservasi*& daftar, int& jumlah, int& kapasitas, const string& namaFile = "data_booking_tiket.txt") {
        ifstream file(namaFile.c_str());
        if (!file.is_open()) {
            return false;
        }

        int jumlahDariFile = 0;
        file >> jumlahDariFile;
        file.ignore(10000, '\n');

        for (int i = 0; i < jumlahDariFile; i++) {
            string baris;
            if (!getline(file, baris)) break;

            stringstream ss(baris);
            string idRes, idPel, idPaket, kursiStr, hargaStr, tanggal, status, tanggalPesan;

            getline(ss, idRes, '|');
            getline(ss, idPel, '|');
            getline(ss, idPaket, '|');
            getline(ss, kursiStr, '|');
            getline(ss, hargaStr, '|');
            getline(ss, tanggal, '|');
            getline(ss, status, '|');
            getline(ss, tanggalPesan, '|');

            tumbuhkanKapasitasJikaPenuh(jumlah, kapasitas);
            if (jumlah >= kapasitas) {
                Reservasi* temp = new Reservasi[kapasitas];
                for (int k = 0; k < jumlah; k++) temp[k] = daftar[k];
                delete[] daftar;
                daftar = temp;
            }

            int jmlKursi = kursiStr.empty() ? 0 : stoi(kursiStr);
            double totalHarga = hargaStr.empty() ? 0.0 : stod(hargaStr);

            Reservasi r(idRes, idPel, idPaket, jmlKursi, totalHarga, tanggal, status);
            r.muatTanggalPemesanan(tanggalPesan);

            daftar[jumlah] = r;
            jumlah++;
        }
        file.close();
        cout << "[Persistensi] Data " << jumlah << " booking tiket berhasil dimuat dari '" << namaFile << "'.\n";
        return true;
    }
};

#endif
