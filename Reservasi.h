#ifndef RESERVASI_H
#define RESERVASI_H

#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <ctime>

using namespace std;

class ReservasiException : public runtime_error {
public:
    explicit ReservasiException(const string& pesan) : runtime_error(pesan) {}
};

class Reservasi {
public:
    static const int MAKS_RIWAYAT = 30;

private:
    string idReservasi;
    string idPelanggan;
    string idPaket;
    int jumlahKursi;
    double totalHarga;
    string tanggal;
    string statusReservasi;
    string tanggalPemesanan;

    string riwayatStatus[MAKS_RIWAYAT];
    int jumlahRiwayat;

    static int totalReservasiDibuat;
    static const int PANJANG_MINIMUM_ID = 5;

    string dapatkanTanggalSistemSekarang() const {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        char buffer[12];
        strftime(buffer, sizeof(buffer), "%d-%m-%Y", now);
        return string(buffer);
    }

    void catatRiwayatStatus(const string& keterangan) {
        ostringstream oss;
        oss << "[" << dapatkanTanggalSistemSekarang() << "] " << keterangan;
        if (jumlahRiwayat < MAKS_RIWAYAT) {
            riwayatStatus[jumlahRiwayat] = oss.str();
            jumlahRiwayat++;
        } else {
            for (int i = 1; i < MAKS_RIWAYAT; i++) {
                riwayatStatus[i - 1] = riwayatStatus[i];
            }
            riwayatStatus[MAKS_RIWAYAT - 1] = oss.str();
        }
    }

    bool isStatusValid(const string& status) const {
        return status == "PENDING" || status == "LUNAS" ||
               status == "REFUNDED" || status == "DIBATALKAN";
    }

public:
    Reservasi() {
        this->idReservasi = "";
        this->idPelanggan = "";
        this->idPaket = "";
        this->jumlahKursi = 0;
        this->totalHarga = 0.0;
        this->tanggal = "-";
        this->statusReservasi = "PENDING";
        this->tanggalPemesanan = dapatkanTanggalSistemSekarang();
        this->jumlahRiwayat = 0;
        totalReservasiDibuat++;
        catatRiwayatStatus("Reservasi dibuat menggunakan konstruktor default.");
    }

    Reservasi(const string& idRes, const string& idPel, const string& idPkt,
              int jmlKrs, double totHrg, const string& tglKrn, const string& stat) {
        this->idReservasi = idRes;
        this->idPelanggan = idPel;
        this->idPaket = idPkt;
        this->jumlahKursi = (jmlKrs > 0) ? jmlKrs : 0;
        this->totalHarga = (totHrg >= 0.0) ? totHrg : 0.0;
        this->tanggal = tglKrn;
        this->statusReservasi = isStatusValid(stat) ? stat : "PENDING";
        this->tanggalPemesanan = dapatkanTanggalSistemSekarang();
        this->jumlahRiwayat = 0;
        totalReservasiDibuat++;
        catatRiwayatStatus("Reservasi baru dibuat untuk pelanggan: " + idPel);
    }

    Reservasi(const Reservasi& lain) {
        idReservasi = lain.idReservasi;
        idPelanggan = lain.idPelanggan;
        idPaket = lain.idPaket;
        jumlahKursi = lain.jumlahKursi;
        totalHarga = lain.totalHarga;
        tanggal = lain.tanggal;
        statusReservasi = lain.statusReservasi;
        tanggalPemesanan = lain.tanggalPemesanan;
        jumlahRiwayat = lain.jumlahRiwayat;
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatStatus[i] = lain.riwayatStatus[i];
        }
        totalReservasiDibuat++;
    }

    Reservasi& operator=(const Reservasi& lain) {
        if (this == &lain) {
            return *this;
        }
        idReservasi = lain.idReservasi;
        idPelanggan = lain.idPelanggan;
        idPaket = lain.idPaket;
        jumlahKursi = lain.jumlahKursi;
        totalHarga = lain.totalHarga;
        tanggal = lain.tanggal;
        statusReservasi = lain.statusReservasi;
        tanggalPemesanan = lain.tanggalPemesanan;
        jumlahRiwayat = lain.jumlahRiwayat;
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatStatus[i] = lain.riwayatStatus[i];
        }
        return *this;
    }

    virtual ~Reservasi() {
        totalReservasiDibuat--;
    }

    string getIdReservasi() const { return idReservasi; }
    void setIdReservasi(const string& idRes) {
        if (!validasiFormatIdReservasi(idRes)) {
            throw ReservasiException("Format ID Reservasi tidak valid: " + idRes);
        }
        idReservasi = idRes;
    }

    string getIdPelanggan() const { return idPelanggan; }
    void setIdPelanggan(const string& idPel) {
        if (idPel.empty()) {
            throw ReservasiException("ID Pelanggan tidak boleh kosong.");
        }
        idPelanggan = idPel;
    }

    string getIdPaket() const { return idPaket; }
    void setIdPaket(const string& idPkt) {
        if (idPkt.empty()) {
            throw ReservasiException("ID Paket tidak boleh kosong.");
        }
        idPaket = idPkt;
    }

    int getJumlahKursi() const { return jumlahKursi; }
    void setJumlahKursi(int jmlKrs) {
        if (jmlKrs <= 0) {
            throw ReservasiException("Jumlah kursi harus bernilai positif.");
        }
        jumlahKursi = jmlKrs;
    }

    double getTotalHarga() const { return totalHarga; }
    void setTotalHarga(double totHrg) {
        if (totHrg < 0.0) {
            throw ReservasiException("Total harga tidak boleh bernilai negatif.");
        }
        totalHarga = totHrg;
    }

    string getTanggal() const { return tanggal; }
    void setTanggal(const string& tglKrn) {
        if (tglKrn.empty()) {
            throw ReservasiException("Tanggal keberangkatan tidak boleh kosong.");
        }
        tanggal = tglKrn;
    }

    string getStatusReservasi() const { return statusReservasi; }

    void setStatusReservasi(const string& stat) {
        if (!isStatusValid(stat)) {
            throw ReservasiException("Status reservasi tidak dikenali: " + stat);
        }
        catatRiwayatStatus("Status diubah dari '" + statusReservasi + "' menjadi '" + stat + "'.");
        statusReservasi = stat;
    }

    string getTanggalPemesanan() const { return tanggalPemesanan; }

    // Dipakai khusus oleh modul persistensi (PersistensiData) untuk memulihkan
    // tanggal pemesanan asli saat data reservasi dimuat kembali dari file .txt.
    void muatTanggalPemesanan(const string& tgl) {
        if (!tgl.empty()) {
            tanggalPemesanan = tgl;
        }
    }

    int getJumlahRiwayat() const { return jumlahRiwayat; }

    string getRiwayatKe(int index) const {
        if (index < 0 || index >= jumlahRiwayat) {
            throw ReservasiException("Index riwayat status di luar batas.");
        }
        return riwayatStatus[index];
    }

    static bool validasiFormatIdReservasi(const string& id) {
        if (id.empty()) {
            cout << "[Sistem Validasi] Error: Kode ID Reservasi tidak boleh kosong!\n";
            return false;
        }
        if (static_cast<int>(id.length()) < PANJANG_MINIMUM_ID) {
            cout << "[Sistem Validasi] Error: Kode ID Reservasi terlalu pendek!\n";
            return false;
        }
        return true;
    }

    static string generateIdReservasi(int urutan) {
        ostringstream oss;
        oss << "RSV-" << (urutan < 1000 ? "0" : "") << (urutan < 100 ? "0" : "")
            << (urutan < 10 ? "0" : "") << urutan;
        return oss.str();
    }

    static int getTotalReservasiDibuat() {
        return totalReservasiDibuat;
    }

    double hitungHargaPerKursi() const {
        if (jumlahKursi <= 0) {
            return 0.0;
        }
        return totalHarga / jumlahKursi;
    }

    bool apakahPending() const { return statusReservasi == "PENDING"; }
    bool apakahLunas() const { return statusReservasi == "LUNAS"; }
    bool apakahRefunded() const { return statusReservasi == "REFUNDED"; }
    bool apakahDibatalkan() const { return statusReservasi == "DIBATALKAN"; }

    void konfirmasiPembayaran() {
        if (!apakahPending()) {
            throw ReservasiException("Hanya reservasi berstatus PENDING yang dapat dikonfirmasi pembayarannya.");
        }
        catatRiwayatStatus("Pembayaran dikonfirmasi, status menjadi LUNAS.");
        statusReservasi = "LUNAS";
    }

    void tandaiRefunded() {
        if (!apakahLunas()) {
            throw ReservasiException("Hanya reservasi berstatus LUNAS yang dapat direfund.");
        }
        catatRiwayatStatus("Dana reservasi dikembalikan, status menjadi REFUNDED.");
        statusReservasi = "REFUNDED";
    }

    void batalkanReservasi(const string& alasan) {
        if (apakahLunas() || apakahRefunded()) {
            throw ReservasiException("Reservasi yang sudah LUNAS/REFUNDED tidak dapat dibatalkan langsung, ajukan Refund terlebih dahulu.");
        }
        if (apakahDibatalkan()) {
            throw ReservasiException("Reservasi ini sudah berstatus DIBATALKAN sebelumnya.");
        }
        catatRiwayatStatus("Reservasi dibatalkan, alasan: " + alasan);
        statusReservasi = "DIBATALKAN";
    }

    string getRingkasanSingkat() const {
        ostringstream oss;
        oss << "Reservasi[" << idReservasi << ", pelanggan=" << idPelanggan
            << ", paket=" << idPaket << ", kursi=" << jumlahKursi
            << ", total=Rp" << fixed << setprecision(2) << totalHarga
            << ", status=" << statusReservasi << "]";
        return oss.str();
    }

    void tampilkanDetailReservasi() const {
        cout << "\n+=======================================================+\n";
        cout << "|                 DETAIL DATA RESERVASI                 |\n";
        cout << "+=======================================================+\n";
        cout << "  ID Reservasi     : " << idReservasi << "\n";
        cout << "  ID Pelanggan     : " << idPelanggan << "\n";
        cout << "  ID Paket Travel  : " << idPaket << "\n";
        cout << "  Jumlah Kursi     : " << jumlahKursi << "\n";
        cout << "  Total Harga      : Rp " << fixed << setprecision(2) << totalHarga << "\n";
        cout << "  Harga per Kursi  : Rp " << fixed << setprecision(2) << hitungHargaPerKursi() << "\n";
        cout << "  Tanggal Berangkat: " << tanggal << "\n";
        cout << "  Tanggal Pesan    : " << tanggalPemesanan << "\n";
        cout << "  Status           : [ " << statusReservasi << " ]\n";
        cout << "+=======================================================+\n";
    }

    void tampilkanRiwayatStatus() const {
        cout << "=== Riwayat Status Reservasi " << idReservasi << " ===\n";
        for (int i = 0; i < jumlahRiwayat; i++) {
            cout << (i + 1) << ". " << riwayatStatus[i] << "\n";
        }
    }

    bool operator==(const Reservasi& lain) const {
        return idReservasi == lain.idReservasi;
    }

    bool operator!=(const Reservasi& lain) const {
        return !(*this == lain);
    }

    bool operator<(const Reservasi& lain) const {
        return totalHarga < lain.totalHarga;
    }

    bool operator>(const Reservasi& lain) const {
        return totalHarga > lain.totalHarga;
    }

    friend ostream& operator<<(ostream& out, const Reservasi& reservasi) {
        out << reservasi.getRingkasanSingkat();
        return out;
    }
};

#endif
