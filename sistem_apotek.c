#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define FILENAME "data_apotek.txt"

/* ---------------------- STRUCT & GLOBAL ---------------------- */
typedef struct Obat {
    char kode[10];
    char nama[50];
    char kategori[30];
    int stok;
    long harga;
    char expired[15];
    struct Obat *next;
} Obat;

Obat *head = NULL;

/* ---------------------- PROTOTYPE FUNGSI ---------------------- */
void simpanKeFile();

/* ---------------------- UTILITAS ---------------------- */
void bersihkanLayar() {
    system("cls");
}

void tekanEnter() {
    printf("Tekan ENTER untuk melanjutkan...");
    getchar();
}

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
    >>>>>>>>>>>>>>>>>>  HELPER INPUT (VALIDASI / PROTEKSI) <<<<<<<<<<<<<<<<<<
   ================================================================ */

/* Baca input & trim spasi */
void bacaBaris(char *buffer, int ukuran) {
    if (fgets(buffer, ukuran, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        buffer[0] = '\0';
        clearerr(stdin);
    }

    /* trim spasi di akhir */
    int len = (int) strlen(buffer);
    while (len > 0 && isspace((unsigned char) buffer[len - 1])) {
        buffer[--len] = '\0';
    }

    /* trim spasi di awal */
    int awal = 0;
    while (buffer[awal] != '\0' && isspace((unsigned char) buffer[awal])) {
        awal++;
    }
    if (awal > 0) {
        memmove(buffer, buffer + awal, len - awal + 1);
    }
}

/* Validasi angka bulat */
int isAngkaBulat(const char *str) {
    if (str == NULL || str[0] == '\0') return 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char) str[i])) return 0;
    }
    return 1;
}

/* Validasi input angka (int) */
int inputInt(const char *prompt) {
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        bacaBaris(buffer, sizeof(buffer));
        if (isAngkaBulat(buffer)) {
            return (int) strtol(buffer, NULL, 10);
        }
        printf("\nInput harus berupa angka bulat. Coba lagi.\n");
    }
}

/* Validasi input angka (long) */
long inputLong(const char *prompt) {
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        bacaBaris(buffer, sizeof(buffer));
        if (isAngkaBulat(buffer)) {
            return strtol(buffer, NULL, 10);
        }
        printf("\nInput harus berupa angka bulat. Coba lagi.\n");
    }
}

/* Validasi format tanggal */
int isTanggalValid(const char *str) {
    if (strlen(str) != 10) return 0;

    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (str[i] != '-') return 0;
        } else {
            if (!isdigit((unsigned char) str[i])) return 0;
        }
    }

    int hari  = (str[0] - '0') * 10 + (str[1] - '0');
    int bulan = (str[3] - '0') * 10 + (str[4] - '0');

    if (hari < 1 || hari > 31) return 0;
    if (bulan < 1 || bulan > 12) return 0;

    return 1;
}

/* Bandingkan tanggal */
int bandingTanggal(const char *t1, const char *t2) {
    int h1 = (t1[0] - '0') * 10 + (t1[1] - '0');
    int b1 = (t1[3] - '0') * 10 + (t1[4] - '0');
    int y1 = (t1[6] - '0') * 1000 + (t1[7] - '0') * 100 + (t1[8] - '0') * 10 + (t1[9] - '0');

    int h2 = (t2[0] - '0') * 10 + (t2[1] - '0');
    int b2 = (t2[3] - '0') * 10 + (t2[4] - '0');
    int y2 = (t2[6] - '0') * 1000 + (t2[7] - '0') * 100 + (t2[8] - '0') * 10 + (t2[9] - '0');

    if (y1 != y2) return y1 - y2;
    if (b1 != b2) return b1 - b2;
    return h1 - h2;
}

/* Validasi input tanggal */
void inputTanggal(char *dest, int maxLen, const char *prompt) {
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        bacaBaris(buffer, sizeof(buffer));
        if (isTanggalValid(buffer)) {
            strncpy(dest, buffer, maxLen - 1);
            dest[maxLen - 1] = '\0';
            return;
        }
        printf("\nFormat tanggal harus dd-mm-yyyy dan berupa angka (contoh: 25-12-2026). Coba lagi.\n");
    }
}

/* Validasi input teks */
void inputTeks(char *dest, int maxLen, const char *prompt) {
    char buffer[200];
    while (1) {
        printf("%s", prompt);
        bacaBaris(buffer, sizeof(buffer));
        if (strlen(buffer) > 0) {
            strncpy(dest, buffer, maxLen - 1);
            dest[maxLen - 1] = '\0';
            return;
        }
        printf("\nInput tidak boleh kosong. Coba lagi.\n");
    }
}

/* Validasi input kode */
void inputKode(char *dest, int maxLen, const char *prompt) {
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        bacaBaris(buffer, sizeof(buffer));

        if (strlen(buffer) == 0) {
            printf("\nKode tidak boleh kosong. Coba lagi.\n");
            continue;
        }

        strncpy(dest, buffer, maxLen - 1);
        dest[maxLen - 1] = '\0';
        return;
    }
}

/* ================================================================
    >>>>>>>>>>>>>>>>>>  BAGIAN A  <<<<<<<<<<<<<<<<<<
   ================================================================ */
    
/* ---------------------- TAMBAH DATA ---------------------- */
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
    posisi = inputInt("Pilihan: ");

    if (posisi != 1 && posisi != 2 && posisi != 3) {
        printf("\nPilihan tidak valid!\n");
        tekanEnter();
        return;
    }

    if (posisi == 2) {
        if (head == NULL) {
            printf("\nData masih kosong, tidak bisa menyisipkan di tengah. Data akan ditambahkan sebagai data pertama.\n");
            posisi = 1;
        } else {
            inputKode(kodeSetelah, sizeof(kodeSetelah), "Sisipkan setelah kode obat apa? ");

            nodeSetelah = cariNodeByKode(kodeSetelah);
            if (nodeSetelah == NULL) {
                printf("\nKode obat '%s' tidak ditemukan! Data batal ditambahkan.\n", kodeSetelah);
                tekanEnter();
                return;
            }
        }
    }

    inputKode(kode, sizeof(kode), "Kode Obat  : ");

    if (cariNodeByKode(kode) != NULL) {
        printf("\nKode obat '%s' sudah terdaftar! Data batal ditambahkan.\n", kode);
        tekanEnter();
        return;
    }

    Obat *baru = (Obat*) malloc(sizeof(Obat));
    if (baru == NULL) {
        printf("Alokasi memori gagal!\n");
        return;
    }

    strcpy(baru->kode, kode);

    inputTeks(baru->nama, sizeof(baru->nama), "Nama Obat  : ");
    inputTeks(baru->kategori, sizeof(baru->kategori), "Kategori   : ");

    baru->stok  = inputInt("Stok       : ");
    baru->harga = inputLong("Harga      : ");

    inputTanggal(baru->expired, sizeof(baru->expired), "Expired (dd-mm-yyyy): ");

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
    printf("\nData obat berhasil ditambahkan & disimpan ke file!\n");

    tekanEnter();
}

/* ---------------------- TAMPIL DATA ---------------------- */
void tampilkanData() {
    printf("\n=== DATA OBAT APOTEK ===\n");
    printf("=================================================================================\n");
    printf("%-8s %-20s %-20s %-6s %-12s %-12s\n",
           "Kode", "Nama", "Kategori", "Stok", "Harga", "Expired");
    printf("=================================================================================\n");

    if (head == NULL) {
        printf("Belum ada data obat.\n");
    } else {
        Obat *temp = head;
        while (temp != NULL) {
            printf("%-8s %-20s %-20s %-6d %-12ld %-12s\n",
                   temp->kode, temp->nama, temp->kategori,
                   temp->stok, temp->harga, temp->expired);
            temp = temp->next;
        }
    }
    printf("=================================================================================\n");

    tekanEnter();
}

/* ================================================================
    >>>>>>>>>>>>>>>>>>  BAGIAN B  <<<<<<<<<<<<<<<<<<
   ================================================================ */

/* ---------------------- UPDATE DATA---------------------- */
void updateData() {
    char kode[10];
    Obat *node = NULL;

    printf("\n=== UPDATE DATA OBAT ===\n");

    if (head == NULL) {
        printf("Data masih kosong!\n");
        tekanEnter();
        return;
    }

    inputKode(kode, sizeof(kode), "Masukkan kode obat yang mau diupdate: ");

    node = cariNodeByKode(kode);
    if (node == NULL) {
        printf("\nObat dengan kode '%s' tidak ditemukan.\n", kode);
        tekanEnter();
        return;
    }

    /* Menu pilih field mana yang mau diupdate, bukan update seluruh data sekaligus */
    int selesai = 0;
    while (!selesai) {
        printf("\n=== UPDATE DATA OBAT ===\n");
        printf("Data saat ini:\n");
        printf("1. Nama     : %s\n", node->nama);
        printf("2. Kategori : %s\n", node->kategori);
        printf("3. Stok     : %d\n", node->stok);
        printf("4. Harga    : %ld\n", node->harga);
        printf("5. Expired  : %s\n", node->expired);
        printf("6. Selesai & simpan\n");

        int pilihanField = inputInt("Field yang mau diubah: ");

        switch (pilihanField) {
            case 1:
                inputTeks(node->nama, sizeof(node->nama), "Nama baru     : ");
                printf("\nNama berhasil diubah.\n");
                break;
            case 2:
                inputTeks(node->kategori, sizeof(node->kategori), "Kategori baru : ");
                printf("\nKategori berhasil diubah.\n");
                break;
            case 3:
                node->stok = inputInt("Stok baru     : ");
                printf("\nStok berhasil diubah.\n");
                break;
            case 4:
                node->harga = inputLong("Harga baru    : ");
                printf("\nHarga berhasil diubah.\n");
                break;
            case 5:
                inputTanggal(node->expired, sizeof(node->expired), "Expired baru (dd-mm-yyyy): ");
                printf("\nTanggal expired berhasil diubah.\n");
                break;
            case 6:
                selesai = 1;
                break;
            default:
                printf("\nPilihan tidak valid!\n");
        }
    }

    simpanKeFile();
    printf("\nPerubahan berhasil disimpan ke file!\n");
    tekanEnter();
}

/* ---------------------- DELETE DATA ---------------------- */
void hapusData() {
    char kode[10];
    Obat *curr = NULL, *prev = NULL;

    printf("\n=== HAPUS DATA OBAT ===\n");

    if (head == NULL) {
        printf("Data masih kosong!\n");
        tekanEnter();
        return;
    }

    inputKode(kode, sizeof(kode), "Masukkan kode obat yang mau dihapus: ");

    curr = head;
    prev = NULL;
    while (curr != NULL && strcmp(curr->kode, kode) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("\nObat dengan kode '%s' tidak ditemukan.\n", kode);
        tekanEnter();
        return;
    }

    if (curr == head) {
        head = head->next;
    } else {
        prev->next = curr->next;
    }

    free(curr);

    simpanKeFile();
    printf("\nData dengan kode %s berhasil dihapus!\n", kode);
    tekanEnter();
}

/* ================================================================
    >>>>>>>>>>>>>>>>>>  BAGIAN C  <<<<<<<<<<<<<<<<<<
   ================================================================ */

/* ---------------------- SEARCH DATA ---------------------- */
void searchData() {
    if (head == NULL) {
        printf("\nData masih kosong!\n");
        tekanEnter();
        return;
    }

    printf("\n=== CARI DATA OBAT ===\n");
    printf("1. Berdasarkan Kode\n");
    printf("2. Berdasarkan Nama\n");
    printf("3. Berdasarkan Kategori\n");

    int pilihan = inputInt("Pilih jenis pencarian: ");

    if (pilihan < 1 || pilihan > 3) {
        printf("\nPilihan tidak valid!\n");
        tekanEnter();
        return;
    }

    char keyword[50];

    if (pilihan == 1) {
        inputTeks(keyword, sizeof(keyword), "Masukkan kode obat: ");
    } else if (pilihan == 2) {
        printf("\n(Catatan: Pencarian ini case-sensitive. Contoh: \"Obat\" berbeda dengan \"obat\",\n");
        printf("karena dalam ASCII huruf kapital dan non-kapital itu beda.)\n");
        inputTeks(keyword, sizeof(keyword), "Masukkan kata kunci nama: ");
    } else {
        printf("\n(Catatan: Pencarian ini case-sensitive. Contoh: \"Obat\" berbeda dengan \"obat\",\n");
        printf("karena dalam ASCII huruf kapital dan non-kapital itu beda.)\n");
        inputTeks(keyword, sizeof(keyword), "Masukkan kata kunci kategori: ");
    }

    printf("\n=== HASIL PENCARIAN ===\n");

    int ketemu = 0;

    if (pilihan == 1) {
        Obat *hasil = cariNodeByKode(keyword);
        if (hasil != NULL) {
            printf("-------------------------------\n");
            printf("Kode      : %s\n", hasil->kode);
            printf("Nama      : %s\n", hasil->nama);
            printf("Kategori  : %s\n", hasil->kategori);
            printf("Stok      : %d\n", hasil->stok);
            printf("Harga     : %ld\n", hasil->harga);
            printf("Expired   : %s\n", hasil->expired);
            printf("-------------------------------\n");
            ketemu = 1;
        }
    } else {
        Obat *temp = head;
        while (temp != NULL) {
            if ((pilihan == 2 && strstr(temp->nama, keyword) != NULL) ||
                (pilihan == 3 && strstr(temp->kategori, keyword) != NULL)) {

                printf("-------------------------------\n");
                printf("Kode      : %s\n", temp->kode);
                printf("Nama      : %s\n", temp->nama);
                printf("Kategori  : %s\n", temp->kategori);
                printf("Stok      : %d\n", temp->stok);
                printf("Harga     : %ld\n", temp->harga);
                printf("Expired   : %s\n", temp->expired);
                printf("-------------------------------\n");
                ketemu = 1;
            }
            temp = temp->next;
        }
    }

    if (ketemu == 0) {
        if (pilihan == 1) {
            printf("Tidak ada obat dengan kode \"%s\".\n\n", keyword);
        } else if (pilihan == 2) {
            printf("Tidak ada nama yang mengandung \"%s\".\n\n", keyword);
        } else {
            printf("Tidak ada kategori yang mengandung \"%s\".\n\n", keyword);
        }
        tekanEnter();
    } else {
        printf("\n");
        tekanEnter();
    }
}

/* ---------------------- SORT DATA ---------------------- */
void sortData() {
    if (head == NULL || head->next == NULL) {
        printf("\nData tidak cukup untuk diurutkan (minimal butuh 2 data).\n");
        tekanEnter();
        return;
    }

    printf("\n=== SORT DATA OBAT ===\n");
    printf("Urutkan berdasarkan:\n");
    printf("1. Kode\n");
    printf("2. Nama (A-Z)\n");
    printf("3. Harga\n");

    int pilihan = inputInt("Pilihan: ");

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
                if (strcmp(ptr1->kode, ptr1->next->kode) > 0) tukar = 1;
            } else if (pilihan == 2) {
                if (strcmp(ptr1->nama, ptr1->next->nama) > 0) tukar = 1;
            } else if (pilihan == 3) {
                if (ptr1->harga > ptr1->next->harga) tukar = 1;
            }

            if (tukar == 1) {
                char tempKode[10], tempNama[50], tempKategori[30], tempExpired[15];
                int tempStok;
                long tempHarga;

                strcpy(tempKode, ptr1->kode);
                strcpy(tempNama, ptr1->nama);
                strcpy(tempKategori, ptr1->kategori);
                tempStok = ptr1->stok;
                tempHarga = ptr1->harga;
                strcpy(tempExpired, ptr1->expired);

                strcpy(ptr1->kode, ptr1->next->kode);
                strcpy(ptr1->nama, ptr1->next->nama);
                strcpy(ptr1->kategori, ptr1->next->kategori);
                ptr1->stok = ptr1->next->stok;
                ptr1->harga = ptr1->next->harga;
                strcpy(ptr1->expired, ptr1->next->expired);

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
        lptr = ptr1;
    } while (swapped);

    printf("\nData berhasil diurutkan!\n");
    simpanKeFile();
    tampilkanData();
}

/* ================================================================
    >>>>>>>>>>>>>>>>>>  BAGIAN D  <<<<<<<<<<<<<<<<<<
   ================================================================ */

/* ---------------------- RATA-RATA OBAT ---------------------- */
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
    printf("========================================\n");

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
    Obat *last = NULL;

    while (fgets(baris, sizeof(baris), file) != NULL) {

        Obat *temp = (Obat *) malloc(sizeof(Obat));
        temp->next = NULL;

        char *token;

        token = strtok(baris, "|");
        if (token == NULL) { free(temp); continue; }
        strcpy(temp->kode, token);

        token = strtok(NULL, "|");
        if (token == NULL) { free(temp); continue; }
        strcpy(temp->nama, token);

        token = strtok(NULL, "|");
        if (token == NULL) { free(temp); continue; }
        strcpy(temp->kategori, token);

        token = strtok(NULL, "|");
        if (token == NULL) { free(temp); continue; }
        temp->stok = atoi(token);

        token = strtok(NULL, "|");
        if (token == NULL) { free(temp); continue; }
        temp->harga = atol(token);

        token = strtok(NULL, "\n");
        if (token == NULL) { free(temp); continue; }
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

/* ---------------------- BEBASKAN MEMORI (fungsi bersama) ---------------------- */
void bebaskanMemori() {
    Obat *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

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
        printf("7. Rata-Rata Obat\n");
        printf("8. Keluar\n");
        printf("========================================\n");

        pilihan = inputInt("Pilihan: ");

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