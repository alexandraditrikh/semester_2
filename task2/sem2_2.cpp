#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

struct Elem {
    int ind;
    double pi;
    Elem* next;
};
typedef Elem* Polinom;

Polinom a(ifstream& in) {
    int r;
    in >> r;

    Polinom top = nullptr;

    for (int i = 0; i < r; ++i) {
        double pi;
        if (!(in >> pi)) break;
        if (pi != 0) {
            Polinom newPolinom = new Elem;
            newPolinom->ind = i;
            newPolinom->pi = pi;
            newPolinom->next = nullptr;

            if (!top) top = newPolinom;
            else {
                Polinom cur = top;
                while (cur->next) cur = cur->next;
                cur->next = newPolinom;
            }
        }
    }
    return top;
}

void b(ofstream& out, Polinom& P) {
    out << "P(x) = ";

    if (!P) {
        out << "0";
        return;
    }

    for (Polinom cur = P; cur; cur = cur->next) {
        out << cur->pi;
        if (cur->ind) out << "e^(-" << cur->ind << "x)";
        if (cur->next) out << " + ";
    }
}

Polinom c(Polinom P, Polinom Q) {
    Polinom res = nullptr;

    int kmax = 0;
    for (Polinom p = P; p; p = p->next) {
        for (Polinom q = Q; q; q = q->next) {
            int k = p->ind + q->ind;
            if (k > kmax) kmax = k;
        }
    }

    for (int k = 0; k <= kmax; ++k) {
        double sum = 0;

        for (Polinom p = P; p; p = p->next) {
            for (Polinom q = Q; q; q = q->next) {
                if (p->ind + q->ind == k) {
                    sum += p->pi * q->pi;
                }
            }
        }

        if (sum != 0) {
            Polinom newPolinom = new Elem;
            newPolinom->ind = k;
            newPolinom->pi = sum;
            newPolinom->next = nullptr;

            if (!res) res = newPolinom;
            else {
                Polinom cur = res;
                while (cur->next) cur = cur->next;
                cur->next = newPolinom;
            }
        }
    }
    return res;
}

Polinom d(Polinom p, Polinom q) {

    Polinom res = nullptr;

    while (p || q)
    {
        Polinom newPolinom = new Elem;
        newPolinom->next = nullptr;

        if (p && (!q || p->ind < q->ind)) {
            newPolinom->pi = p->pi;
            newPolinom->ind = p->ind;
            p = p->next;
        }
        else if (q && (!p || q->ind < p->ind))
        {
            newPolinom->pi = q->pi;
            newPolinom->ind = q->ind;
            q = q->next;
        }
        else {
            newPolinom->pi = p->pi + q->pi;
            newPolinom->ind = p->ind;
            p = p->next;
            q = q->next;
        }
        if (newPolinom->pi != 0) {
            if (!res) res = newPolinom;
            else {
                Polinom cur = res;
                while (cur->next) cur = cur->next;
                cur->next = newPolinom;
            }
        }
        else {
            delete newPolinom;
        }
        return res;
    }
}

double e(Polinom P, double a) {
    double res = 0;
    for (Polinom p = P; p; p = p->next) {
        res += p->pi * exp(-p->ind * a);
    }
    return res;
}

void del(Polinom P) {
    Polinom temp;
    while (P) {
        temp = P;
        P = P->next;
        delete temp;
    }
}

int main() {
    setlocale(LC_ALL, "RU");

    // a
    ifstream in1("input1.txt");
    if (!in1) {
        cerr << "Не удалось открыть input1.txt!" << endl;
        return 1;
    }
    ifstream in2("input2.txt");
    if (!in2) {
        cerr << "Не удалось открыть input2.txt!" << endl;
        in1.close();
        return 1;
    }

    Polinom P1 = a(in1);
    Polinom P2 = a(in2);

    in1.close();
    in2.close();

    if (!P1 || !P2) {
        cerr << "Один из полиномов пуст!" << endl;
        del(P1);
        del(P2);
        return 1;
    }

    // b
    Polinom P3 = d(P1, P2);
    Polinom P4 = c(P3, P1);

    ofstream out_b("output_b.txt");
    if (!out_b) {
        cerr << "Не удалось создать output_b.txt!" << endl;
        del(P1);
        del(P2);
        del(P3);
        del(P4);
        return 1;
    }
    b(out_b, P4);
    out_b.close();

    // c
   /* double a_val;
    cout << "Введите a: ";
    cin >> a_val;
    cout << "P4(a) = " << e(P4, a_val) << endl;*/

    del(P1);
    del(P2);
    del(P3);
    del(P4);
}