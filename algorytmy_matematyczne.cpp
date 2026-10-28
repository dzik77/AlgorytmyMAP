#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// =====================================================================
// SKALA PRZYDATNOSCI NA OI  -  [OI: x/10]
//   10    - absolutna podstawa, trzeba umiec napisac z pamieci w 2 minuty
//   8-9   - bardzo czesto sie przydaje, obowiazkowe
//   6-7   - pojawia sie regularnie, warto znac
//   4-5   - przydaje sie od czasu do czasu (raczej II/III etap)
//   1-3   - rzadkie, niszowe, glownie na trudniejsze zadania
// =====================================================================

// =====================================================================
// 1. DZIELNIKI I ROZKLAD NA CZYNNIKI PIERWSZE W O(sqrt(x))
// =====================================================================

// [OI: 10/10] Podstawa - pojawia sie w mnostwie zadan, takze jako fragment wiekszego rozwiazania.
// Wszystkie dzielniki x (posortowane), O(sqrt(x))
vector<ll> dzielniki(ll x) {
    vector<ll> male, duze;
    for (ll d = 1; d * d <= x; d++) {
        if (x % d == 0) {
            male.push_back(d);
            if (d != x / d) duze.push_back(x / d);
        }
    }
    reverse(duze.begin(), duze.end());
    for (ll d : duze) male.push_back(d);
    return male;
}

// [OI: 10/10] Podstawa - rozklad pojedynczej liczby do ~1e12 bez zadnego preprocessingu.
// Rozklad na czynniki pierwsze: pary (p, wykladnik), O(sqrt(x))
vector<pair<ll, int>> rozklad(ll x) {
    vector<pair<ll, int>> res;
    for (ll p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            int k = 0;
            while (x % p == 0) x /= p, k++;
            res.push_back({p, k});
        }
    }
    if (x > 1) res.push_back({x, 1}); // zostala liczba pierwsza > sqrt(x)
    return res;
}

// [OI: 9/10] Bardzo czesto potrzebne, trywialne do napisania.
// Test pierwszosci, O(sqrt(x))
bool czyPierwsza(ll x) {
    if (x < 2) return false;
    for (ll d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

// =====================================================================
// 2. SITO ERATOSTENESA, O(N log log N)
// =====================================================================

// [OI: 10/10] Absolutna podstawa - gdy pytan o pierwszosc jest duzo, a liczby sa <= ~1e7.
const int N = 1000000;

bool zlozona[N + 1];      // zlozona[i] == true  ->  i nie jest pierwsza
vector<int> pierwsze;     // lista liczb pierwszych <= N

void sito() {
    zlozona[0] = zlozona[1] = true;
    for (ll i = 2; i <= N; i++) {
        if (zlozona[i]) continue;
        pierwsze.push_back(i);
        for (ll j = i * i; j <= N; j += i) zlozona[j] = true;
    }
}

// =====================================================================
// 3. SPF (smallest prime factor) - rozklad w O(log x) dla x <= N
// =====================================================================

int spf[N + 1];

// [OI: 9/10] Bardzo czeste - gdy trzeba rozlozyc wiele liczb (np. caly ciag z wejscia).
// Sito liniowe, O(N); przy okazji wypelnia liste pierwszych
void liczSpf() {
    vector<int> pr;
    for (int i = 2; i <= N; i++) {
        if (spf[i] == 0) spf[i] = i, pr.push_back(i);
        for (int p : pr) {
            if (p > spf[i] || (ll)i * p > N) break;
            spf[i * p] = p;
        }
    }
}

// [OI: 9/10] Idzie w parze z liczSpf - bez tego spf nie ma sensu.
// Rozklad x <= N przy uzyciu spf, O(log x)
vector<pair<int, int>> rozkladSpf(int x) {
    vector<pair<int, int>> res;
    while (x > 1) {
        int p = spf[x], k = 0;
        while (x % p == 0) x /= p, k++;
        res.push_back({p, k});
    }
    return res;
}

// [OI: 7/10] Warto znac - szybkie dzielniki wielu liczb bez trzymania list dla wszystkich.
// Dzielniki x <= N generowane z rozkladu (spf)
vector<int> dzielnikiSpf(int x) {
    vector<int> res = {1};
    for (auto [p, k] : rozkladSpf(x)) {
        int sz = res.size();
        int pot = 1;
        for (int e = 1; e <= k; e++) {
            pot *= p;
            for (int i = 0; i < sz; i++) res.push_back(res[i] * pot);
        }
    }
    sort(res.begin(), res.end());
    return res;
}

// =====================================================================
// 4. PREPROCESSING DLA WSZYSTKICH LICZB <= M
// =====================================================================

const int M = 200000;

// [OI: 8/10] Bardzo przydatne - sam trik "for d, for k += d" (suma harmoniczna) to klasyka OI.
// Lista dzielnikow kazdej liczby (sito harmoniczne), O(M log M) czasu i pamieci
vector<int> dziel[M + 1];

void liczDzielnikiWszystkich() {
    for (int d = 1; d <= M; d++)
        for (int k = d; k <= M; k += d) dziel[k].push_back(d);
}

// [OI: 6/10] Pojawia sie regularnie, ale zwykle jako wariacja sita harmonicznego wyzej.
// Liczba dzielnikow (tau) i suma dzielnikow (sigma), O(M log M)
int ileDziel[M + 1];
ll sumaDziel[M + 1];

void liczTauSigma() {
    for (int d = 1; d <= M; d++)
        for (int k = d; k <= M; k += d) ileDziel[k]++, sumaDziel[k] += d;
}

// [OI: 6/10] Warto znac - zadania z NWD(i, n) = 1, liczenie par wzglednie pierwszych.
// Funkcja Eulera phi dla wszystkich liczb, O(M log log M)
int phi[M + 1];

void liczPhi() {
    for (int i = 0; i <= M; i++) phi[i] = i;
    for (int i = 2; i <= M; i++) {
        if (phi[i] != i) continue; // i nie jest pierwsza
        for (int j = i; j <= M; j += i) phi[j] -= phi[j] / i;
    }
}

// [OI: 5/10] Od czasu do czasu - wlaczenia-wylaczenia po dzielnikach (II/III etap).
// Funkcja Mobiusa mu dla wszystkich liczb, O(M log M)
int mu[M + 1];

void liczMu() {
    mu[1] = 1;
    for (int i = 1; i <= M; i++)
        for (int j = 2 * i; j <= M; j += i) mu[j] -= mu[i];
}

// =====================================================================
// 5. KOMBINATORYKA MODULO
// =====================================================================

const ll MOD = 1000000007; // musi byc pierwsze dla odwrotnosci z Fermata

// [OI: 10/10] Absolutna podstawa - szybkie potegowanie trzeba miec w palcach.
// a^b % mod, O(log b)
ll potega(ll a, ll b, ll mod = MOD) {
    ll res = 1;
    a %= mod;
    if (a < 0) a += mod;
    while (b > 0) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

// [OI: 9/10] Bardzo czeste - kazde "dzielenie" modulo przez to przechodzi.
// Odwrotnosc modularna (mod pierwsze, z malego tw. Fermata)
ll odwrotnosc(ll a) { return potega(a, MOD - 2); }

const int F = 1000000;
ll silnia[F + 1], odwSilnia[F + 1];

// [OI: 9/10] Bardzo czeste - preprocessing pod kazde zadanie kombinatoryczne.
// Silnie i odwrotnosci silni, O(F)
void liczSilnie() {
    silnia[0] = 1;
    for (int i = 1; i <= F; i++) silnia[i] = silnia[i - 1] * i % MOD;
    odwSilnia[F] = odwrotnosc(silnia[F]);
    for (int i = F; i > 0; i--) odwSilnia[i - 1] = odwSilnia[i] * i % MOD;
}

// [OI: 10/10] Absolutna podstawa - zliczanie "na ile sposobow" prawie zawsze go uzywa.
// Dwumian Newtona C(n, k) % MOD, O(1) po preprocessingu
ll C(int n, int k) {
    if (k < 0 || k > n || n < 0) return 0;
    return silnia[n] * odwSilnia[k] % MOD * odwSilnia[n - k] % MOD;
}

// [OI: 6/10] Przydaje sie, ale to jedna linijka z silni - latwo wyprowadzic na miejscu.
// Wariacje bez powtorzen P(n, k) = n! / (n-k)!
ll P(int n, int k) {
    if (k < 0 || k > n || n < 0) return 0;
    return silnia[n] * odwSilnia[n - k] % MOD;
}

// [OI: 7/10] Warto znac - "stars and bars" (rozdzielanie k kulek do n pudelek) wraca czesto.
// Kombinacje z powtorzeniami: wybor k z n rodzajow = C(n+k-1, k)
ll CzPowt(int n, int k) { return C(n + k - 1, k); }

// [OI: 5/10] Od czasu do czasu - poprawne nawiasowania, sciezki, drzewa binarne.
// Liczby Catalana: Cat(n) = C(2n, n) / (n+1)
ll catalan(int n) { return C(2 * n, n) * odwrotnosc(n + 1) % MOD; }

// [OI: 5/10] Od czasu do czasu - gdy n jest za duze na tablice silni.
// C(n, k) dla duzego n (np. 1e18) i malego k, O(k log MOD)
ll Cduze(ll n, int k) {
    if (k < 0 || k > n) return 0;
    ll licznik = 1, mian = 1;
    for (int i = 0; i < k; i++) {
        licznik = licznik * ((n - i) % MOD) % MOD;
        mian = mian * (i + 1) % MOD;
    }
    return licznik * odwrotnosc(mian) % MOD;
}

// [OI: 7/10] Warto znac - najprostszy i najbezpieczniejszy sposob, gdy n <= ~5000
// albo modul nie jest pierwszy; czesto jako czesc DP.
// Trojkat Pascala - dziala dla DOWOLNEGO modulo (nie musi byc pierwsze), O(K^2)
const int K = 2000;
int pascal[K + 1][K + 1];

void liczPascala(int mod) {
    for (int n = 0; n <= K; n++) {
        pascal[n][0] = 1;
        for (int k = 1; k <= n; k++)
            pascal[n][k] = (pascal[n - 1][k - 1] + pascal[n - 1][k]) % mod;
    }
}

// =====================================================================
// PRZYKLADY
// =====================================================================

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // sqrt
    cout << "Dzielniki 360: ";
    for (ll d : dzielniki(360)) cout << d << ' ';
    cout << "\nRozklad 360: ";
    for (auto [p, k] : rozklad(360)) cout << p << '^' << k << ' ';
    cout << "\nczyPierwsza(1000000007): " << czyPierwsza(1000000007) << '\n';

    // sito
    sito();
    cout << "Liczb pierwszych <= " << N << ": " << pierwsze.size() << '\n';

    // spf
    liczSpf();
    cout << "Rozklad spf 997920: ";
    for (auto [p, k] : rozkladSpf(997920)) cout << p << '^' << k << ' ';
    cout << "\nDzielniki spf 84: ";
    for (int d : dzielnikiSpf(84)) cout << d << ' ';
    cout << '\n';

    // preprocessing
    liczDzielnikiWszystkich();
    liczTauSigma();
    liczPhi();
    liczMu();
    cout << "Dzielniki 12 (preprocess): ";
    for (int d : dziel[12]) cout << d << ' ';
    cout << "\ntau(12)=" << ileDziel[12] << " sigma(12)=" << sumaDziel[12]
         << " phi(12)=" << phi[12] << " mu(30)=" << mu[30] << '\n';

    // kombinatoryka
    liczSilnie();
    cout << "10! = " << silnia[10] << '\n';
    cout << "C(10, 3) = " << C(10, 3) << '\n';
    cout << "C(1000000, 500000) % MOD = " << C(1000000, 500000) << '\n';
    cout << "P(10, 3) = " << P(10, 3) << '\n';
    cout << "CzPowt(3, 2) = " << CzPowt(3, 2) << '\n';
    cout << "Catalan(5) = " << catalan(5) << '\n';
    cout << "Cduze(1e18, 2) = " << Cduze(1000000000000000000LL, 2) << '\n';
    liczPascala(1000000);
    cout << "Pascal C(30, 15) % 1e6 = " << pascal[30][15] << '\n';
}
