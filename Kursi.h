#ifndef KURSI_H
#define KURSI_H

#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <stdexcept>
#include <sstream>

using namespace std;

class KursiException : public runtime_error {
public:
    explicit KursiException(const string& pesan) : runtime_error(pesan) {}
};

class Kursi {
private:
    string idKursi;
    string idBusTerkait;
    string nomorKursi;
    int baris;
    char kolomHuruf;
    string tipeKursi;
    string statusKursi;
    string idPenumpangSaatIni;
    double biayaTambahan;

    static int totalKursiDibuat;

    static string buatNomorKursi(int barisIn, char kolomIn) {
        ostringstream oss;
        oss << barisIn << kolomIn;
        return oss.str();
    }

    void inisialisasiDasar() {
        statusKursi = "Kosong";
        idPenumpangSaatIni = "-";
    }

public:
    Kursi() {
        idKursi = "";
        idBusTerkait = "";
        nomorKursi = "-";
        baris = 0;
        kolomHuruf = '-';
        tipeKursi = "Reguler";
        statusKursi = "Kosong";
        idPenumpangSaatIni = "-";
        biayaTambahan = 0.0;
        totalKursiDibuat++;
    }

    // Konstruktor legacy: posisi diturunkan dari string nomor kursi (mis. "3B")
    Kursi(const string& idK, const string& idBus, const string& nomor, const string& tipe) {
        idKursi = idK;
        idBusTerkait = idBus;
        nomorKursi = nomor;
        parseNomorKursi(nomor, baris, kolomHuruf);
        tipeKursi = (tipe == "VIP" || tipe == "Reguler") ? tipe : "Reguler";
        inisialisasiDasar();
        biayaTambahan = (tipeKursi == "VIP") ? 50000.0 : 0.0;
        totalKursiDibuat++;
    }

    Kursi(const string& idK, const string& idBus, const string& nomor, const string& tipe, double tambahan) {
        idKursi = idK;
        idBusTerkait = idBus;
        nomorKursi = nomor;
        parseNomorKursi(nomor, baris, kolomHuruf);
        tipeKursi = (tipe == "VIP" || tipe == "Reguler") ? tipe : "Reguler";
        inisialisasiDasar();
        biayaTambahan = (tambahan >= 0.0) ? tambahan : 0.0;
        totalKursiDibuat++;
    }

    // Konstruktor 2D: posisi baris & kolom diberikan langsung (dipakai oleh DenahKursiBus)
    Kursi(const string& idK, const string& idBus, int barisIn, char kolomIn, const string& tipe) {
        idKursi = idK;
        idBusTerkait = idBus;
        baris = barisIn;
        kolomHuruf = static_cast<char>(toupper(static_cast<unsigned char>(kolomIn)));
        nomorKursi = buatNomorKursi(baris, kolomHuruf);
        tipeKursi = (tipe == "VIP" || tipe == "Reguler") ? tipe : "Reguler";
        inisialisasiDasar();
        biayaTambahan = (tipeKursi == "VIP") ? 50000.0 : 0.0;
        totalKursiDibuat++;
    }

    // Rule of Five
    Kursi(const Kursi& lain)
        : idKursi(lain.idKursi), idBusTerkait(lain.idBusTerkait), nomorKursi(lain.nomorKursi),
          baris(lain.baris), kolomHuruf(lain.kolomHuruf), tipeKursi(lain.tipeKursi),
          statusKursi(lain.statusKursi), idPenumpangSaatIni(lain.idPenumpangSaatIni),
          biayaTambahan(lain.biayaTambahan) {
        totalKursiDibuat++;
    }

    Kursi& operator=(const Kursi& lain) {
        if (this == &lain) {
            return *this;
        }
        idKursi = lain.idKursi;
        idBusTerkait = lain.idBusTerkait;
        nomorKursi = lain.nomorKursi;
        baris = lain.baris;
        kolomHuruf = lain.kolomHuruf;
        tipeKursi = lain.tipeKursi;
        statusKursi = lain.statusKursi;
        idPenumpangSaatIni = lain.idPenumpangSaatIni;
        biayaTambahan = lain.biayaTambahan;
        return *this;
    }

    Kursi(Kursi&& lain) noexcept
        : idKursi(std::move(lain.idKursi)), idBusTerkait(std::move(lain.idBusTerkait)),
          nomorKursi(std::move(lain.nomorKursi)), baris(lain.baris), kolomHuruf(lain.kolomHuruf),
          tipeKursi(std::move(lain.tipeKursi)), statusKursi(std::move(lain.statusKursi)),
          idPenumpangSaatIni(std::move(lain.idPenumpangSaatIni)), biayaTambahan(lain.biayaTambahan) {
        totalKursiDibuat++;
    }

    Kursi& operator=(Kursi&& lain) noexcept {
        if (this == &lain) {
            return *this;
        }
        idKursi = std::move(lain.idKursi);
        idBusTerkait = std::move(lain.idBusTerkait);
        nomorKursi = std::move(lain.nomorKursi);
        baris = lain.baris;
        kolomHuruf = lain.kolomHuruf;
        tipeKursi = std::move(lain.tipeKursi);
        statusKursi = std::move(lain.statusKursi);
        idPenumpangSaatIni = std::move(lain.idPenumpangSaatIni);
        biayaTambahan = lain.biayaTambahan;
        return *this;
    }

    virtual ~Kursi() {
        totalKursiDibuat--;
    }

    // Getter & Setter
    string getIdKursi() const { return idKursi; }
    void setIdKursi(const string& idK) { idKursi = idK; }

    string getIdBusTerkait() const { return idBusTerkait; }
    void setIdBusTerkait(const string& idBus) { idBusTerkait = idBus; }

    string getNomorKursi() const { return nomorKursi; }
    void setNomorKursi(const string& nomor) {
        nomorKursi = nomor;
        parseNomorKursi(nomor, baris, kolomHuruf);
    }

    int getBaris() const { return baris; }
    char getKolomHuruf() const { return kolomHuruf; }

    string getTipeKursi() const { return tipeKursi; }
    void setTipeKursi(const string& tipe) {
        if (tipe == "VIP" || tipe == "Reguler") {
            tipeKursi = tipe;
        } else {
            cout << "[Validasi Gagal] Tipe kursi harus 'VIP' atau 'Reguler'" << endl;
        }
    }

    string getStatusKursi() const { return statusKursi; }

    string getIdPenumpangSaatIni() const { return idPenumpangSaatIni; }

    double getBiayaTambahan() const { return biayaTambahan; }
    void setBiayaTambahan(double tambahan) {
        if (tambahan >= 0.0) {
            biayaTambahan = tambahan;
        } else {
            cout << "[Validasi Gagal] Biaya tambahan tidak boleh negatif" << endl;
        }
    }

    bool isKosong() const { return statusKursi == "Kosong"; }
    bool isDipesan() const { return statusKursi == "Dipesan"; }
    bool isTerisi() const { return statusKursi == "Terisi"; }
    bool isNonaktif() const { return statusKursi == "Nonaktif"; }
    bool isVIP() const { return tipeKursi == "VIP"; }

    // Menandai slot sebagai tidak tersedia untuk dipesan (dipakai DenahKursiBus
    // ketika kapasitas bus tidak habis membagi jumlah kolom denah).
    void nonaktifkanSlot() {
        statusKursi = "Nonaktif";
        idPenumpangSaatIni = "-";
    }

    bool pesanKursi(const string& idPelanggan) {
        if (!isKosong()) {
            cout << "[Gagal] Kursi " << nomorKursi << " sedang tidak tersedia untuk dipesan." << endl;
            return false;
        }
        statusKursi = "Dipesan";
        idPenumpangSaatIni = idPelanggan;
        return true;
    }

    bool checkInKursi() {
        if (!isDipesan()) {
            cout << "[Gagal] Kursi " << nomorKursi << " belum berstatus Dipesan, tidak dapat check-in." << endl;
            return false;
        }
        statusKursi = "Terisi";
        return true;
    }

    bool kosongkanKursi() {
        if (isKosong() || isNonaktif()) {
            cout << "[Info] Kursi " << nomorKursi << " memang sudah kosong/tidak aktif." << endl;
            return false;
        }
        statusKursi = "Kosong";
        idPenumpangSaatIni = "-";
        return true;
    }

    bool batalkanPesananKursi() {
        if (!isDipesan()) {
            cout << "[Gagal] Kursi " << nomorKursi << " tidak dalam status Dipesan." << endl;
            return false;
        }
        statusKursi = "Kosong";
        idPenumpangSaatIni = "-";
        return true;
    }

    double hitungTotalHarga(double hargaDasarPaket) const {
        return hargaDasarPaket + biayaTambahan;
    }

    bool operator==(const Kursi& lain) const { return idKursi == lain.idKursi; }
    bool operator<(const Kursi& lain) const { return nomorKursi < lain.nomorKursi; }
    bool operator>(const Kursi& lain) const { return nomorKursi > lain.nomorKursi; }

    friend ostream& operator<<(ostream& out, const Kursi& k) {
        out << "[" << k.nomorKursi << "] " << k.tipeKursi << " - " << k.statusKursi;
        return out;
    }

    void tampilkanDetailKursi() const {
        cout << endl;
        cout << "+-------------------------------------------------------+" << endl;
        cout << "  KARTU DETAIL KURSI: [" << nomorKursi << "]" << endl;
        cout << "+-------------------------------------------------------+" << endl;
        cout << "  > ID Kursi         : " << idKursi << endl;
        cout << "  > Armada Bus       : " << idBusTerkait << endl;
        cout << "  > Posisi (B/K)     : Baris " << baris << ", Kolom " << kolomHuruf << endl;
        cout << "  > Tipe Kursi       : " << tipeKursi << endl;
        cout << "  > Biaya Tambahan   : Rp " << fixed << setprecision(2) << biayaTambahan << endl;
        cout << "  > Status Saat Ini  : " << statusKursi << endl;
        if (!isKosong() && !isNonaktif()) {
            cout << "  > Ditempati Oleh   : " << idPenumpangSaatIni << endl;
        }
        cout << "+-------------------------------------------------------+" << endl;
    }

    // Parsing "3B" -> baris=3, kolom='B'. Aman terhadap format yang tidak sesuai.
    static void parseNomorKursi(const string& nomor, int& barisOut, char& kolomOut) {
        barisOut = 0;
        kolomOut = '-';
        if (nomor.empty()) {
            return;
        }
        char hurufTerakhir = nomor[nomor.size() - 1];
        if (!isalpha(static_cast<unsigned char>(hurufTerakhir))) {
            return;
        }
        string bagianAngka = nomor.substr(0, nomor.size() - 1);
        if (bagianAngka.empty()) {
            return;
        }
        for (char c : bagianAngka) {
            if (!isdigit(static_cast<unsigned char>(c))) {
                return;
            }
        }
        barisOut = stoi(bagianAngka);
        kolomOut = static_cast<char>(toupper(static_cast<unsigned char>(hurufTerakhir)));
    }

    static void buatLayoutKursiBus(Kursi* daftarKursi, int& jumlahKursi, const string& idBus,
                                    int kapasitas, int jumlahBarisVIP) {
        jumlahKursi = 0;
        for (int i = 0; i < kapasitas; i++) {
            string idK = idBus + "-K" + to_string(i + 1);
            int barisKe = (i / 4) + 1;
            char kolomKe = char('A' + (i % 4));
            string tipe = (i < jumlahBarisVIP * 4) ? "VIP" : "Reguler";
            daftarKursi[jumlahKursi] = Kursi(idK, idBus, barisKe, kolomKe, tipe);
            jumlahKursi++;
        }
    }

    static int cariIndexByNomor(const Kursi* daftarKursi, int jumlahKursi, string nomor) {
        for (int i = 0; i < jumlahKursi; i++) {
            if (daftarKursi[i].getNomorKursi() == nomor) {
                return i;
            }
        }
        return -1;
    }

    static int hitungKursiKosong(const Kursi* daftarKursi, int jumlahKursi) {
        int total = 0;
        for (int i = 0; i < jumlahKursi; i++) {
            if (daftarKursi[i].isKosong()) {
                total++;
            }
        }
        return total;
    }

    static int hitungKursiTerisi(const Kursi* daftarKursi, int jumlahKursi) {
        int total = 0;
        for (int i = 0; i < jumlahKursi; i++) {
            if (daftarKursi[i].isTerisi() || daftarKursi[i].isDipesan()) {
                total++;
            }
        }
        return total;
    }

    static int filterKursiKosong(const Kursi* daftarKursi, int jumlahKursi, int hasilIndex[]) {
        int jumlahHasil = 0;
        for (int i = 0; i < jumlahKursi; i++) {
            if (daftarKursi[i].isKosong()) {
                hasilIndex[jumlahHasil] = i;
                jumlahHasil++;
            }
        }
        return jumlahHasil;
    }

    static int filterKursiVIP(const Kursi* daftarKursi, int jumlahKursi, int hasilIndex[]) {
        int jumlahHasil = 0;
        for (int i = 0; i < jumlahKursi; i++) {
            if (daftarKursi[i].isVIP()) {
                hasilIndex[jumlahHasil] = i;
                jumlahHasil++;
            }
        }
        return jumlahHasil;
    }

    static void tampilkanPetaKursi(const Kursi* daftarKursi, int jumlahKursi) {
        cout << endl;
        cout << "+=======================================================+" << endl;
        cout << "|                  PETA KURSI ARMADA BUS                |" << endl;
        cout << "+=======================================================+" << endl;
        for (int i = 0; i < jumlahKursi; i++) {
            cout << " [" << setw(4) << daftarKursi[i].getNomorKursi() << "] ";
            if (daftarKursi[i].isVIP()) {
                cout << "(VIP) ";
            } else {
                cout << "      ";
            }
            if (daftarKursi[i].isKosong()) {
                cout << "Kosong";
            } else if (daftarKursi[i].isDipesan()) {
                cout << "Dipesan";
            } else if (daftarKursi[i].isTerisi()) {
                cout << "Terisi";
            } else {
                cout << "Nonaktif";
            }
            cout << endl;
        }
        cout << "---------------------------------------------------------" << endl;
        cout << "Kosong: " << hitungKursiKosong(daftarKursi, jumlahKursi)
             << " | Terisi/Dipesan: " << hitungKursiTerisi(daftarKursi, jumlahKursi) << endl;
        cout << "+=======================================================+" << endl;
    }

    static int getTotalKursiDibuat() {
        return totalKursiDibuat;
    }
};

// Definisi static member di luar class -- WAJIB ada agar tidak terjadi
// linker error "undefined reference to Kursi::totalKursiDibuat".


/**
 * @class DenahKursiBus
 * @brief Kelas manajer yang merepresentasikan kursi satu armada bus sebagai
 * denah dua dimensi (baris x kolom) untuk keperluan visualisasi dan booking.
 *
 * Denah menggunakan array 2D berukuran tetap (bukan STL container) dengan
 * gang penumpang (aisle) di antara kolom B dan C, meniru tata letak bus
 * 2-2 pada umumnya. Slot yang melebihi kapasitas aktual bus otomatis
 * ditandai "Nonaktif" agar tidak dapat dipesan.
 */
class DenahKursiBus {
public:
    static const int MAKS_BARIS = 20;
    static const int JUMLAH_KOLOM = 4; // Kolom A, B (kiri) | C, D (kanan)

private:
    Kursi grid[MAKS_BARIS][JUMLAH_KOLOM];
    string idBusTerkait;
    int jumlahBarisAktif;
    int kapasitasAktif;

    static int totalDenahDibuat;

    string formatSelKursi(const Kursi& k) const {
        string simbol;
        if (k.isNonaktif()) {
            simbol = "X";
        } else if (k.isKosong()) {
            simbol = "K";
        } else if (k.isDipesan()) {
            simbol = "D";
        } else {
            simbol = "T";
        }
        if (k.isVIP() && !k.isNonaktif()) {
            simbol += "*";
        }
        ostringstream oss;
        oss << "[" << setw(2) << left << simbol << "]";
        return oss.str();
    }

public:
    DenahKursiBus() {
        idBusTerkait = "";
        jumlahBarisAktif = 0;
        kapasitasAktif = 0;
        totalDenahDibuat++;
    }

    DenahKursiBus(const string& idBus, int kapasitas, int jumlahBarisVIP) {
        if (kapasitas <= 0) {
            throw KursiException("Kapasitas denah kursi harus bernilai positif.");
        }

        idBusTerkait = idBus;
        jumlahBarisAktif = (kapasitas + JUMLAH_KOLOM - 1) / JUMLAH_KOLOM;

        if (jumlahBarisAktif > MAKS_BARIS) {
            throw KursiException("Kapasitas bus melebihi batas maksimum baris denah (" +
                                  to_string(MAKS_BARIS * JUMLAH_KOLOM) + " kursi).");
        }

        kapasitasAktif = kapasitas;
        int barisVIPValid = (jumlahBarisVIP < 0) ? 0 : jumlahBarisVIP;
        int penghitungKursi = 0;

        for (int b = 0; b < jumlahBarisAktif; b++) {
            for (int k = 0; k < JUMLAH_KOLOM; k++) {
                if (penghitungKursi < kapasitas) {
                    string idK = idBus + "-K" + to_string(penghitungKursi + 1);
                    string tipe = (b < barisVIPValid) ? "VIP" : "Reguler";
                    grid[b][k] = Kursi(idK, idBus, b + 1, char('A' + k), tipe);
                    penghitungKursi++;
                } else {
                    grid[b][k] = Kursi();
                    grid[b][k].nonaktifkanSlot();
                }
            }
        }
        totalDenahDibuat++;
    }

    // Rule of Five
    DenahKursiBus(const DenahKursiBus& lain)
        : idBusTerkait(lain.idBusTerkait), jumlahBarisAktif(lain.jumlahBarisAktif),
          kapasitasAktif(lain.kapasitasAktif) {
        for (int b = 0; b < MAKS_BARIS; b++) {
            for (int k = 0; k < JUMLAH_KOLOM; k++) {
                grid[b][k] = lain.grid[b][k];
            }
        }
        totalDenahDibuat++;
    }

    DenahKursiBus& operator=(const DenahKursiBus& lain) {
        if (this == &lain) {
            return *this;
        }
        idBusTerkait = lain.idBusTerkait;
        jumlahBarisAktif = lain.jumlahBarisAktif;
        kapasitasAktif = lain.kapasitasAktif;
        for (int b = 0; b < MAKS_BARIS; b++) {
            for (int k = 0; k < JUMLAH_KOLOM; k++) {
                grid[b][k] = lain.grid[b][k];
            }
        }
        return *this;
    }

    DenahKursiBus(DenahKursiBus&& lain) noexcept
        : idBusTerkait(std::move(lain.idBusTerkait)), jumlahBarisAktif(lain.jumlahBarisAktif),
          kapasitasAktif(lain.kapasitasAktif) {
        for (int b = 0; b < MAKS_BARIS; b++) {
            for (int k = 0; k < JUMLAH_KOLOM; k++) {
                grid[b][k] = std::move(lain.grid[b][k]);
            }
        }
        totalDenahDibuat++;
    }

    DenahKursiBus& operator=(DenahKursiBus&& lain) noexcept {
        if (this == &lain) {
            return *this;
        }
        idBusTerkait = std::move(lain.idBusTerkait);
        jumlahBarisAktif = lain.jumlahBarisAktif;
        kapasitasAktif = lain.kapasitasAktif;
        for (int b = 0; b < MAKS_BARIS; b++) {
            for (int k = 0; k < JUMLAH_KOLOM; k++) {
                grid[b][k] = std::move(lain.grid[b][k]);
            }
        }
        return *this;
    }

    virtual ~DenahKursiBus() {
        totalDenahDibuat--;
    }

    // Getter
    string getIdBusTerkait() const { return idBusTerkait; }
    int getJumlahBarisAktif() const { return jumlahBarisAktif; }
    int getKapasitasAktif() const { return kapasitasAktif; }

    static int getTotalDenahDibuat() { return totalDenahDibuat; }

    // Validasi posisi
    bool validasiPosisi(int barisInput, char kolomInput) const {
        if (barisInput < 1 || barisInput > jumlahBarisAktif) {
            return false;
        }
        int idxKolom = toupper(static_cast<unsigned char>(kolomInput)) - 'A';
        if (idxKolom < 0 || idxKolom >= JUMLAH_KOLOM) {
            return false;
        }
        if (grid[barisInput - 1][idxKolom].isNonaktif()) {
            return false;
        }
        return true;
    }

    const Kursi& getKursiAt(int barisInput, char kolomInput) const {
        int idxKolom = toupper(static_cast<unsigned char>(kolomInput)) - 'A';
        if (barisInput < 1 || barisInput > jumlahBarisAktif || idxKolom < 0 || idxKolom >= JUMLAH_KOLOM) {
            throw KursiException("Posisi kursi (baris/kolom) di luar batas denah.");
        }
        return grid[barisInput - 1][idxKolom];
    }

    // Operasi booking berbasis posisi 2D
    bool pesanKursiPosisi(int barisInput, char kolomInput, const string& idPelanggan) {
        if (!validasiPosisi(barisInput, kolomInput)) {
            cout << "[Denah Kursi] Error: Posisi baris " << barisInput << " kolom "
                 << static_cast<char>(toupper(static_cast<unsigned char>(kolomInput)))
                 << " tidak valid atau tidak tersedia!" << endl;
            return false;
        }
        int idxKolom = toupper(static_cast<unsigned char>(kolomInput)) - 'A';
        return grid[barisInput - 1][idxKolom].pesanKursi(idPelanggan);
    }

    bool checkInPosisi(int barisInput, char kolomInput) {
        if (!validasiPosisi(barisInput, kolomInput)) {
            cout << "[Denah Kursi] Error: Posisi kursi tidak valid untuk check-in!" << endl;
            return false;
        }
        int idxKolom = toupper(static_cast<unsigned char>(kolomInput)) - 'A';
        return grid[barisInput - 1][idxKolom].checkInKursi();
    }

    bool batalkanPosisi(int barisInput, char kolomInput) {
        if (!validasiPosisi(barisInput, kolomInput)) {
            cout << "[Denah Kursi] Error: Posisi kursi tidak valid untuk dibatalkan!" << endl;
            return false;
        }
        int idxKolom = toupper(static_cast<unsigned char>(kolomInput)) - 'A';
        return grid[barisInput - 1][idxKolom].batalkanPesananKursi();
    }

    bool kosongkanPosisi(int barisInput, char kolomInput) {
        if (!validasiPosisi(barisInput, kolomInput)) {
            cout << "[Denah Kursi] Error: Posisi kursi tidak valid untuk dikosongkan!" << endl;
            return false;
        }
        int idxKolom = toupper(static_cast<unsigned char>(kolomInput)) - 'A';
        return grid[barisInput - 1][idxKolom].kosongkanKursi();
    }

    bool cariPosisiKosongPertama(int& barisHasil, char& kolomHasil) const {
        for (int b = 0; b < jumlahBarisAktif; b++) {
            for (int k = 0; k < JUMLAH_KOLOM; k++) {
                if (grid[b][k].isKosong()) {
                    barisHasil = b + 1;
                    kolomHasil = char('A' + k);
                    return true;
                }
            }
        }
        return false;
    }

    // Statistik
    int hitungTotalKosong() const {
        int total = 0;
        for (int b = 0; b < jumlahBarisAktif; b++) {
            for (int k = 0; k < JUMLAH_KOLOM; k++) {
                if (grid[b][k].isKosong()) {
                    total++;
                }
            }
        }
        return total;
    }

    int hitungTotalTerisiAtauDipesan() const {
        int total = 0;
        for (int b = 0; b < jumlahBarisAktif; b++) {
            for (int k = 0; k < JUMLAH_KOLOM; k++) {
                if (grid[b][k].isDipesan() || grid[b][k].isTerisi()) {
                    total++;
                }
            }
        }
        return total;
    }

    int hitungTotalVIP() const {
        int total = 0;
        for (int b = 0; b < jumlahBarisAktif; b++) {
            for (int k = 0; k < JUMLAH_KOLOM; k++) {
                if (grid[b][k].isVIP() && !grid[b][k].isNonaktif()) {
                    total++;
                }
            }
        }
        return total;
    }

    // Visualisasi denah 2D
    void tampilkanDenah2D() const {
        cout << "\n+=========================================================+\n";
        cout << "|            DENAH KURSI 2D - ARMADA: " << left << setw(11) << idBusTerkait << "|\n";
        cout << "+=========================================================+\n";
        cout << "  Baris |    A       B    ||    C       D   \n";
        cout << " -------+----------------++-----------------\n";
        for (int b = 0; b < jumlahBarisAktif; b++) {
            cout << "   " << setw(3) << right << (b + 1) << "  |";
            for (int k = 0; k < JUMLAH_KOLOM; k++) {
                if (k == 2) {
                    cout << "|";
                }
                cout << " " << formatSelKursi(grid[b][k]) << " ";
            }
            cout << "\n";
        }
        cout << "-----------------------------------------------------------\n";
        cout << "Legend: [K ]=Kosong  [D ]=Dipesan  [T ]=Terisi  [X ]=Nonaktif  (*)=VIP\n";
        cout << "Kosong: " << hitungTotalKosong()
             << " | Terisi/Dipesan: " << hitungTotalTerisiAtauDipesan()
             << " | Kursi VIP: " << hitungTotalVIP()
             << " | Kapasitas Aktif: " << kapasitasAktif << "\n";
        cout << "+=========================================================+\n";
    }

    // Formulir interaktif pemesanan kursi via denah 2D
    bool formulirPemesananInteraktif(const string& idPelanggan) {
        tampilkanDenah2D();

        int barisInput = 0;
        char kolomInput = ' ';
        bool sukses = false;
        int percobaan = 0;
        const int MAKS_PERCOBAAN = 5;

        do {
            cout << " -> Pilih Nomor Baris Kursi (1-" << jumlahBarisAktif << ") : ";
            cin >> barisInput;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Sistem Validasi] Error: Input baris harus berupa angka!\n";
                percobaan++;
                continue;
            }

            cout << " -> Pilih Kolom Kursi (A-D)         : ";
            cin >> kolomInput;

            if (!validasiPosisi(barisInput, kolomInput)) {
                cout << "[Sistem Validasi] Error: Posisi kursi tidak valid atau tidak tersedia!\n";
                percobaan++;
                continue;
            }

            sukses = pesanKursiPosisi(barisInput, kolomInput, idPelanggan);
            if (!sukses) {
                percobaan++;
            }
        } while (!sukses && percobaan < MAKS_PERCOBAAN);

        if (sukses) {
            cout << "+---------------------------------------------------------+\n";
            cout << "| [Sukses] Kursi " << barisInput
                 << static_cast<char>(toupper(static_cast<unsigned char>(kolomInput)))
                 << " berhasil dipesan atas nama " << idPelanggan << ".\n";
            cout << "+---------------------------------------------------------+\n";
        } else {
            cout << "[Gagal] Batas percobaan pemesanan kursi tercapai.\n";
        }
        return sukses;
    }

    friend ostream& operator<<(ostream& out, const DenahKursiBus& denah) {
        out << "DenahKursiBus[" << denah.idBusTerkait << ", baris=" << denah.jumlahBarisAktif
            << ", kapasitas=" << denah.kapasitasAktif
            << ", kosong=" << denah.hitungTotalKosong() << "]";
        return out;
    }
};

#endif
