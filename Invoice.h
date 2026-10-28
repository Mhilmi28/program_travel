#ifndef INVOICE_H
#define INVOICE_H

#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <stdexcept>
#include <sstream>

using namespace std;

class InvoiceException : public runtime_error {
public:
    explicit InvoiceException(const string& pesan) : runtime_error(pesan) {}
};

class Invoice {
public:
    static const int MAKS_RIWAYAT = 30;

private:
    string idInvoice;
    string tanggalCetak;
    string idReservasiTerkait;
    string namaPelanggan;
    string namaPaketTravel;
    double subTotalHarga;
    double potonganKupon;
    double tarifPajakPersen;
    string statusPembayaran;
    string metodePembayaran;
    string tanggalJatuhTempo;

    string riwayatStatus[MAKS_RIWAYAT];
    int jumlahRiwayat;

    static int totalInvoiceTerbit;
    static const int PANJANG_MINIMUM_ID = 8;

    string dapatkanTanggalSistemSekarang() const {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        char buffer[12];
        strftime(buffer, sizeof(buffer), "%d-%m-%Y", now);
        return string(buffer);
    }

    void catatRiwayatStatus(const string& keterangan) {
        if (jumlahRiwayat < MAKS_RIWAYAT) {
            riwayatStatus[jumlahRiwayat] = "[" + tanggalCetak + "] " + keterangan;
            jumlahRiwayat++;
        } else {
            for (int i = 1; i < MAKS_RIWAYAT; i++) {
                riwayatStatus[i - 1] = riwayatStatus[i];
            }
            riwayatStatus[MAKS_RIWAYAT - 1] = "[" + tanggalCetak + "] " + keterangan;
        }
    }

    bool isStatusValid(const string& status) const {
        return status == "LUNAS" || status == "BELUM_BAYAR" || status == "BATAL";
    }

public:
    Invoice() {
        idInvoice = "INV-KOSONG";
        tanggalCetak = dapatkanTanggalSistemSekarang();
        idReservasiTerkait = "";
        namaPelanggan = "-";
        namaPaketTravel = "-";
        subTotalHarga = 0.0;
        potonganKupon = 0.0;
        tarifPajakPersen = 0.11;
        statusPembayaran = "BELUM_BAYAR";
        metodePembayaran = "-";
        tanggalJatuhTempo = "-";
        jumlahRiwayat = 0;
        totalInvoiceTerbit++;
        catatRiwayatStatus("Invoice dibuat menggunakan konstruktor default.");
    }

    Invoice(const string& idInv, const string& idRes, const string& pelanggan,
            const string& paket, double subTotal, double diskon, const string& status) {
        this->idInvoice = idInv;
        this->tanggalCetak = dapatkanTanggalSistemSekarang();
        this->idReservasiTerkait = idRes;
        this->namaPelanggan = pelanggan;
        this->namaPaketTravel = paket;

        this->subTotalHarga = (subTotal >= 0.0) ? subTotal : 0.0;
        this->potonganKupon = (diskon >= 0.0) ? diskon : 0.0;
        this->tarifPajakPersen = 0.11;
        this->statusPembayaran = isStatusValid(status) ? status : "BELUM_BAYAR";
        this->metodePembayaran = "-";
        this->tanggalJatuhTempo = "-";
        jumlahRiwayat = 0;
        totalInvoiceTerbit++;
        catatRiwayatStatus("Invoice diterbitkan untuk pelanggan: " + pelanggan);
    }

    Invoice(const string& idInv, const string& idRes, const string& pelanggan,
            const string& paket, double subTotal, double diskon, const string& status,
            const string& jatuhTempo) {
        this->idInvoice = idInv;
        this->tanggalCetak = dapatkanTanggalSistemSekarang();
        this->idReservasiTerkait = idRes;
        this->namaPelanggan = pelanggan;
        this->namaPaketTravel = paket;

        this->subTotalHarga = (subTotal >= 0.0) ? subTotal : 0.0;
        this->potonganKupon = (diskon >= 0.0) ? diskon : 0.0;
        this->tarifPajakPersen = 0.11;
        this->statusPembayaran = isStatusValid(status) ? status : "BELUM_BAYAR";
        this->metodePembayaran = "-";
        this->tanggalJatuhTempo = jatuhTempo;
        jumlahRiwayat = 0;
        totalInvoiceTerbit++;
        catatRiwayatStatus("Invoice diterbitkan dengan jatuh tempo: " + jatuhTempo);
    }

    Invoice(const Invoice& lain) {
        idInvoice = lain.idInvoice;
        tanggalCetak = lain.tanggalCetak;
        idReservasiTerkait = lain.idReservasiTerkait;
        namaPelanggan = lain.namaPelanggan;
        namaPaketTravel = lain.namaPaketTravel;
        subTotalHarga = lain.subTotalHarga;
        potonganKupon = lain.potonganKupon;
        tarifPajakPersen = lain.tarifPajakPersen;
        statusPembayaran = lain.statusPembayaran;
        metodePembayaran = lain.metodePembayaran;
        tanggalJatuhTempo = lain.tanggalJatuhTempo;
        jumlahRiwayat = lain.jumlahRiwayat;
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatStatus[i] = lain.riwayatStatus[i];
        }
        totalInvoiceTerbit++;
    }

    Invoice& operator=(const Invoice& lain) {
        if (this == &lain) {
            return *this;
        }
        idInvoice = lain.idInvoice;
        tanggalCetak = lain.tanggalCetak;
        idReservasiTerkait = lain.idReservasiTerkait;
        namaPelanggan = lain.namaPelanggan;
        namaPaketTravel = lain.namaPaketTravel;
        subTotalHarga = lain.subTotalHarga;
        potonganKupon = lain.potonganKupon;
        tarifPajakPersen = lain.tarifPajakPersen;
        statusPembayaran = lain.statusPembayaran;
        metodePembayaran = lain.metodePembayaran;
        tanggalJatuhTempo = lain.tanggalJatuhTempo;
        jumlahRiwayat = lain.jumlahRiwayat;
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatStatus[i] = lain.riwayatStatus[i];
        }
        return *this;
    }

    virtual ~Invoice() {
        totalInvoiceTerbit--;
    }

    string getIdInvoice() const { return idInvoice; }
    void setIdInvoice(const string& id) {
        if (!validasiFormatInvoiceId(id)) {
            throw InvoiceException("Format ID Invoice tidak valid: " + id);
        }
        idInvoice = id;
    }

    string getTanggalCetak() const { return tanggalCetak; }
    void setTanggalCetak(const string& tgl) { tanggalCetak = tgl; }

    string getIdReservasiTerkait() const { return idReservasiTerkait; }
    void setIdReservasiTerkait(const string& idRes) { idReservasiTerkait = idRes; }

    string getNamaPelanggan() const { return namaPelanggan; }
    void setNamaPelanggan(const string& nama) { namaPelanggan = nama; }

    string getNamaPaketTravel() const { return namaPaketTravel; }
    void setNamaPaketTravel(const string& paket) { namaPaketTravel = paket; }

    double getSubTotalHarga() const { return subTotalHarga; }
    void setSubTotalHarga(double subTotal) {
        if (subTotal < 0.0) {
            throw InvoiceException("Subtotal harga tidak boleh bernilai negatif.");
        }
        subTotalHarga = subTotal;
    }

    double getPotonganKupon() const { return potonganKupon; }
    void setPotonganKupon(double diskon) {
        if (diskon < 0.0) {
            throw InvoiceException("Potongan kupon tidak boleh bernilai negatif.");
        }
        potonganKupon = diskon;
    }

    double getTarifPajakPersen() const { return tarifPajakPersen; }
    void setTarifPajakPersen(double pajak) {
        if (pajak < 0.0 || pajak > 1.0) {
            throw InvoiceException("Tarif pajak harus berada pada rentang 0 sampai 1 (0% - 100%).");
        }
        tarifPajakPersen = pajak;
    }

    string getStatusPembayaran() const { return statusPembayaran; }

    void setStatusPembayaran(const string& status) {
        if (!isStatusValid(status)) {
            throw InvoiceException("Status pembayaran tidak dikenali: " + status);
        }
        if (statusPembayaran == "BATAL") {
            throw InvoiceException("Invoice yang sudah BATAL tidak dapat diubah statusnya kembali.");
        }
        catatRiwayatStatus("Status diubah dari '" + statusPembayaran + "' menjadi '" + status + "'.");
        statusPembayaran = status;
    }

    string getMetodePembayaran() const { return metodePembayaran; }
    void setMetodePembayaran(const string& metode) { metodePembayaran = metode; }

    string getTanggalJatuhTempo() const { return tanggalJatuhTempo; }
    void setTanggalJatuhTempo(const string& tgl) { tanggalJatuhTempo = tgl; }

    int getJumlahRiwayat() const { return jumlahRiwayat; }

    string getRiwayatKe(int index) const {
        if (index < 0 || index >= jumlahRiwayat) {
            throw InvoiceException("Index riwayat status di luar batas.");
        }
        return riwayatStatus[index];
    }

    static bool validasiFormatInvoiceId(const string& id) {
        if (id.empty()) {
            cout << "[Sistem Validasi] Error: Kode ID Invoice tidak boleh kosong!\n";
            return false;
        }
        if (static_cast<int>(id.length()) < PANJANG_MINIMUM_ID) {
            cout << "[Sistem Validasi] Error: Kode ID Invoice tidak valid (terlalu pendek)!\n";
            return false;
        }
        if (id.substr(0, 4) != "INV-") {
            cout << "[Sistem Validasi] Error: Kode ID Invoice harus diawali dengan prefix 'INV-'!\n";
            return false;
        }
        return true;
    }

    static string generateInvoiceId(const string& tanggalYYYYMMDD, int urutan) {
        ostringstream oss;
        oss << "INV-" << tanggalYYYYMMDD << "-"
            << (urutan < 1000 ? "0" : "") << (urutan < 100 ? "0" : "")
            << (urutan < 10 ? "0" : "") << urutan;
        return oss.str();
    }

    static int getTotalInvoiceTerbit() {
        return totalInvoiceTerbit;
    }

    double hitungHargaSetelahDiskon() const {
        double hasil = this->subTotalHarga - this->potonganKupon;
        return (hasil > 0.0) ? hasil : 0.0;
    }

    double hitungNominalPajakPPN() const {
        return hitungHargaSetelahDiskon() * this->tarifPajakPersen;
    }

    double hitungGrandTotalAkhir() const {
        return hitungHargaSetelahDiskon() + hitungNominalPajakPPN();
    }

    double hitungDendaKeterlambatan(int hariTerlambat, double persenDendaPerHari = 0.02) const {
        if (hariTerlambat <= 0) {
            return 0.0;
        }
        return hitungGrandTotalAkhir() * persenDendaPerHari * hariTerlambat;
    }

    double hitungTotalTagihanDenganDenda(int hariTerlambat) const {
        return hitungGrandTotalAkhir() + hitungDendaKeterlambatan(hariTerlambat);
    }

    bool apakahLunas() const {
        return statusPembayaran == "LUNAS";
    }

    bool apakahBatal() const {
        return statusPembayaran == "BATAL";
    }

    bool apakahBelumBayar() const {
        return statusPembayaran == "BELUM_BAYAR";
    }

    void konfirmasiPembayaran(const string& metode) {
        if (apakahBatal()) {
            throw InvoiceException("Invoice yang sudah dibatalkan tidak dapat dikonfirmasi pembayarannya.");
        }
        metodePembayaran = metode;
        catatRiwayatStatus("Pembayaran dikonfirmasi melalui metode: " + metode);
        statusPembayaran = "LUNAS";
    }

    void batalkanInvoice(const string& alasan) {
        if (apakahLunas()) {
            throw InvoiceException("Invoice yang sudah LUNAS tidak dapat dibatalkan.");
        }
        catatRiwayatStatus("Invoice dibatalkan, alasan: " + alasan);
        statusPembayaran = "BATAL";
    }

    string getRingkasanSingkat() const {
        ostringstream oss;
        oss << "Invoice[" << idInvoice << ", pelanggan=" << namaPelanggan
            << ", total=Rp" << fixed << setprecision(2) << hitungGrandTotalAkhir()
            << ", status=" << statusPembayaran << "]";
        return oss.str();
    }

    void tampilkanRiwayatStatus() const {
        cout << "=== Riwayat Status Invoice " << idInvoice << " ===\n";
        for (int i = 0; i < jumlahRiwayat; i++) {
            cout << (i + 1) << ". " << riwayatStatus[i] << "\n";
        }
    }

    void cetakNotaFakturInvoice() const {
        cout << "\n+-----------------------------------------------------------+\n";
        cout << "|                 FAKTUR BUKTI TRANSAKSI RESMI              |\n";
        cout << "|                    UTAMA TRAVEL INDONESIA                 |\n";
        cout << "+-----------------------------------------------------------+\n";
        cout << "  NO. INVOICE   : " << this->idInvoice << "\n";
        cout << "  TANGGAL CETAK : " << this->tanggalCetak << "\n";
        cout << "  JATUH TEMPO   : " << this->tanggalJatuhTempo << "\n";
        cout << "  ID RESERVASI  : " << this->idReservasiTerkait << "\n";
        cout << "  PELANGGAN     : " << this->namaPelanggan << "\n";
        cout << "  METODE BAYAR  : " << this->metodePembayaran << "\n";
        cout << "  STATUS BILLING: [ " << this->statusPembayaran << " ]\n";
        cout << "+-----------------------------------------------------------+\n";
        cout << "  Rincian Item Perjalanan:\n";
        cout << "  * " << this->namaPaketTravel << "\n";
        cout << "-------------------------------------------------------------\n";

        cout << "  ( + ) Subtotal Bruto       : Rp "
             << fixed << setprecision(2) << this->subTotalHarga << "\n";
        cout << "  ( - ) Potongan Kupon Promo : Rp "
             << fixed << setprecision(2) << this->potonganKupon << "\n";
        cout << "  ( + ) Pajak PPN (" << (tarifPajakPersen * 100) << "%)      : Rp "
             << fixed << setprecision(2) << this->hitungNominalPajakPPN() << "\n";

        cout << "-------------------------------------------------------------\n";
        cout << "  ( = ) GRAND TOTAL NET      : Rp "
             << fixed << setprecision(2) << this->hitungGrandTotalAkhir() << "\n";
        cout << "+-----------------------------------------------------------+\n";
        cout << "|   Terima kasih telah mempercayakan perjalanan Anda bersama|\n";
        cout << "|             layanan prima Utama Travel Agent.             |\n";
        cout << "+-----------------------------------------------------------+\n";
    }

    bool operator==(const Invoice& lain) const {
        return idInvoice == lain.idInvoice;
    }

    bool operator!=(const Invoice& lain) const {
        return !(*this == lain);
    }

    bool operator<(const Invoice& lain) const {
        return hitungGrandTotalAkhir() < lain.hitungGrandTotalAkhir();
    }

    bool operator>(const Invoice& lain) const {
        return hitungGrandTotalAkhir() > lain.hitungGrandTotalAkhir();
    }

    friend ostream& operator<<(ostream& out, const Invoice& invoice) {
        out << invoice.getRingkasanSingkat();
        return out;
    }
};

#endif
