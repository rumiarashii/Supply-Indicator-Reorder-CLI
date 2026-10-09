#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::getline;
using std::ifstream;
using std::left;
using std::ofstream;
using std::setprecision;
using std::setw;
using std::string;

typedef int Array[50];
typedef string ArrayString[50];
typedef double ArrayDouble[50];

int hitungDefisit(int stok, int minStok) {
  if (minStok > stok) {
    return minStok - stok;
  } else {
    return 0; // tidak defisit
  }
}

// FIX: parameter hargaPerUnit & return diubah ke double (aslinya int) agar
// nilai tidak terpotong
double nilaiStok(int stok, double hargaPerUnit) { return stok * hargaPerUnit; }

// Pass-by-reference (&n): agar nilai n yang dibaca file ter-update di fungsi
// main
void inputDataFile(int &n, ArrayString namaBarang, ArrayString kodeBarang,
                   Array jumlahStok, Array minStock, Array defisitStok,
                   ArrayDouble hargaPerUnit) {

  string namaFile = "gudang.txt";

  ifstream file(namaFile);

  // FIX: jika file belum ada, buatkan file gudang.txt kosong dengan jumlah 0
  // agar tidak crash
  if (!file.is_open()) {
    ofstream buatFile(namaFile);
    if (buatFile.is_open()) {
      buatFile << "0\n";
      buatFile.close();
      cout << "FILE GUDANG.TXT BELUM ADA, TELAH DIBUAT FILE" << endl;
    } else {
      cout << "GAGAL MEMBUKA FILE!, PASTIKAN FILE GUDANG.TXT ADA" << endl;
    }
    n = 0;
    return;
  }

  file >> n;

  for (int i = 0; i < n; i++) {
    file >> kodeBarang[i] >> namaBarang[i] >> jumlahStok[i] >> minStock[i] >>
        hargaPerUnit[i];
    defisitStok[i] = hitungDefisit(jumlahStok[i], minStock[i]);
  }

  file.close();
  cout << "FILE BERHASIL DIBACA!" << endl;
}

void inputData(int &n, ArrayString namaBarang, ArrayString kodeBarang,
               Array jumlahStok, Array minStock, Array defisitStok,
               ArrayDouble hargaPerUnit) {
  cout << "Masukkan jumlah barang: ";
  cin >> n;
  cin.ignore();

  cout << "\n";

  for (int i = 0; i < n; i++) {
    cout << "\n-----INPUT BARANG " << i + 1 << "-----\n";
    cout << "Masukkan kode barang   : ";
    getline(cin, kodeBarang[i]);
    cout << "Masukkan nama barang   : ";
    getline(cin, namaBarang[i]);
    cout << "Masukkan Jumlah Stok   : ";
    cin >> jumlahStok[i];
    cin.ignore();
    cout << "Masukkan Minimum Stok  : ";
    cin >> minStock[i];
    cin.ignore();
    cout << "Harga Per Unit         : Rp ";
    cin >> hargaPerUnit[i];
    cin.ignore();

    defisitStok[i] = hitungDefisit(jumlahStok[i], minStock[i]);
  }
}

// Algoritma: Bubble Sort (Descending) untuk mengurutkan defisit terbesar ke
// terkecil Karena memakai parallel array, setiap kali posisi ditukar, semua
// array harus ikut ditukar
void sortingData(int n, Array defisit, ArrayString namaBarang,
                 ArrayString kodeBarang, Array jumlahStok, Array minStok,
                 ArrayDouble hargaPerUnit) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (defisit[j + 1] > defisit[j]) {
        string temp = namaBarang[j];
        namaBarang[j] = namaBarang[j + 1];
        namaBarang[j + 1] = temp;

        int defisitTemp = defisit[j];
        defisit[j] = defisit[j + 1];
        defisit[j + 1] = defisitTemp;

        string kodeTemp = kodeBarang[j];
        kodeBarang[j] = kodeBarang[j + 1];
        kodeBarang[j + 1] = kodeTemp;

        int stokTemp = jumlahStok[j];
        jumlahStok[j] = jumlahStok[j + 1];
        jumlahStok[j + 1] = stokTemp;

        int minTemp = minStok[j];
        minStok[j] = minStok[j + 1];
        minStok[j + 1] = minTemp;

        // FIX: hargaTemp diubah ke double (aslinya int) agar tipe data tidak
        // terpotong
        double hargaTemp = hargaPerUnit[j];
        hargaPerUnit[j] = hargaPerUnit[j + 1];
        hargaPerUnit[j + 1] = hargaTemp;
      }
    }
  }
}

// Fungsi export laporan ke file teks (.txt)
void exportKeTxt(int n, Array defisit, ArrayString namaBarang,
                 ArrayString kodeBarang, Array jumlahStok, Array minStok,
                 ArrayDouble hargaPerUnit, double nilaiGudang) {

  ofstream file("laporan_gudang.txt");
  if (!file.is_open()) {
    cout << "Gagal membuat file laporan_gudang.txt!" << endl;
    return;
  }

  file << "============================= LAPORAN GUDANG "
          "============================\n\n";
  file << left << setw(10) << "Kode" << setw(25) << "Nama Barang" << setw(8)
       << "Stok" << setw(8) << "Min" << setw(10) << "Defisit" << setw(20)
       << "Total Nilai"
       << "Status\n";

  for (int i = 0; i < n; i++) {
    double nilaiBarang = nilaiStok(jumlahStok[i], hargaPerUnit[i]);
    string status = (jumlahStok[i] <= minStok[i]) ? "REORDER!" : "AMAN";
    string tanda = (defisit[i] > 0) ? "-" : "+";

    file << left << setw(10) << kodeBarang[i] << setw(25) << namaBarang[i]
         << setw(8) << jumlahStok[i] << setw(8) << minStok[i] << tanda
         << setw(8) << defisit[i] << "Rp " << setw(20) << fixed
         << setprecision(0) << nilaiBarang << status << "\n";
  }

  file << "\nTOTAL VALUASI GUDANG : Rp " << fixed << setprecision(0)
       << nilaiGudang << endl;
  file.close();
  cout << "Laporan berhasil diexport ke 'laporan_gudang.txt'!" << endl;
}

// Fungsi export laporan ke file .csv (bisa dibuka langsung di Excel)
void exportKeCsv(int n, Array defisit, ArrayString namaBarang,
                 ArrayString kodeBarang, Array jumlahStok, Array minStok,
                 ArrayDouble hargaPerUnit, double nilaiGudang) {

  ofstream file("laporan_gudang.csv");
  if (!file.is_open()) {
    cout << "Gagal membuat file laporan_gudang.csv!" << endl;
    return;
  }

  file << "Kode,Nama Barang,Stok,Min Stok,Defisit,Total Nilai,Status\n";

  for (int i = 0; i < n; i++) {
    double nilaiBarang = nilaiStok(jumlahStok[i], hargaPerUnit[i]);
    string status = (jumlahStok[i] <= minStok[i]) ? "REORDER!" : "AMAN";

    file << kodeBarang[i] << ",\"" << namaBarang[i] << "\"," << jumlahStok[i]
         << "," << minStok[i] << "," << defisit[i] << "," << fixed
         << setprecision(0) << nilaiBarang << "," << status << "\n";
  }

  file << ",,,,TOTAL VALUASI," << fixed << setprecision(0) << nilaiGudang
       << ",\n";
  file.close();
  cout << "Laporan berhasil diexport ke 'laporan_gudang.csv' (Siap dibuka di "
          "Excel)!"
       << endl;
}

void cetakData(int n, Array defisit, ArrayString namaBarang,
               ArrayString kodeBarang, Array jumlahStok, Array minStok,
               ArrayDouble hargaPerUnit) {

  sortingData(n, defisit, namaBarang, kodeBarang, jumlahStok, minStok,
              hargaPerUnit);

  cout << "\n============================= LAPORAN GUDANG "
          "============================\n";

  cout << left << setw(10) << "Kode" << setw(25) << "Nama Barang" << setw(8)
       << "Stok" << setw(8) << "Min" << setw(10) << "Defisit" << setw(20)
       << "Total Nilai"
       << "Status\n";

  double nilaiGudang = 0;

  for (int i = 0; i < n; i++) {
    double nilaiBarang = nilaiStok(jumlahStok[i], hargaPerUnit[i]);
    nilaiGudang += nilaiBarang;

    string status = (jumlahStok[i] <= minStok[i]) ? "REORDER!" : "AMAN";
    // FIX: Tanda minus hanya jika defisit > 0 agar tidak menghasilkan -0 saat
    // stok == minStok
    string tanda = (defisit[i] > 0) ? "-" : "+";

    cout << left << setw(10) << kodeBarang[i] << setw(25) << namaBarang[i]
         << setw(8) << jumlahStok[i] << setw(8) << minStok[i] << tanda
         << setw(8) << defisit[i] << "Rp " << setw(20) << fixed
         << setprecision(0) << nilaiBarang << status << "\n";
  }

  cout << "\nTOTAL VALUASI GUDANG : Rp " << fixed << setprecision(0)
       << nilaiGudang << endl;

  // Menu export file di CLI
  int pilihanExport;
  cout << "\nMau export laporan? (0: Tidak, 1: TXT, 2: CSV/Excel): ";
  if (cin >> pilihanExport) {
    if (pilihanExport == 1) {
      exportKeTxt(n, defisit, namaBarang, kodeBarang, jumlahStok, minStok,
                  hargaPerUnit, nilaiGudang);
    } else if (pilihanExport == 2) {
      exportKeCsv(n, defisit, namaBarang, kodeBarang, jumlahStok, minStok,
                  hargaPerUnit, nilaiGudang);
    }
  }
}

int main() {
  int n;

  Array jumlahStok;
  Array minStok;
  Array defisitStok;
  ArrayDouble hargaPerUnit;

  ArrayString kodeBarang;
  ArrayString namaBarang;

  int pilihan;

  while (true) {
    cout << "MAU INPUT OTOMATIS ATAU MANUAL (MALAS) (0/1): ";

    // FIX: Tangani cin.fail() agar tidak terjadi infinite loop jika user
    // memasukkan karakter/huruf
    if (!(cin >> pilihan)) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Input tidak valid! Masukkan angka 0 atau 1." << endl;
      continue;
    }

    if (pilihan == 0) {
      inputDataFile(n, namaBarang, kodeBarang, jumlahStok, minStok, defisitStok,
                    hargaPerUnit);
      break;
    } else if (pilihan == 1) {
      inputData(n, namaBarang, kodeBarang, jumlahStok, minStok, defisitStok,
                hargaPerUnit);
      break;
    }
  }

  if (n > 0) {
    cetakData(n, defisitStok, namaBarang, kodeBarang, jumlahStok, minStok,
              hargaPerUnit);
  }

  return 0;
}