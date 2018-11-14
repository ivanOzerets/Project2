#ifndef MAP_H
#define MAP_H

#include <QString>

class Map {
public:
    class Entry {
    public:
        Entry() {
            key = 0;
            souvenir = "";
            price = 0;
        }

        Entry(const int k, const QString s, const double p) {
            key = k;
            souvenir = s;
            price = p;
        }

        int getKey() {return key;}
        void setKey(const int k) {key = k;}

        QString getSouvenir() {return souvenir;}
        void setSouvenir(const QString s) {souvenir = s;}

        double getPrice() {return price;}
        void setPrice(const double p) {price = p;}
    private:
        int key;
        QString souvenir;
        double price;
    };

    class Iterator {
    public:
        Iterator(Entry *u) {v = u;}
        Entry& operator*() const {return *v;}
        bool operator==(const Iterator& p) const {return v == p.v;}
        bool operator!=(const Iterator& p) const {return v != p.v;}

        Iterator& operator++() {
            v = ++v;
            return *this;
        }

        Iterator& operator--() {
            v = --v;
            return *this;
        }
    private:
        Entry *v;
    };

    Map() {
        N = 25;
        map = new Entry*[N];
        for(int i = 0; i < N; i++) {
            map[i] = nullptr;
        }
        length = 0;
    }

    ~Map() {
        for(int i = 0; i < N; i++) {
            if(map[i] != nullptr) {
                delete map[i];
            }
        }
        delete map;
    }

    int size() const {return length;}
    bool empty() const {return length == 0;}

    Iterator find(const int k) {
        int hashCode = doubleHashing(k);
        if(hashCode < 0) {
            return this->end();
        }
        else {
            Iterator entry( map[hashCode] );
            return entry;
        }
    }

    Iterator put(const int k,
                 const QString s,
                 const double p) {
        int hashCode = doubleHashing(k);
        if(hashCode < 0) {
            return this->end();
        }
        else {
            if( map[hashCode] == nullptr ) {
                length++;
            }
            Entry *entry;
            entry = new Entry(k, s, p);
            map[hashCode] = entry;
            Iterator e( map[hashCode] );
            return e;
        }
    }


    void erase(const int k) {
        int hashCode = doubleHashing(k);
        if(hashCode >= 0) {
            Entry *p = map[hashCode];
            delete p;
            map[hashCode] = nullptr;
        }
    }


    void erase(const Iterator& p) {
        int k = (*p).getKey();
        erase(k);
    }

    Iterator begin() {
        int i = 0;
        while(map[i] == nullptr && i < N) {
            i++;
        }
        if(map[i] == nullptr) {
            return this->end();
        }
        else {
            Iterator entry( map[i] );
            return entry;
        }
    }

    Iterator end() {
        int i = N-1;
        while(map[i] == nullptr && i > -1) {
            i--;
        }
        Iterator entry( map[i-1] );
        ++entry;
        return entry;
    }

    int doubleHashing(const int& k) const {
        int i = k % N;  // h(k)
        int j = 0;      // j = 0, 1, ..., 25
        int hashCode = i + j * (13 - (k % 13)); // array index #
        int count = 0;  // track if array is full
        do {
            // Possibility #1: the cell is empty
            if(map[hashCode] == nullptr) {
                return hashCode;
            }
            // Possibility #2: the cell contains an entry, and the keys are equal
            else if(map[hashCode]->getKey() == k) {
                return hashCode;
            }
            // Possibility #3: the cell contain an entry, but the keys are unequal
            else {
                j++;
                hashCode = i + j * (13 - (k % 13));
                count++;
            }
        } while(count < N); // end if array is full
        return -1;  // return -1 if array is full
    }

private:
    Entry* *map;    // map array
    int length;     // # of entries
    int N;          // array size
};

#endif // MAP_H
