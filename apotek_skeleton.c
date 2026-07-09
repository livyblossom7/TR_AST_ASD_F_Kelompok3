/* ================================================================
   PROGRAM DATA APOTEK
   Fitur:
   - Linked List (Singly Linked List)
   - CRUD (Create, Read, Update, Delete)
   - Sorting (berdasarkan Nama / Harga / Stok, ascending)
   - Searching (berdasarkan Kode / Nama / Kategori, case-sensitive)
   - File Handling (simpan & muat otomatis dari data_apotek.txt)

   ================================================================
   PANDUAN PEMBAGIAN TUGAS — BACA DULU SEBELUM NGODING!
   ================================================================
   Ini adalah file KERANGKA (skeleton). Struct "Obat", prototype
   fungsi, main(), dan fungsi-fungsi bersama (cariNodeByKode,
   bersihkanLayar, tekanEnter, bebaskanMemori) SUDAH JADI dan bisa
   langsung di-compile — TAPI fungsi di dalam kotak banner (Person
   A/B/C/D) masih KOSONG / belum diimplementasikan (isinya cuma
   printf "belum diimplementasikan"). Tugas kalian adalah mengisi
   fungsi di kotak banner masing-masing sampai lengkap.

   Struct "Obat" di bawah adalah PATOKAN BERSAMA — field-nya JANGAN
   diubah sendiri, biar kerjaan semua orang tetap nyambung waktu
   digabung.

   Cara kerja:
   1. Semua orang copy file ini duluan sebagai starting point.
   2. Kerjakan fungsi bagian masing-masing di FILE INI (bukan file
      terpisah), tapi HANYA di dalam kotak banner yang sesuai nama
      kalian di bawah. Jangan sentuh kotak banner milik orang lain.
      Di dalam tiap kotak sudah ada komentar TODO yang jelasin
      langkah-langkah apa yang perlu dikerjakan.
   3. Kalau sudah selesai, kirim/tempel ULANG isi kotak banner kalian
      saja ke satu orang "integrator" yang akan menempelkannya ke
      file master.
   4. Integrator tinggal cari banner yang sesuai nama, lalu ganti isi
      fungsi di dalamnya dengan versi final dari tiap orang, lalu
      compile ulang seluruh file.

   Kotak banner yang ada:
   - PERSON A -> tambahData(), tampilkanData()
   - PERSON B -> updateData(), hapusData()
   - PERSON C -> searchData(), sortData()
   - PERSON D -> simpanKeFile(), muatDariFile(), rataRataHarga()

   (fungsi cariNodeByKode, bersihkanLayar, tekanEnter, bebaskanMemori,
   dan main() adalah fungsi "penghubung" bersama — sudah jadi & jangan
   diubah kecuali disepakati bareng-bareng)
   ================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "data_apotek.txt"

/* ---------------------- STRUCT & GLOBAL ---------------------- */
typedef struct Obat {
    char kode[10];
    char nama[50];
    char kategori[30];
    int stok;
    long harga;
    char expired[15];   /* format: dd-mm-yyyy */
    struct Obat *next;
} Obat;

Obat *head = NULL;

/* ---------------------- PROTOTYPE FUNGSI ---------------------- */
void tambahData();
void tampilkanData();
void updateData();
void hapusData();
void searchData();
void sortData();
void rataRataHarga();
void simpanKeFile();
void muatDariFile();
void bersihkanLayar();
void tekanEnter();
Obat* cariNodeByKode(char kode[]);
void bebaskanMemori();

/* ---------------------- FUNGSI UTAMA (MAIN) ---------------------- */
int main() {
    int pilihan;

    muatDariFile();

    do {
        bersihkanLayar();
        printf("========================================\n");
        printf("               DATA APOTEK\n");
        printf("========================================\n");
        printf("1. Tambah Data\n");
        printf("2. Tampilkan Semua Data\n");
        printf("3. Update Data\n");
        printf("4. Hapus Data\n");
        printf("5. Search Data\n");
        printf("6. Sort Data\n");
        printf("7. Rata-rata Harga Obat\n");
        printf("8. Keluar\n");
        printf("========================================\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);
        getchar(); /* buang newline sisa scanf */

        switch (pilihan) {
            case 1: tambahData(); break;
            case 2: tampilkanData(); break;
            case 3: updateData(); break;
            case 4: hapusData(); break;
            case 5: searchData(); break;
            case 6: sortData(); break;
            case 7: rataRataHarga(); break;
            case 8:
                printf("\nTerima kasih telah menggunakan program Data Apotek!\n");
                break;
            default:
                printf("\nPilihan tidak valid!\n");
                tekanEnter();
        }
    } while (pilihan != 8);

    bebaskanMemori();
    return 0;
}

/* ---------------------- UTILITAS ---------------------- */
void bersihkanLayar() {
    /* Dikosongkan agar riwayat tetap terlihat saat dites di terminal.
       Jika ingin membersihkan layar otomatis, aktifkan salah satu baris di bawah
       sesuai sistem operasi:
       system("cls");   // Windows
       system("clear"); // Linux/Mac
    */
}

void tekanEnter() {
    printf("Tekan ENTER untuk lanjut...");
    getchar();
}

/* Mencari node berdasarkan kode obat, mengembalikan pointer node atau NULL */
Obat* cariNodeByKode(char kode[]) {
    Obat *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->kode, kode) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/* ================================================================
   >>>>>>>>>>>>>>>>>>  BAGIAN PERSON A  <<<<<<<<<<<<<<<<<<
   Tugas: fungsi tambahData() (bisa pilih sisip di awal/tengah/akhir)
   dan tampilkanData()
   Paste/tulis kode kalian DI DALAM kotak ini saja.
   ================================================================ */
void tambahData() {
    char kode[10];
    int posisi;
    char kodeSetelah[10];
    Obat *nodeSetelah = NULL;

    printf("\n=== TAMBAH DATA OBAT ===\n");
    printf("Mau menambahkan data di posisi mana?\n");
    printf("1. Awal\n");
    printf("2. Tengah\n");
    printf("3. Akhir\n");
    printf("Pilihan: ");
    scanf("%d", &posisi);
    getchar();

    if (posisi != 1 && posisi != 2 && posisi != 3) {
        printf("\nPilihan posisi tidak valid!\n");
        tekanEnter();
        return;
    }

    if (posisi == 2) {
        if (head == NULL) {
            printf("\nData masih kosong, tidak bisa menyisipkan di tengah. Data akan ditambahkan sebagai data pertama.\n");
            posisi = 1;
        } else {
            printf("Sisipkan setelah kode obat apa? ");
            scanf("%9s", kodeSetelah);
            getchar();

            nodeSetelah = cariNodeByKode(kodeSetelah);
            if (nodeSetelah == NULL) {
                printf("\nKode obat '%s' tidak ditemukan! Data tidak ditambahkan.\n", kodeSetelah);
                tekanEnter();
                return;
            }
        }
    }

    printf("Kode Obat  : ");
    scanf("%9s", kode);
    getchar();

    if (cariNodeByKode(kode) != NULL) {
        printf("\nKode obat '%s' sudah ada! Data tidak ditambahkan.\n", kode);
        tekanEnter();
        return;
    }

    Obat *baru = (Obat*) malloc(sizeof(Obat));
    if (baru == NULL) {
        printf("Alokasi memori gagal!\n");
        return;
    }

    strcpy(baru->kode, kode);

    printf("Nama Obat  : ");
    scanf(" %49[^\n]", baru->nama);

    printf("Kategori   : ");
    scanf(" %29[^\n]", baru->kategori);

    printf("Stok       : ");
    scanf("%d", &baru->stok);

    printf("Harga      : ");
    scanf("%ld", &baru->harga);

    printf("Expired (dd-mm-yyyy): ");
    scanf(" %14s", baru->expired);

    baru->next = NULL;

    if (posisi == 1) {
        baru->next = head;
        head = baru;
    } else if (posisi == 2) {
        baru->next = nodeSetelah->next;
        nodeSetelah->next = baru;
    } else {
        if (head == NULL) {
            head = baru;
        } else {
            Obat *temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = baru;
        }
    }

    simpanKeFile();
    printf("\nData obat '%s' berhasil ditambahkan!\n", baru->nama);

    tekanEnter();
}

void tampilkanData() {
    printf("\n=== DATA OBAT APOTEK ===\n");
    if (head == NULL) {
        printf("Belum ada data obat.\n");
        tekanEnter();
        return;
    }

    printf("%-8s %-20s %-15s %-6s %-10s %-12s\n",
           "Kode", "Nama", "Kategori", "Stok", "Harga", "Expired");
    printf("--------------------------------------------------------------------\n");

    Obat *temp = head;
    while (temp != NULL) {
        printf("%-8s %-20s %-15s %-6d %-10ld %-12s\n",
               temp->kode, temp->nama, temp->kategori,
               temp->stok, temp->harga, temp->expired);
        temp = temp->next;
    }

    tekanEnter();
}




/* >>>>>>>>>>>>>>>>>> AKHIR BAGIAN PERSON A <<<<<<<<<<<<<<<<<< */


/* ================================================================
   >>>>>>>>>>>>>>>>>>  BAGIAN PERSON B  <<<<<<<<<<<<<<<<<<
   Tugas: fungsi updateData() dan hapusData()
   Paste/tulis kode kalian DI DALAM kotak ini saja.
   ================================================================ */

/* ---------------------- UPDATE ---------------------- */
void updateData() {
    /* ============================================================
       TODO Person B — isi fungsi ini!

       Yang perlu dilakukan:
       1. Minta input kode obat yang mau diupdate
       2. Cari node-nya pakai cariNodeByKode(kode)
       3. Kalau NULL (tidak ketemu), kasih pesan error, tekanEnter(),
          lalu return
       4. Kalau ketemu, tampilkan data lama-nya sebagai referensi
       5. Minta input data baru (nama, kategori, stok, harga, expired)
          dan langsung timpa/overwrite field di node yang ditemukan
       6. Panggil simpanKeFile() supaya perubahan tersimpan ke file
       7. Panggil tekanEnter() di akhir
       ============================================================ */

    printf("\n[updateData belum diimplementasikan]\n");
    tekanEnter();
}

/* ---------------------- DELETE ---------------------- */
void hapusData() {
    /* ============================================================
       TODO Person B — isi fungsi ini!

       Yang perlu dilakukan:
       1. Minta input kode obat yang mau dihapus
       2. Kalau head == NULL, kasih pesan "Data masih kosong!", return
       3. Cari node yang kode-nya cocok, sambil simpan juga pointer
          ke node SEBELUMNYA (butuh 2 variabel pointer: current & prev)
          karena buat hapus node di singly linked list, node sebelumnya
          harus disambungkan ke node SETELAH yang dihapus
       4. Kalau tidak ketemu, kasih pesan error, return
       5. Kalau node yang dihapus adalah head, geser head ke
          node->next. Kalau bukan head, sambungkan prev->next ke
          node->next
       6. free() node yang sudah dihapus
       7. Panggil simpanKeFile() supaya perubahan tersimpan
       8. Panggil tekanEnter() di akhir
       ============================================================ */

    printf("\n[hapusData belum diimplementasikan]\n");
    tekanEnter();
}

/* >>>>>>>>>>>>>>>>>> AKHIR BAGIAN PERSON B <<<<<<<<<<<<<<<<<< */


/* ================================================================
   >>>>>>>>>>>>>>>>>>  BAGIAN PERSON C  <<<<<<<<<<<<<<<<<<
   Tugas: fungsi searchData() (kode/nama/kategori, case-sensitive)
   dan sortData() (ascending saja: nama/harga/stok)
   Paste/tulis kode kalian DI DALAM kotak ini saja.
   ================================================================ */

/* ---------------------- SEARCH ---------------------- */
void searchData() {
    /* ============================================================
       TODO Person C — isi fungsi ini!

       Yang perlu dilakukan:
       1. Tampilkan menu pilihan: 1. Cari berdasarkan Kode
                                    2. Cari berdasarkan Nama
                                    3. Cari berdasarkan Kategori
       2. Kalau head == NULL, kasih pesan "Data masih kosong!", return
       3. Untuk pilihan Kode -> pakai cariNodeByKode(kode)
       4. Untuk pilihan Nama / Kategori -> loop semua node, cek
          pakai strstr(temp->nama, kata) atau strstr(temp->kategori, kategori)
          supaya bisa cari sebagian kata (substring), bukan cuma exact match
       5. PENTING: pencarian ini case-sensitive (jangan pakai tolower()).
          Sebelum user input nama/kategori, tampilkan dulu keterangan
          singkat bahwa "Adi" dan "adi" akan dianggap berbeda karena
          nilai ASCII huruf besar & kecil berbeda
       6. Kalau data ditemukan, tampilkan detailnya. Kalau tidak ada
          satupun yang cocok, tampilkan "Data tidak ditemukan!"
       7. Panggil tekanEnter() di akhir
       ============================================================ */

    printf("\n[searchData belum diimplementasikan]\n");
    tekanEnter();
}

/* ---------------------- SORT ---------------------- */
void sortData() {
    /* ============================================================
       TODO Person C — isi fungsi ini!

       Yang perlu dilakukan:
       1. Kalau data kurang dari 2 (head == NULL || head->next == NULL),
          kasih pesan "Data tidak cukup untuk diurutkan", return
       2. Tampilkan menu pilihan urutkan berdasarkan: 1. Nama
                                                        2. Harga
                                                        3. Stok
          (HANYA ascending / kecil ke besar, tidak perlu tanya
          ascending/descending lagi)
       3. Implementasikan bubble sort di linked list. Karena ini
          singly linked list, cara paling gampang: TUKAR ISI DATA
          antar 2 node yang berdekatan (bukan tukar pointer/alamat),
          pakai variabel temp buat nampung sementara pas nukar
       4. Bandingkan pakai strcmp() untuk nama (kalau hasilnya > 0
          berarti perlu ditukar), atau operator > biasa untuk
          harga/stok (angka)
       5. Setelah selesai sorting, panggil simpanKeFile() dan
          tampilkanData() supaya user langsung lihat hasilnya
       ============================================================ */

    printf("\n[sortData belum diimplementasikan]\n");
    tekanEnter();
}

/* >>>>>>>>>>>>>>>>>> AKHIR BAGIAN PERSON C <<<<<<<<<<<<<<<<<< */


/* ================================================================
   >>>>>>>>>>>>>>>>>>  BAGIAN PERSON D  <<<<<<<<<<<<<<<<<<
   Tugas: fungsi rataRataHarga(), simpanKeFile(), muatDariFile()
   Paste/tulis kode kalian DI DALAM kotak ini saja.
   ================================================================ */

/* ---------------------- RATA-RATA HARGA ---------------------- */
void rataRataHarga() {
    /* ============================================================
       TODO Person D — isi fungsi ini!

       Yang perlu dilakukan:
       1. Kalau head == NULL, kasih pesan "Data masih kosong!", return
       2. Loop semua node dari head, sambil hitung:
          - total harga semua obat (buat rata-rata nanti)
          - total nilai stok = harga * stok, dijumlah semua node
          - jumlah data (counter, ditambah 1 tiap loop)
       3. Rata-rata harga = total harga / jumlah data (ingat cast ke
          float biar hasilnya nggak dibulatkan ke bawah)
       4. Cetak hasilnya: jumlah jenis obat, rata-rata harga, dan
          total nilai stok
       5. Panggil tekanEnter() di akhir
       ============================================================ */

    printf("\n[rataRataHarga belum diimplementasikan]\n");
    tekanEnter();
}

/* ---------------------- FILE HANDLING ---------------------- */
void simpanKeFile() {
    /* ============================================================
       TODO Person D — isi fungsi ini!

       Yang perlu dilakukan:
       1. Buka file "data_apotek.txt" (pakai konstanta FILENAME) mode
          "w" (write, akan menimpa isi file lama)
       2. Kalau fopen() gagal (return NULL), kasih pesan error, return
       3. Loop semua node dari head, tulis tiap data ke file dalam
          satu baris, dipisah pakai karakter '|' sebagai delimiter,
          contoh format: kode|nama|kategori|stok|harga|expired
          (pakai fprintf ke file, bukan printf ke layar)
       4. Tutup file pakai fclose() di akhir

       Catatan: fungsi ini dipanggil otomatis setiap kali ada
       perubahan data (tambah/update/hapus/sort), jadi harus benar-
       benar menulis ULANG seluruh isi list, bukan cuma nambahin baris.
       ============================================================ */

    printf("[simpanKeFile belum diimplementasikan]\n");
}

void muatDariFile() {
    /* ============================================================
       TODO Person D — isi fungsi ini!

       Yang perlu dilakukan:
       1. Buka file "data_apotek.txt" mode "r" (read)
       2. Kalau fopen() gagal (NULL) -> berarti file belum ada
          (pertama kali program dijalankan), cukup return saja,
          jangan dianggap error
       3. Baca file baris per baris pakai fgets()
       4. Tiap baris, pecah pakai strtok() dengan delimiter "|" untuk
          dapatkan tiap field (kode, nama, kategori, stok, harga,
          expired) — ingat stok pakai atoi() dan harga pakai atol()
          karena hasil strtok() itu string, bukan angka
       5. Alokasikan node baru (malloc), isi field-nya dari hasil
          parsing, lalu sambungkan ke akhir linked list (mirip logic
          nyisip di akhir pada tambahData())
       6. Tutup file pakai fclose() setelah semua baris dibaca

       Fungsi ini dipanggil SEKALI di awal main(), sebelum menu
       muncul, supaya data dari sesi sebelumnya otomatis termuat lagi.
       ============================================================ */

    printf("[muatDariFile belum diimplementasikan]\n");
}

/* >>>>>>>>>>>>>>>>>> AKHIR BAGIAN PERSON D <<<<<<<<<<<<<<<<<< */


/* ---------------------- BEBASKAN MEMORI (fungsi bersama) ---------------------- */
void bebaskanMemori() {
    Obat *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
