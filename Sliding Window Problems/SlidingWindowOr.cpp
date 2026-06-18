#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int OR(int a, int b) {
    return a | b;
}

template <class T, auto F>
struct AggStack {
    stack<pair<T, T>> S;

    void push(T val) {
        if (S.empty())
            S.push({val, val});
        else
            S.push({val, F(val, S.top().second)});
    }

    T top() {
        return S.top().first;
    }

    T pop() {
        T ret = S.top().first;
        S.pop();
        return ret;
    }

    bool empty() {
        return S.empty();
    }

    int size() {
        return S.size();
    }

    T get() {
        return S.top().second;
    }

    void swap(AggStack<T, F>& ST) {
        S.swap(ST.S);
    }
};

template <class T, auto F>
struct AggQueue {
    AggStack<T, F> In, Out;

    void push(T val) {
        In.push(val);
    }

    T pop() {
        if (Out.empty()) {
            while (!In.empty())
                Out.push(In.pop());
        }
        return Out.pop();
    }

    T get() {
        if (In.empty()) return Out.get();
        if (Out.empty()) return In.get();
        return F(In.get(), Out.get());
    }
};

template <class T, auto F>
struct AggDeque {
    AggStack<T, F> L, R, I;

    void push_front(T val) {
        L.push(val);
    }

    void push_back(T val) {
        R.push(val);
    }

    void move() {
        bool swa = false;

        if (R.empty()) {
            swa = true;
            L.swap(R);
        }

        int SZ = R.size() / 2;

        while (SZ--)
            I.push(R.pop());

        while (!R.empty())
            L.push(R.pop());

        while (!I.empty())
            R.push(I.pop());

        if (swa)
            L.swap(R);
    }

    T front() {
        if (L.empty())
            move();
        return L.top();
    }

    T back() {
        if (R.empty())
            move();
        return R.top();
    }

    T pop_front() {
        if (L.empty())
            move();
        return L.pop();
    }

    T pop_back() {
        if (R.empty())
            move();
        return R.pop();
    }

    bool empty() {
        return L.empty() && R.empty();
    }

    int size() {
        return L.size() + R.size();
    }

    T get() {
        if (L.empty()) return R.get();
        if (R.empty()) return L.get();
        return F(L.get(), R.get());
    }

    void swap(AggDeque<T, F>& S) {
        L.swap(S.L);
        R.swap(S.R);
    }
};

void solve_using_agg_q() {
    int n, k;
    cin >> n >> k;

    ll x, a, b, c;
    cin >> x >> a >> b >> c;

    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        if (i == 0)
            arr[i] = (int)x;
        else
            arr[i] = (int)((a * 1LL * arr[i - 1] + b) % c);
    }

    AggQueue<int, OR> Q;

    ll ans = 0;
    int i = 0;

    for (int j = 0; j < n; j++) {
        Q.push(arr[j]);

        if (j - i + 1 < k)
            continue;

        ans ^= Q.get();

        Q.pop();
        i++;
    }

    cout << ans << '\n';
}

void solve_using_prefix_suffix_arrays() {
    int n, k;
    cin >> n >> k;

    ll x, a, b, c;
    cin >> x >> a >> b >> c;

    vector<int> arr(n);

    for (int i = 0; i < n; i++) {
        if (i == 0)
            arr[i] = (int)x;
        else
            arr[i] = (int)((a * 1LL * arr[i - 1] + b) % c);
    }

    vector<int> pf_ors(n);

    for (int i = 0; i < n; i++) {
        if (i % k == 0)
            pf_ors[i] = arr[i];
        else
            pf_ors[i] = pf_ors[i - 1] | arr[i];
    }

    vector<int> sf_ors(n);

    sf_ors[n - 1] = arr[n - 1];

    for (int i = n - 2; i >= 0; i--) {
        if (i % k == k - 1)
            sf_ors[i] = arr[i];
        else
            sf_ors[i] = arr[i] | sf_ors[i + 1];
    }

    ll ans = 0;

    for (int j = k - 1; j < n; j++) {
        int cur = pf_ors[j] | sf_ors[j - k + 1];
        ans ^= cur;
    }

    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // solve_using_agg_q();
    solve_using_prefix_suffix_arrays();

    return 0;
}