#ifndef OUTPUTDATA_H
#define OUTPUTDATA_H

#include <iostream>
#include <string>
#include <iomanip>

#include "User.h"
#include "Admin.h"
#include "Petugas.h"
#include "Pelanggan.h"
#include "PaketTravel.h"
#include "ArmadaBus.h"
#include "Kursi.h"
#include "Reservasi.h"
#include "Pembayaran.h"
#include "VoucherPromo.h"
#include "Refund.h"
#include "Invoice.h"
#include "LaporanKeuangan.h"
#include "SecurityLogger.h"

using namespace std;

class OutputData {
private:
    static const int LEBAR_STANDAR = 73;
    static const int LEBAR_KOTAK = 57;
    static const int LEBAR_BAR_MAKSIMAL = 40;

    void cetakGaris(char karakter, int panjang) const {
        for (int i = 0; i < panjang; i++) {
            cout << karakter;
        }
        cout << endl;
    }

    void cetakJudulTengah(string judul, int lebar) const {
        int panjangTeks = (int) judul.length();
        int sisaRuang = lebar - panjangTeks;
        int kiri = sisaRuang / 2;
        int kanan = sisaRuang - kiri;
        for (int i = 0; i < kiri; i++) {
            cout << " ";
        }
        cout << judul;
        for (int i = 0; i < kanan; i++) {
            cout << " ";
        }
        cout << endl;
    }

public:
    OutputData() {}

    virtual ~OutputData() {}

    void tampilHeaderAplikasi(string judul) const {
        cout << endl;
        cetakGaris('+', LEBAR_KOTAK);
        cout << "|";
        cetakJudulTengah(judul, LEBAR_KOTAK - 2);
        cout << "|" << endl;
        cetakGaris('+', LEBAR_KOTAK);
    }

    void tampilFooterAplikasi() const {
        cetakGaris('+', LEBAR_KOTAK);
        cout << endl;
    }

    void tampilPesanSukses(string pesan) const {
        cout << "[SUKSES] " << pesan << endl;
    }

    void tampilPesanError(string pesan) const {
        cout << "[ERROR] " << pesan << endl;
    }

    void tampilPesanWarning(string pesan) const {
        cout << "[PERINGATAN] " << pesan << endl;
    }

    void tampilPesanInfo(string pesan) const {
        cout << "[INFO] " << pesan << endl;
    }

    void tampilKonfirmasiAksi(string aksi) const {
        cetakGaris('-', 50);
        cout << "Apakah Anda yakin ingin " << aksi << "? (y/t)" << endl;
        cetakGaris('-', 50);
    }

    void tampilLoadingProses(string pesan) const {
        cout << pesan;
        for (int i = 0; i < 5; i++) {
            cout << ".";
        }
        cout << endl;
    }

    void tampilUser(const User& u) const {
        cout << endl;
        cetakGaris('+', LEBAR_KOTAK);
        cetakJudulTengah("DATA ENTITAS USER", LEBAR_KOTAK);
        cetakGaris('+', LEBAR_KOTAK);
        cout << "  > ID Register : " << u.getIdUser() << endl;
        cout << "  > Username    : " << u.getUsername() << endl;
        cout << "  > Hak Akses   : " << u.getRole() << endl;
        cetakGaris('+', LEBAR_KOTAK);
    }

    void tampilAdmin(const Admin& a) const {
        cout << endl;
        cetakGaris('+', LEBAR_KOTAK);
        cetakJudulTengah("PROFIL SUPER ADMIN", LEBAR_KOTAK);
        cetakGaris('+', LEBAR_KOTAK);
        cout << "  > ID Admin    : " << a.getIdUser() << endl;
        cout << "  > Username    : " << a.getUsername() << endl;
        cout << "  > Hak Akses   : " << a.getRole() << endl;
        cout << "  > Hak Otoritas: " << a.getHakAkses() << endl;
        cetakGaris('+', LEBAR_KOTAK);
    }

    void tampilPetugas(const Petugas& p) const {
        cout << endl;
        cetakGaris('+', LEBAR_KOTAK);
        cetakJudulTengah("PROFIL DATA PETUGAS", LEBAR_KOTAK);
        cetakGaris('+', LEBAR_KOTAK);
        cout << "  > ID Petugas  : " << p.getIdPetugas() << endl;
        cout << "  > Nama Lengkap: " << p.getNamaPetugas() << endl;
        cout << "  > Username    : " << p.getUsername() << endl;
        cout << "  > Hak Akses   : " << p.getRole() << endl;
        cetakGaris('+', LEBAR_KOTAK);
    }

    void tampilPelanggan(const Pelanggan& p) const {
        cout << endl;
        cetakGaris('+', LEBAR_KOTAK);
        cetakJudulTengah("KARTU PROFIL PELANGGAN", LEBAR_KOTAK);
        cetakGaris('+', LEBAR_KOTAK);
        cout << "  > ID Pelanggan: " << p.getIdPelanggan() << endl;
        cout << "  > Nama Lengkap: " << p.getNama() << endl;
        cout << "  > Alamat Email: " << p.getEmail() << endl;
        cout << "  > No. HP Aktif: " << p.getNoHP() << endl;
        cout << "  > Username    : " << p.getUsername() << endl;
        cetakGaris('+', LEBAR_KOTAK);
    }

    void tampilSemuaPetugas(const Petugas* daftarPetugas, int jumlahPetugas) const {
        cout << endl;
        cetakGaris('=', LEBAR_STANDAR);
        cetakJudulTengah("DAFTAR PETUGAS OPERASIONAL", LEBAR_STANDAR);
        cetakGaris('=', LEBAR_STANDAR);
        if (jumlahPetugas == 0 || daftarPetugas == nullptr) {
            tampilPesanInfo("Belum ada data petugas yang terdaftar di dalam database.");
        } else {
            for (int i = 0; i < jumlahPetugas; i++) {
                cout << " " << (i + 1) << ". ID: " << daftarPetugas[i].getIdPetugas()
                     << " | Nama: " << daftarPetugas[i].getNamaPetugas()
                     << " | Username: " << daftarPetugas[i].getUsername() << endl;
            }
        }
        cetakGaris('=', LEBAR_STANDAR);
    }

    void tampilSemuaPelanggan(const Pelanggan* daftarPelanggan, int jumlahPelanggan) const {
        cout << endl;
        cetakGaris('=', LEBAR_STANDAR);
        cetakJudulTengah("DAFTAR PELANGGAN TERDAFTAR", LEBAR_STANDAR);
        cetakGaris('=', LEBAR_STANDAR);
        if (jumlahPelanggan == 0 || daftarPelanggan == nullptr) {
            tampilPesanInfo("Repositori kosong, belum ada pelanggan yang melakukan registrasi.");
        } else {
            for (int i = 0; i < jumlahPelanggan; i++) {
                cout << " " << (i + 1) << ". ID: " << daftarPelanggan[i].getIdPelanggan()
                     << " | Nama: " << daftarPelanggan[i].getNama()
                     << " | Kontak: " << daftarPelanggan[i].getNoHP()
                     << " | Uname: " << daftarPelanggan[i].getUsername() << endl;
            }
        }
        cetakGaris('=', LEBAR_STANDAR);
    }

    void tampilDetailPaket(const PaketTravel& paket) const {
        cout << endl;
        cetakGaris('-', LEBAR_STANDAR);
        cout << " [" << paket.getIdPaket() << "] " << paket.getNamaPaket() << endl;
        cout << "    > Destinasi: " << paket.getDestinasi() << endl;
        cout << "    > Jadwal   : " << paket.getJadwal() << endl;
        cout << "    > Harga    : Rp " << fixed << setprecision(2) << paket.getHarga() << endl;
        cetakGaris('-', LEBAR_STANDAR);
    }

    void tampilSemuaPaket(const PaketTravel* daftarPaket, int jumlahPaket) const {
        cout << endl;
        cetakGaris('=', LEBAR_STANDAR);
        cetakJudulTengah("KATALOG PRODUK PAKET TRAVEL", LEBAR_STANDAR);
        cetakGaris('=', LEBAR_STANDAR);
        if (jumlahPaket == 0 || daftarPaket == nullptr) {
            tampilPesanInfo("Mohon maaf, saat ini belum ada paket travel yang dibuka.");
        } else {
            for (int i = 0; i < jumlahPaket; i++) {
                cout << " " << (i + 1) << ". [" << daftarPaket[i].getIdPaket() << "] "
                     << daftarPaket[i].getNamaPaket() << endl;
                cout << "    > Destinasi: " << daftarPaket[i].getDestinasi() << endl;
                cout << "    > Jadwal   : " << daftarPaket[i].getJadwal() << endl;
                cout << "    > Harga    : Rp " << fixed << setprecision(2)
                     << daftarPaket[i].getHarga() << endl;
                cetakGaris('-', LEBAR_STANDAR);
            }
        }
    }

    void tampilDetailReservasi(const Reservasi& reservasi) const {
        cout << endl;
        cetakGaris('-', LEBAR_STANDAR);
        cout << " ID Booking: " << reservasi.getIdReservasi()
             << " | Pelanggan ID: " << reservasi.getIdPelanggan()
             << " | Paket ID: " << reservasi.getIdPaket() << endl;
        cout << "    > Kursi: " << reservasi.getJumlahKursi() << " Pax"
             << " | Tanggal: " << reservasi.getTanggal() << endl;
        cout << "    > Subtotal: Rp " << fixed << setprecision(2) << reservasi.getTotalHarga()
             << " | Status: [ " << reservasi.getStatusReservasi() << " ]" << endl;
        cetakGaris('-', LEBAR_STANDAR);
    }

    void tampilSemuaReservasi(const Reservasi* daftarReservasi, int jumlahReservasi) const {
        cout << endl;
        cetakGaris('=', LEBAR_STANDAR);
        cetakJudulTengah("DAFTAR DATA RESERVASI MASUK", LEBAR_STANDAR);
        cetakGaris('=', LEBAR_STANDAR);
        if (jumlahReservasi == 0 || daftarReservasi == nullptr) {
            tampilPesanInfo("Belum ada transaksi reservasi masuk pada sistem.");
        } else {
            for (int i = 0; i < jumlahReservasi; i++) {
                cout << " " << (i + 1) << ". ID Booking: " << daftarReservasi[i].getIdReservasi()
                     << " | Pelanggan ID: " << daftarReservasi[i].getIdPelanggan()
                     << " | Paket ID: " << daftarReservasi[i].getIdPaket() << endl;
                cout << "    > Kursi: " << daftarReservasi[i].getJumlahKursi() << " Pax"
                     << " | Tanggal: " << daftarReservasi[i].getTanggal() << endl;
                cout << "    > Subtotal: Rp " << fixed << setprecision(2)
                     << daftarReservasi[i].getTotalHarga()
                     << " | Status: [ " << daftarReservasi[i].getStatusReservasi() << " ]" << endl;
                cetakGaris('-', LEBAR_STANDAR);
            }
        }
    }

    void taporanKeuangan(const LaporanKeuangan& laporan) const {
        cout << endl;
        cetakGaris('+', LEBAR_KOTAK);
        cetakJudulTengah("LAPORAN KEUANGAN & LABA RUGI", LEBAR_KOTAK);
        cetakGaris('+', LEBAR_KOTAK);
        cout << "  > Periode Pembukuan : " << laporan.getPeriode() << endl;
        cout << "  > Total Pendapatan  : Rp " << fixed << setprecision(2)
             << laporan.getTotalPendapatan() << endl;
        cout << "  > Total Klaim Refund: Rp " << fixed << setprecision(2)
             << laporan.getTotalRefund() << endl;
        cetakGaris('-', LEBAR_KOTAK);
        cout << "  > PENDAPATAN BERSIH : Rp " << fixed << setprecision(2)
             << laporan.hitungPendapatanBersih() << endl;
        cetakGaris('+', LEBAR_KOTAK);
    }

    void tampilSecurityLog(const SecurityLogger* daftarLog, int jumlahLog) const {
        cout << endl;
        cetakGaris('=', LEBAR_STANDAR);
        cetakJudulTengah("SYSTEM SECURITY AUDIT LOG TRAILS", LEBAR_STANDAR);
        cetakGaris('=', LEBAR_STANDAR);
        if (jumlahLog == 0 || daftarLog == nullptr) {
            tampilPesanInfo("Log aman. Belum ada rekam aktivitas mencurigakan saat ini.");
        } else {
            for (int i = 0; i < jumlahLog; i++) {
                cout << " [" << daftarLog[i].getWaktu() << "] LOG-ID: " << daftarLog[i].getIdLog()
                     << " -> Aktivitas: " << daftarLog[i].getAktivitas() << endl;
            }
        }
        cetakGaris('=', LEBAR_STANDAR);
    }

    void tampilMenuUtama() const {
        cout << endl;
        cetakGaris('=', 45);
        cetakJudulTengah("MENU UTAMA SISTEM TRAVEL", 45);
        cetakGaris('=', 45);
        cout << " 1. Login" << endl;
        cout << " 2. Registrasi Akun Pelanggan" << endl;
        cout << " 3. Lihat Katalog Paket Travel" << endl;
        cout << " 0. Keluar Aplikasi" << endl;
        cetakGaris('=', 45);
    }

    void tampilMenuAdmin() const {
        cout << endl;
        cetakGaris('=', 45);
        cetakJudulTengah("MENU DASHBOARD ADMIN", 45);
        cetakGaris('=', 45);
        cout << " 1. Kelola Data Petugas" << endl;
        cout << " 2. Kelola Data Pelanggan" << endl;
        cout << " 3. Kelola Paket Travel" << endl;
        cout << " 4. Lihat Laporan Keuangan" << endl;
        cout << " 5. Lihat Log Keamanan Sistem" << endl;
        cout << " 0. Logout" << endl;
        cetakGaris('=', 45);
    }

    void tampilMenuPetugas() const {
        cout << endl;
        cetakGaris('=', 45);
        cetakJudulTengah("MENU DASHBOARD PETUGAS", 45);
        cetakGaris('=', 45);
        cout << " 1. Kelola Reservasi Masuk" << endl;
        cout << " 2. Verifikasi Pembayaran" << endl;
        cout << " 3. Proses Klaim Refund" << endl;
        cout << " 0. Logout" << endl;
        cetakGaris('=', 45);
    }

    void tampilMenuPelanggan() const {
        cout << endl;
        cetakGaris('=', 45);
        cetakJudulTengah("MENU DASHBOARD PELANGGAN", 45);
        cetakGaris('=', 45);
        cout << " 1. Lihat Katalog Paket Travel" << endl;
        cout << " 2. Booking Paket Travel" << endl;
        cout << " 3. Riwayat Transaksi Saya" << endl;
        cout << " 4. Ajukan Refund" << endl;
        cout << " 0. Logout" << endl;
        cetakGaris('=', 45);
    }

    void tampilRingkasanDashboard(int totalPelanggan, int totalPetugas, int totalPaket,
                                   int totalReservasi, double totalPendapatan) const {
        cout << endl;
        cetakGaris('=', LEBAR_STANDAR);
        cetakJudulTengah("RINGKASAN DASHBOARD SISTEM TRAVEL", LEBAR_STANDAR);
        cetakGaris('=', LEBAR_STANDAR);
        cout << left << setw(35) << "  Total Pelanggan Terdaftar" << ": " << totalPelanggan << endl;
        cout << left << setw(35) << "  Total Petugas Aktif" << ": " << totalPetugas << endl;
        cout << left << setw(35) << "  Total Paket Travel Tersedia" << ": " << totalPaket << endl;
        cout << left << setw(35) << "  Total Reservasi Masuk" << ": " << totalReservasi << endl;
        cout << left << setw(35) << "  Total Pendapatan Terkumpul" << ": Rp "
             << fixed << setprecision(2) << totalPendapatan << endl;
        cetakGaris('=', LEBAR_STANDAR);
    }

    void tampilGrafikBatangSederhana(string label[], double nilai[], int jumlahData, string judul) const {
        cout << endl;
        cetakGaris('=', LEBAR_STANDAR);
        cetakJudulTengah(judul, LEBAR_STANDAR);
        cetakGaris('=', LEBAR_STANDAR);

        if (jumlahData <= 0) {
            tampilPesanInfo("Tidak ada data untuk ditampilkan pada grafik.");
            cetakGaris('=', LEBAR_STANDAR);
            return;
        }

        double nilaiMaksimal = nilai[0];
        for (int i = 1; i < jumlahData; i++) {
            if (nilai[i] > nilaiMaksimal) {
                nilaiMaksimal = nilai[i];
            }
        }

        if (nilaiMaksimal <= 0.0) {
            nilaiMaksimal = 1.0;
        }

        for (int i = 0; i < jumlahData; i++) {
            int panjangBar = (int) ((nilai[i] / nilaiMaksimal) * LEBAR_BAR_MAKSIMAL);
            cout << left << setw(15) << label[i] << "| ";
            for (int j = 0; j < panjangBar; j++) {
                cout << "#";
            }
            cout << " " << fixed << setprecision(2) << nilai[i] << endl;
        }
        cetakGaris('=', LEBAR_STANDAR);
    }

    void tampilTabelPerbandingan(string namaKolomKiri, string namaKolomKanan,
                                  string dataKiri[], string dataKanan[], int jumlahBaris) const {
        cetakGaris('-', LEBAR_STANDAR);
        cout << left << setw(35) << namaKolomKiri << "| " << namaKolomKanan << endl;
        cetakGaris('-', LEBAR_STANDAR);
        for (int i = 0; i < jumlahBaris; i++) {
            cout << left << setw(35) << dataKiri[i] << "| " << dataKanan[i] << endl;
        }
        cetakGaris('-', LEBAR_STANDAR);
    }

    void tampilProgressBar(string label, int persen) const {
        if (persen < 0) {
            persen = 0;
        }
        if (persen > 100) {
            persen = 100;
        }
        int panjangTerisi = (persen * LEBAR_BAR_MAKSIMAL) / 100;
        cout << left << setw(20) << label << "[";
        for (int i = 0; i < LEBAR_BAR_MAKSIMAL; i++) {
            if (i < panjangTerisi) {
                cout << "=";
            } else {
                cout << " ";
            }
        }
        cout << "] " << persen << "%" << endl;
    }

    void tampilGarisPembatas() const {
        cetakGaris('-', LEBAR_STANDAR);
    }

    void tampilGarisTebal() const {
        cetakGaris('=', LEBAR_STANDAR);
    }
};

#endif
