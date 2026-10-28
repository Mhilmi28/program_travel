#ifndef VOUCHERPROMO_H
#define VOUCHERPROMO_H

#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>

using namespace std;

class VoucherPromo {
private:
    static const int MAX_RIWAYAT_PEMAKAIAN = 100;

    string kodeVoucher;
    string namaPromo;
    string jenisDiskon;
    double nilaiDiskon;
    double minimalPembelian;
    double maksimalPotongan;
    string tanggalMulai;
    string tanggalBerakhir;
    int kuotaTotal;
    int kuotaTerpakai;
    bool statusAktif;

    string riwayatPemakaiIdPelanggan[MAX_RIWAYAT_PEMAKAIAN];
    double riwayatNilaiPotongan[MAX_RIWAYAT_PEMAKAIAN];
    int jumlahRiwayatPemakaian;

    static int totalVoucherDibuat;
    static double grandTotalPotonganDiberikan;

public:
    VoucherPromo() {
        kodeVoucher = "";
        namaPromo = "-";
        jenisDiskon = "PERSEN";
        nilaiDiskon = 0.0;
        minimalPembelian = 0.0;
        maksimalPotongan = 0.0;
        tanggalMulai = "-";
        tanggalBerakhir = "-";
        kuotaTotal = 0;
        kuotaTerpakai = 0;
        statusAktif = true;
        jumlahRiwayatPemakaian = 0;
        totalVoucherDibuat++;
    }

    VoucherPromo(const string& kode, const string& nama, const string& jenis, double nilai,
                 double minBeli, double maksPotongan, const string& mulai, const string& berakhir, int kuota) {
        kodeVoucher = kode;
        namaPromo = nama;
        jenisDiskon = (jenis == "PERSEN" || jenis == "NOMINAL") ? jenis : "PERSEN";
        nilaiDiskon = (nilai >= 0.0) ? nilai : 0.0;
        minimalPembelian = (minBeli >= 0.0) ? minBeli : 0.0;
        maksimalPotongan = (maksPotongan >= 0.0) ? maksPotongan : 0.0;
        tanggalMulai = mulai;
        tanggalBerakhir = berakhir;
        kuotaTotal = (kuota > 0) ? kuota : 0;
        kuotaTerpakai = 0;
        statusAktif = true;
        jumlahRiwayatPemakaian = 0;
        totalVoucherDibuat++;
    }

    virtual ~VoucherPromo() {}

    string getKodeVoucher() const { return kodeVoucher; }
    void setKodeVoucher(const string& kode) { kodeVoucher = kode; }

    string getNamaPromo() const { return namaPromo; }
    void setNamaPromo(const string& nama) { namaPromo = nama; }

    string getJenisDiskon() const { return jenisDiskon; }
    void setJenisDiskon(const string& jenis) {
        if (jenis == "PERSEN" || jenis == "NOMINAL") {
            jenisDiskon = jenis;
        } else {
            cout << "[Validasi Gagal] Jenis diskon harus PERSEN atau NOMINAL" << endl;
        }
    }

    double getNilaiDiskon() const { return nilaiDiskon; }
    void setNilaiDiskon(double nilai) {
        if (nilai >= 0.0) {
            nilaiDiskon = nilai;
        } else {
            cout << "[Validasi Gagal] Nilai diskon tidak boleh negatif" << endl;
        }
    }

    double getMinimalPembelian() const { return minimalPembelian; }
    void setMinimalPembelian(double minBeli) {
        if (minBeli >= 0.0) {
            minimalPembelian = minBeli;
        }
    }

    double getMaksimalPotongan() const { return maksimalPotongan; }
    void setMaksimalPotongan(double maksPotongan) {
        if (maksPotongan >= 0.0) {
            maksimalPotongan = maksPotongan;
        }
    }

    string getTanggalMulai() const { return tanggalMulai; }
    void setTanggalMulai(const string& mulai) { tanggalMulai = mulai; }

    string getTanggalBerakhir() const { return tanggalBerakhir; }
    void setTanggalBerakhir(const string& berakhir) { tanggalBerakhir = berakhir; }

    int getKuotaTotal() const { return kuotaTotal; }
    void setKuotaTotal(int kuota) {
        if (kuota > 0) {
            kuotaTotal = kuota;
        }
    }

    int getKuotaTerpakai() const { return kuotaTerpakai; }
    int getKuotaTersisa() const { return kuotaTotal - kuotaTerpakai; }

    bool getStatusAktif() const { return statusAktif; }
    void setStatusAktif(bool status) { statusAktif = status; }

    int getJumlahRiwayatPemakaian() const { return jumlahRiwayatPemakaian; }

    static bool validasiFormatKode(const string& kode) {
        if (kode.length() < 4) {
            cout << "[Validasi Gagal] Kode voucher minimal 4 karakter" << endl;
            return false;
        }
        for (size_t i = 0; i < kode.length(); i++) {
            if (isspace(static_cast<unsigned char>(kode[i]))) {
                cout << "[Validasi Gagal] Kode voucher tidak boleh mengandung spasi" << endl;
                return false;
            }
        }
        return true;
    }

    static bool validasiFormatTanggal(const string& tgl) {
        if (tgl.length() != 10) {
            return false;
        }
        if (tgl[2] != '-' || tgl[5] != '-') {
            return false;
        }
        for (int i = 0; i < 10; i++) {
            if (i == 2 || i == 5) {
                continue;
            }
            if (!isdigit(static_cast<unsigned char>(tgl[i]))) {
                return false;
            }
        }
        return true;
    }

    bool isKuotaHabis() const {
        return kuotaTerpakai >= kuotaTotal;
    }

    bool isMemenuhiSyaratMinimal(double totalBelanja) const {
        return totalBelanja >= minimalPembelian;
    }

    bool isBisaDipakai(double totalBelanja) const {
        if (!statusAktif) {
            return false;
        }
        if (isKuotaHabis()) {
            return false;
        }
        if (!isMemenuhiSyaratMinimal(totalBelanja)) {
            return false;
        }
        return true;
    }

    double hitungPotongan(double totalBelanja) const {
        if (!isBisaDipakai(totalBelanja)) {
            return 0.0;
        }
        double potongan = 0.0;
        if (jenisDiskon == "PERSEN") {
            potongan = totalBelanja * (nilaiDiskon / 100.0);
        } else {
            potongan = nilaiDiskon;
        }
        if (maksimalPotongan > 0.0 && potongan > maksimalPotongan) {
            potongan = maksimalPotongan;
        }
        if (potongan > totalBelanja) {
            potongan = totalBelanja;
        }
        return potongan;
    }

    double hitungHargaSetelahVoucher(double totalBelanja) const {
        return totalBelanja - hitungPotongan(totalBelanja);
    }

    bool pakaiVoucher(const string& idPelanggan, double totalBelanja) {
        if (!isBisaDipakai(totalBelanja)) {
            cout << "[Voucher Ditolak] Voucher '" << kodeVoucher << "' tidak dapat digunakan untuk transaksi ini." << endl;
            return false;
        }
        double potongan = hitungPotongan(totalBelanja);
        kuotaTerpakai++;
        grandTotalPotonganDiberikan += potongan;

        if (jumlahRiwayatPemakaian < MAX_RIWAYAT_PEMAKAIAN) {
            riwayatPemakaiIdPelanggan[jumlahRiwayatPemakaian] = idPelanggan;
            riwayatNilaiPotongan[jumlahRiwayatPemakaian] = potongan;
            jumlahRiwayatPemakaian++;
        }
        return true;
    }

    bool nonaktifkanVoucher() {
        if (!statusAktif) {
            cout << "[Info] Voucher ini sudah nonaktif sebelumnya" << endl;
            return false;
        }
        statusAktif = false;
        return true;
    }

    bool aktifkanKembaliVoucher() {
        if (statusAktif) {
            cout << "[Info] Voucher ini sudah aktif sebelumnya" << endl;
            return false;
        }
        statusAktif = true;
        return true;
    }

    bool tambahKuota(int jumlahTambahan) {
        if (jumlahTambahan <= 0) {
            cout << "[Gagal] Jumlah tambahan kuota tidak valid" << endl;
            return false;
        }
        kuotaTotal += jumlahTambahan;
        return true;
    }

    bool validasiData() const {
        if (kodeVoucher.empty() || namaPromo == "-") {
            return false;
        }
        if (nilaiDiskon <= 0.0 || kuotaTotal <= 0) {
            return false;
        }
        return true;
    }

    bool operator==(const VoucherPromo& lain) const {
        return kodeVoucher == lain.kodeVoucher;
    }

    bool operator<(const VoucherPromo& lain) const {
        return nilaiDiskon < lain.nilaiDiskon;
    }

    bool operator>(const VoucherPromo& lain) const {
        return nilaiDiskon > lain.nilaiDiskon;
    }

    friend ostream& operator<<(ostream& out, const VoucherPromo& v) {
        out << "[" << v.kodeVoucher << "] " << v.namaPromo << " - "
            << (v.jenisDiskon == "PERSEN" ? to_string((int) v.nilaiDiskon) + "%" : "Rp" + to_string((int) v.nilaiDiskon));
        return out;
    }

    void inputFormulirVoucher() {
        string inputKode, inputNama, inputMulai, inputBerakhir;
        int pilihanJenis = 0;
        double inputNilai = 0.0;
        double inputMinBeli = 0.0;
        double inputMaksPotongan = 0.0;
        int inputKuota = 0;

        cout << endl;
        cout << "+-------------------------------------------------------+" << endl;
        cout << "|             FORMULIR PEMBUATAN VOUCHER PROMO          |" << endl;
        cout << "+-------------------------------------------------------+" << endl;

        do {
            cout << " -> Kode Voucher Baru (cth: LIBUR25) : ";
            cin >> inputKode;
        } while (!validasiFormatKode(inputKode));
        kodeVoucher = inputKode;

        cin.ignore(10000, '\n');
        cout << " -> Nama Promo                       : ";
        getline(cin, inputNama);
        namaPromo = inputNama;

        cout << " -> Pilih Jenis Diskon [1] Persen [2] Nominal: ";
        cin >> pilihanJenis;
        jenisDiskon = (pilihanJenis == 2) ? "NOMINAL" : "PERSEN";

        cout << " -> Nilai Diskon                     : ";
        cin >> inputNilai;
        nilaiDiskon = (inputNilai >= 0.0) ? inputNilai : 0.0;

        cout << " -> Minimal Pembelian (Rp)           : ";
        cin >> inputMinBeli;
        minimalPembelian = (inputMinBeli >= 0.0) ? inputMinBeli : 0.0;

        cout << " -> Maksimal Potongan (Rp, 0=tanpa batas): ";
        cin >> inputMaksPotongan;
        maksimalPotongan = (inputMaksPotongan >= 0.0) ? inputMaksPotongan : 0.0;

        do {
            cout << " -> Tanggal Mulai (DD-MM-YYYY)       : ";
            cin >> inputMulai;
        } while (!validasiFormatTanggal(inputMulai));
        tanggalMulai = inputMulai;

        do {
            cout << " -> Tanggal Berakhir (DD-MM-YYYY)    : ";
            cin >> inputBerakhir;
        } while (!validasiFormatTanggal(inputBerakhir));
        tanggalBerakhir = inputBerakhir;

        do {
            cout << " -> Kuota Total Pemakaian            : ";
            cin >> inputKuota;
        } while (inputKuota <= 0);
        kuotaTotal = inputKuota;

        statusAktif = true;

        cout << "+-------------------------------------------------------+" << endl;
        cout << "| [Sukses] Voucher promo baru berhasil dibuat.          |" << endl;
        cout << "+-------------------------------------------------------+" << endl;
    }

    void tampilkanKartuVoucher() const {
        cout << endl;
        cout << "+=======================================================+" << endl;
        cout << "|                  KARTU VOUCHER PROMO                  |" << endl;
        cout << "+=======================================================+" << endl;
        cout << "  > Kode Voucher    : " << kodeVoucher << endl;
        cout << "  > Nama Promo      : " << namaPromo << endl;
        if (jenisDiskon == "PERSEN") {
            cout << "  > Potongan        : " << nilaiDiskon << "%" << endl;
        } else {
            cout << "  > Potongan        : Rp " << fixed << setprecision(2) << nilaiDiskon << endl;
        }
        cout << "  > Minimal Belanja : Rp " << fixed << setprecision(2) << minimalPembelian << endl;
        cout << "  > Maks. Potongan  : " << (maksimalPotongan > 0.0 ? ("Rp " + to_string((int) maksimalPotongan)) : "Tanpa Batas") << endl;
        cout << "  > Masa Berlaku    : " << tanggalMulai << " s/d " << tanggalBerakhir << endl;
        cout << "  > Kuota           : " << kuotaTerpakai << " / " << kuotaTotal << " terpakai" << endl;
        cout << "  > Status          : " << (statusAktif ? "Aktif" : "Nonaktif") << endl;
        cout << "+=======================================================+" << endl;
    }

    void tampilkanRiwayatPemakaian() const {
        cout << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "RIWAYAT PEMAKAIAN VOUCHER: " << kodeVoucher << endl;
        cout << "---------------------------------------------------------" << endl;
        if (jumlahRiwayatPemakaian == 0) {
            cout << "Belum ada pemakaian voucher ini" << endl;
        }
        for (int i = 0; i < jumlahRiwayatPemakaian; i++) {
            cout << (i + 1) << ". Pelanggan: " << riwayatPemakaiIdPelanggan[i]
                 << " | Potongan: Rp " << fixed << setprecision(2) << riwayatNilaiPotongan[i] << endl;
        }
        cout << "---------------------------------------------------------" << endl;
    }

    static int cariIndexByKode(const VoucherPromo* daftarVoucher, int jumlah, string kode) {
        for (int i = 0; i < jumlah; i++) {
            if (daftarVoucher[i].getKodeVoucher() == kode) {
                return i;
            }
        }
        return -1;
    }

    static int filterVoucherAktif(const VoucherPromo* daftarVoucher, int jumlah, int hasilIndex[]) {
        int jumlahHasil = 0;
        for (int i = 0; i < jumlah; i++) {
            if (daftarVoucher[i].getStatusAktif() && !daftarVoucher[i].isKuotaHabis()) {
                hasilIndex[jumlahHasil] = i;
                jumlahHasil++;
            }
        }
        return jumlahHasil;
    }

    static void urutkanBerdasarkanNilaiDiskon(VoucherPromo* daftarVoucher, int jumlah) {
        for (int i = 0; i < jumlah - 1; i++) {
            for (int j = 0; j < jumlah - 1 - i; j++) {
                if (daftarVoucher[j].getNilaiDiskon() < daftarVoucher[j + 1].getNilaiDiskon()) {
                    VoucherPromo temp = daftarVoucher[j];
                    daftarVoucher[j] = daftarVoucher[j + 1];
                    daftarVoucher[j + 1] = temp;
                }
            }
        }
    }

    static double hitungTotalPotonganTerdistribusi(const VoucherPromo* daftarVoucher, int jumlah) {
        double total = 0.0;
        for (int i = 0; i < jumlah; i++) {
            for (int j = 0; j < daftarVoucher[i].getJumlahRiwayatPemakaian(); j++) {
                total += daftarVoucher[i].riwayatNilaiPotongan[j];
            }
        }
        return total;
    }

    static int getTotalVoucherDibuat() {
        return totalVoucherDibuat;
    }

    static double getGrandTotalPotonganDiberikan() {
        return grandTotalPotonganDiberikan;
    }
};

#endif
