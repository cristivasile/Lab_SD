#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <algorithm>
#include <chrono>

using namespace std;

//folosite pentru a afla timpul in milisecunde. Sursa: https://www.delftstack.com/howto/cpp/how-to-get-time-in-milliseconds-cpp/
using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;
// 

unsigned long arr[100000000], auxiliary[100000001], copie[100000000];

//Toate functiile care nu tin strict de generare sau sortare sunt declarate dedesubt si scrise dupa main.

//Functie care genereaza un numar unsigned long aleator
unsigned long llrand(); //Sursa: https://stackoverflow.com/questions/28115724/getting-big-random-numbers-in-c-c
//Functie care returneaza timpul in milisecunde.
long long get_time(); //Sursa: https://www.delftstack.com/howto/cpp/how-to-get-time-in-milliseconds-cpp/

bool test_sort(unsigned long v[], int lungime);  //Functie care verifica daca array-ul este sortat. lungime = p, unde 10^p este lungimea array-ului.
void afis(unsigned long v[], int lungime);  //Functie care afiseaza un array, folosita pentru debugging. lungime = p, unde 10^p este lungimea array-ului.
int ordin(unsigned long x);                // returneaza ordinul in baza 10 a unui numar natural
int ordin(unsigned long x, int baza);                // functia ordin, dar cu baza precizata
unsigned long maxim_arr(unsigned long v[], int lungime);  //functie care returneaza maximul dintr-un array 
void copy(unsigned long v[], unsigned long w[], int lungime); //functie de copiere a unui array V intr-un array W
void empty(unsigned long v[], int lungime); //functie de golire (inlocuirea cu 0 a tuturor elementelor) a unui array, folosita pentru golirea array-ului auxiliar inainte de 
float to_sec(long long start, long long finish); //transforma unitatile de timp din milisecunde in secunde

//genereaza un array constant (cu o valoare random)
void generare_constant(unsigned long v[], int lungime, int maxim) { //lungime = p, unde 10^p este lungimea array-ului. La fel pentru maxim.
	lungime = pow(10, lungime);
	maxim = pow(10, maxim);
	v[0] = llrand() % maxim;
	for (int i = 1; i < lungime; i++) {
		v[i] = v[0];
	}
}

//genereaza un array cu valori aleatoare
void generare_random(unsigned long v[], int lungime, int maxim) { //lungime = p, unde 10^p este lungimea array-ului. La fel pentru maxim.

	lungime = pow(10, lungime);
	maxim = pow(10, maxim);
	for (int i = 0; i < lungime; i++) {
		v[i] = llrand() % maxim;
	}
}

//genereaza un array strict crescator (se calculeaza o ratie, putere a lui 10, si se adauga inca un numar aleator de o cifra in cazul in care ratia este mai mare ca 10)
void generare_crescator(unsigned long v[], int lungime, int maxim) {//lungime = p, unde 10^p este lungimea array-ului. La fel pentru maxim.
	lungime = pow(10, lungime);
	maxim = pow(10, maxim);
	int ratie = maxim / lungime;
	bool OK = 0;
	if (ratie > 1) {
		OK = 1;
	}
	v[0] = 0;
	for (int i = 1; i < lungime; i++) {
		v[i] = (long long)ratie * i + (long long)rand()%10*OK;
	}
}

//genereaza un array strict descrescator (se calculeaza o ratie, putere a lui 10, si se aduna inca un numar aleator de o cifra in cazul in care ratia este mai mare ca 10)
void generare_descrescator(unsigned long v[], int lungime, int maxim) {//lungime = p, unde 10^p este lungimea array-ului. La fel pentru maxim.
	lungime = pow(10, lungime);
	maxim = pow(10, maxim);
	int ratie = maxim / lungime;
	bool OK = 0;
	if (ratie > 1) {
		OK = 1;
	}
	v[lungime] = 0;
	v[0] = lungime-1;
	for (int i = 1; i < lungime-1; i++) {
		v[i] = (long long)ratie * ((long long)lungime-i-1) + (long long)rand() % 10 * OK;
	}
}

void bubble_sort(unsigned long v[], int lungime) { //lungime = p, unde 10^p este lungimea array-ului.
	bool OK =1;

	lungime = pow(10, lungime);
	for (int i = 0; i < lungime - 1 && OK; i++)
	{
		OK = 0;												//variabila OK contorizeaza daca s-a facut vreo interschimbare in aceasta parcurgere. Daca nu, array-ul este sortat si se poate iesi din sortare.
		for (int j = 0; j < lungime - i - 1; j++) {
			if (v[j + 1] < v[j]) {
				swap(v[j], v[j + 1]);
				OK = 1;
			}
		}
	}
}

bool counting_sort(unsigned long v[], int lungime, int maxim) {//lungime = p, unde 10^p este lungimea array-ului. La fel pentru maxim.
	if (maxim > 8) {									//se trateaza cazul in care nu avem destui indici in array-ul de frecventa, returnand 0.
		return 0;
	}
	
	lungime = pow(10, lungime);
	maxim = pow(10, maxim);
	
	for (int i = 0; i < lungime; i++) {
		auxiliary[v[i]] ++;
	}
	int p = 0;
	for (int i = 0; i < maxim; i++) 
		for (int j = 0; j < auxiliary[i]; j++){
			v[p++] = i;
	}
	return 1;
}

void radix_sort_baza(unsigned long v[], int lungime, int ordin, int baza) { //lungimea este p, unde 10^p este lungimea array-ului.

	lungime = pow(10, lungime);
	unsigned long ord = 1;
	for (int i = 0; i < ordin; i++) {
		int p = 0;

		for (int j = 0; j < baza; j++) {
			for (int k = 0; k < lungime; k++) {
				if ((v[k] / ord) % baza == j) {
					auxiliary[p++] = v[k];
				}
			}
		}
		for (int j = 0; j < lungime; j++) {	//se copiaza array-ul sortat dupa unitatea curenta
			v[j] = auxiliary[j];
		}
		ord *= baza;		//se mareste unitatea dupa care sortam
		}
	}

void radix_sort_biti(unsigned long v[], int lungime, int ordin) { //lungimea este p, unde 10^p este lungimea array-ului.
	
	lungime = pow(10, lungime);
	int shift = 0;
	unsigned long mask;
	
	for (int i = 0; i < ordin; i++) {
		int p = 0;
		mask = 1 << shift; //se creeaza o masca pentru a determina bitul de pe pozitia pe care suntem

		for (int j = 0; j < lungime; j++) {
			if ((v[j]&mask)== 0)	// se verifica daca bitul curent este 0
				auxiliary[p++] = v[j];
		}

		for (int j = 0; j < lungime; j++) {
			if ((v[j]&mask) != 0)	// se verifica daca bitul curent este 1, caz in care v[j]&mask va avea forma 0b0...(x ori)...010....(y ori)...0
				auxiliary[p++] = v[j];
		}


		for (int j = 0; j < lungime; j++) {	//se copiaza array-ul sortat dupa bitul curent
			v[j] = auxiliary[j];
		}
		shift++;		//se trece la urmatorul bit
	}
}

void merge_sort(unsigned long v[], int primul, int ultimul) { //spre deosebire de celelalte sortari, ultimul index este inclus (deci se apeleaza cu 10^p-1)
	if (primul < ultimul) {
		int mijloc = (primul + ultimul) / 2;
		merge_sort(v, primul, mijloc);
		merge_sort(v, mijloc + 1, ultimul);
		int index = 0;
		int i1 = primul;
		int i2 = mijloc+1;
		while (i1 <= mijloc && i2 <= ultimul) {  //interclasare
			if (v[i1] < v[i2]) {
				auxiliary[index++] = v[i1++];
			}
			else if (v[i2] < v[i1]) {
				auxiliary[index++] = v[i2++];
			}
			else { auxiliary[index++] = v[i1++]; }
		}
		while (i1 <= mijloc)					//completare la final
			auxiliary[index++] = v[i1++];
		while (i2 <= ultimul)
			auxiliary[index++] = v[i2++];

		index = 0;
		for (int i = primul; i <= ultimul; i++)		//mutare inapoi in array
			v[i] = auxiliary[index++];
	}
}

//Aveam probleme de stackoverflow asa ca am folosit o metoda de loop simulat fara recursie. Sursa: https://stackoverflow.com/questions/22285951/c-getting-stackoverflow-error-in-quicksort-function
void quick_sort(unsigned long v[], int primul, int ultimul) { //spre deosebire de celelalte sortari, ultimul index este inclus (deci se apeleaza cu 10^p-1)
	unsigned long pivot;
	int i, j, mijloc;

	TailRecurse:
	if (primul < ultimul) {
		mijloc = (primul + ultimul) / 2;

		if ((v[primul]<v[ultimul] && v[primul]>v[mijloc]) || (v[primul] > v[ultimul] && v[primul] < v[mijloc])) //se calculeaza mediana din 3 si se muta pe ultima pozitie pentru a alege pivotul
			swap(v[primul], v[ultimul]);
		else if ((v[mijloc]<v[ultimul] && v[mijloc]>v[primul]) || (v[mijloc] > v[ultimul] && v[mijloc] < v[primul]))
			swap(v[mijloc], v[ultimul]);

		pivot = v[ultimul];

		i = primul - 1;
		for (j = primul; j < ultimul; j++) {  //se efectueaza algoritmul de "partitionare"
			if (v[j] < pivot) {
				i++;
				swap(v[i], v[j]);
			}
		}
		swap(v[i + 1], v[ultimul]);
		

		if (i + 1 < mijloc) {	// se apeleaza recursiv pentru divizunea mai mica si se simuleaza un loop pentru divizunea mai mare
			quick_sort(v, primul, i);  
			primul = i + 2; goto TailRecurse;
		}
		else {
			quick_sort(v, i + 2, ultimul);
			ultimul = i; goto TailRecurse;
		}
	}
}

//varianta alternativa de quicksort, care verifica daca array-ul este deja sortat la fiecare parcurgere (reduce timpul de executare cand avem numere cu frecventa ridicata)
void quick_sort_verificare(unsigned long v[], int primul, int ultimul); 

//Functie care ruleaza toate (sau aproape toate) sortarile pe un vector si afiseaza timpii de rulare.
void rulare(unsigned long v[], int lungime, int maxim, int quick = 0) { //Variabila quick este folosita pentru a ocoli quicksortul in anumite cazuri

	long long start, finish;
	int order;
	float timp_intermediar;
	unsigned long max;

	cout << ": N=10^" << lungime << " MAX=10^" << maxim<<"\n";

	if(maxim<=8)
	empty(auxiliary, maxim);	//daca numarul maxim este mai mic decat 10^8 (se poate rula counting sort) atunci se curata vectorul auxiliar

	copy(v, copie, lungime);
	cout << "C++ STL sort: ";
	long n = pow(10, lungime);
	start = get_time();
	sort(copie, copie+n);
	finish = get_time();
	if (test_sort(copie, lungime) != 1)
		cout << "ESUAT";
	else cout <<"REUSIT!, durata:"<< to_sec(start, finish) << " sec.\n";

	copy(v, copie, lungime);
	cout << "Bubble sort: ";
	if ((lungime > 5 || (lungime == 5 && lungime< maxim)) && quick > 2) {
		cout << "IGNORAT. (timpul de executare ar fi foarte mare)\n";
	}
	else {
		start = get_time();
		bubble_sort(copie, lungime);
		finish = get_time();
		if (test_sort(copie, lungime) != 1)
			cout << "ESUAT";
		else cout << "REUSIT!, durata:" << to_sec(start, finish) << " sec.\n";
	}

	copy(v, copie, lungime);
	cout << "Counting sort: ";
	start = get_time();
	bool OK=counting_sort(copie, lungime, maxim);
	finish = get_time();
	if (OK && test_sort(copie, lungime)) {
		cout << "REUSIT!, durata:" << to_sec(start, finish) << " sec.\n";
	}
	else if (OK == 0) {
		cout << "ESUAT! (numere prea mari)\n";
	}
	else cout << "ESUAT!";

	copy(v, copie, lungime);
	cout << "Radix sort in baza 8: ";

	start = get_time();
	order = ordin(maxim_arr(v, lungime), 8);
	radix_sort_baza(copie, lungime, order,8);
	finish = get_time();
	if (test_sort(copie, lungime) != 1)
		cout << "ESUAT";
	else cout << "REUSIT!, durata:" << to_sec(start, finish) << " sec.\n";

	copy(v, copie, lungime);
	cout << "Radix sort in baza 2: ";

	start = get_time();
	order = ordin(maxim_arr(v, lungime), 2);
	radix_sort_baza(copie, lungime, order, 2);
	finish = get_time();
	if (test_sort(copie, lungime) != 1)
		cout << "ESUAT";
	else cout << "REUSIT!, durata:" << to_sec(start, finish) << " sec.\n";

	copy(v, copie, lungime);
	cout << "Radix sort (op pe biti): ";

	start = get_time(); 
	order = ordin(maxim_arr(v,lungime), 2);
	radix_sort_biti(copie, lungime,order);
	finish = get_time();
	if (test_sort(copie, lungime) != 1)
		cout << "ESUAT";
	else cout << "REUSIT!, durata:" << to_sec(start, finish) << " sec.\n";

	copy(v, copie, lungime);
	cout << "Merge sort: ";
	start = get_time();
	merge_sort(copie, 0, pow(10, lungime)-1);
	finish = get_time();
	if (test_sort(copie, lungime) != 1)
		cout << "ESUAT";
	else cout << "REUSIT!, durata:" << to_sec(start, finish) << " sec.\n";

	copy(v, copie, lungime);
	cout << "Quick sort: ";
	if ((quick == 1 && lungime > 5) || (quick == 4 &&lungime>5&&lungime>maxim&&lungime-maxim>=3)) {
		cout << "IGNORAT. (timpul de executare ar fi foarte mare)\n";
	}
	else {
		start = get_time();
		quick_sort(copie, 0, pow(10, lungime) - 1);
		finish = get_time();
		if (test_sort(copie, lungime) != 1)
			cout << "ESUAT!\n";
		else cout << "REUSIT!, durata:" << to_sec(start, finish) << " sec.\n";
	}

	copy(v, copie, lungime);
	cout << "Quick sort cu verificare: ";
	start = get_time();
	quick_sort_verificare(copie, 0, pow(10, lungime) - 1);
	finish = get_time();
	if (test_sort(copie, lungime) != 1)
		cout << "ESUAT";
	else cout << "REUSIT!, durata:" << to_sec(start, finish) << " sec.\n";


}

//Functie de generare a mai multor vectori cu marimi predefinite.
void run_constant() {

	cout << "\nSe ruleaza pentru numere constante:\n";
	int val[4][2] = { {3, 8}, {5,2}, {8, 3}, {8, 9} };
	for (int i = 0; i < 4; i++) {
		cout << "\nTest #" << i+1;
		empty(arr, val[i][0]);
		generare_constant(arr, val[i][0], val[i][1]);
		rulare(arr, val[i][0], val[i][1], 1);
	}
	cout << "\n\n\n\n";
}

//Functie de generare a mai multor vectori cu marimi predefinite.
void run_crescator() {

	cout << "\nSe ruleaza pentru numere crescatoare:\n";
	int val[5][2] = { {4,4}, {4, 9}, {7, 9}};
	for (int i = 0; i < 3; i++) {
		cout << "\nTest #" << i + 1;
		empty(arr, val[i][0]);
		generare_crescator(arr, val[i][0], val[i][1]);
		rulare(arr, val[i][0], val[i][1],2);
	}
	cout << "\n\n\n\n";
}

//Functie de generare a mai multor vectori cu marimi predefinite.
void run_descrescator() {

	cout << "\nSe ruleaza pentru numere descrescatoare:\n";
	int val[5][2] = { {4,4}, {4, 9}, {7, 9} };
	for (int i = 0; i < 3; i++) {
		cout << "\nTest #" << i + 1;
		empty(arr, val[i][0]);
		generare_descrescator(arr, val[i][0], val[i][1]);
		rulare(arr, val[i][0], val[i][1],3);
	}
	cout << "\n\n\n\n";

}

//Functie de generare a mai multor vectori cu marimi predefinite.
void run_random() {

	cout << "\nSe ruleaza pentru numere aleatoare:\n";
	int val[5][2] = { {5,2}, {4, 8}, {6,8}, {6, 9}, {7, 9} };
	for (int i = 0; i < 5; i++) {
		cout << "\nTest #" << i + 1;
		empty(arr, val[i][0]);
		generare_descrescator(arr, val[i][0], val[i][1]);
		rulare(arr, val[i][0], val[i][1], 4);
	}
	cout << "\n\n\n\n";
}

//Meniul de rulare al programului.
void meniu();

int main()
{
srand(time(NULL));
meniu();
return 0;
}

//Sursa: https://stackoverflow.com/questions/28115724/getting-big-random-numbers-in-c-c
unsigned long llrand() {
	unsigned long long r = 0;

	for (int i = 0; i < 5; ++i) {
		r = (unsigned long)(r << 15) | (unsigned long)(rand() & 0x7FFF);
	}

	return (unsigned long)r & 0xFFFFFFFFFFFFFFFFULL;
}

//Sursa: https://www.delftstack.com/howto/cpp/how-to-get-time-in-milliseconds-cpp/
long long get_time() {
	auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	return millisec_since_epoch;
}

bool test_sort(unsigned long v[], int lungime) {
	lungime = pow(10, lungime);
	for (int i = 1; i < lungime; i++) {
		if (v[i] < v[i - 1]) {
			return 0;
		}
	}
	return 1;
}

void afis(unsigned long v[], int lungime) {
	lungime = pow(10, lungime);
	for (int i = 0; i < lungime; i++) {
		cout << v[i] << " ";
	}

}

void quick_sort_verificare(unsigned long v[], int primul, int ultimul) { //spre deosebire de celelalte sortari, ultimul index este inclus (deci se apeleaza cu 10^p-1)
	unsigned long pivot;
	int i, j, mijloc = (primul + ultimul) / 2;

	if (primul < ultimul) {
		bool OK = 1;
		for (int i = primul; i < ultimul; i++)    //se verifica daca array-ul este sortat. Aceasta metoda este eficienta cand avem multe numere care se repeta.
			if (v[i] > v[i + 1]) {
				OK = 0;
				break;
			}
		if (OK == 1)
			return;

		if (v[primul] < v[ultimul]) {	//se calculeaza mediana din 3 si se muta pe ultima pozitie pentru a alege pivotul
			if (v[primul] > v[mijloc])
				swap(v[primul], v[ultimul]);
			else if (v[mijloc] < v[ultimul])
				swap(v[mijloc], v[ultimul]);
		}
		else {
			if (v[primul] < v[mijloc])
				swap(v[primul], v[ultimul]);
			else if (v[mijloc] > v[ultimul])
				swap(v[mijloc], v[ultimul]);
		}

		pivot = v[ultimul];

		i = primul - 1;
		for (j = primul; j < ultimul; j++) {  //se efectueaza algoritmul de "partitionare"
			if (v[j] <= pivot) {
				i++;
				swap(v[i], v[j]);
			}
		}
		swap(v[i + 1], v[ultimul]);

		quick_sort_verificare(v, primul, i);  //se apeleaza recursiv pentru elementele mai mic si mai mari ca pivotul
		quick_sort_verificare(v, i + 2, ultimul);
	}
}

int ordin(unsigned long x) {                 // returneaza ordinul in baza 10
	int nr = 0;
	while (x != 0) {
		x = x / 10;
		nr++;
	}
	return nr;
}

int ordin(unsigned long x, int baza) {                 // returneaza ordinul in o anumita baza
	int nr = 0;
	while (x != 0) {
		x = x / baza;
		nr++;
	}
	return nr;
}

unsigned long maxim_arr(unsigned long v[], int lungime) {  //functie care returneaza maximul dintr-un array 
	unsigned long macs = 0;
	lungime = pow(10, lungime);
	for (int i = 0; i < lungime; i++) {
		if (v[i] > macs) {
			macs = v[i];
		}
	}
	return macs;
}

void copy(unsigned long v[], unsigned long w[], int lungime) {
	lungime = pow(10, lungime);
	for (int i = 0; i < lungime; i++)
		w[i] = v[i];
}

void empty(unsigned long v[], int lungime) {
	lungime = pow(10, lungime);
	for (int i = 0; i <= lungime; i++)
		v[i] = 0;
}

float to_sec(long long start, long long finish) //transforma unitatile de timp din milisecunde in secunde
{
	return (float)(finish - start) / 1000;
}

void meniu() {
	int x, a, b;

	cout << "Alegeti tipul de input:\n1. Predefinit;\n2. Array cu dimensiuni specificate.\n";
	cin >> x;
	while (int(x) != x || x > 2 || x < 1) {				//se trateaza cazul in care alegerea nu este un numar intre 1 si 5.
		cout << "Alegere invalida, reincercati!\n";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> x;
	}
	cout << "\n";
	if (x == 1) {
		cout << "Alegeti tipul de array:\n1. Array cu o valoare constanta;\n2. Array cu valori strict crescatoare;\n3. Array cu valori strict descrescatoare;\n4. Array cu valori aleatoare;\n5. Toate tipurile mentionate, secvential.\n";
		cin >> x;
		while (int(x) != x || x > 5 || x < 1) {				//se trateaza cazul in care alegerea nu este un numar intre 1 si 5.
			cout << "Alegere invalida, reincercati!\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> x;
		}

		switch (x) {
		case 1:
			run_constant();
			break;
		case 2:
			run_crescator();
			break;
		case 3:
			run_descrescator();
			break;
		case 4:
			run_random();
			break;
		case 5:
			run_constant();
			run_crescator();
			run_descrescator();
			run_random();
			break;
		}
	}

	else {
		cout << "Alegeti tipul de array:\n1. Array cu o valoare constanta;\n2. Array cu valori strict crescatoare;\n3. Array cu valori strict descrescatoare;\n4. Array cu valori aleatoare;\n";
		cin >> x;
		while (int(x) != x || x > 4 || x < 1) {				//se trateaza cazul in care alegerea nu este un numar intre 1 si 5.
			cout << "Alegere invalida, reincercati!\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> x;
		}


		cout << "Alegeti a si b, unde N=10^a si MAX=10^b\nObs: a<=8, b<=9";
		if (x == 2 || x == 3)
			cout << ", a<=b";
		cout << "!\n";

	Citire:
		cout << "a= ";
		cin >> a;

		while (int(a) != a || a > 8 || a < 1) {				//se trateaza cazul in care alegerea nu este un numar intre 1 si 5.
			cout << "Alegere invalida, reincercati!\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> a;
		}

		cout << "b= ";
		cin >> b;

		while (int(b) != b || b > 9 || b < 1) {				//se trateaza cazul in care alegerea nu este un numar intre 1 si 5.
			cout << "Alegere invalida, reincercati!\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin >> b;
		}

		if ((x == 2 || x == 3) && (a > b)) {
			cout << "a nu poate fi mai mic ca b. Reincercati!\n";
			goto Citire;
		}


		switch (x) {
		case 1:
			generare_constant(arr, a, b);
			cout << "\nSe ruleaza pentru un array de numere constante de forma: ";
			rulare(arr, a, b, 1);
			break;
		case 2:
			generare_crescator(arr, a, b);
			cout << "\nSe ruleaza pentru un array de numere crescatoare de forma: ";
			rulare(arr, a, b);
			break;
		case 3:
			generare_descrescator(arr, a, b);
			cout << "\nSe ruleaza pentru un array de numere descrescatoare de forma: ";
			rulare(arr, a, b);
			break;
		case 4:
			generare_random(arr, a, b);
			cout << "\nSe ruleaza pentru un array cu numere aleatoare de forma: ";
			rulare(arr, a, b, 4);
			break;
		}
	}
}
