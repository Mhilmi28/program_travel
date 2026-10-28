#include <iostream>
#include <string>
#include <iomanip>

// Menyertakan seluruh dependensi arsitektur subsistem header
#include "User.h"
#include "Admin.h"
#include "Petugas.h"
#include "Pelanggan.h"
#include "PaketTravel.h"
#include "ArmadaBus.h"
#include "Kursi.h"
#include "Reservasi.h"
#include "Pembayaran.h"
#include "VoucherPromo.h"
#include "Refund.h"
#include "Invoice.h"
#include "LaporanKeuangan.h"
#include "SecurityLogger.h"
#include "Session.h"
#include "LoginSystem.h"
#include "RegisterSystem.h"
#include "InputData.h"
#include "OutputData.h"
#include "PersistensiData.h"


// DEFINISI VARIABEL STATIS GLOBAL (ALOKASI FISIK LINKER)
int User::totalUserTerdaftar = 0;
int Admin::totalAdminTerdaftar = 0;
int Petugas::totalPetugasTerdaftar = 0;
int Kursi::totalKursiDibuat = 0;
int Reservasi::totalReservasiDibuat = 0;
int InputData::totalTransaksiDiproses = 0; 
int Pembayaran::totalTransaksiTercatat = 0;
double Pembayaran::grandTotalPembayaranSukses = 0.0;
int VoucherPromo::totalVoucherDibuat = 0;
double VoucherPromo::grandTotalPotonganDiberikan = 0.0;
int LaporanKeuangan::totalLaporanDibuat = 0;
double LaporanKeuangan::grandTotalPendapatanSemua = 0.0;
double LaporanKeuangan::grandTotalRefundSemua = 0.0;
long SecurityLogger::jumlahLogDibuatKeseluruhan = 0;
int LoginSystem::totalLoginBerhasil = 0;
int LoginSystem::totalLoginGagal = 0;
int LoginSystem::totalLogoutTercatat = 0;
int LoginSystem::totalAkunTerkunci = 0;
long Session::totalLogoutSepanjangWaktu = 0;
long Session::totalLoginSepanjangWaktu = 0;
int Session::totalSesiAktifSaatIni = 0;
int Invoice::totalInvoiceTerbit = 0;
int Refund::totalRefundDiajukan = 0;
int ArmadaBus::totalArmadaTerdaftar = 0;
int DenahKursiBus::totalDenahDibuat = 0;

int main() {
	
    // ALOKASI MEMORI DINAMIS AWAL (HEAP STORAGE MANAGEMENT)
 
    int kapasitasPetugas = 5;
    int jumlahPetugas = 0;
    Petugas* databasePetugas = new Petugas[kapasitasPetugas];

    int kapasitasPelanggan = 5;
    int jumlahPelanggan = 0;
    Pelanggan* databasePelanggan = new Pelanggan[kapasitasPelanggan];

    int kapasitasPaket = 20;
    int jumlahPaket = 0;
    PaketTravel* databasePaket = new PaketTravel[kapasitasPaket];

    int kapasitasArmada = 10;
    int jumlahArmada = 0;
    ArmadaBus* databaseArmada = new ArmadaBus[kapasitasArmada];

    int kapasitasReservasi = 50;
    int jumlahReservasi = 0;
    Reservasi* databaseReservasi = new Reservasi[kapasitasReservasi];

    int kapasitasVoucher = 10;
    int jumlahVoucher = 0;
    VoucherPromo* databaseVoucher = new VoucherPromo[kapasitasVoucher];

    int kapasitasLog = 100;
    int jumlahLog = 0;
    SecurityLogger* databaseLog = new SecurityLogger[kapasitasLog];

    // NAMA FILE PENYIMPANAN SESI PERSISTEN (REGISTRASI, LOGIN, BOOKING TIKET)
    const string FILE_PELANGGAN = "data_pelanggan.txt";
    const string FILE_PETUGAS = "data_petugas.txt";
    const string FILE_RESERVASI = "data_booking_tiket.txt";
    const string FILE_LOGIN_LOG = "data_login_log.txt";

    // Instansiasi objek pengendali controller sistem
    Session sesiAktif;
    LoginSystem authController;
    RegisterSystem regController;
    InputData dataEntryGate;
    OutputData viewRenderer;

    // MEMUAT DATA PERSISTEN DARI SESI SEBELUMNYA (JIKA FILE TERSEDIA)
    bool pelangganDimuatDariFile = PersistensiData::muatDataPelanggan(databasePelanggan, jumlahPelanggan, kapasitasPelanggan, FILE_PELANGGAN);
    bool petugasDimuatDariFile = PersistensiData::muatDataPetugas(databasePetugas, jumlahPetugas, kapasitasPetugas, FILE_PETUGAS);
    PersistensiData::muatDataReservasi(databaseReservasi, jumlahReservasi, kapasitasReservasi, FILE_RESERVASI);
    authController.muatLogDariFile(FILE_LOGIN_LOG);

    // INJEKSI DATA BAWAAN AWAL (AUTOMATIC DATA SEEDING)
    // Memuat 10 Paket Wisata Indonesia secara otomatis dari static utility PaketTravel
    PaketTravel::inisialisasiPaketBawaan(databasePaket, jumlahPaket);

    // Akun tester hanya diseed jika BELUM ada file data persisten dari sesi sebelumnya,
    // supaya tidak menduplikasi akun setiap kali aplikasi dijalankan ulang.
    if (!pelangganDimuatDariFile || jumlahPelanggan == 0) {
        databasePelanggan[jumlahPelanggan] = Pelanggan("USR-PLG01", "budi", "budi123", "PLG01", "Budi Santoso", "budi@email.com", "081234567890");
        jumlahPelanggan++;
        PersistensiData::simpanDataPelanggan(databasePelanggan, jumlahPelanggan, FILE_PELANGGAN);
    }

    if (!petugasDimuatDariFile || jumlahPetugas == 0) {
        databasePetugas[jumlahPetugas] = Petugas("USR-PTG01", "staf", "staf123", "PTG01", "Andi Wijaya");
        jumlahPetugas++;
        PersistensiData::simpanDataPetugas(databasePetugas, jumlahPetugas, FILE_PETUGAS);
    }

    // Menginisialisasi modul pencatatan keuangan internal untuk periode berjalan
    LaporanKeuangan akuntansiGlobal("Juni 2026", 0.0, 0.0);

    // Variables penampung input navigasi
    int menuUtamaPilihan = 0;
    bool aplikasiBerjalan = true;

    // STATE MACHINE CONTROL LOOP (MAIN CORRIDOR ENGINE)
    while (aplikasiBerjalan) {
        // JIKA BELUM ADA PENGGUNA YANG LOGIN (MENU PUBLIC / VISITOR)
        if (!sesiAktif.isLoginActive()) {
            std::cout << "\n=======================================================\n";
            std::cout << "           SISTEM INFORMASI MANAJEMEN UTAMA TRAVEL      \n";
            std::cout << "               PORTAL LAYANAN UTAMA KONSOL              \n";
            std::cout << "=======================================================\n";
            std::cout << "  1. Lihat Katalog Paket Wisata Nusantara\n";
            std::cout << "  2. Registrasi Akun Member Pelanggan Baru\n";
            std::cout << "  3. Portal Masuk / Login Akun (Semua Level)\n";
            std::cout << "  4. Keluar dari Aplikasi System\n";
            std::cout << "-------------------------------------------------------\n";
            std::cout << "  Masukkan Pilihan Opsi Anda (1-4): ";
            std::cin >> menuUtamaPilihan;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "[Sistem] Error: Masukan wajib berupa angka!\n";
                continue;
            }

            switch (menuUtamaPilihan) {
                case 1:
                    viewRenderer.tampilSemuaPaket(databasePaket, jumlahPaket);
                    break;
                case 2:
                    regController.prosesRegistrasiPelanggan(databasePelanggan, jumlahPelanggan, kapasitasPelanggan);
                    PersistensiData::simpanDataPelanggan(databasePelanggan, jumlahPelanggan, FILE_PELANGGAN);
                    break;
                case 3: {
                    int opsiLevel = 0;
                    std::string uNameInput, passInput;
                    authController.tampilkanFormulirLoginAplikasi();
                    std::cout << "  Pilih Level Otoritas Anda:\n";
                    std::cout << "  [1] Super Admin | [2] Staf Petugas | [3] Member Pelanggan\n";
                    std::cout << "  Pilihan Level (1-3): ";
                    std::cin >> opsiLevel;
                    
                    std::cout << "  Masukkan Username: "; std::cin >> uNameInput;
                    std::cout << "  Masukkan Password: "; std::cin >> passInput;

                    if (opsiLevel == 1) {
                        authController.loginAdmin(uNameInput, passInput, sesiAktif);
                    } else if (opsiLevel == 2) {
                        authController.loginPetugas(uNameInput, passInput, databasePetugas, jumlahPetugas, sesiAktif);
                    } else if (opsiLevel == 3) {
                        authController.loginPelanggan(uNameInput, passInput, databasePelanggan, jumlahPelanggan, sesiAktif);
                    } else {
                        std::cout << "[Otentikasi] Level hak akses tidak dikenali oleh enkripsi sistem!\n";
                    }
                    // Simpan sesi/riwayat login (berhasil maupun gagal) ke file setiap kali ada percobaan login.
                    authController.simpanLogKeFile(FILE_LOGIN_LOG);
                    break;
                }
                case 4:
                    aplikasiBerjalan = false;
                    std::cout << "\n[Info] Terima kasih! Menutup koneksi database dan keluar aplikasi.\n";
                    break;
                default:
                    std::cout << "[Sistem] Peringatan: Pilihan menu di luar batas regulasi!\n";
                    break;
            }
        }
        // JIKA LEVEL YANG AKTIF ADALAH SUPER ADMIN
        else if (sesiAktif.getRoleAktif() == "Admin") {
            int menuAdminPilihan = 0;
            std::cout << "\n=======================================================\n";
            std::cout << "                 PANEL KONTROL SUPER ADMIN              \n";
            std::cout << "         Sesi Aktif: " << sesiAktif.getUsernameAktif() << " [ID: " << sesiAktif.getIdUserAktif() << "]\n";
            std::cout << "=======================================================\n";
            std::cout << "  1. Tambah / Registrasi Akun Staf Petugas Baru\n";
            std::cout << "  2. Lihat Daftar Seluruh Staf Petugas Lapangan\n";
            std::cout << "  3. Lihat Daftar Seluruh Member Pelanggan Travel\n";
            std::cout << "  4. Tambah / Buka Blok Produk Paket Travel Baru\n";
            std::cout << "  5. Periksa Laporan Neraca Keuangan Konsolidasi\n";
            std::cout << "  6. Audit Jejak Digital Jejak Keamanan (Security Log)\n";
            std::cout << "  7. Akhiri Sesi Kerja / Logout Admin\n";
            std::cout << "-------------------------------------------------------\n";
            std::cout << "  Pilihan Eksekusi Admin (1-7): ";
            std::cin >> menuAdminPilihan;

            switch (menuAdminPilihan) {
                case 1:
                    regController.prosesRegistrasiPetugas(databasePetugas, jumlahPetugas, kapasitasPetugas);
                    PersistensiData::simpanDataPetugas(databasePetugas, jumlahPetugas, FILE_PETUGAS);
                    break;
                case 2:
                    viewRenderer.tampilSemuaPetugas(databasePetugas, jumlahPetugas);
                    break;
                case 3:
                    viewRenderer.tampilSemuaPelanggan(databasePelanggan, jumlahPelanggan);
                    break;
                case 4:
                    dataEntryGate.inputPaketBaru(databasePaket, jumlahPaket, kapasitasPaket);
                    break;
                case 5:
                    viewRenderer.taporanKeuangan(akuntansiGlobal);
                    break;
                case 6:
                    viewRenderer.tampilSecurityLog(databaseLog, jumlahLog);
                    break;
                case 7:
                    authController.logoutUser(sesiAktif.getUsernameAktif(), sesiAktif.getRoleAktif());
                    sesiAktif.hancurkanSesiAktif();
                    authController.simpanLogKeFile(FILE_LOGIN_LOG);
                    std::cout << "[Logout] Sesi Super Admin berhasil ditutup dengan aman.\n";
                    break;
                default:
                    std::cout << "[Admin] Peringatan: Pilihan menu salah.\n";
                    break;
            }
        }
        // JIKA LEVEL YANG AKTIF ADALAH STAF PETUGAS
        else if (sesiAktif.getRoleAktif() == "Petugas") {
            int menuPetugasPilihan = 0;
            std::cout << "\n=======================================================\n";
            std::cout << "                PANEL OPERASIONAL STAF PETUGAS          \n";
            std::cout << "         Sesi Aktif: " << sesiAktif.getUsernameAktif() << " [ID: " << sesiAktif.getIdUserAktif() << "]\n";
            std::cout << "=======================================================\n";
            std::cout << "  1. Daftarkan Unit Armada Bus Operasional Baru\n";
            std::cout << "  2. Lihat Rekapitulasi Seluruh Manifes Reservasi Masuk\n";
            std::cout << "  3. Alihkan Kendaraan Bermasalah ke Bengkel Perbaikan\n";
            std::cout << "  4. Akhiri Sesi Tugas / Logout Petugas\n";
            std::cout << "-------------------------------------------------------\n";
            std::cout << "  Pilihan Eksekusi Staf (1-4): ";
            std::cin >> menuPetugasPilihan;

            switch (menuPetugasPilihan) {
                case 1:
                    dataEntryGate.inputArmadaBaru(databaseArmada, jumlahArmada, kapasitasArmada);
                    break;
                case 2:
                    viewRenderer.tampilSemuaReservasi(databaseReservasi, jumlahReservasi);
                    break;
                case 3: {
                    std::string idTargetBus;
                    std::cout << " -> Masukkan ID Bus yang Rusak/Bermasalah: ";
                    std::cin >> idTargetBus;
                    bool busDitemukan = false;
                    for (int i = 0; i < jumlahArmada; i++) {
                        if (databaseArmada[i].getIdBus() == idTargetBus) {
                            databaseArmada[i].kirimKeBengkelPerbaikan();
                            busDitemukan = true;
                            if (jumlahLog < kapasitasLog) {
                                databaseLog[jumlahLog] = SecurityLogger("Bus ID " + idTargetBus + " dikirim ke bengkel perbaikan",
                                                                        "WARNING", sesiAktif.getUsernameAktif());
                                jumlahLog++;
                            }
                            break;
                        }
                    }
                    if (!busDitemukan) std::cout << "[Logistics] Bus dengan ID tersebut tidak ada di inventaris.\n";
                    break;
                }
                case 4:
                    authController.logoutUser(sesiAktif.getUsernameAktif(), sesiAktif.getRoleAktif());
                    sesiAktif.hancurkanSesiAktif();
                    authController.simpanLogKeFile(FILE_LOGIN_LOG);
                    std::cout << "[Logout] Sesi Staf Lapangan telah berakhir secara normal.\n";
                    break;
                default:
                    std::cout << "[Petugas] Peringatan: Menu salah.\n";
                    break;
            }
        }
        // JIKA LEVEL YANG AKTIF ADALAH MEMBER PELANGGAN (CUSTOMER DASHBOARD)
        else if (sesiAktif.getRoleAktif() == "Pelanggan") {
            int menuPelangganPilihan = 0;
            std::cout << "\n=======================================================\n";
            std::cout << "                 DASHBOARD MEMBER PELANGGAN             \n";
            std::cout << "         Selamat Datang Kembali: " << sesiAktif.getUsernameAktif() << "\n";
            std::cout << "=======================================================\n";
            std::cout << "  1. Jelajahi Katalog Paket Wisata Aktif\n";
            std::cout << "  2. Lakukan Booking Reservasi Tiket Travel Baru\n";
            std::cout << "  3. Bayar Tagihan Transaksi Pending (Proses Pelunasan)\n";
            std::cout << "  4. Ajukan Pembatalan Tur / Klaim Refund Dana Kembali\n";
            std::cout << "  5. Akhiri Sesi Belanja / Logout Akun\n";
            std::cout << "-------------------------------------------------------\n";
            std::cout << "  Pilihan Menu Anda (1-5): ";
            std::cin >> menuPelangganPilihan;

            switch (menuPelangganPilihan) {
                case 1:
                    viewRenderer.tampilSemuaPaket(databasePaket, jumlahPaket);
                    break;
                case 2:
                    dataEntryGate.prosesReservasiBaru(databaseReservasi, jumlahReservasi, kapasitasReservasi, 
                                                      sesiAktif.getIdUserAktif(), databasePaket, jumlahPaket);
                    PersistensiData::simpanDataReservasi(databaseReservasi, jumlahReservasi, FILE_RESERVASI);
                    break;
                case 3: {
                    std::string targetIdRes;
                    std::cout << " -> Masukkan ID Booking Reservasi yang Akan Dibayar: ";
                    std::cin >> targetIdRes;
                    bool reservasiDitemukan = false;
                    
                    for (int i = 0; i < jumlahReservasi; i++) {
                        if (databaseReservasi[i].getIdReservasi() == targetIdRes && 
                            databaseReservasi[i].getIdPelanggan() == sesiAktif.getIdUserAktif()) {
                            
                            reservasiDitemukan = true;
                            if (databaseReservasi[i].getStatusReservasi() == "LUNAS") {
                                std::cout << "[Finansial] Transaksi ini sudah berstatus LUNAS sebelumnya.\n";
                            } else {
                                Pembayaran kasMasuk;
                                kasMasuk.inputFormulirPembayaran(targetIdRes, databaseReservasi[i].getTotalHarga());
                                
                                // Mutasi status reservasi menjadi Lunas pasca pembayaran sukses
                                databaseReservasi[i].setStatusReservasi("LUNAS");
                                akuntansiGlobal.tambahPendapatan(databaseReservasi[i].getTotalHarga(),
                                                                  "Pelunasan Reservasi " + targetIdRes);
                                
                                // Cetak Invoice Dokumen Resmi Belanja Pelanggan
                                Invoice notaCetak("INV-2026-" + targetIdRes, targetIdRes, sesiAktif.getUsernameAktif(), 
                                                  databaseReservasi[i].getIdPaket(), databaseReservasi[i].getTotalHarga(), 0.0, "LUNAS");
                                notaCetak.cetakNotaFakturInvoice();
                                PersistensiData::simpanDataReservasi(databaseReservasi, jumlahReservasi, FILE_RESERVASI);
                            }
                            break;
                        }
                    }
                    if (!reservasiDitemukan) std::cout << "[Finansial] ID Booking tidak ditemukan pada riwayat akun Anda.\n";
                    break;
                }
                case 4: {
                    std::string targetRefundId;
                    std::cout << " -> Masukkan ID Booking Reservasi yang Ingin Dibatalkan: ";
                    std::cin >> targetRefundId;
                    bool refundDitemukan = false;

                    for (int i = 0; i < jumlahReservasi; i++) {
                        if (databaseReservasi[i].getIdReservasi() == targetRefundId && 
                            databaseReservasi[i].getIdPelanggan() == sesiAktif.getIdUserAktif()) {
                            
                            refundDitemukan = true;
                            if (databaseReservasi[i].getStatusReservasi() == "REFUNDED") {
                                std::cout << "[Refund] Transaksi telah dibatalkan sebelumnya.\n";
                            } else {
                                double danaAsli = databaseReservasi[i].getTotalHarga();
                                Refund klaimKas;
                                klaimKas.inputFormulirRefund(targetRefundId, danaAsli);
                                
                                databaseReservasi[i].setStatusReservasi("REFUNDED");
                                akuntansiGlobal.tambahRefund(klaimKas.getNominalRefund(),
                                                              "Refund Reservasi " + targetRefundId);
                                klaimKas.cetakNotaPersetujuanRefund();
                                PersistensiData::simpanDataReservasi(databaseReservasi, jumlahReservasi, FILE_RESERVASI);
                            }
                            break;
                        }
                    }
                    if (!refundDitemukan) std::cout << "[Refund] Gagal! ID Booking tidak valid.\n";
                    break;
                }
                case 5:
                    authController.logoutUser(sesiAktif.getUsernameAktif(), sesiAktif.getRoleAktif());
                    sesiAktif.hancurkanSesiAktif();
                    authController.simpanLogKeFile(FILE_LOGIN_LOG);
                    std::cout << "[Logout] Sesi belanja Anda aman. Sampai jumpa kembali!\n";
                    break;
                default:
                    std::cout << "[Pelanggan] Opsi menu tidak tersedia.\n";
                    break;
            }
        }
    }

    // PENYIMPANAN AKHIR SEBAGAI JARING PENGAMAN (SAFETY NET) SEBELUM APLIKASI DITUTUP
    PersistensiData::simpanDataPelanggan(databasePelanggan, jumlahPelanggan, FILE_PELANGGAN);
    PersistensiData::simpanDataPetugas(databasePetugas, jumlahPetugas, FILE_PETUGAS);
    PersistensiData::simpanDataReservasi(databaseReservasi, jumlahReservasi, FILE_RESERVASI);
    authController.simpanLogKeFile(FILE_LOGIN_LOG);

    // DEALOKASI MEMORI DINAMIS (SAFE MEMORY CLEANUP ENGINES)
    delete[] databasePetugas;
    delete[] databasePelanggan;
    delete[] databasePaket;
    delete[] databaseArmada;
    delete[] databaseReservasi;
    delete[] databaseVoucher;
    delete[] databaseLog;

    std::cout << "[Sistem Kernel] Memori dinamis heap dibersihkan seutuhnya. Safe exit code 0.\n";
    return 0;
}
