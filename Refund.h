#ifndef REFUND_H
#define REFUND_H

#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <ctime>

using namespace std;

class RefundException : public runtime_error {
public:
    explicit RefundException(const string& pesan) : runtime_error(pesan) {}
};

class Refund {
public:
    static const int MAKS_RIWAYAT = 30;

private:
    string idRefund;
    string idReservasiTerkait;
    string alasan;
    double totalBayarAwal;
    double nominalRefund;
    string statusRefund;
    double persentasePengembalian;
    string tanggalPengajuan;
    string tanggalProses;
    string alasanPenolakan;

    string riwayatStatus[MAKS_RIWAYAT];
    int jumlahRiwayat;

    static int totalRefundDiajukan;
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
        return status == "PROSES" || status == "DISETUJUI" || status == "DITOLAK";
    }

    bool apakahStatusFinal() const {
        return statusRefund == "DISETUJUI" || statusRefund == "DITOLAK";
    }

public:
    Refund() {
        this->idRefund = "";
        this->idReservasiTerkait = "";
        this->alasan = "-";
        this->totalBayarAwal = 0.0;
        this->nominalRefund = 0.0;
        this->statusRefund = "PROSES";
        this->persentasePengembalian = 0.80;
        this->tanggalPengajuan = dapatkanTanggalSistemSekarang();
        this->tanggalProses = "-";
        this->alasanPenolakan = "-";
        this->jumlahRiwayat = 0;
        totalRefundDiajukan++;
        catatRiwayatStatus("Dokumen refund dibuat menggunakan konstruktor default.");
    }

    Refund(const string& idRfd, const string& idRes, const string& alasanKet,
           double nominal, const string& status) {
        this->idRefund = idRfd;
        this->idReservasiTerkait = idRes;
        this->alasan = alasanKet;
        this->totalBayarAwal = 0.0;
        this->nominalRefund = (nominal >= 0.0) ? nominal : 0.0;
        this->statusRefund = isStatusValid(status) ? status : "PROSES";
        this->persentasePengembalian = 0.80;
        this->tanggalPengajuan = dapatkanTanggalSistemSekarang();
        this->tanggalProses = "-";
        this->alasanPenolakan = "-";
        this->jumlahRiwayat = 0;
        totalRefundDiajukan++;
        catatRiwayatStatus("Klaim refund diajukan untuk reservasi: " + idRes);
    }

    Refund(const string& idRfd, const string& idRes, const string& alasanKet,
           double nominal, const string& status, double persentaseKebijakan) {
        this->idRefund = idRfd;
        this->idReservasiTerkait = idRes;
        this->alasan = alasanKet;
        this->totalBayarAwal = 0.0;
        this->nominalRefund = (nominal >= 0.0) ? nominal : 0.0;
        this->statusRefund = isStatusValid(status) ? status : "PROSES";
        this->persentasePengembalian = (persentaseKebijakan >= 0.0 && persentaseKebijakan <= 1.0)
                                            ? persentaseKebijakan : 0.80;
        this->tanggalPengajuan = dapatkanTanggalSistemSekarang();
        this->tanggalProses = "-";
        this->alasanPenolakan = "-";
        this->jumlahRiwayat = 0;
        totalRefundDiajukan++;
        catatRiwayatStatus("Klaim refund diajukan dengan kebijakan khusus: "
                           + to_string(static_cast<int>(persentasePengembalian * 100)) + "%");
    }

    Refund(const Refund& lain) {
        idRefund = lain.idRefund;
        idReservasiTerkait = lain.idReservasiTerkait;
        alasan = lain.alasan;
        totalBayarAwal = lain.totalBayarAwal;
        nominalRefund = lain.nominalRefund;
        statusRefund = lain.statusRefund;
        persentasePengembalian = lain.persentasePengembalian;
        tanggalPengajuan = lain.tanggalPengajuan;
        tanggalProses = lain.tanggalProses;
        alasanPenolakan = lain.alasanPenolakan;
        jumlahRiwayat = lain.jumlahRiwayat;
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatStatus[i] = lain.riwayatStatus[i];
        }
        totalRefundDiajukan++;
    }

    Refund& operator=(const Refund& lain) {
        if (this == &lain) {
            return *this;
        }
        idRefund = lain.idRefund;
        idReservasiTerkait = lain.idReservasiTerkait;
        alasan = lain.alasan;
        totalBayarAwal = lain.totalBayarAwal;
        nominalRefund = lain.nominalRefund;
        statusRefund = lain.statusRefund;
        persentasePengembalian = lain.persentasePengembalian;
        tanggalPengajuan = lain.tanggalPengajuan;
        tanggalProses = lain.tanggalProses;
        alasanPenolakan = lain.alasanPenolakan;
        jumlahRiwayat = lain.jumlahRiwayat;
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatStatus[i] = lain.riwayatStatus[i];
        }
        return *this;
    }

    virtual ~Refund() {
        totalRefundDiajukan--;
    }

    string getIdRefund() const { return idRefund; }
    void setIdRefund(const string& idRfd) {
        if (!validasiFormatIdRefund(idRfd)) {
            throw RefundException("Format ID Refund tidak valid: " + idRfd);
        }
        idRefund = idRfd;
    }

    string getIdReservasiTerkait() const { return idReservasiTerkait; }
    void setIdReservasiTerkait(const string& idRes) { idReservasiTerkait = idRes; }

    string getAlasan() const { return alasan; }
    void setAlasan(const string& als) {
        if (als.empty()) {
            throw RefundException("Alasan pembatalan tidak boleh kosong.");
        }
        alasan = als;
    }

    double getTotalBayarAwal() const { return totalBayarAwal; }
    void setTotalBayarAwal(double nominal) {
        if (nominal < 0.0) {
            throw RefundException("Total bayar awal tidak boleh bernilai negatif.");
        }
        totalBayarAwal = nominal;
    }

    double getNominalRefund() const { return nominalRefund; }
    void setNominalRefund(double nominal) {
        if (nominal < 0.0) {
            throw RefundException("Nominal refund tidak boleh bernilai negatif.");
        }
        nominalRefund = nominal;
    }

    string getStatusRefund() const { return statusRefund; }

    void setStatusRefund(const string& status) {
        if (!isStatusValid(status)) {
            throw RefundException("Status refund tidak dikenali: " + status);
        }
        if (apakahStatusFinal()) {
            throw RefundException("Status refund yang sudah final (" + statusRefund + ") tidak dapat diubah lagi.");
        }
        catatRiwayatStatus("Status diubah dari '" + statusRefund + "' menjadi '" + status + "'.");
        statusRefund = status;
        tanggalProses = dapatkanTanggalSistemSekarang();
    }

    double getPersentasePengembalian() const { return persentasePengembalian; }
    void setPersentasePengembalian(double persen) {
        if (persen < 0.0 || persen > 1.0) {
            throw RefundException("Persentase pengembalian harus berada pada rentang 0 sampai 1.");
        }
        persentasePengembalian = persen;
    }

    string getTanggalPengajuan() const { return tanggalPengajuan; }
    string getTanggalProses() const { return tanggalProses; }
    string getAlasanPenolakan() const { return alasanPenolakan; }

    int getJumlahRiwayat() const { return jumlahRiwayat; }

    string getRiwayatKe(int index) const {
        if (index < 0 || index >= jumlahRiwayat) {
            throw RefundException("Index riwayat status di luar batas.");
        }
        return riwayatStatus[index];
    }

    static bool validasiFormatIdRefund(const string& id) {
        if (id.empty()) {
            cout << "[Sistem Validasi] Error: Kode ID Refund tidak boleh kosong!\n";
            return false;
        }
        if (static_cast<int>(id.length()) < PANJANG_MINIMUM_ID) {
            cout << "[Sistem Validasi] Error: Kode ID Refund terlalu pendek!\n";
            return false;
        }
        return true;
    }

    static string generateIdRefund(int urutan) {
        ostringstream oss;
        oss << "RFD-" << (urutan < 1000 ? "0" : "") << (urutan < 100 ? "0" : "")
            << (urutan < 10 ? "0" : "") << urutan;
        return oss.str();
    }

    static int getTotalRefundDiajukan() {
        return totalRefundDiajukan;
    }

    double kalkulasiOtomatisDanaRefund(double totalBayarAwalTransaksi) {
        if (totalBayarAwalTransaksi < 0.0) {
            throw RefundException("Total bayar awal transaksi tidak boleh bernilai negatif.");
        }
        this->totalBayarAwal = totalBayarAwalTransaksi;
        this->nominalRefund = totalBayarAwalTransaksi * this->persentasePengembalian;
        return this->nominalRefund;
    }

    double hitungPotonganPinalti() const {
        return totalBayarAwal - nominalRefund;
    }

    bool apakahSedangProses() const {
        return statusRefund == "PROSES";
    }

    bool apakahDisetujui() const {
        return statusRefund == "DISETUJUI";
    }

    bool apakahDitolak() const {
        return statusRefund == "DITOLAK";
    }

    void setujuiRefund() {
        if (apakahStatusFinal()) {
            throw RefundException("Klaim refund ini sudah berstatus final, tidak dapat disetujui lagi.");
        }
        catatRiwayatStatus("Klaim refund disetujui.");
        statusRefund = "DISETUJUI";
        tanggalProses = dapatkanTanggalSistemSekarang();
    }

    void tolakRefund(const string& alasanPenolakanKlaim) {
        if (apakahStatusFinal()) {
            throw RefundException("Klaim refund ini sudah berstatus final, tidak dapat ditolak lagi.");
        }
        if (alasanPenolakanKlaim.empty()) {
            throw RefundException("Alasan penolakan klaim tidak boleh kosong.");
        }
        alasanPenolakan = alasanPenolakanKlaim;
        catatRiwayatStatus("Klaim refund ditolak, alasan: " + alasanPenolakanKlaim);
        statusRefund = "DITOLAK";
        nominalRefund = 0.0;
        tanggalProses = dapatkanTanggalSistemSekarang();
    }

    void inputFormulirRefund(const string& idRes, double totalBayarAwalInput) {
        this->idReservasiTerkait = idRes;

        cout << "\n+-------------------------------------------------------+\n";
        cout << "|             FORMULIR PENGAJUAN KLAIM REFUND DANA      |\n";
        cout << "+-------------------------------------------------------+\n";
        cout << "  > Kode Booking Target : " << this->idReservasiTerkait << "\n";
        cout << "  > Total Bayar Asli    : Rp " << fixed << setprecision(2) << totalBayarAwalInput << "\n";

        double estimasiRefund = kalkulasiOtomatisDanaRefund(totalBayarAwalInput);

        cout << "  > Kebijakan Proteksi  : Pengembalian " << (this->persentasePengembalian * 100) << "%\n";
        cout << "  > Estimasi Dana Cair  : Rp " << fixed << setprecision(2) << estimasiRefund << "\n";
        cout << "---------------------------------------------------------\n";

        string idInput;
        do {
            cout << "  [Input] Buat ID Dokumen Refund Baru (cth: RFD01): ";
            cin >> idInput;
        } while (!validasiFormatIdRefund(idInput));
        this->idRefund = idInput;

        cin.ignore(10000, '\n');
        string alasanInput;
        do {
            cout << "  [Input] Tulis Alasan Pembatalan Perjalanan: ";
            getline(cin, alasanInput);
            if (alasanInput.empty()) {
                cout << "[Sistem Validasi] Error: Alasan tidak boleh kosong!\n";
            }
        } while (alasanInput.empty());
        this->alasan = alasanInput;

        catatRiwayatStatus("Formulir pengajuan diisi secara manual melalui CLI.");
        setujuiRefund();

        cout << "+-------------------------------------------------------+\n";
        cout << "| [Sukses] Pengajuan Refund Berhasil Diproses & Disetujui|\n";
        cout << "+-------------------------------------------------------+\n";
    }

    string getRingkasanSingkat() const {
        ostringstream oss;
        oss << "Refund[" << idRefund << ", reservasi=" << idReservasiTerkait
            << ", nominal=Rp" << fixed << setprecision(2) << nominalRefund
            << ", status=" << statusRefund << "]";
        return oss.str();
    }

    void tampilkanRiwayatStatus() const {
        cout << "=== Riwayat Status Refund " << idRefund << " ===\n";
        for (int i = 0; i < jumlahRiwayat; i++) {
            cout << (i + 1) << ". " << riwayatStatus[i] << "\n";
        }
    }

    void cetakNotaPersetujuanRefund() const {
        cout << "\n+-----------------------------------------------------------+\n";
        cout << "|                 NOTA PERSETUJUAN REFUND DANA              |\n";
        cout << "|                    UTAMA TRAVEL INDONESIA                 |\n";
        cout << "+-----------------------------------------------------------+\n";
        cout << "  ID DOKUMEN RFD : " << this->idRefund << "\n";
        cout << "  KODE RESERVASI : " << this->idReservasiTerkait << "\n";
        cout << "  TANGGAL AJUAN  : " << this->tanggalPengajuan << "\n";
        cout << "  TANGGAL PROSES : " << this->tanggalProses << "\n";
        cout << "  ALASAN KLAIM   : " << this->alasan << "\n";
        cout << "  STATUS PROSES  : [ " << this->statusRefund << " ]\n";
        if (apakahDitolak()) {
            cout << "  ALASAN TOLAK   : " << this->alasanPenolakan << "\n";
        }
        cout << "-------------------------------------------------------------\n";
        cout << "  TOTAL BAYAR AWAL : Rp " << fixed << setprecision(2) << this->totalBayarAwal << "\n";
        cout << "  POTONGAN PINALTI : Rp " << fixed << setprecision(2) << this->hitungPotonganPinalti() << "\n";
        cout << "  NOMINAL CAIR     : Rp " << fixed << setprecision(2) << this->nominalRefund << "\n";
        cout << "-------------------------------------------------------------\n";
        cout << "     Dana bersih akan ditransfer balik ke rekening penyetor  \n";
        cout << "     paling lambat dalam 1x24 jam hari kerja perbankan.     \n";
        cout << "+-----------------------------------------------------------+\n";
    }

    bool operator==(const Refund& lain) const {
        return idRefund == lain.idRefund;
    }

    bool operator!=(const Refund& lain) const {
        return !(*this == lain);
    }

    bool operator<(const Refund& lain) const {
        return nominalRefund < lain.nominalRefund;
    }

    bool operator>(const Refund& lain) const {
        return nominalRefund > lain.nominalRefund;
    }

    friend ostream& operator<<(ostream& out, const Refund& refund) {
        out << refund.getRingkasanSingkat();
        return out;
    }
};

#endif
