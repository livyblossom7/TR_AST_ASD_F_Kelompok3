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

/* ================================================================
   >>>>>>>>>>>>>>>>>>  BAGIAN PERSON B  <<<<<<<<<<<<<<<<<<
   Tugas: fungsi updateData() dan hapusData()
   Paste/tulis kode kalian DI DALAM kotak ini saja.
   ================================================================ */

/* ---------------------- UPDATE ---------------------- */
void updateData() {
    char kode[10];
    Obat *node;
 
    printf("\n=== UPDATE DATA OBAT ===\n");
 
    if (head == NULL) {
        printf("Data masih kosong!\n");
        tekanEnter();
        return;
    }
 
    printf("Masukkan kode obat yang mau diupdate: ");
    scanf("%s", kode);
 
    node = cariNodeByKode(kode);
    if (node == NULL) {
        printf("Data dengan kode %s tidak ditemukan!\n", kode);
        tekanEnter();
        return;
    }
 
    printf("\nData lama:\n");
    printf("Kode     : %s\n", node->kode);
    printf("Nama     : %s\n", node->nama);
    printf("Kategori : %s\n", node->kategori);
    printf("Stok     : %d\n", node->stok);
    printf("Harga    : %ld\n", node->harga);
    printf("Expired  : %s\n", node->expired);
 
    printf("\nMasukkan data baru (kode tidak bisa diubah):\n");
    printf("Nama baru               : ");
    scanf("%s", node->nama);
    printf("Kategori baru           : ");
    scanf("%s", node->kategori);
    printf("Stok baru               : ");
    scanf("%d", &node->stok);
    printf("Harga baru              : ");
    scanf("%ld", &node->harga);
    printf("Expired baru (dd-mm-yyyy): ");
    scanf("%s", node->expired);
 
    simpanKeFile();
    printf("\nData berhasil diupdate!\n");
    tekanEnter();
}
 
/* ---------------------- DELETE ---------------------- */
void hapusData() {
    char kode[10];
    Obat *curr, *prev;

    printf("\n=== HAPUS DATA OBAT ===\n");

    if (head == NULL) {
        printf("Data masih kosong!\n");
        tekanEnter();
        return;
    }

    printf("Masukkan kode obat yang mau dihapus: ");
    scanf("%s", kode);

    /* cari node yang kodenya cocok, sambil simpan pointer node sebelumnya */
    curr = head;
    prev = NULL;
    while (curr != NULL && strcmp(curr->kode, kode) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("Data dengan kode %s tidak ditemukan!\n", kode);
        tekanEnter();
        return;
    }

    if (curr == head) {
        /* yang dihapus adalah head (termasuk kalau cuma 1 data) */
        head = head->next;
    } else {
        /* yang dihapus di tengah atau di akhir */
        prev->next = curr->next;
    }

    free(curr);

    simpanKeFile();
    printf("\nData dengan kode %s berhasil dihapus!\n", kode);
    tekanEnter();
}


/* ================================================================
   >>>>>>>>>>>>>>>>>>  BAGIAN PERSON C  <<<<<<<<<<<<<<<<<<
   Tugas: fungsi searchData() (kode/nama/kategori, case-sensitive)
   dan sortData() (ascending saja: nama/harga/stok)
   Paste/tulis kode kalian DI DALAM kotak ini saja.
   ================================================================ */

/* ---------------------- SEARCH ---------------------- */

void searchData() {
    if (head == NULL) {
        printf("\nData masih kosong!\n");
        tekanEnter();
        return;
    }

    int pilihan;
    char keyword[50];
    int ketemu = 0;

    printf("\n=== SEARCH DATA OBAT ===\n");
    printf("1. Cari berdasarkan Kode\n");
    printf("2. Cari berdasarkan Nama\n");
    printf("3. Cari berdasarkan Kategori\n");
    printf("Pilihan: ");
    scanf("%d", &pilihan);
    getchar();

    if (pilihan < 1 || pilihan > 3) {
        printf("\nPilihan tidak valid!\n");
        tekanEnter();
        return;
    }

    printf("\n(Catatan: Pencarian ini case-sensitive ya. 'Obat' beda dengan 'obat')\n");
    printf("Masukkan kata kunci: ");
    scanf(" %49[^\n]", keyword);
    getchar();

    printf("\n=== HASIL PENCARIAN ===\n");

    if (pilihan == 1) {
        Obat *hasil = cariNodeByKode(keyword);
        if (hasil != NULL) {
            printf("- Kode: %s | Nama: %s | Kategori: %s | Stok: %d | Harga: %ld | Exp: %s\n", 
                   hasil->kode, hasil->nama, hasil->kategori, hasil->stok, hasil->harga, hasil->expired);
            ketemu = 1;
        }
    } else {
        Obat *temp = head;
        while (temp != NULL) {
            if ((pilihan == 2 && strstr(temp->nama, keyword) != NULL) ||
                (pilihan == 3 && strstr(temp->kategori, keyword) != NULL)) {
                
                printf("- Kode: %s | Nama: %s | Kategori: %s | Stok: %d | Harga: %ld | Exp: %s\n", 
                       temp->kode, temp->nama, temp->kategori, temp->stok, temp->harga, temp->expired);
                ketemu = 1;
            }
            temp = temp->next;
        }
    }

    if (ketemu == 0) {
        printf("Data tidak ditemukan!\n");
    }
    
    printf("\n");
    tekanEnter();
}

/* ---------------------- SORT ---------------------- */
void sortData() {
    if (head == NULL || head->next == NULL) {
        printf("\nData tidak cukup untuk diurutkan (minimal butuh 2 data).\n");
        tekanEnter();
        return;
    }

    int pilihan;
    printf("\n=== SORT DATA OBAT ===\n");
    printf("Urutkan (Ascending) berdasarkan:\n");
    printf("1. Nama\n");
    printf("2. Harga\n");
    printf("3. Stok\n");
    printf("Pilihan: ");
    scanf("%d", &pilihan);
    getchar();

    if (pilihan < 1 || pilihan > 3) {
        printf("\nPilihan tidak valid!\n");
        tekanEnter();
        return;
    }

    int swapped;
    Obat *ptr1;
    Obat *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            int tukar = 0;

            if (pilihan == 1) {
                if (strcmp(ptr1->nama, ptr1->next->nama) > 0) tukar = 1;
            } else if (pilihan == 2) {
                if (ptr1->harga > ptr1->next->harga) tukar = 1;
            } else if (pilihan == 3) {
                if (ptr1->stok > ptr1->next->stok) tukar = 1;
            }

            if (tukar == 1) {
                // Tukar isi datanya, jangan pointernya (sesuai instruksi)
                char tempKode[10], tempNama[50], tempKategori[30], tempExpired[15];
                int tempStok;
                long tempHarga;

                // Copy dari ptr1 ke temp
                strcpy(tempKode, ptr1->kode);
                strcpy(tempNama, ptr1->nama);
                strcpy(tempKategori, ptr1->kategori);
                tempStok = ptr1->stok;
                tempHarga = ptr1->harga;
                strcpy(tempExpired, ptr1->expired);

                // Copy dari ptr1->next ke ptr1
                strcpy(ptr1->kode, ptr1->next->kode);
                strcpy(ptr1->nama, ptr1->next->nama);
                strcpy(ptr1->kategori, ptr1->next->kategori);
                ptr1->stok = ptr1->next->stok;
                ptr1->harga = ptr1->next->harga;
                strcpy(ptr1->expired, ptr1->next->expired);

                // Copy dari temp ke ptr1->next
                strcpy(ptr1->next->kode, tempKode);
                strcpy(ptr1->next->nama, tempNama);
                strcpy(ptr1->next->kategori, tempKategori);
                ptr1->next->stok = tempStok;
                ptr1->next->harga = tempHarga;
                strcpy(ptr1->next->expired, tempExpired);

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1; // Optimasi bubble sort, bagian akhir udah pasti urut
    } while (swapped);

    printf("\nData berhasil diurutkan!\n");
    simpanKeFile();
    tampilkanData();
}

/* ================================================================
   >>>>>>>>>>>>>>>>>>  BAGIAN PERSON D  <<<<<<<<<<<<<<<<<<
   Tugas: fungsi rataRataHarga(), simpanKeFile(), muatDariFile()
   Paste/tulis kode kalian DI DALAM kotak ini saja.
   ================================================================ */

/* ---------------------- RATA-RATA HARGA ---------------------- */
void rataRataHarga() {
    if (head == NULL) {
        printf("\nData masih kosong!\n");
        tekanEnter();
        return;
    }

    Obat *temp = head;
    long totalHarga = 0;
    long totalNilaiStok = 0;
    int jumlahData = 0;
    float rataRata;

    while (temp != NULL) {
        totalHarga += temp->harga;
        totalNilaiStok += temp->harga * temp->stok;
        jumlahData++;

        temp = temp->next;
    }

    rataRata = (float) totalHarga / jumlahData;

    printf("\n========================================\n");
    printf("         RATA-RATA HARGA OBAT\n");
    printf("========================================\n");
    printf("Jumlah Jenis Obat : %d\n", jumlahData);
    printf("Rata-rata Harga   : Rp %.2f\n", rataRata);
    printf("Total Nilai Stok  : Rp %ld\n", totalNilaiStok);

    tekanEnter();
}

/* ---------------------- FILE HANDLING ---------------------- */
void simpanKeFile() {
    FILE *file = fopen(FILENAME, "w");

    if (file == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    Obat *temp = head;

    while (temp != NULL) {
        fprintf(file, "%s|%s|%s|%d|%ld|%s\n",
                temp->kode,
                temp->nama,
                temp->kategori,
                temp->stok,
                temp->harga,
                temp->expired);

        temp = temp->next;
    }

    fclose(file);
}


void muatDariFile() {
    FILE *file = fopen(FILENAME, "r");

    if (file == NULL) {
        return;
    }

    char baris[200];
    Obat *last = NULL;   /* pointer lokal ke node terakhir yang sudah disambung */

    while (fgets(baris, sizeof(baris), file) != NULL) {

        Obat *temp = (Obat *)malloc(sizeof(Obat));
        temp->next = NULL;

        char *token;

        token = strtok(baris, "|");
        strcpy(temp->kode, token);

        token = strtok(NULL, "|");
        strcpy(temp->nama, token);

        token = strtok(NULL, "|");
        strcpy(temp->kategori, token);

        token = strtok(NULL, "|");
        temp->stok = atoi(token);

        token = strtok(NULL, "|");
        temp->harga = atol(token);

        token = strtok(NULL, "\n");
        strcpy(temp->expired, token);

        if (head == NULL) {
            head = temp;
        } else {
            last->next = temp;
        }
        last = temp;
    }

    fclose(file);
}