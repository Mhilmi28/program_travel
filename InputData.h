#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <iostream>
#include <string>
#include <stdexcept>
#include "PaketTravel.h"
#include "ArmadaBus.h"
#include "Reservasi.h"

using namespace std;

class InputDataException : public runtime_error {
public:
    explicit InputDataException(const string& pesan) : runtime_error(pesan) {}
};

class InputData {
private:
    static int totalTransaksiDiproses;

    bool teksKosong(const string& teks) const {
        return teks.empty();
    }

    int inputAngkaBulatPositif(const string& label) const {
        int nilai = -1;
        do {
            cout << label;
            cin >> nilai;
            if (cin.fail() || nilai <= 0) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Sistem Validasi] Error: Masukan harus berupa angka bulat positif!\n";
                nilai = -1;
            }
        } while (nilai <= 0);
        return nilai;
    }

    double inputAngkaDesimalPositif(const string& label) const {
        double nilai = -1.0;
        do {
            cout << label;
            cin >> nilai;
            if (cin.fail() || nilai < 0) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Sistem Validasi] Error: Masukan harus berupa angka positif!\n";
                nilai = -1.0;
            }
        } while (nilai < 0);
        return nilai;
    }

public:
    InputData() {}
    virtual ~InputData() {}

    static int getTotalTransaksiDiproses() {
        return totalTransaksiDiproses;
    }

    int cariIndexPaket(const PaketTravel* daftarPaket, int jumlahPaket, const string& idPaket) const {
        for (int i = 0; i < jumlahPaket; i++) {
            if (daftarPaket[i].getIdPaket() == idPaket) {
                return i;
            }
        }
        return -1;
    }

    int cariIndexArmada(const ArmadaBus* daftarArmada, int jumlahArmada, const string& idBus) const {
        for (int i = 0; i < jumlahArmada; i++) {
            if (daftarArmada[i].getIdBus() == idBus) {
                return i;
            }
        }
        return -1;
    }

    bool validasiIdPaketUnik(const PaketTravel* daftarPaket, int jumlahPaket, const string& idPaket) const {
        return cariIndexPaket(daftarPaket, jumlahPaket, idPaket) == -1;
    }

    bool validasiIdArmadaUnik(const ArmadaBus* daftarArmada, int jumlahArmada, const string& idBus) const {
        return cariIndexArmada(daftarArmada, jumlahArmada, idBus) == -1;
    }

    void inputPaketBaru(PaketTravel*& daftarPaket, int& jumlahPaket, int& kapasitasPaket) {
        if (kapasitasPaket <= 0) {
            throw InputDataException("Kapasitas awal daftar paket tidak valid.");
        }

        if (jumlahPaket >= kapasitasPaket) {
            int kapasitasBaru = kapasitasPaket * 2;
            PaketTravel* temp = new PaketTravel[kapasitasBaru];
            for (int i = 0; i < jumlahPaket; i++) {
                temp[i] = daftarPaket[i];
            }
            delete[] daftarPaket;
            daftarPaket = temp;
            kapasitasPaket = kapasitasBaru;
        }

        string id, nama, dest, jdwl;

        cout << "\n+-------------------------------------------------------+\n";
        cout << "|               INPUT DATA PRODUK PAKET TRAVEL BARU     |\n";
        cout << "+-------------------------------------------------------+\n";

        do {
            cout << " -> Kode ID Paket Baru (cth: PKT11) : ";
            cin >> id;
            if (!validasiIdPaketUnik(daftarPaket, jumlahPaket, id)) {
                cout << "[Sistem Validasi] Error: Kode ID Paket sudah terdaftar, gunakan kode lain!\n";
                id = "";
            }
        } while (id.empty());

        cin.ignore(10000, '\n');

        do {
            cout << " -> Nama Paket Destinasi            : ";
            getline(cin, nama);
            if (teksKosong(nama)) {
                cout << "[Sistem Validasi] Error: Nama paket tidak boleh kosong!\n";
            }
        } while (teksKosong(nama));

        do {
            cout << " -> Rincian Lokasi Wisata           : ";
            getline(cin, dest);
            if (teksKosong(dest)) {
                cout << "[Sistem Validasi] Error: Rincian lokasi wisata tidak boleh kosong!\n";
            }
        } while (teksKosong(dest));

        double hrg = inputAngkaDesimalPositif(" -> Harga Paket Per-Pax (Rp)        : ");

        cin.ignore(10000, '\n');

        do {
            cout << " -> Jadwal Keberangkatan Rutin      : ";
            getline(cin, jdwl);
            if (teksKosong(jdwl)) {
                cout << "[Sistem Validasi] Error: Jadwal keberangkatan tidak boleh kosong!\n";
            }
        } while (teksKosong(jdwl));

        daftarPaket[jumlahPaket] = PaketTravel(id, nama, dest, hrg, jdwl);
        jumlahPaket++;
        totalTransaksiDiproses++;

        cout << "+-------------------------------------------------------+\n";
        cout << "| [Sukses] Data Paket Travel baru berhasil disimpan!    |\n";
        cout << "+-------------------------------------------------------+\n";
    }

    void tampilkanDaftarPaket(const PaketTravel* daftarPaket, int jumlahPaket) const {
        cout << "\n+=======================================================+\n";
        cout << "|                 DAFTAR PAKET TRAVEL TERSEDIA          |\n";
        cout << "+=======================================================+\n";
        if (jumlahPaket == 0) {
            cout << "  (Belum ada data paket travel yang terdaftar)\n";
        }
        for (int i = 0; i < jumlahPaket; i++) {
            cout << "  " << (i + 1) << ". ID: " << daftarPaket[i].getIdPaket()
                 << " | Harga: Rp " << daftarPaket[i].getHarga() << "\n";
        }
        cout << "+=======================================================+\n";
    }

    bool hapusPaket(PaketTravel* daftarPaket, int& jumlahPaket, const string& idPaket) {
        int index = cariIndexPaket(daftarPaket, jumlahPaket, idPaket);
        if (index == -1) {
            cout << "[Sistem Validasi] Error: Kode ID Paket tidak ditemukan!\n";
            return false;
        }
        for (int i = index; i < jumlahPaket - 1; i++) {
            daftarPaket[i] = daftarPaket[i + 1];
        }
        jumlahPaket--;
        cout << "[Sukses] Data Paket Travel dengan ID " << idPaket << " berhasil dihapus.\n";
        return true;
    }

    void inputArmadaBaru(ArmadaBus*& daftarArmada, int& jumlahArmada, int& kapasitasArmada) {
        if (kapasitasArmada <= 0) {
            throw InputDataException("Kapasitas awal daftar armada tidak valid.");
        }

        if (jumlahArmada >= kapasitasArmada) {
            int kapasitasBaru = kapasitasArmada * 2;
            ArmadaBus* temp = new ArmadaBus[kapasitasBaru];
            for (int i = 0; i < jumlahArmada; i++) {
                temp[i] = daftarArmada[i];
            }
            delete[] daftarArmada;
            daftarArmada = temp;
            kapasitasArmada = kapasitasBaru;
        }

        string idBus, nopol;
        int kapKursi;

        cout << "\n+-------------------------------------------------------+\n";
        cout << "|               INPUT DATA ARMADA BUS BARU              |\n";
        cout << "+-------------------------------------------------------+\n";

        do {
            cout << " -> Kode ID Bus Baru (cth: BUS01)   : ";
            cin >> idBus;
            if (!validasiIdArmadaUnik(daftarArmada, jumlahArmada, idBus)) {
                cout << "[Sistem Validasi] Error: Kode ID Bus sudah terdaftar, gunakan kode lain!\n";
                idBus = "";
            }
        } while (idBus.empty());

        do {
            cout << " -> Nomor Polisi / Plat Kendaraan   : ";
            cin >> nopol;
        } while (!ArmadaBus::validasiPlatNomor(nopol));

        do {
            cout << " -> Kapasitas Kursi Bus (Angka)     : ";
            cin >> kapKursi;
            if (cin.fail() || !ArmadaBus::validasiKapasitas(kapKursi)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "[Sistem Validasi] Error: Kapasitas harus antara "
                     << ArmadaBus::KAPASITAS_MINIMUM << " dan "
                     << ArmadaBus::KAPASITAS_MAKSIMUM << " kursi!\n";
                kapKursi = 0;
            }
        } while (kapKursi == 0);

        daftarArmada[jumlahArmada] = ArmadaBus(idBus, nopol, kapKursi, "Tersedia");
        jumlahArmada++;
        totalTransaksiDiproses++;

        cout << "+-------------------------------------------------------+\n";
        cout << "| [Sukses] Data Unit Armada Bus berhasil disimpan!      |\n";
        cout << "+-------------------------------------------------------+\n";
    }

    void tampilkanDaftarArmada(const ArmadaBus* daftarArmada, int jumlahArmada) const {
        cout << "\n+=======================================================+\n";
        cout << "|                 DAFTAR UNIT ARMADA BUS                |\n";
        cout << "+=======================================================+\n";
        if (jumlahArmada == 0) {
            cout << "  (Belum ada data armada bus yang terdaftar)\n";
        }
        for (int i = 0; i < jumlahArmada; i++) {
            daftarArmada[i].tampilkanSpesifikasiBus();
        }
    }

    bool hapusArmada(ArmadaBus* daftarArmada, int& jumlahArmada, const string& idBus) {
        int index = cariIndexArmada(daftarArmada, jumlahArmada, idBus);
        if (index == -1) {
            cout << "[Sistem Validasi] Error: Kode ID Bus tidak ditemukan!\n";
            return false;
        }
        for (int i = index; i < jumlahArmada - 1; i++) {
            daftarArmada[i] = daftarArmada[i + 1];
        }
        jumlahArmada--;
        cout << "[Sukses] Data Armada Bus dengan ID " << idBus << " berhasil dihapus.\n";
        return true;
    }

    bool ubahStatusArmada(ArmadaBus* daftarArmada, int jumlahArmada, const string& idBus, const string& statusBaru) {
        int index = cariIndexArmada(daftarArmada, jumlahArmada, idBus);
        if (index == -1) {
            cout << "[Sistem Validasi] Error: Kode ID Bus tidak ditemukan!\n";
            return false;
        }
        return daftarArmada[index].setStatusBus(statusBaru);
    }

    void prosesReservasiBaru(Reservasi*& daftarReservasi, int& jumlahReservasi, int& kapasitasReservasi,
                              const string& idPelanggan, const PaketTravel* daftarPaket, int jumlahPaket) {
        if (kapasitasReservasi <= 0) {
            throw InputDataException("Kapasitas awal daftar reservasi tidak valid.");
        }

        if (jumlahReservasi >= kapasitasReservasi) {
            int kapasitasBaru = kapasitasReservasi * 2;
            Reservasi* temp = new Reservasi[kapasitasBaru];
            for (int i = 0; i < jumlahReservasi; i++) {
                temp[i] = daftarReservasi[i];
            }
            delete[] daftarReservasi;
            daftarReservasi = temp;
            kapasitasReservasi = kapasitasBaru;
        }

        string idRes, idPaketTarget, tgl;
        double hargaPaket = 0.0;
        int indexPaket = -1;

        cout << "\n+-------------------------------------------------------+\n";
        cout << "|               FORMULIR TRANSAKSI RESERVASI BARU       |\n";
        cout << "+-------------------------------------------------------+\n";
        cout << " -> Buat ID Booking Baru (cth: RSV01) : ";
        cin >> idRes;

        do {
            cout << " -> Masukkan Kode ID Paket Pilihan    : ";
            cin >> idPaketTarget;
            indexPaket = cariIndexPaket(daftarPaket, jumlahPaket, idPaketTarget);
            if (indexPaket == -1) {
                cout << "[Reservasi Gagal] Kode Paket Travel tidak valid / tidak ditemukan!\n";
            }
        } while (indexPaket == -1);

        hargaPaket = daftarPaket[indexPaket].getHarga();

        int jmlKursi = inputAngkaBulatPositif(" -> Jumlah Kursi yang Dipesan (Pax)   : ");

        do {
            cout << " -> Tanggal Keberangkatan (DD/MM/YYYY): ";
            cin >> tgl;
            if (teksKosong(tgl)) {
                cout << "[Sistem Validasi] Error: Tanggal keberangkatan tidak boleh kosong!\n";
            }
        } while (teksKosong(tgl));

        double totalSewa = hargaPaket * jmlKursi;

        daftarReservasi[jumlahReservasi] = Reservasi(idRes, idPelanggan, idPaketTarget, jmlKursi, totalSewa, tgl, "PENDING");
        jumlahReservasi++;
        totalTransaksiDiproses++;

        cout << "+-------------------------------------------------------+\n";
        cout << " [Sukses] Booking Berhasil dibuat dengan Status PENDING!\n";
        cout << " Total Tagihan: Rp " << totalSewa << "\n";
        cout << "+-------------------------------------------------------+\n";
    }
};

#endif
