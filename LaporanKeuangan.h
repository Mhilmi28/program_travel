#ifndef LAPORANKEUANGAN_H
#define LAPORANKEUANGAN_H

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std;

class LaporanKeuangan {
private:
    static const int MAX_DATA = 200;

    string periode;
    double totalPendapatan;
    double totalRefund;
    int jumlahTransaksiSukses;
    int jumlahKlaimRefund;

    double riwayatPendapatan[MAX_DATA];
    string keteranganPendapatan[MAX_DATA];
    int jumlahDataPendapatan;

    double riwayatRefund[MAX_DATA];
    string keteranganRefund[MAX_DATA];
    int jumlahDataRefund;

    double targetPendapatanBulanan;
    double biayaOperasional;
    double persenPajak;
    double persenKomisiAgen;

    static int totalLaporanDibuat;
    static double grandTotalPendapatanSemua;
    static double grandTotalRefundSemua;

    void cetakGarisPemisah(char karakter, int panjang) const {
        for (int i = 0; i < panjang; i++) {
            cout << karakter;
        }
        cout << endl;
    }

public:
    LaporanKeuangan() {
        periode = "01-2026";
        totalPendapatan = 0.0;
        totalRefund = 0.0;
        jumlahTransaksiSukses = 0;
        jumlahKlaimRefund = 0;
        jumlahDataPendapatan = 0;
        jumlahDataRefund = 0;
        targetPendapatanBulanan = 0.0;
        biayaOperasional = 0.0;
        persenPajak = 0.11;
        persenKomisiAgen = 0.05;
        totalLaporanDibuat++;
    }

    LaporanKeuangan(string periodeAwal, double targetBulanan, double biayaOps) {
        periode = periodeAwal;
        totalPendapatan = 0.0;
        totalRefund = 0.0;
        jumlahTransaksiSukses = 0;
        jumlahKlaimRefund = 0;
        jumlahDataPendapatan = 0;
        jumlahDataRefund = 0;
        targetPendapatanBulanan = targetBulanan;
        biayaOperasional = biayaOps;
        persenPajak = 0.11;
        persenKomisiAgen = 0.05;
        totalLaporanDibuat++;
    }

    LaporanKeuangan(string periodeAwal, double targetBulanan, double biayaOps, double pajak, double komisi) {
        periode = periodeAwal;
        totalPendapatan = 0.0;
        totalRefund = 0.0;
        jumlahTransaksiSukses = 0;
        jumlahKlaimRefund = 0;
        jumlahDataPendapatan = 0;
        jumlahDataRefund = 0;
        targetPendapatanBulanan = targetBulanan;
        biayaOperasional = biayaOps;
        persenPajak = pajak;
        persenKomisiAgen = komisi;
        totalLaporanDibuat++;
    }

    LaporanKeuangan(const LaporanKeuangan &sumber) {
        periode = sumber.periode;
        totalPendapatan = sumber.totalPendapatan;
        totalRefund = sumber.totalRefund;
        jumlahTransaksiSukses = sumber.jumlahTransaksiSukses;
        jumlahKlaimRefund = sumber.jumlahKlaimRefund;
        jumlahDataPendapatan = sumber.jumlahDataPendapatan;
        jumlahDataRefund = sumber.jumlahDataRefund;
        targetPendapatanBulanan = sumber.targetPendapatanBulanan;
        biayaOperasional = sumber.biayaOperasional;
        persenPajak = sumber.persenPajak;
        persenKomisiAgen = sumber.persenKomisiAgen;

        for (int i = 0; i < jumlahDataPendapatan; i++) {
            riwayatPendapatan[i] = sumber.riwayatPendapatan[i];
            keteranganPendapatan[i] = sumber.keteranganPendapatan[i];
        }
        for (int i = 0; i < jumlahDataRefund; i++) {
            riwayatRefund[i] = sumber.riwayatRefund[i];
            keteranganRefund[i] = sumber.keteranganRefund[i];
        }
        totalLaporanDibuat++;
    }

    virtual ~LaporanKeuangan() {}

    LaporanKeuangan& operator=(const LaporanKeuangan &sumber) {
        if (this == &sumber) {
            return *this;
        }
        periode = sumber.periode;
        totalPendapatan = sumber.totalPendapatan;
        totalRefund = sumber.totalRefund;
        jumlahTransaksiSukses = sumber.jumlahTransaksiSukses;
        jumlahKlaimRefund = sumber.jumlahKlaimRefund;
        jumlahDataPendapatan = sumber.jumlahDataPendapatan;
        jumlahDataRefund = sumber.jumlahDataRefund;
        targetPendapatanBulanan = sumber.targetPendapatanBulanan;
        biayaOperasional = sumber.biayaOperasional;
        persenPajak = sumber.persenPajak;
        persenKomisiAgen = sumber.persenKomisiAgen;

        for (int i = 0; i < jumlahDataPendapatan; i++) {
            riwayatPendapatan[i] = sumber.riwayatPendapatan[i];
            keteranganPendapatan[i] = sumber.keteranganPendapatan[i];
        }
        for (int i = 0; i < jumlahDataRefund; i++) {
            riwayatRefund[i] = sumber.riwayatRefund[i];
            keteranganRefund[i] = sumber.keteranganRefund[i];
        }
        return *this;
    }

    void setPeriode(string periodeBaru) {
        if (periodeBaru.length() == 7 && periodeBaru[2] == '-') {
            periode = periodeBaru;
        } else {
            cout << "Format periode tidak valid, gunakan MM-YYYY" << endl;
        }
    }

    void setTargetPendapatanBulanan(double target) {
        if (target >= 0.0) {
            targetPendapatanBulanan = target;
        }
    }

    void setBiayaOperasional(double biaya) {
        if (biaya >= 0.0) {
            biayaOperasional = biaya;
        }
    }

    void setPersenPajak(double pajak) {
        if (pajak >= 0.0 && pajak <= 1.0) {
            persenPajak = pajak;
        }
    }

    void setPersenKomisiAgen(double komisi) {
        if (komisi >= 0.0 && komisi <= 1.0) {
            persenKomisiAgen = komisi;
        }
    }

    string getPeriode() const { return periode; }
    double getTotalPendapatan() const { return totalPendapatan; }
    double getTotalRefund() const { return totalRefund; }
    int getJumlahTransaksiSukses() const { return jumlahTransaksiSukses; }
    int getJumlahKlaimRefund() const { return jumlahKlaimRefund; }
    double getTargetPendapatanBulanan() const { return targetPendapatanBulanan; }
    double getBiayaOperasional() const { return biayaOperasional; }
    double getPersenPajak() const { return persenPajak; }
    double getPersenKomisiAgen() const { return persenKomisiAgen; }
    int getJumlahDataPendapatan() const { return jumlahDataPendapatan; }
    int getJumlahDataRefund() const { return jumlahDataRefund; }

    bool tambahPendapatan(double jumlah, string keterangan) {
        if (jumlah <= 0.0) {
            cout << "Jumlah pendapatan tidak valid" << endl;
            return false;
        }
        if (jumlahDataPendapatan >= MAX_DATA) {
            cout << "Kapasitas data pendapatan sudah penuh" << endl;
            return false;
        }
        riwayatPendapatan[jumlahDataPendapatan] = jumlah;
        keteranganPendapatan[jumlahDataPendapatan] = keterangan;
        jumlahDataPendapatan++;
        totalPendapatan += jumlah;
        jumlahTransaksiSukses++;
        grandTotalPendapatanSemua += jumlah;
        return true;
    }

    bool tambahRefund(double jumlah, string keterangan) {
        if (jumlah <= 0.0) {
            cout << "Jumlah refund tidak valid" << endl;
            return false;
        }
        if (jumlahDataRefund >= MAX_DATA) {
            cout << "Kapasitas data refund sudah penuh" << endl;
            return false;
        }
        riwayatRefund[jumlahDataRefund] = jumlah;
        keteranganRefund[jumlahDataRefund] = keterangan;
        jumlahDataRefund++;
        totalRefund += jumlah;
        jumlahKlaimRefund++;
        grandTotalRefundSemua += jumlah;
        return true;
    }

    bool hapusPendapatan(int index) {
        if (index < 0 || index >= jumlahDataPendapatan) {
            cout << "Index pendapatan tidak ditemukan" << endl;
            return false;
        }
        totalPendapatan -= riwayatPendapatan[index];
        jumlahTransaksiSukses--;
        for (int i = index; i < jumlahDataPendapatan - 1; i++) {
            riwayatPendapatan[i] = riwayatPendapatan[i + 1];
            keteranganPendapatan[i] = keteranganPendapatan[i + 1];
        }
        jumlahDataPendapatan--;
        return true;
    }

    bool hapusRefund(int index) {
        if (index < 0 || index >= jumlahDataRefund) {
            cout << "Index refund tidak ditemukan" << endl;
            return false;
        }
        totalRefund -= riwayatRefund[index];
        jumlahKlaimRefund--;
        for (int i = index; i < jumlahDataRefund - 1; i++) {
            riwayatRefund[i] = riwayatRefund[i + 1];
            keteranganRefund[i] = keteranganRefund[i + 1];
        }
        jumlahDataRefund--;
        return true;
    }

    bool editPendapatan(int index, double jumlahBaru) {
        if (index < 0 || index >= jumlahDataPendapatan || jumlahBaru <= 0.0) {
            cout << "Data tidak valid untuk diedit" << endl;
            return false;
        }
        totalPendapatan -= riwayatPendapatan[index];
        totalPendapatan += jumlahBaru;
        riwayatPendapatan[index] = jumlahBaru;
        return true;
    }

    bool editRefund(int index, double jumlahBaru) {
        if (index < 0 || index >= jumlahDataRefund || jumlahBaru <= 0.0) {
            cout << "Data tidak valid untuk diedit" << endl;
            return false;
        }
        totalRefund -= riwayatRefund[index];
        totalRefund += jumlahBaru;
        riwayatRefund[index] = jumlahBaru;
        return true;
    }

    int cariPendapatanByKeterangan(string keyword) const {
        for (int i = 0; i < jumlahDataPendapatan; i++) {
            if (keteranganPendapatan[i] == keyword) {
                return i;
            }
        }
        return -1;
    }

    int cariRefundByKeterangan(string keyword) const {
        for (int i = 0; i < jumlahDataRefund; i++) {
            if (keteranganRefund[i] == keyword) {
                return i;
            }
        }
        return -1;
    }

    double cariPendapatanTerbesar() const {
        if (jumlahDataPendapatan == 0) {
            return 0.0;
        }
        double terbesar = riwayatPendapatan[0];
        for (int i = 1; i < jumlahDataPendapatan; i++) {
            if (riwayatPendapatan[i] > terbesar) {
                terbesar = riwayatPendapatan[i];
            }
        }
        return terbesar;
    }

    double cariPendapatanTerkecil() const {
        if (jumlahDataPendapatan == 0) {
            return 0.0;
        }
        double terkecil = riwayatPendapatan[0];
        for (int i = 1; i < jumlahDataPendapatan; i++) {
            if (riwayatPendapatan[i] < terkecil) {
                terkecil = riwayatPendapatan[i];
            }
        }
        return terkecil;
    }

    double cariRefundTerbesar() const {
        if (jumlahDataRefund == 0) {
            return 0.0;
        }
        double terbesar = riwayatRefund[0];
        for (int i = 1; i < jumlahDataRefund; i++) {
            if (riwayatRefund[i] > terbesar) {
                terbesar = riwayatRefund[i];
            }
        }
        return terbesar;
    }

    double cariRefundTerkecil() const {
        if (jumlahDataRefund == 0) {
            return 0.0;
        }
        double terkecil = riwayatRefund[0];
        for (int i = 1; i < jumlahDataRefund; i++) {
            if (riwayatRefund[i] < terkecil) {
                terkecil = riwayatRefund[i];
            }
        }
        return terkecil;
    }

    void urutkanPendapatanTerbesar() {
        for (int i = 0; i < jumlahDataPendapatan - 1; i++) {
            for (int j = 0; j < jumlahDataPendapatan - 1 - i; j++) {
                if (riwayatPendapatan[j] < riwayatPendapatan[j + 1]) {
                    double tempJumlah = riwayatPendapatan[j];
                    riwayatPendapatan[j] = riwayatPendapatan[j + 1];
                    riwayatPendapatan[j + 1] = tempJumlah;

                    string tempKeterangan = keteranganPendapatan[j];
                    keteranganPendapatan[j] = keteranganPendapatan[j + 1];
                    keteranganPendapatan[j + 1] = tempKeterangan;
                }
            }
        }
    }

    void urutkanRefundTerbesar() {
        for (int i = 0; i < jumlahDataRefund - 1; i++) {
            for (int j = 0; j < jumlahDataRefund - 1 - i; j++) {
                if (riwayatRefund[j] < riwayatRefund[j + 1]) {
                    double tempJumlah = riwayatRefund[j];
                    riwayatRefund[j] = riwayatRefund[j + 1];
                    riwayatRefund[j + 1] = tempJumlah;

                    string tempKeterangan = keteranganRefund[j];
                    keteranganRefund[j] = keteranganRefund[j + 1];
                    keteranganRefund[j + 1] = tempKeterangan;
                }
            }
        }
    }

    double hitungPendapatanBersih() const {
        return totalPendapatan - totalRefund;
    }

    double hitungPajak() const {
        double bersih = hitungPendapatanBersih();
        if (bersih <= 0.0) {
            return 0.0;
        }
        return bersih * persenPajak;
    }

    double hitungKomisiAgen() const {
        return totalPendapatan * persenKomisiAgen;
    }

    double hitungPendapatanSetelahPajak() const {
        return hitungPendapatanBersih() - hitungPajak();
    }

    double hitungLabaBersihOperasional() const {
        return hitungPendapatanSetelahPajak() - hitungKomisiAgen() - biayaOperasional;
    }

    double hitungRataRataPendapatan() const {
        if (jumlahDataPendapatan == 0) {
            return 0.0;
        }
        return totalPendapatan / jumlahDataPendapatan;
    }

    double hitungRataRataRefund() const {
        if (jumlahDataRefund == 0) {
            return 0.0;
        }
        return totalRefund / jumlahDataRefund;
    }

    double hitungPersentaseRefund() const {
        if (totalPendapatan <= 0.0) {
            return 0.0;
        }
        return (totalRefund / totalPendapatan) * 100.0;
    }

    bool isTargetTercapai() const {
        return totalPendapatan >= targetPendapatanBulanan;
    }

    double hitungSelisihTarget() const {
        return targetPendapatanBulanan - totalPendapatan;
    }

    double hitungPersentasePencapaianTarget() const {
        if (targetPendapatanBulanan <= 0.0) {
            return 0.0;
        }
        return (totalPendapatan / targetPendapatanBulanan) * 100.0;
    }

    bool validasiPeriode() const {
        if (periode.length() != 7) {
            return false;
        }
        if (periode[2] != '-') {
            return false;
        }
        return true;
    }

    void resetLaporan() {
        totalPendapatan = 0.0;
        totalRefund = 0.0;
        jumlahTransaksiSukses = 0;
        jumlahKlaimRefund = 0;
        jumlahDataPendapatan = 0;
        jumlahDataRefund = 0;
    }

    LaporanKeuangan operator+(const LaporanKeuangan &lain) const {
        LaporanKeuangan gabungan;
        gabungan.periode = periode + "/" + lain.periode;
        gabungan.targetPendapatanBulanan = targetPendapatanBulanan + lain.targetPendapatanBulanan;
        gabungan.biayaOperasional = biayaOperasional + lain.biayaOperasional;
        gabungan.persenPajak = persenPajak;
        gabungan.persenKomisiAgen = persenKomisiAgen;

        for (int i = 0; i < jumlahDataPendapatan; i++) {
            gabungan.tambahPendapatan(riwayatPendapatan[i], keteranganPendapatan[i]);
        }
        for (int i = 0; i < lain.jumlahDataPendapatan; i++) {
            gabungan.tambahPendapatan(lain.riwayatPendapatan[i], lain.keteranganPendapatan[i]);
        }
        for (int i = 0; i < jumlahDataRefund; i++) {
            gabungan.tambahRefund(riwayatRefund[i], keteranganRefund[i]);
        }
        for (int i = 0; i < lain.jumlahDataRefund; i++) {
            gabungan.tambahRefund(lain.riwayatRefund[i], lain.keteranganRefund[i]);
        }
        return gabungan;
    }

    bool operator==(const LaporanKeuangan &lain) const {
        return fabs(hitungPendapatanBersih() - lain.hitungPendapatanBersih()) < 0.0001;
    }

    bool operator<(const LaporanKeuangan &lain) const {
        return hitungPendapatanBersih() < lain.hitungPendapatanBersih();
    }

    bool operator>(const LaporanKeuangan &lain) const {
        return hitungPendapatanBersih() > lain.hitungPendapatanBersih();
    }

    friend ostream& operator<<(ostream &out, const LaporanKeuangan &laporan) {
        out << "Periode          : " << laporan.periode << endl;
        out << "Total Pendapatan : Rp" << fixed << setprecision(2) << laporan.totalPendapatan << endl;
        out << "Total Refund     : Rp" << fixed << setprecision(2) << laporan.totalRefund << endl;
        out << "Pendapatan Bersih: Rp" << fixed << setprecision(2) << laporan.hitungPendapatanBersih() << endl;
        return out;
    }

    void tampilkanRingkasan() const {
        cetakGarisPemisah('=', 60);
        cout << "RINGKASAN LAPORAN KEUANGAN PERIODE " << periode << endl;
        cetakGarisPemisah('=', 60);
        cout << left << setw(30) << "Total Pendapatan"
             << ": Rp" << fixed << setprecision(2) << totalPendapatan << endl;
        cout << left << setw(30) << "Total Refund"
             << ": Rp" << fixed << setprecision(2) << totalRefund << endl;
        cout << left << setw(30) << "Pendapatan Bersih"
             << ": Rp" << fixed << setprecision(2) << hitungPendapatanBersih() << endl;
        cout << left << setw(30) << "Pajak"
             << ": Rp" << fixed << setprecision(2) << hitungPajak() << endl;
        cout << left << setw(30) << "Komisi Agen"
             << ": Rp" << fixed << setprecision(2) << hitungKomisiAgen() << endl;
        cout << left << setw(30) << "Biaya Operasional"
             << ": Rp" << fixed << setprecision(2) << biayaOperasional << endl;
        cout << left << setw(30) << "Laba Bersih Operasional"
             << ": Rp" << fixed << setprecision(2) << hitungLabaBersihOperasional() << endl;
        cout << left << setw(30) << "Jumlah Transaksi Sukses"
             << ": " << jumlahTransaksiSukses << endl;
        cout << left << setw(30) << "Jumlah Klaim Refund"
             << ": " << jumlahKlaimRefund << endl;
        cout << left << setw(30) << "Target Bulanan"
             << ": Rp" << fixed << setprecision(2) << targetPendapatanBulanan << endl;
        cout << left << setw(30) << "Status Target"
             << ": " << (isTargetTercapai() ? "Tercapai" : "Belum Tercapai") << endl;
        cetakGarisPemisah('=', 60);
    }

    void tampilkanDetailPendapatan() const {
        cetakGarisPemisah('-', 60);
        cout << "DETAIL RIWAYAT PENDAPATAN" << endl;
        cetakGarisPemisah('-', 60);
        if (jumlahDataPendapatan == 0) {
            cout << "Belum ada data pendapatan" << endl;
        }
        for (int i = 0; i < jumlahDataPendapatan; i++) {
            cout << setw(3) << (i + 1) << ". " << left << setw(30) << keteranganPendapatan[i]
                 << ": Rp" << fixed << setprecision(2) << riwayatPendapatan[i] << endl;
        }
        cetakGarisPemisah('-', 60);
    }

    void tampilkanDetailRefund() const {
        cetakGarisPemisah('-', 60);
        cout << "DETAIL RIWAYAT REFUND" << endl;
        cetakGarisPemisah('-', 60);
        if (jumlahDataRefund == 0) {
            cout << "Belum ada data refund" << endl;
        }
        for (int i = 0; i < jumlahDataRefund; i++) {
            cout << setw(3) << (i + 1) << ". " << left << setw(30) << keteranganRefund[i]
                 << ": Rp" << fixed << setprecision(2) << riwayatRefund[i] << endl;
        }
        cetakGarisPemisah('-', 60);
    }

    void tampilkanLaporanLengkap() const {
        tampilkanRingkasan();
        tampilkanDetailPendapatan();
        tampilkanDetailRefund();
    }

    bool simpanKeFile(string namaFile) const {
        ofstream file(namaFile.c_str());
        if (!file.is_open()) {
            cout << "Gagal membuka file untuk disimpan" << endl;
            return false;
        }
        file << periode << endl;
        file << totalPendapatan << endl;
        file << totalRefund << endl;
        file << jumlahTransaksiSukses << endl;
        file << jumlahKlaimRefund << endl;
        file << targetPendapatanBulanan << endl;
        file << biayaOperasional << endl;
        file << persenPajak << endl;
        file << persenKomisiAgen << endl;
        file << jumlahDataPendapatan << endl;
        for (int i = 0; i < jumlahDataPendapatan; i++) {
            file << keteranganPendapatan[i] << endl;
            file << riwayatPendapatan[i] << endl;
        }
        file << jumlahDataRefund << endl;
        for (int i = 0; i < jumlahDataRefund; i++) {
            file << keteranganRefund[i] << endl;
            file << riwayatRefund[i] << endl;
        }
        file.close();
        return true;
    }

    bool muatDariFile(string namaFile) {
        ifstream file(namaFile.c_str());
        if (!file.is_open()) {
            cout << "Gagal membuka file untuk dimuat" << endl;
            return false;
        }
        resetLaporan();
        getline(file, periode);
        file >> totalPendapatan;
        file >> totalRefund;
        file >> jumlahTransaksiSukses;
        file >> jumlahKlaimRefund;
        file >> targetPendapatanBulanan;
        file >> biayaOperasional;
        file >> persenPajak;
        file >> persenKomisiAgen;
        file >> jumlahDataPendapatan;
        file.ignore();
        for (int i = 0; i < jumlahDataPendapatan; i++) {
            getline(file, keteranganPendapatan[i]);
            file >> riwayatPendapatan[i];
            file.ignore();
        }
        file >> jumlahDataRefund;
        file.ignore();
        for (int i = 0; i < jumlahDataRefund; i++) {
            getline(file, keteranganRefund[i]);
            file >> riwayatRefund[i];
            file.ignore();
        }
        file.close();
        return true;
    }

    static int getTotalLaporanDibuat() {
        return totalLaporanDibuat;
    }

    static double getGrandTotalPendapatanSemua() {
        return grandTotalPendapatanSemua;
    }

    static double getGrandTotalRefundSemua() {
        return grandTotalRefundSemua;
    }

    static void resetStatistikGlobal() {
        totalLaporanDibuat = 0;
        grandTotalPendapatanSemua = 0.0;
        grandTotalRefundSemua = 0.0;
    }
};


#endif
