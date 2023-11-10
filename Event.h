#include <string>
#include <iostream>
#include <stdexcept>

#ifndef EVENT_INCLUDED
#define EVENT_INCLUDED

class Event {
    public:
        Event() = delete; // default ctor forbidden
        Event(const char* str) { std::string dateString(str); interpreter(dateString); }
        Event(std::string dateString) { interpreter(dateString); }
        ~Event() = default; // dtor

        Event(const Event &ob) { // copy ctor
            this->y = ob.y;
            this->m = ob.m;
            this->d = ob.d;
        }

        Event &operator=(const Event &ob) { // assignment operator
            y = ob.y;
            m = ob.m;
            d = ob.d;
            return *this;
        }

        void set(int year, int month, int day);
        std::string fmt() const;
        std::string fmt(std::string format) const;
        int year() const;
        int month() const;
        int day() const;

        Event &operator++(); // preincrement operator
        Event operator++(int); // postincrement operator
        Event &operator--(); // predecrement operator
        Event operator--(int); // postdecrement operator

        bool operator==(const Event rhs) const;
        bool operator!=(const Event rhs) const;
        bool operator<=(const Event rhs) const;
        bool operator>=(const Event rhs) const;
        bool operator<(const Event rhs) const;
        bool operator>(const Event rhs) const;


        friend std::ostream &operator << (std::ostream &os, Event &e);
        friend std::ostream &operator << (std::ostream &os, const Event &e);

    private:
        void interpreter(std::string dateString); // helper function

        tm* findDayMonthYear(std::string date); // helper function
        int validateDateChars(std::string date); // helper function
        tm* getAllTimeInfo(int year, int day); // helper function

        int y, m, d;
};

#endif /* EVENT_INCLUDED */