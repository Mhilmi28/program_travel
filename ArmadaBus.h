#ifndef ARMADABUS_H
#define ARMADABUS_H

#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <ctime>

using namespace std;

class ArmadaBusException : public runtime_error {
public:
    explicit ArmadaBusException(const string& pesan) : runtime_error(pesan) {}
};

class ArmadaBus {
public:
    static const int KAPASITAS_MINIMUM = 10;
    static const int KAPASITAS_MAKSIMUM = 60;
    static const int BATAS_KILOMETER_SERVIS = 5000;
    static const int MAKS_RIWAYAT = 50;

private:
    string idBus;
    string platNomor;
    int kapasitas;
    string statusBus;

    string merkBus;
    int tahunPembuatan;
    long totalKilometer;
    long kilometerSejakServisTerakhir;

    string riwayatStatus[MAKS_RIWAYAT];
    int jumlahRiwayat;

    static int totalArmadaTerdaftar;

    string ambilWaktuSekarang() const {
        time_t sekarang = time(nullptr);
        string waktu = ctime(&sekarang);
        if (!waktu.empty() && waktu.back() == '\n') {
            waktu.pop_back();
        }
        return waktu;
    }

    void catatRiwayatStatus(const string& keterangan) {
        ostringstream oss;
        oss << "[" << ambilWaktuSekarang() << "] " << keterangan;
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

    string normalisasiPlatNomor(const string& plat) const {
        string tempPlat = plat;
        for (auto &c : tempPlat) {
            c = toupper(static_cast<unsigned char>(c));
        }
        return tempPlat;
    }

public:
    ArmadaBus() {
        idBus = "";
        platNomor = "";
        kapasitas = 0;
        statusBus = "Tersedia";
        merkBus = "";
        tahunPembuatan = 0;
        totalKilometer = 0;
        kilometerSejakServisTerakhir = 0;
        jumlahRiwayat = 0;
        totalArmadaTerdaftar++;
        catatRiwayatStatus("Unit armada bus dibuat menggunakan konstruktor default.");
    }

    ArmadaBus(const string& id, const string& plat, int kap, const string& status) {
        this->idBus = id;
        this->platNomor = normalisasiPlatNomor(plat);
        this->kapasitas = (kap > 0) ? kap : 0;

        if (status == "Tersedia" || status == "Jalan" || status == "Perbaikan") {
            this->statusBus = status;
        } else {
            this->statusBus = "Tersedia";
        }

        merkBus = "";
        tahunPembuatan = 0;
        totalKilometer = 0;
        kilometerSejakServisTerakhir = 0;
        jumlahRiwayat = 0;
        totalArmadaTerdaftar++;
        catatRiwayatStatus("Unit armada bus baru terdaftar dengan status awal: " + this->statusBus);
    }

    ArmadaBus(const string& id, const string& plat, int kap, const string& status,
               const string& merk, int tahun) {
        this->idBus = id;
        this->platNomor = normalisasiPlatNomor(plat);
        this->kapasitas = (kap > 0) ? kap : 0;

        if (status == "Tersedia" || status == "Jalan" || status == "Perbaikan") {
            this->statusBus = status;
        } else {
            this->statusBus = "Tersedia";
        }

        this->merkBus = merk;
        this->tahunPembuatan = tahun;
        totalKilometer = 0;
        kilometerSejakServisTerakhir = 0;
        jumlahRiwayat = 0;
        totalArmadaTerdaftar++;
        catatRiwayatStatus("Unit armada bus baru terdaftar dengan merk: " + merk);
    }

    ArmadaBus(const ArmadaBus& lain) {
        idBus = lain.idBus;
        platNomor = lain.platNomor;
        kapasitas = lain.kapasitas;
        statusBus = lain.statusBus;
        merkBus = lain.merkBus;
        tahunPembuatan = lain.tahunPembuatan;
        totalKilometer = lain.totalKilometer;
        kilometerSejakServisTerakhir = lain.kilometerSejakServisTerakhir;
        jumlahRiwayat = lain.jumlahRiwayat;
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatStatus[i] = lain.riwayatStatus[i];
        }
        totalArmadaTerdaftar++;
    }

    ArmadaBus& operator=(const ArmadaBus& lain) {
        if (this == &lain) {
            return *this;
        }
        idBus = lain.idBus;
        platNomor = lain.platNomor;
        kapasitas = lain.kapasitas;
        statusBus = lain.statusBus;
        merkBus = lain.merkBus;
        tahunPembuatan = lain.tahunPembuatan;
        totalKilometer = lain.totalKilometer;
        kilometerSejakServisTerakhir = lain.kilometerSejakServisTerakhir;
        jumlahRiwayat = lain.jumlahRiwayat;
        for (int i = 0; i < jumlahRiwayat; i++) {
            riwayatStatus[i] = lain.riwayatStatus[i];
        }
        return *this;
    }

    virtual ~ArmadaBus() {
        totalArmadaTerdaftar--;
    }

    string getIdBus() const { return idBus; }
    void setIdBus(const string& id) { idBus = id; }

    string getPlatNomor() const { return platNomor; }

    void setPlatNomor(const string& plat) {
        if (!validasiPlatNomor(plat)) {
            throw ArmadaBusException("Format plat nomor tidak valid: " + plat);
        }
        this->platNomor = normalisasiPlatNomor(plat);
    }

    int getKapasitas() const { return kapasitas; }

    void setKapasitas(int kap) {
        if (!validasiKapasitas(kap)) {
            throw ArmadaBusException("Kapasitas harus berada pada rentang 10 sampai 60 kursi.");
        }
        this->kapasitas = kap;
    }

    string getStatusBus() const { return statusBus; }

    bool setStatusBus(const string& status) {
        if (status == "Tersedia" || status == "Jalan" || status == "Perbaikan") {
            catatRiwayatStatus("Status diubah dari '" + this->statusBus + "' menjadi '" + status + "'.");
            this->statusBus = status;
            return true;
        }
        cout << "[Armada Logistics] Error: Status '" << status << "' tidak dikenali oleh sistem regulasi!\n";
        return false;
    }

    string getMerkBus() const { return merkBus; }

    void setMerkBus(const string& merk) {
        this->merkBus = merk;
    }

    int getTahunPembuatan() const { return tahunPembuatan; }

    void setTahunPembuatan(int tahun) {
        if (!validasiTahunPembuatan(tahun)) {
            throw ArmadaBusException("Tahun pembuatan tidak valid.");
        }
        this->tahunPembuatan = tahun;
    }

    long getTotalKilometer() const { return totalKilometer; }

    long getKilometerSejakServisTerakhir() const { return kilometerSejakServisTerakhir; }

    int getJumlahRiwayat() const { return jumlahRiwayat; }

    string getRiwayatKe(int index) const {
        if (index < 0 || index >= jumlahRiwayat) {
            throw ArmadaBusException("Index riwayat status di luar batas.");
        }
        return riwayatStatus[index];
    }

    static bool validasiPlatNomor(const string& plat) {
        if (plat.empty()) {
            cout << "[Sistem Validasi] Error: Plat nomor polisi tidak boleh kosong!\n";
            return false;
        }
        if (plat.length() < 4 || plat.length() > 10) {
            cout << "[Sistem Validasi] Error: Ukuran panjang karakter plat nomor tidak valid (4-10 Karakter)!\n";
            return false;
        }
        for (char c : plat) {
            if (isspace(static_cast<unsigned char>(c))) {
                cout << "[Sistem Validasi] Error: Tulis plat nomor tanpa menggunakan karakter spasi!\n";
                return false;
            }
            if (!isalnum(static_cast<unsigned char>(c))) {
                cout << "[Sistem Validasi] Error: Plat nomor hanya boleh berisi kombinasi huruf dan angka saja!\n";
                return false;
            }
        }
        return true;
    }

    static bool validasiKapasitas(int kap) {
        return kap >= KAPASITAS_MINIMUM && kap <= KAPASITAS_MAKSIMUM;
    }

    static bool validasiTahunPembuatan(int tahun) {
        time_t sekarang = time(nullptr);
        tm* waktuLokal = localtime(&sekarang);
        int tahunSekarang = 1900 + waktuLokal->tm_year;
        return tahun >= 1980 && tahun <= tahunSekarang;
    }

    bool apakahSiapJalan() const {
        return (this->statusBus == "Tersedia");
    }

    bool apakahSedangJalan() const {
        return (this->statusBus == "Jalan");
    }

    bool apakahSedangPerbaikan() const {
        return (this->statusBus == "Perbaikan");
    }

    void jadwalkanKeberangkatan() {
        if (!apakahSiapJalan()) {
            throw ArmadaBusException("Bus ID " + idBus + " tidak dapat diberangkatkan, status saat ini: " + statusBus);
        }
        setStatusBus("Jalan");
        cout << "[Armada Logistics] Info: Bus ID " << idBus << " [" << platNomor << "] resmi diberangkatkan.\n";
    }

    void kembaliDariPerjalanan(long jarakTempuhKm) {
        if (!apakahSedangJalan()) {
            throw ArmadaBusException("Bus ID " + idBus + " tidak sedang dalam perjalanan.");
        }
        if (jarakTempuhKm < 0) {
            throw ArmadaBusException("Jarak tempuh tidak boleh bernilai negatif.");
        }
        tambahKilometer(jarakTempuhKm);
        setStatusBus("Tersedia");
        cout << "[Armada Logistics] Info: Bus ID " << idBus << " telah kembali dari perjalanan sejauh "
             << jarakTempuhKm << " km.\n";
    }

    void kirimKeBengkelPerbaikan(const string& alasan = "Rutin") {
        catatRiwayatStatus("Dikirim ke bengkel, alasan: " + alasan);
        this->statusBus = "Perbaikan";
        kilometerSejakServisTerakhir = 0;
        cout << "[Armada Logistics] Info: Bus ID " << this->idBus << " [" << this->platNomor
             << "] dialihkan ke mode Maintenance. Alasan: " << alasan << "\n";
    }

    void selesaiPerbaikan() {
        if (!apakahSedangPerbaikan()) {
            throw ArmadaBusException("Bus ID " + idBus + " tidak sedang dalam status perbaikan.");
        }
        setStatusBus("Tersedia");
        cout << "[Armada Logistics] Info: Bus ID " << idBus << " telah selesai diperbaiki dan siap dijalankan.\n";
    }

    void tambahKilometer(long jarakKm) {
        if (jarakKm < 0) {
            throw ArmadaBusException("Penambahan kilometer tidak boleh bernilai negatif.");
        }
        totalKilometer += jarakKm;
        kilometerSejakServisTerakhir += jarakKm;
    }

    bool apakahPerluServis() const {
        return kilometerSejakServisTerakhir >= BATAS_KILOMETER_SERVIS;
    }

    string getKategoriKapasitas() const {
        if (kapasitas <= 20) {
            return "Kecil";
        } else if (kapasitas <= 40) {
            return "Sedang";
        }
        return "Besar";
    }

    int getUsiaOperasional() const {
        if (tahunPembuatan <= 0) {
            return 0;
        }
        time_t sekarang = time(nullptr);
        tm* waktuLokal = localtime(&sekarang);
        int tahunSekarang = 1900 + waktuLokal->tm_year;
        return tahunSekarang - tahunPembuatan;
    }

    static string generateIdBus(int urutan) {
        ostringstream oss;
        oss << "BUS-" << (urutan < 1000 ? "0" : "") << (urutan < 100 ? "0" : "")
            << (urutan < 10 ? "0" : "") << urutan;
        return oss.str();
    }

    static int getTotalArmadaTerdaftar() {
        return totalArmadaTerdaftar;
    }

    void inputFormulirBus() {
        string inputId, inputPlat;
        int inputKap = 0;
        int opsiStatus = 0;

        cout << "\n+-------------------------------------------------------+\n";
        cout << "|             FORMULIR PENDAFTARAN ARMADA BUS BARU       |\n";
        cout << "+-------------------------------------------------------+\n";

        cout << " -> Masukkan Kode ID Bus Baru   : ";
        cin >> inputId;
        this->idBus = inputId;

        do {
            cout << " -> Masukkan Nomor Plat (No-Space): ";
            cin >> inputPlat;
        } while (!validasiPlatNomor(inputPlat));
        this->platNomor = normalisasiPlatNomor(inputPlat);

        do {
            cout << " -> Masukkan Kapasitas Kursi    : ";
            cin >> inputKap;
            if (cin.fail() || !validasiKapasitas(inputKap)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Sistem Validasi] Error: Kapasitas harus antara " << KAPASITAS_MINIMUM
                     << " dan " << KAPASITAS_MAKSIMUM << " kursi!\n";
                inputKap = 0;
            }
        } while (inputKap == 0);
        this->kapasitas = inputKap;

        do {
            cout << " -> Tentukan Status Kesiapan Awal Bus:\n";
            cout << "    [1] Tersedia (Siap dijadwalkan jalan)\n";
            cout << "    [2] Jalan    (Sedang aktif di rute travel)\n";
            cout << "    [3] Perbaikan (Sedang di bengkel / rusak)\n";
            cout << "    Pilihan Anda (1-3): ";
            cin >> opsiStatus;

            if (cin.fail() || opsiStatus < 1 || opsiStatus > 3) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Sistem Validasi] Error: Masukan salah! Pilih opsi angka 1 sampai 3.\n";
                opsiStatus = 0;
            }
        } while (opsiStatus == 0);

        if (opsiStatus == 1) this->statusBus = "Tersedia";
        else if (opsiStatus == 2) this->statusBus = "Jalan";
        else this->statusBus = "Perbaikan";

        catatRiwayatStatus("Formulir pendaftaran diisi secara manual melalui CLI.");

        cout << "+-------------------------------------------------------+\n";
        cout << "| [Sukses] Data spesifikasi armada bus berhasil diset.  |\n";
        cout << "+-------------------------------------------------------+\n";
    }

    void tampilkanSpesifikasiBus() const {
        cout << "\n+=======================================================+\n";
        cout << "|               KARTU INVENTARIS UNIT ARMADA            |\n";
        cout << "+=======================================================+\n";
        cout << "  > Kode Identifikasi  : " << this->idBus << "\n";
        cout << "  > Nomor Plat Polisi  : " << this->platNomor << "\n";
        cout << "  > Kapasitas Muatan   : " << this->kapasitas << " Kursi Penumpang ("
             << getKategoriKapasitas() << ")\n";
        cout << "  > Merk Bus           : " << (merkBus.empty() ? "-" : merkBus) << "\n";
        cout << "  > Tahun Pembuatan    : " << (tahunPembuatan > 0 ? to_string(tahunPembuatan) : "-") << "\n";
        cout << "  > Total Kilometer    : " << totalKilometer << " km\n";
        cout << "  > Status Manajemen   : ";

        if (this->statusBus == "Tersedia") {
            cout << "[ READY / TERSEDIA ]\n";
        } else if (this->statusBus == "Jalan") {
            cout << "[ ON TRIP / SEDANG JALAN ]\n";
        } else {
            cout << "[ UNDER MAINTENANCE / PERBAIKAN ]\n";
        }

        if (apakahPerluServis()) {
            cout << "  > Peringatan         : Bus ini sudah melebihi batas servis berkala!\n";
        }
        cout << "+=======================================================+\n";
    }

    void tampilkanRiwayatStatus() const {
        cout << "=== Riwayat Status Bus ID " << idBus << " ===\n";
        for (int i = 0; i < jumlahRiwayat; i++) {
            cout << (i + 1) << ". " << riwayatStatus[i] << "\n";
        }
    }

    bool operator==(const ArmadaBus& lain) const {
        return idBus == lain.idBus && platNomor == lain.platNomor;
    }

    bool operator!=(const ArmadaBus& lain) const {
        return !(*this == lain);
    }

    bool operator<(const ArmadaBus& lain) const {
        return kapasitas < lain.kapasitas;
    }

    bool operator>(const ArmadaBus& lain) const {
        return kapasitas > lain.kapasitas;
    }

    friend ostream& operator<<(ostream& out, const ArmadaBus& bus) {
        out << "ArmadaBus[" << bus.idBus << ", plat=" << bus.platNomor
            << ", kapasitas=" << bus.kapasitas << ", status=" << bus.statusBus << "]";
        return out;
    }
};

#endif
