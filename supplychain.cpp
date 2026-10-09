#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>


using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;
using std::fixed;
using std::setprecision;
using std::ifstream;
using std::left;
using std::setw;

typedef int Array[50];
typedef string ArrayString[50];
typedef double ArrayDouble[50];


int hitungDefisit(int stok, int minStok){
    if (minStok > stok){
        return minStok - stok;
    } else {
        return 0; //tidak defisit
    }
}

int nilaiStok(int stok, int hargaPerUnit){
    return stok * hargaPerUnit;
}

void inputDataFile(int &n, ArrayString namaBarang, ArrayString kodeBarang, Array jumlahStok
    , Array minStock, Array defisitStok, ArrayDouble hargaPerUnit){
    
    string namaFile = "gudang.txt";

    ifstream file(namaFile);

    if (!file.is_open()){
        cout << "GAGAL MEMBUKA FILE!, PASTIKAN FILE GUDANG.TXT ADA" << endl;
        n = 0;
        return;
    } 

    file >> n;

    for (int i = 0; i < n; i++){
        file >> kodeBarang[i] >> namaBarang[i] >> jumlahStok[i] >> minStock[i] >> hargaPerUnit[i];
        defisitStok[i] = hitungDefisit(jumlahStok[i], minStock[i]);
    }

    file.close();
    cout << "FILE BERHASIL DIBACA!" << endl;
}

void inputData(int &n, ArrayString namaBarang, ArrayString kodeBarang, Array jumlahStok, Array minStock
    , Array defisitStok, ArrayDouble hargaPerUnit){
    cout << "Masukkan jumlah barang: "; cin >> n;
    cin.ignore();

    cout << "\n";

    for (int i = 0; i < n; i++){
        cout << "\n-----INPUT BARANG " << i + 1 << "-----\n";
        cout << "Masukkan kode barang   : "; getline(cin, kodeBarang[i]);
        cout << "Masukkan nama barang   : "; getline(cin, namaBarang[i]);
        cout << "Masukkan Jumlah Stok   : "; cin >> jumlahStok[i]; cin.ignore();
        cout << "Masukkan Minimum Stok  : "; cin >> minStock[i]; cin.ignore();
        cout << "Harga Per Unit         : Rp "; cin >> hargaPerUnit[i]; cin.ignore();

        defisitStok[i] = hitungDefisit(jumlahStok[i], minStock[i]);
    }
}

void sortingData(int n, Array defisit,  ArrayString namaBarang, ArrayString kodeBarang,
     Array jumlahStok, Array minStok, ArrayDouble hargaPerUnit){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n - i - 1; j++){
            if (defisit[j + 1] > defisit[j]){
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
                jumlahStok[j] = jumlahStok[j+1];
                jumlahStok[j + 1] = stokTemp;

                int minTemp = minStok[j];
                minStok[j] = minStok[j + 1];
                minStok[j + 1] = minTemp;

                int hargaTemp = hargaPerUnit[j];
                hargaPerUnit[j] = hargaPerUnit[j + 1];
                hargaPerUnit[j + 1] = hargaTemp;
            }
        }
    }
}


void cetakData(int n, Array defisit, ArrayString namaBarang, ArrayString kodeBarang, 
    Array jumlahStok, Array minStok, ArrayDouble hargaPerUnit){

    sortingData(n, defisit, namaBarang, kodeBarang, jumlahStok, minStok, hargaPerUnit);

    cout << "\n====LAPORAN GUDANG====\n";

    cout << left
         << setw(10) << "Kode"
         << setw(25) << "Nama Barang"
         << setw(8) << "Stok"
         << setw(8) << "Min"
         << setw(10) << "Defisit"
         << setw(20) << "Total Nilai"
         << "Status\n";

    double nilaiGudang = 0;

    for (int i = 0; i < n; i++){
        double nilaiBarang = nilaiStok(jumlahStok[i], hargaPerUnit[i]);
        nilaiGudang += nilaiBarang;

        string status = (jumlahStok[i] <= minStok[i]) ? "REORDER!" : "AMAN";
        string tanda = (jumlahStok[i] <= minStok[i]) ? "-" : "+";

        
        
        cout << left
             << setw(10) << kodeBarang[i]
             << setw(25) << namaBarang[i]
             << setw(8) << jumlahStok[i] 
             << setw(8) << minStok[i]
             << tanda << setw(8) << defisit[i]
             << "Rp " << setw(20) << fixed << setprecision(0) <<  nilaiBarang
             << status << "\n";
    }

    cout << "\nTOTAL VALUASI GUDANG : Rp " << fixed << setprecision(0) <<  nilaiGudang << endl;
}


int main(){
    int n;
    
    Array jumlahStok;
    Array minStok;
    Array defisitStok;
    ArrayDouble hargaPerUnit;

    ArrayString kodeBarang;
    ArrayString namaBarang;

    
    

    int pilihan;


    while(true){
        cout << "MAU INPUT OTOMATIS ATAU MANUAL (MALAS) (0/1): "; cin >> pilihan;

        if (pilihan == 0){
            inputDataFile(n, namaBarang, kodeBarang, jumlahStok, minStok, defisitStok, hargaPerUnit);
            break;
        } else if(pilihan == 1) {
            inputData(n, namaBarang, kodeBarang, jumlahStok, minStok, defisitStok, hargaPerUnit);
            break;
        } 
    }

    cetakData(n, defisitStok, namaBarang, kodeBarang, jumlahStok, minStok, hargaPerUnit);
   
    

    
    
}