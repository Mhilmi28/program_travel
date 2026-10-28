#ifndef PEMBAYARAN_H
#define PEMBAYARAN_H

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

class Pembayaran {
private:
    string idPembayaran;
    string idReservasiTerkait;
    string metodePembayaran;
    double totalBayar;
    string statusPembayaran;
    double biayaAdmin;
    double uangDiterima;
    double kembalian;
    string kodeReferensi;
    string catatanTambahan;
    int waktuTransaksi;

    static int totalTransaksiTercatat;
    static double grandTotalPembayaranSukses;

    static string bangkitkanKodeReferensi() {
        int kodeAcak = (rand() % 900000) + 100000;
        return "REF" + to_string(kodeAcak);
    }

public:
    Pembayaran() {
        idPembayaran = "";
        idReservasiTerkait = "";
        metodePembayaran = "-";
        totalBayar = 0.0;
        statusPembayaran = "PENDING";
        biayaAdmin = 0.0;
        uangDiterima = 0.0;
        kembalian = 0.0;
        kodeReferensi = "-";
        catatanTambahan = "-";
        waktuTransaksi = 0;
        totalTransaksiTercatat++;
    }

    Pembayaran(const string& idPay, const string& idRes, const string& metode, double total, const string& status) {
        idPembayaran = idPay;
        idReservasiTerkait = idRes;
        metodePembayaran = metode;
        totalBayar = (total >= 0.0) ? total : 0.0;
        statusPembayaran = status;
        biayaAdmin = hitungBiayaAdminByMetode(metode, totalBayar);
        uangDiterima = 0.0;
        kembalian = 0.0;
        kodeReferensi = "-";
        catatanTambahan = "-";
        waktuTransaksi = 0;
        totalTransaksiTercatat++;
    }

    virtual ~Pembayaran() {}

    string getIdPembayaran() const { return idPembayaran; }
    void setIdPembayaran(const string& idPay) { idPembayaran = idPay; }

    string getIdReservasiTerkait() const { return idReservasiTerkait; }
    void setIdReservasiTerkait(const string& idRes) { idReservasiTerkait = idRes; }

    string getMetodePembayaran() const { return metodePembayaran; }
    void setMetodePembayaran(const string& metode) {
        metodePembayaran = metode;
        biayaAdmin = hitungBiayaAdminByMetode(metode, totalBayar);
    }

    double getTotalBayar() const { return totalBayar; }
    void setTotalBayar(double total) {
        if (total >= 0.0) {
            totalBayar = total;
            biayaAdmin = hitungBiayaAdminByMetode(metodePembayaran, totalBayar);
        } else {
            cout << "[Sistem Finansial] Peringatan: Nominal total bayar tidak boleh negatif!" << endl;
        }
    }

    string getStatusPembayaran() const { return statusPembayaran; }
    void setStatusPembayaran(const string& status) { statusPembayaran = status; }

    double getBiayaAdmin() const { return biayaAdmin; }
    double getUangDiterima() const { return uangDiterima; }
    double getKembalian() const { return kembalian; }
    string getKodeReferensi() const { return kodeReferensi; }

    string getCatatanTambahan() const { return catatanTambahan; }
    void setCatatanTambahan(const string& catatan) { catatanTambahan = catatan; }

    static double hitungBiayaAdminByMetode(string metode, double tagihan) {
        if (metode == "TRANSFER_BANK") {
            return 5000.0;
        } else if (metode == "E_WALLET") {
            return tagihan * 0.015;
        } else if (metode == "KARTU_KREDIT") {
            return (tagihan * 0.025) + 2000.0;
        }
        return 0.0;
    }

    double hitungTotalTagihanKeseluruhan() const {
        return totalBayar + biayaAdmin;
    }

    bool prosesPembayaran(double nominalDibayarkan) {
        if (nominalDibayarkan < hitungTotalTagihanKeseluruhan()) {
            cout << "[Sistem Finansial] Pembayaran Ditolak! Nominal kurang dari tagihan total (termasuk biaya admin)." << endl;
            statusPembayaran = "GAGAL";
            return false;
        }
        uangDiterima = nominalDibayarkan;
        kembalian = nominalDibayarkan - hitungTotalTagihanKeseluruhan();
        statusPembayaran = "SUKSES";
        kodeReferensi = bangkitkanKodeReferensi();
        waktuTransaksi = totalTransaksiTercatat;
        grandTotalPembayaranSukses += hitungTotalTagihanKeseluruhan();
        return true;
    }

    bool konfirmasiManual() {
        if (statusPembayaran == "SUKSES") {
            cout << "[Info] Pembayaran ini sudah berstatus SUKSES sebelumnya" << endl;
            return false;
        }
        statusPembayaran = "SUKSES";
        kodeReferensi = bangkitkanKodeReferensi();
        grandTotalPembayaranSukses += hitungTotalTagihanKeseluruhan();
        return true;
    }

    bool batalkanPembayaran(string alasan) {
        if (statusPembayaran == "SUKSES") {
            cout << "[Gagal] Pembayaran yang sudah SUKSES tidak dapat dibatalkan secara langsung, ajukan refund." << endl;
            return false;
        }
        statusPembayaran = "GAGAL";
        catatanTambahan = alasan;
        return true;
    }

    bool isLunas() const {
        return statusPembayaran == "SUKSES";
    }

    bool isPending() const {
        return statusPembayaran == "PENDING";
    }

    bool isGagal() const {
        return statusPembayaran == "GAGAL";
    }

    bool validasiData() const {
        if (idPembayaran.empty() || idReservasiTerkait.empty()) {
            return false;
        }
        if (totalBayar < 0.0) {
            return false;
        }
        return true;
    }

    bool operator==(const Pembayaran& lain) const {
        return idPembayaran == lain.idPembayaran;
    }

    bool operator<(const Pembayaran& lain) const {
        return totalBayar < lain.totalBayar;
    }

    bool operator>(const Pembayaran& lain) const {
        return totalBayar > lain.totalBayar;
    }

    friend ostream& operator<<(ostream& out, const Pembayaran& p) {
        out << "[" << p.idPembayaran << "] Rp" << fixed << setprecision(2) << p.totalBayar
            << " (" << p.statusPembayaran << ")";
        return out;
    }

    void inputFormulirPembayaran(const string& idRes, double tagihanHarusDibayar) {
        idReservasiTerkait = idRes;
        totalBayar = tagihanHarusDibayar;

        int pilihanMetode = 0;
        double inputUang = 0.0;

        cout << endl;
        cout << "+-------------------------------------------------------+" << endl;
        cout << "|             FORMULIR PROSES PELUNASAN TAGIHAN         |" << endl;
        cout << "+-------------------------------------------------------+" << endl;
        cout << "  > Kode Tagihan Reservasi : " << idReservasiTerkait << endl;
        cout << "  > Total Wajib Bayar      : Rp " << fixed << setprecision(2) << totalBayar << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "  Pilih Saluran/Kanal Pembayaran:" << endl;
        cout << "  1. Transfer Bank Mandiri / BCA (Virtual Account)" << endl;
        cout << "  2. Dompet Digital / E-Wallet (OVO, GoPay, Dana)" << endl;
        cout << "  3. Kartu Kredit / Visa / Mastercard" << endl;

        do {
            cout << "  -> Masukkan Pilihan Kanal (1-3): ";
            cin >> pilihanMetode;
            if (cin.fail() || pilihanMetode < 1 || pilihanMetode > 3) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "  [Sistem] Error: Pilihan tidak tersedia! Input wajib angka 1-3." << endl;
                pilihanMetode = 0;
            }
        } while (pilihanMetode == 0);

        if (pilihanMetode == 1) {
            metodePembayaran = "TRANSFER_BANK";
        } else if (pilihanMetode == 2) {
            metodePembayaran = "E_WALLET";
        } else {
            metodePembayaran = "KARTU_KREDIT";
        }
        biayaAdmin = hitungBiayaAdminByMetode(metodePembayaran, totalBayar);

        cout << "---------------------------------------------------------" << endl;
        cout << "  > Biaya Admin Kanal      : Rp " << fixed << setprecision(2) << biayaAdmin << endl;
        cout << "  > Total Tagihan Akhir    : Rp " << fixed << setprecision(2)
             << hitungTotalTagihanKeseluruhan() << endl;
        cout << "---------------------------------------------------------" << endl;

        cout << "  [Konfirmasi] Masukkan ID Pembayaran Baru (cth: PAY001): ";
        cin >> idPembayaran;

        do {
            cout << "  -> Ketik Nominal Uang yang Ditransfer: Rp ";
            cin >> inputUang;
            if (cin.fail() || inputUang < hitungTotalTagihanKeseluruhan()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "  [Sistem Finansial] Pembayaran Ditolak! Uang yang dimasukkan kurang atau tidak valid." << endl;
                inputUang = -1.0;
            }
        } while (inputUang < 0.0);

        prosesPembayaran(inputUang);

        cout << "+-------------------------------------------------------+" << endl;
        cout << "| [Sukses] Dana Diterima! Status reservasi Anda LUNAS.  |" << endl;
        cout << "+-------------------------------------------------------+" << endl;
    }

    void cetakKuitansiResmi() const {
        cout << endl;
        cout << "+-----------------------------------------------------------+" << endl;
        cout << "|                    KUITANSI PEMBAYARAN SAH                |" << endl;
        cout << "|                    UTAMA TRAVEL INDONESIA                 |" << endl;
        cout << "+-----------------------------------------------------------+" << endl;
        cout << "  ID TRANSAKSI   : " << idPembayaran << endl;
        cout << "  RELASI BOOKING : " << idReservasiTerkait << endl;
        cout << "  METODE PAYMENT : " << metodePembayaran << endl;
        cout << "  KODE REFERENSI : " << kodeReferensi << endl;
        cout << "  STATUS PROSES  : [ " << statusPembayaran << " ]" << endl;
        cout << "-------------------------------------------------------------" << endl;
        cout << "  SUBTOTAL       : Rp " << fixed << setprecision(2) << totalBayar << endl;
        cout << "  BIAYA ADMIN    : Rp " << fixed << setprecision(2) << biayaAdmin << endl;
        cout << "  TOTAL SETORAN  : Rp " << fixed << setprecision(2) << hitungTotalTagihanKeseluruhan() << endl;
        if (uangDiterima > 0.0) {
            cout << "  UANG DITERIMA  : Rp " << fixed << setprecision(2) << uangDiterima << endl;
            cout << "  KEMBALIAN      : Rp " << fixed << setprecision(2) << kembalian << endl;
        }
        cout << "-------------------------------------------------------------" << endl;
        cout << "        Kuitansi ini dicetak otomatis secara digital         " << endl;
        cout << "          sebagai bukti pelunasan yang sah hukum.            " << endl;
        cout << "+-----------------------------------------------------------+" << endl;
    }

    bool simpanKeFile(string namaFile) const {
        ofstream file(namaFile.c_str());
        if (!file.is_open()) {
            cout << "[Gagal] Tidak dapat membuka file untuk menyimpan pembayaran" << endl;
            return false;
        }
        file << idPembayaran << endl;
        file << idReservasiTerkait << endl;
        file << metodePembayaran << endl;
        file << totalBayar << endl;
        file << statusPembayaran << endl;
        file << biayaAdmin << endl;
        file << kodeReferensi << endl;
        file.close();
        return true;
    }

    static int cariIndexById(const Pembayaran* daftarPembayaran, int jumlah, string id) {
        for (int i = 0; i < jumlah; i++) {
            if (daftarPembayaran[i].getIdPembayaran() == id) {
                return i;
            }
        }
        return -1;
    }

    static int filterByStatus(const Pembayaran* daftarPembayaran, int jumlah, string status, int hasilIndex[]) {
        int jumlahHasil = 0;
        for (int i = 0; i < jumlah; i++) {
            if (daftarPembayaran[i].getStatusPembayaran() == status) {
                hasilIndex[jumlahHasil] = i;
                jumlahHasil++;
            }
        }
        return jumlahHasil;
    }

    static int filterByMetode(const Pembayaran* daftarPembayaran, int jumlah, string metode, int hasilIndex[]) {
        int jumlahHasil = 0;
        for (int i = 0; i < jumlah; i++) {
            if (daftarPembayaran[i].getMetodePembayaran() == metode) {
                hasilIndex[jumlahHasil] = i;
                jumlahHasil++;
            }
        }
        return jumlahHasil;
    }

    static double hitungTotalPembayaranSukses(const Pembayaran* daftarPembayaran, int jumlah) {
        double total = 0.0;
        for (int i = 0; i < jumlah; i++) {
            if (daftarPembayaran[i].isLunas()) {
                total += daftarPembayaran[i].getTotalBayar();
            }
        }
        return total;
    }

    static double hitungTotalBiayaAdminTerkumpul(const Pembayaran* daftarPembayaran, int jumlah) {
        double total = 0.0;
        for (int i = 0; i < jumlah; i++) {
            if (daftarPembayaran[i].isLunas()) {
                total += daftarPembayaran[i].getBiayaAdmin();
            }
        }
        return total;
    }

    static void urutkanBerdasarkanTotal(Pembayaran* daftarPembayaran, int jumlah, bool naik) {
        for (int i = 0; i < jumlah - 1; i++) {
            for (int j = 0; j < jumlah - 1 - i; j++) {
                bool tukar;
                if (naik) {
                    tukar = daftarPembayaran[j].getTotalBayar() > daftarPembayaran[j + 1].getTotalBayar();
                } else {
                    tukar = daftarPembayaran[j].getTotalBayar() < daftarPembayaran[j + 1].getTotalBayar();
                }
                if (tukar) {
                    Pembayaran temp = daftarPembayaran[j];
                    daftarPembayaran[j] = daftarPembayaran[j + 1];
                    daftarPembayaran[j + 1] = temp;
                }
            }
        }
    }

    static int getTotalTransaksiTercatat() {
        return totalTransaksiTercatat;
    }

    static double getGrandTotalPembayaranSukses() {
        return grandTotalPembayaranSukses;
    }
};

#endif
