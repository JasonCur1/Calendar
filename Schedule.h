#include "Event.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <iterator>
#include <cstddef>

#ifndef SCHEDULE_INCLUDED
#define SCHEDULE_INCLUDED

class Schedule {
    private:
        std::vector<Event> events;
        void sort();

    public: 
        using iterator = decltype(events)::const_iterator;

        Schedule() = default; // default ctor
        ~Schedule() = default; // dtor
        Schedule(std::istream &is) { read(is); }
        Schedule(const char* str) { 
            std::string file(str); 
            std::ifstream is(file);
            if (!is) {
                throw std::runtime_error(file + " cannot be opened."); 
            } else {
                read(is);
            }
        }

        Schedule(const std::string str) {
            std::string file(str);
            std::ifstream is(file); 
            if (!is) {
                throw std::runtime_error(file + " cannot be opened."); 
            } else {
                read(is);
            }
        }

        Schedule(const Schedule &ob) { // copy ctor
            for (size_t i = 0; i < ob.events.size(); i++) {
                this->events.push_back(ob.events[i]);
            }
        }

        Schedule &operator=(const Schedule &ob) { // assignment operator
            for (size_t i = 0; i < ob.events.size(); i++) {
                this->events.push_back(ob.events[i]);
            }
            return *this;
        }

        const Event &operator[](size_t index) { // non-const [] subscript operator
            if (index >= size()) {
                throw std::range_error("index '" + std::to_string(index) + "' is out of bounds. There are '" + std::to_string(size()) + "' events in this schedule."); 
            }

            return events[index];
        }

        const Event &operator[](size_t index) const { // const [] subscript operator
            if (index >= size()) {
                throw std::range_error("index '" + std::to_string(index) + "' is out of bounds. There are '" + std::to_string(size()) + "' events in this schedule."); 
            }

            return events[index];
        }

        void read(std::istream &is);
        void clear();
        size_t size() const;
        bool empty() const;

        friend std::ostream &operator << (std::ostream &os, Schedule &s);
        friend std::ostream &operator << (std::ostream &os, const Schedule &s);

        iterator begin() const;
        iterator end() const;
};

#endif /* EVENT_INCLUDED */