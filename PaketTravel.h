#ifndef PAKETTRAVEL_H
#define PAKETTRAVEL_H

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

class PaketTravel {
private:
    string idPaket;
    string namaPaket;
    string destinasi;
    long long harga;
    string jadwal;
    string kategori;
    int durasiHari;
    int kapasitasMaksimal;
    int kuotaTersisa;
    double persenDiskon;
    double rating;
    bool statusAktif;

public:
    PaketTravel() {
        idPaket = "";
        namaPaket = "-";
        destinasi = "-";
        harga = 0.0;
        jadwal = "-";
        kategori = "Domestik";
        durasiHari = 1;
        kapasitasMaksimal = 40;
        kuotaTersisa = 40;
        persenDiskon = 0.0;
        rating = 0.0;
        statusAktif = true;
    }

    PaketTravel(const string& id, const string& nama, const string& dest, double hrg, const string& jdwl) {
        idPaket = id;
        namaPaket = nama;
        destinasi = dest;
        harga = (hrg >= 0.0) ? hrg : 0.0;
        jadwal = jdwl;
        kategori = "Domestik";
        durasiHari = 3;
        kapasitasMaksimal = 40;
        kuotaTersisa = 40;
        persenDiskon = 0.0;
        rating = 0.0;
        statusAktif = true;
    }

    PaketTravel(const string& id, const string& nama, const string& dest, double hrg, const string& jdwl,
                const string& kat, int durasi, int kapasitas, double diskon) {
        idPaket = id;
        namaPaket = nama;
        destinasi = dest;
        harga = (hrg >= 0.0) ? hrg : 0.0;
        jadwal = jdwl;
        kategori = kat;
        durasiHari = (durasi > 0) ? durasi : 1;
        kapasitasMaksimal = (kapasitas > 0) ? kapasitas : 1;
        kuotaTersisa = kapasitasMaksimal;
        persenDiskon = (diskon >= 0.0 && diskon <= 1.0) ? diskon : 0.0;
        rating = 0.0;
        statusAktif = true;
    }

    PaketTravel(const PaketTravel& sumber) {
        idPaket = sumber.idPaket;
        namaPaket = sumber.namaPaket;
        destinasi = sumber.destinasi;
        harga = sumber.harga;
        jadwal = sumber.jadwal;
        kategori = sumber.kategori;
        durasiHari = sumber.durasiHari;
        kapasitasMaksimal = sumber.kapasitasMaksimal;
        kuotaTersisa = sumber.kuotaTersisa;
        persenDiskon = sumber.persenDiskon;
        rating = sumber.rating;
        statusAktif = sumber.statusAktif;
    }

    virtual ~PaketTravel() {}

    PaketTravel& operator=(const PaketTravel& sumber) {
        if (this == &sumber) {
            return *this;
        }
        idPaket = sumber.idPaket;
        namaPaket = sumber.namaPaket;
        destinasi = sumber.destinasi;
        harga = sumber.harga;
        jadwal = sumber.jadwal;
        kategori = sumber.kategori;
        durasiHari = sumber.durasiHari;
        kapasitasMaksimal = sumber.kapasitasMaksimal;
        kuotaTersisa = sumber.kuotaTersisa;
        persenDiskon = sumber.persenDiskon;
        rating = sumber.rating;
        statusAktif = sumber.statusAktif;
        return *this;
    }

    string getIdPaket() const { return idPaket; }
    void setIdPaket(const string& id) { idPaket = id; }

    string getNamaPaket() const { return namaPaket; }
    void setNamaPaket(const string& nama) { namaPaket = nama; }

    string getDestinasi() const { return destinasi; }
    void setDestinasi(const string& dest) { destinasi = dest; }

    double getHarga() const { return harga; }
    void setHarga(double hrg) {
        if (hrg >= 0.0) {
            harga = hrg;
        } else {
            cout << "[Validasi Gagal] Harga tidak boleh negatif" << endl;
        }
    }

    string getJadwal() const { return jadwal; }
    void setJadwal(const string& jdwl) { jadwal = jdwl; }

    string getKategori() const { return kategori; }
    void setKategori(const string& kat) { kategori = kat; }

    int getDurasiHari() const { return durasiHari; }
    void setDurasiHari(int durasi) {
        if (durasi > 0) {
            durasiHari = durasi;
        } else {
            cout << "[Validasi Gagal] Durasi hari harus lebih dari nol" << endl;
        }
    }

    int getKapasitasMaksimal() const { return kapasitasMaksimal; }
    void setKapasitasMaksimal(int kapasitas) {
        if (kapasitas > 0) {
            kapasitasMaksimal = kapasitas;
            if (kuotaTersisa > kapasitasMaksimal) {
                kuotaTersisa = kapasitasMaksimal;
            }
        } else {
            cout << "[Validasi Gagal] Kapasitas harus lebih dari nol" << endl;
        }
    }

    int getKuotaTersisa() const { return kuotaTersisa; }

    double getPersenDiskon() const { return persenDiskon; }
    void setPersenDiskon(double diskon) {
        if (diskon >= 0.0 && diskon <= 1.0) {
            persenDiskon = diskon;
        } else {
            cout << "[Validasi Gagal] Persentase diskon harus antara 0 dan 1" << endl;
        }
    }

    double getRating() const { return rating; }
    void setRating(double nilai) {
        if (nilai >= 0.0 && nilai <= 5.0) {
            rating = nilai;
        } else {
            cout << "[Validasi Gagal] Rating harus antara 0 sampai 5" << endl;
        }
    }

    bool getStatusAktif() const { return statusAktif; }
    void setStatusAktif(bool status) { statusAktif = status; }

    double hitungHargaSetelahDiskon() const {
        return harga - (harga * persenDiskon);
    }

    double hitungHargaGroup(int jumlahOrang) const {
        if (jumlahOrang <= 0) {
            return 0.0;
        }
        return hitungHargaSetelahDiskon() * jumlahOrang;
    }

    double hitungTotalPendapatanPotensial() const {
        return hitungHargaSetelahDiskon() * kapasitasMaksimal;
    }

    double hitungHargaPerHari() const {
        if (durasiHari <= 0) {
            return harga;
        }
        return harga / durasiHari;
    }

    bool kurangiKuota(int jumlah) {
        if (jumlah <= 0) {
            cout << "[Gagal] Jumlah pengurangan kuota tidak valid" << endl;
            return false;
        }
        if (jumlah > kuotaTersisa) {
            cout << "[Gagal] Kuota tersisa tidak mencukupi untuk paket " << idPaket << endl;
            return false;
        }
        kuotaTersisa -= jumlah;
        return true;
    }

    bool tambahKuota(int jumlah) {
        if (jumlah <= 0) {
            cout << "[Gagal] Jumlah penambahan kuota tidak valid" << endl;
            return false;
        }
        kuotaTersisa += jumlah;
        if (kuotaTersisa > kapasitasMaksimal) {
            kuotaTersisa = kapasitasMaksimal;
        }
        return true;
    }

    bool isTersedia() const {
        return statusAktif && kuotaTersisa > 0;
    }

    bool isPromo() const {
        return persenDiskon > 0.0;
    }

    bool isPenuh() const {
        return kuotaTersisa <= 0;
    }

    bool validasiData() const {
        if (idPaket.empty()) {
            return false;
        }
        if (namaPaket.empty() || namaPaket == "-") {
            return false;
        }
        if (harga < 0.0) {
            return false;
        }
        return true;
    }

    bool operator==(const PaketTravel& lain) const {
        return idPaket == lain.idPaket;
    }

    bool operator<(const PaketTravel& lain) const {
        return harga < lain.harga;
    }

    bool operator>(const PaketTravel& lain) const {
        return harga > lain.harga;
    }

    friend ostream& operator<<(ostream& out, const PaketTravel& paket) {
        out << "[" << paket.idPaket << "] " << paket.namaPaket << " - " << paket.destinasi
            << " (Rp" << fixed << setprecision(2) << paket.harga << ")";
        return out;
    }

    void tampilkanBrosurPaket() const {
        cout << endl;
        cout << "+-------------------------------------------------------+" << endl;
        cout << "  BROSUR ELEKTRONIK DETIL PAKET: [" << idPaket << "]" << endl;
        cout << "+-------------------------------------------------------+" << endl;
        cout << "  > Judul Tur Wisata   : " << namaPaket << endl;
        cout << "  > Titik Destinasi    : " << destinasi << endl;
        cout << "  > Kategori Paket     : " << kategori << endl;
        cout << "  > Jadwal Berangkat   : " << jadwal << endl;
        cout << "  > Durasi Perjalanan  : " << durasiHari << " Hari" << endl;
        cout << "  > Kapasitas Maksimal : " << kapasitasMaksimal << " Pax" << endl;
        cout << "  > Kuota Tersisa      : " << kuotaTersisa << " Pax" << endl;
        cout << "  > Rating Kepuasan    : " << fixed << setprecision(1) << rating << " / 5.0" << endl;
        if (isPromo()) {
            cout << "  > Harga Normal       : Rp " << fixed << setprecision(2) << harga << " / Pax" << endl;
            cout << "  > Diskon Promo       : " << (persenDiskon * 100.0) << "%" << endl;
            cout << "  > Harga Setelah Promo: Rp " << fixed << setprecision(2)
                 << hitungHargaSetelahDiskon() << " / Pax" << endl;
        } else {
            cout << "  > Investasi Harga    : Rp " << fixed << setprecision(2) << harga << " / Pax" << endl;
        }
        cout << "  > Status Ketersediaan: " << (isTersedia() ? "Tersedia" : "Tidak Tersedia") << endl;
        cout << "+-------------------------------------------------------+" << endl;
    }

    void tampilkanRingkasSatuBaris() const {
        cout << " [" << idPaket << "] " << namaPaket << " | " << destinasi
             << " | Rp " << fixed << setprecision(2) << hitungHargaSetelahDiskon()
             << " | Kuota: " << kuotaTersisa << "/" << kapasitasMaksimal << endl;
    }

    static void inisialisasiPaketBawaan(PaketTravel* daftarPaket, int& jumlahPaket) {
        daftarPaket[0] = PaketTravel("PKT01", "Eksotika Bali Safari", "Denpasar, Ubud, Bedugul", 4500000.0, "Setiap Hari Jumat");
        daftarPaket[1] = PaketTravel("PKT02", "Keajaiban Raja Ampat", "Pulau Wayag, Misool", 14200000.0, "Tanggal 10 & 25 tiap Bulan");
        daftarPaket[2] = PaketTravel("PKT03", "Penjelajah Labuan Bajo", "Pulau Komodo, Padar, Pink Beach", 7800000.0, "Setiap Hari Kamis");
        daftarPaket[3] = PaketTravel("PKT04", "Budaya & Sejarah Jogja", "Borobudur, Prambanan, Malioboro", 1800000.0, "Setiap Hari Sabtu");
        daftarPaket[4] = PaketTravel("PKT05", "Golden Sunrise Bromo", "Gunung Bromo, Air Terjun Madakaripura", 2500000.0, "Setiap Akhir Pekan");
        daftarPaket[5] = PaketTravel("PKT06", "Pesona Danau Toba", "Pulau Samosir, Parapat", 3900000.0, "Tanggal 15 tiap Bulan");
        daftarPaket[6] = PaketTravel("PKT07", "Surga Bawah Laut Bunaken", "Taman Nasional Bunaken, Manado", 5400000.0, "Setiap Hari Rabu");
        daftarPaket[7] = PaketTravel("PKT08", "Misteri Dataran Tinggi Dieng", "Candi Arjuna, Kawah Sikidang, Telaga Warna", 1650000.0, "Setiap Hari Sabtu");
        daftarPaket[8] = PaketTravel("PKT09", "Petualangan Derawan", "Pulau Maratua, Kakaban, Sangalaki", 6800000.0, "Kondisional / By Request");
        daftarPaket[9] = PaketTravel("PKT10", "Warisan Dunia Ujung Kulon", "Pulau Peucang, Konservasi Badak", 2900000.0, "Setiap Hari Jumat");

        daftarPaket[0].setRating(4.8);
        daftarPaket[1].setRating(4.9);
        daftarPaket[2].setRating(4.7);
        daftarPaket[3].setRating(4.5);
        daftarPaket[4].setRating(4.6);
        daftarPaket[5].setRating(4.4);
        daftarPaket[6].setRating(4.7);
        daftarPaket[7].setRating(4.3);
        daftarPaket[8].setRating(4.6);
        daftarPaket[9].setRating(4.2);

        daftarPaket[1].setPersenDiskon(0.10);
        daftarPaket[4].setPersenDiskon(0.15);
        daftarPaket[8].setPersenDiskon(0.05);

        jumlahPaket = 10;
    }

    static int cariIndexById(const PaketTravel* daftarPaket, int jumlahPaket, string id) {
        for (int i = 0; i < jumlahPaket; i++) {
            if (daftarPaket[i].getIdPaket() == id) {
                return i;
            }
        }
        return -1;
    }

    static int cariIndexTermurah(const PaketTravel* daftarPaket, int jumlahPaket) {
        if (jumlahPaket <= 0) {
            return -1;
        }
        int indexTermurah = 0;
        for (int i = 1; i < jumlahPaket; i++) {
            if (daftarPaket[i].getHarga() < daftarPaket[indexTermurah].getHarga()) {
                indexTermurah = i;
            }
        }
        return indexTermurah;
    }

    static int cariIndexTermahal(const PaketTravel* daftarPaket, int jumlahPaket) {
        if (jumlahPaket <= 0) {
            return -1;
        }
        int indexTermahal = 0;
        for (int i = 1; i < jumlahPaket; i++) {
            if (daftarPaket[i].getHarga() > daftarPaket[indexTermahal].getHarga()) {
                indexTermahal = i;
            }
        }
        return indexTermahal;
    }

    static double hitungTotalNilaiKatalog(const PaketTravel* daftarPaket, int jumlahPaket) {
        double total = 0.0;
        for (int i = 0; i < jumlahPaket; i++) {
            total += daftarPaket[i].getHarga();
        }
        return total;
    }

    static double hitungRataRataHarga(const PaketTravel* daftarPaket, int jumlahPaket) {
        if (jumlahPaket <= 0) {
            return 0.0;
        }
        return hitungTotalNilaiKatalog(daftarPaket, jumlahPaket) / jumlahPaket;
    }

    static void urutkanBerdasarkanHarga(PaketTravel* daftarPaket, int jumlahPaket, bool naik) {
        for (int i = 0; i < jumlahPaket - 1; i++) {
            for (int j = 0; j < jumlahPaket - 1 - i; j++) {
                bool tukar;
                if (naik) {
                    tukar = daftarPaket[j].getHarga() > daftarPaket[j + 1].getHarga();
                } else {
                    tukar = daftarPaket[j].getHarga() < daftarPaket[j + 1].getHarga();
                }
                if (tukar) {
                    PaketTravel temp = daftarPaket[j];
                    daftarPaket[j] = daftarPaket[j + 1];
                    daftarPaket[j + 1] = temp;
                }
            }
        }
    }

    static void urutkanBerdasarkanNama(PaketTravel* daftarPaket, int jumlahPaket) {
        for (int i = 0; i < jumlahPaket - 1; i++) {
            for (int j = 0; j < jumlahPaket - 1 - i; j++) {
                if (daftarPaket[j].getNamaPaket() > daftarPaket[j + 1].getNamaPaket()) {
                    PaketTravel temp = daftarPaket[j];
                    daftarPaket[j] = daftarPaket[j + 1];
                    daftarPaket[j + 1] = temp;
                }
            }
        }
    }

    static void urutkanBerdasarkanRating(PaketTravel* daftarPaket, int jumlahPaket) {
        for (int i = 0; i < jumlahPaket - 1; i++) {
            for (int j = 0; j < jumlahPaket - 1 - i; j++) {
                if (daftarPaket[j].getRating() < daftarPaket[j + 1].getRating()) {
                    PaketTravel temp = daftarPaket[j];
                    daftarPaket[j] = daftarPaket[j + 1];
                    daftarPaket[j + 1] = temp;
                }
            }
        }
    }

    static int filterByDestinasi(const PaketTravel* daftarPaket, int jumlahPaket, string keyword, int hasilIndex[]) {
        int jumlahHasil = 0;
        for (int i = 0; i < jumlahPaket; i++) {
            if (daftarPaket[i].getDestinasi().find(keyword) != string::npos) {
                hasilIndex[jumlahHasil] = i;
                jumlahHasil++;
            }
        }
        return jumlahHasil;
    }

    static int filterPaketTersedia(const PaketTravel* daftarPaket, int jumlahPaket, int hasilIndex[]) {
        int jumlahHasil = 0;
        for (int i = 0; i < jumlahPaket; i++) {
            if (daftarPaket[i].isTersedia()) {
                hasilIndex[jumlahHasil] = i;
                jumlahHasil++;
            }
        }
        return jumlahHasil;
    }

    static int filterPaketPromo(const PaketTravel* daftarPaket, int jumlahPaket, int hasilIndex[]) {
        int jumlahHasil = 0;
        for (int i = 0; i < jumlahPaket; i++) {
            if (daftarPaket[i].isPromo()) {
                hasilIndex[jumlahHasil] = i;
                jumlahHasil++;
            }
        }
        return jumlahHasil;
    }

    static void tampilkanSemuaPaket(const PaketTravel* daftarPaket, int jumlahPaket) {
        if (jumlahPaket <= 0) {
            cout << "[Info] Belum ada paket travel yang tersedia" << endl;
            return;
        }
        for (int i = 0; i < jumlahPaket; i++) {
            daftarPaket[i].tampilkanRingkasSatuBaris();
        }
    }
};

#endif
