#include "Schedule.h"
#include <fstream>
#include <stdexcept>

void Schedule::read(std::istream &is) {
    std::string str;

    while (is >> str) {
        try {
            Event e(str);
            events.push_back(e);
        } catch (const std::runtime_error& error) {
            is.setstate(std::ios_base::failbit);
            throw std::runtime_error("Bad date '" + str + "'");
        }
    }
    this->sort();
}

void Schedule::sort() {
    size_t minIndex;
    for (size_t i = 0; i < events.size() - 1; ++i) {
        minIndex = i;
        for (size_t j = i + 1; j < events.size(); ++j) {
            if (events[j].year() < events[minIndex].year()) {
                minIndex = j;
            } else if (events[j].year() == events[minIndex].year() && events[j].month() < events[minIndex].month()) {
                minIndex = j;
            } else if (events[j].year() == events[minIndex].year() && events[j].month() == events[minIndex].month() && events[j].day() < events[minIndex].day()) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            Event temp = events[minIndex];
            events[minIndex] = events[i];
            events[i] = temp;
        }
    }
}

void Schedule::clear() {
    events.clear();
}

size_t Schedule::size() const {
    return events.size();
}

bool Schedule::empty() const {
    if (events.size() == 0) return true;
    return false;
}

Schedule::iterator Schedule::begin() const {
    return events.begin();
}

Schedule::iterator Schedule::end() const {
    return events.end();
}

std::ostream &operator<<(std::ostream &os, Schedule &schedule) {
    for (Event &e : schedule.events) {
        os << e << '\n';
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const Schedule &schedule) {
    Schedule x(schedule);

    for (Event &e : x.events) {
        os << e << '\n';
    }

    return os;
}