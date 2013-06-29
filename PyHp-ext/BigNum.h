#ifndef _BIG_NUM_H_
#define _BIG_NUM_H_
#include <string.h>
#include <sstream>
#include <string>
#include <stdio.h>
using namespace std;

const int base = 10000;
const int width = 4;
const int N = 1000;

string IntToString(int i) {
    string s;
    stringstream ss(s);
    ss << i;
    return ss.str();
}

class Bint {
    public:
        int ln;
        int v[N];
        Bint(int r = 0) {
            for (ln =  0; r > 0; r /= base) {
                v[ln++] = r % base;
            }
        }

        Bint& operator=(const Bint& r) {
            ln = r.ln;
            memset(v,0,sizeof(v));
            for (int i = 0 ; i < r.ln; i++) {
                v[i] = r.v[i];
            }
            return *this;
        }

        Bint operator+(const Bint& other){
            int i, cy = 0;
            for (i = 0; i < ln || i < other.ln || cy > 0; i++) {
                if (i < ln) cy += v[i];
                if (i < other.ln) cy += other.v[i];
                v[i] = cy % base; cy /= base;
            }
            ln = i;
            return *this;
        }

        static string write(const Bint& other){
            string strs = "";
            int i;
            //printf("%d", v.ln == 0 ? 0 : v.v[v.ln - 1]);
            if (other.ln == 0) {
                return "0";
            } else {
                strs += IntToString(other.v[other.ln-1]); 
            }
            char tmp[4];
            for (i = other.ln - 2; i >= 0; i--) {
                //printf("%04d", v.v[i]); // ! 4 == width
                sprintf(tmp,"%04d",other.v[i]);
                strs += tmp;
            }
            return strs;
        }

        friend Bint operator*(const Bint& a, const Bint& b){
            Bint res; res.ln = 0;
            if (0 == b.ln) { res.v[0] = 0; return res; }
            int i, j, cy;
            for (i = 0; i < a.ln; i++) {
                for (j=cy=0; j < b.ln || cy > 0; j++, cy/= base) {
                    if (j < b.ln) cy += a.v[i] * b.v[j];
                    if (i + j < res.ln) cy += res.v[i + j];
                    if (i + j >= res.ln) res.v[res.ln++] = cy % base;
                    else res.v[i + j] = cy % base;
                }
            }
            return res;
        }

        friend Bint operator- (const Bint& a, const Bint& b){
            Bint res; int i, cy = 0;
            for (res.ln = a.ln, i = 0; i < res.ln; i++) {
                res.v[i] = a.v[i] - cy;
                if (i < b.ln) res.v[i] -= b.v[i];
                if (res.v[i] < 0) cy = 1, res.v[i] += base;
                else cy = 0;
            }
            while (res.ln > 0 && res.v[res.ln - 1] == 0) res.ln--;
            return res;
        }

        friend Bint operator / (const Bint& a, const Bint& b)
        {
            // ! b != 0
            Bint tmp, mod, res;
            int i, lf, rg, mid;
            mod.v[0] = mod.ln = 0;
            for (i = a.ln - 1; i >= 0; i--) {
                mod = mod * base + a.v[i];
                for (lf = 0, rg = base -1; lf < rg; ) {
                    mid = (lf + rg + 1) / 2;
                    if (b * mid <= mod) lf = mid;
                    else rg = mid - 1;
                }
                res.v[i] = lf;
                mod = mod - b * lf;
            }
            res.ln = a.ln;
            while (res.ln > 0 && res.v[res.ln - 1] == 0) res.ln--;
            return res;
        }
             
        friend bool operator < (const Bint& a, const Bint& b){
            int i;
            if (a.ln != b.ln) return a.ln < b.ln;
            for (i = a.ln - 1; i >= 0 && a.v[i] == b.v[i]; i--);
            return i < 0 ? 0 : a.v[i] < b.v[i];
        }
        friend bool operator <= (const Bint& a, const Bint& b){
            return !(b < a);
        }

            
};
#endif
