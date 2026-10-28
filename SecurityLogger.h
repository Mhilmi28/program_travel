#ifndef SECURITYLOGGER_H
#define SECURITYLOGGER_H

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <cctype>
#include <stdexcept>
#include <sstream>

using namespace std;

class SecurityLoggerException : public runtime_error {
public:
    explicit SecurityLoggerException(const string& pesan) : runtime_error(pesan) {}
};

class SecurityLogger {
private:
    string idLog;
    string aktivitas;
    string waktu;
    string levelLog;
    string aktorAkun;

    static long jumlahLogDibuatKeseluruhan;

    string generateIdLogOtomatis() {
        time_t t = time(nullptr);
        tm* now = localtime(&t);

        char buffer[16];
        strftime(buffer, sizeof(buffer), "%Y%m%d-%H%M%S", now);

        jumlahLogDibuatKeseluruhan++;
        ostringstream oss;
        oss << "LOG-" << buffer << "-" << setfill('0') << setw(4)
            << (jumlahLogDibuatKeseluruhan % 10000);
        return oss.str();
    }

    string dapatkanWaktuSistem() const {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now);
        return string(buffer);
    }

    string standarisasiLevel(const string& level) const {
        string lvl = level;
        for (auto &c : lvl) {
            c = toupper(static_cast<unsigned char>(c));
        }
        return lvl;
    }

public:
    SecurityLogger() {
        idLog = "";
        aktivitas = "";
        waktu = "";
        levelLog = "INFO";
        aktorAkun = "GUEST";
    }

    SecurityLogger(const string& deskripsiAktivitas, const string& level, const string& aktor) {
        idLog = generateIdLogOtomatis();
        aktivitas = deskripsiAktivitas;
        waktu = dapatkanWaktuSistem();
        aktorAkun = aktor.empty() ? "GUEST" : aktor;

        string lvl = standarisasiLevel(level);
        levelLog = isLevelValid(lvl) ? lvl : "INFO";
    }

    virtual ~SecurityLogger() {}

    string getIdLog() const { return idLog; }
    void setIdLog(const string& id) { idLog = id; }

    string getAktivitas() const { return aktivitas; }
    void setAktivitas(const string& akt) {
        if (!validasiDeskripsiLog(akt)) {
            throw SecurityLoggerException("Deskripsi aktivitas tidak memenuhi standar validasi.");
        }
        aktivitas = akt;
    }

    string getWaktu() const { return waktu; }
    void setWaktu(const string& wkt) { waktu = wkt; }

    string getLevelLog() const { return levelLog; }

    bool setLevelLog(const string& level) {
        string lvl = standarisasiLevel(level);
        if (isLevelValid(lvl)) {
            this->levelLog = lvl;
            return true;
        }
        cout << "[Logger System] Error: Kategori level log '" << level << "' tidak valid!\n";
        return false;
    }

    string getAktorAkun() const { return aktorAkun; }
    void setAktorAkun(const string& aktor) {
        aktorAkun = aktor.empty() ? "GUEST" : aktor;
    }

    static bool isLevelValid(const string& level) {
        return level == "INFO" || level == "WARNING" ||
               level == "SECURITY_ALERT" || level == "CRITICAL";
    }

    static bool validasiDeskripsiLog(const string& deskripsi) {
        if (deskripsi.empty()) {
            cout << "[Sistem Validasi] Error: Deskripsi aktivitas log tidak boleh kosong!\n";
            return false;
        }
        if (deskripsi.length() < 10) {
            cout << "[Sistem Validasi] Error: Deskripsi aktivitas terlalu pendek dan tidak informatif (Minimal 10 karakter)!\n";
            return false;
        }
        return true;
    }

    static long getJumlahLogDibuatKeseluruhan() {
        return jumlahLogDibuatKeseluruhan;
    }

    bool apakahKritis() const {
        return levelLog == "CRITICAL" || levelLog == "SECURITY_ALERT";
    }

    int getBobotUrgensi() const {
        if (levelLog == "CRITICAL") return 4;
        if (levelLog == "SECURITY_ALERT") return 3;
        if (levelLog == "WARNING") return 2;
        return 1;
    }

    void inputLogManual(const string& usernameAktif) {
        string inputDeskripsi;
        int opsiLevel = 0;

        cout << "\n+-------------------------------------------------------+\n";
        cout << "|             FORMULIR PENCATATAN AUDIT LOG MANUAL      |\n";
        cout << "+-------------------------------------------------------+\n";

        this->idLog = generateIdLogOtomatis();
        this->waktu = dapatkanWaktuSistem();
        this->aktorAkun = usernameAktif.empty() ? "GUEST" : usernameAktif;

        cout << " -> ID Audit Log (Auto)     : " << this->idLog << "\n";
        cout << " -> Waktu Eksekusi (Auto)   : " << this->waktu << "\n";
        cout << " -> Aktor Penanggung Jawab  : " << this->aktorAkun << "\n";

        do {
            cout << " -> Pilih Tingkat Urgensi Log:\n";
            cout << "    [1] INFO           (Aktivitas operasional standar)\n";
            cout << "    [2] WARNING        (Deteksi kejanggalan non-fatal)\n";
            cout << "    [3] SECURITY_ALERT (Percobaan akses ilegal/salah password)\n";
            cout << "    [4] CRITICAL       (Kegagalan sistem / kebocoran data)\n";
            cout << "    Pilihan Anda (1-4): ";
            cin >> opsiLevel;

            if (cin.fail() || opsiLevel < 1 || opsiLevel > 4) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Sistem Validasi] Error: Pilihan tidak valid! Masukkan angka 1 sampai 4.\n";
                opsiLevel = 0;
            }
        } while (opsiLevel == 0);

        if (opsiLevel == 1) this->levelLog = "INFO";
        else if (opsiLevel == 2) this->levelLog = "WARNING";
        else if (opsiLevel == 3) this->levelLog = "SECURITY_ALERT";
        else this->levelLog = "CRITICAL";

        cin.ignore(10000, '\n');

        do {
            cout << " -> Masukkan Detail Aktivitas : ";
            getline(cin, inputDeskripsi);
        } while (!validasiDeskripsiLog(inputDeskripsi));
        this->aktivitas = inputDeskripsi;

        cout << "+-------------------------------------------------------+\n";
        cout << "| [Sukses] Data Audit Log Keamanan berhasil disimpan.  |\n";
        cout << "+-------------------------------------------------------+\n";
    }

    void tampilkanBarisLog() const {
        cout << "[" << this->waktu << "] ";

        if (this->levelLog == "CRITICAL") {
            cout << "[!!! CRITICAL !!!]";
        } else if (this->levelLog == "SECURITY_ALERT") {
            cout << "[SEC_ALERT]";
        } else if (this->levelLog == "WARNING") {
            cout << "[WARNING]";
        } else {
            cout << "[INFO]";
        }

        cout << " ID: " << this->idLog
             << " | Aktor: " << this->aktorAkun
             << " -> " << this->aktivitas << "\n";
    }

    void tampilkanDetailBoxLog() const {
        cout << "\n+-------------------------------------------------------+\n";
        cout << "|                  DETAIL INSPEKSI AUDIT LOG            |\n";
        cout << "+-------------------------------------------------------+\n";
        cout << "  > Kode Log ID      : " << this->idLog << "\n";
        cout << "  > Waktu Kejadian   : " << this->waktu << "\n";
        cout << "  > Kategori Level   : " << this->levelLog << "\n";
        cout << "  > Nama Aktor/User  : " << this->aktorAkun << "\n";
        cout << "  > Deskripsi Event  : " << this->aktivitas << "\n";
        cout << "+-------------------------------------------------------+\n";
    }

    bool operator==(const SecurityLogger& lain) const {
        return idLog == lain.idLog;
    }

    bool operator!=(const SecurityLogger& lain) const {
        return !(*this == lain);
    }

    bool operator<(const SecurityLogger& lain) const {
        return getBobotUrgensi() < lain.getBobotUrgensi();
    }

    bool operator>(const SecurityLogger& lain) const {
        return getBobotUrgensi() > lain.getBobotUrgensi();
    }

    friend ostream& operator<<(ostream& out, const SecurityLogger& log) {
        out << "[" << log.waktu << "][" << log.levelLog << "] " << log.aktorAkun
            << " -> " << log.aktivitas;
        return out;
    }
};

class LogRepository {
private:
    SecurityLogger* daftarLog;
    int jumlahLog;
    int kapasitasLog;

    void perbesarKapasitasJikaPenuh() {
        if (jumlahLog >= kapasitasLog) {
            int kapasitasBaru = kapasitasLog * 2;
            SecurityLogger* temp = new SecurityLogger[kapasitasBaru];
            for (int i = 0; i < jumlahLog; i++) {
                temp[i] = daftarLog[i];
            }
            delete[] daftarLog;
            daftarLog = temp;
            kapasitasLog = kapasitasBaru;
        }
    }

public:
    explicit LogRepository(int kapasitasAwal = 10) {
        if (kapasitasAwal <= 0) {
            throw SecurityLoggerException("Kapasitas awal LogRepository harus bernilai positif.");
        }
        kapasitasLog = kapasitasAwal;
        jumlahLog = 0;
        daftarLog = new SecurityLogger[kapasitasLog];
    }

    LogRepository(const LogRepository& lain) {
        kapasitasLog = lain.kapasitasLog;
        jumlahLog = lain.jumlahLog;
        daftarLog = new SecurityLogger[kapasitasLog];
        for (int i = 0; i < jumlahLog; i++) {
            daftarLog[i] = lain.daftarLog[i];
        }
    }

    LogRepository& operator=(const LogRepository& lain) {
        if (this == &lain) {
            return *this;
        }
        delete[] daftarLog;
        kapasitasLog = lain.kapasitasLog;
        jumlahLog = lain.jumlahLog;
        daftarLog = new SecurityLogger[kapasitasLog];
        for (int i = 0; i < jumlahLog; i++) {
            daftarLog[i] = lain.daftarLog[i];
        }
        return *this;
    }

    virtual ~LogRepository() {
        delete[] daftarLog;
    }

    void tambahLog(const SecurityLogger& log) {
        perbesarKapasitasJikaPenuh();
        daftarLog[jumlahLog] = log;
        jumlahLog++;
    }

    void catatLogBaru(const string& deskripsi, const string& level, const string& aktor) {
        perbesarKapasitasJikaPenuh();
        daftarLog[jumlahLog] = SecurityLogger(deskripsi, level, aktor);
        jumlahLog++;
    }

    int getJumlahLog() const { return jumlahLog; }

    const SecurityLogger& getLogKe(int index) const {
        if (index < 0 || index >= jumlahLog) {
            throw SecurityLoggerException("Index log di luar batas.");
        }
        return daftarLog[index];
    }

    int cariIndexById(const string& idLog) const {
        for (int i = 0; i < jumlahLog; i++) {
            if (daftarLog[i].getIdLog() == idLog) {
                return i;
            }
        }
        return -1;
    }

    int hitungLogPerLevel(const string& level) const {
        int total = 0;
        for (int i = 0; i < jumlahLog; i++) {
            if (daftarLog[i].getLevelLog() == level) {
                total++;
            }
        }
        return total;
    }

    int hitungLogKritis() const {
        int total = 0;
        for (int i = 0; i < jumlahLog; i++) {
            if (daftarLog[i].apakahKritis()) {
                total++;
            }
        }
        return total;
    }

    void tampilkanSemuaLog() const {
        cout << "\n=== SELURUH RIWAYAT AUDIT LOG (" << jumlahLog << " entri) ===\n";
        for (int i = 0; i < jumlahLog; i++) {
            daftarLog[i].tampilkanBarisLog();
        }
    }

    void tampilkanLogByLevel(const string& level) const {
        cout << "\n=== AUDIT LOG DENGAN LEVEL: " << level << " ===\n";
        bool adaData = false;
        for (int i = 0; i < jumlahLog; i++) {
            if (daftarLog[i].getLevelLog() == level) {
                daftarLog[i].tampilkanBarisLog();
                adaData = true;
            }
        }
        if (!adaData) {
            cout << "  (Tidak ada log dengan level ini)\n";
        }
    }

    void tampilkanLogByAktor(const string& aktor) const {
        cout << "\n=== AUDIT LOG OLEH AKTOR: " << aktor << " ===\n";
        bool adaData = false;
        for (int i = 0; i < jumlahLog; i++) {
            if (daftarLog[i].getAktorAkun() == aktor) {
                daftarLog[i].tampilkanBarisLog();
                adaData = true;
            }
        }
        if (!adaData) {
            cout << "  (Tidak ada log dari aktor ini)\n";
        }
    }

    void tampilkanRingkasanStatistik() const {
        cout << "\n+=======================================================+\n";
        cout << "|             STATISTIK RINGKASAN AUDIT LOG             |\n";
        cout << "+=======================================================+\n";
        cout << "  Total Seluruh Log   : " << jumlahLog << "\n";
        cout << "  Total INFO          : " << hitungLogPerLevel("INFO") << "\n";
        cout << "  Total WARNING       : " << hitungLogPerLevel("WARNING") << "\n";
        cout << "  Total SECURITY_ALERT: " << hitungLogPerLevel("SECURITY_ALERT") << "\n";
        cout << "  Total CRITICAL      : " << hitungLogPerLevel("CRITICAL") << "\n";
        cout << "+=======================================================+\n";
    }
};

#endif
