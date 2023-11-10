#include "Event.h"
#include <iomanip>

void Event::set(int year, int month, int day) {    
    this->y = year;
    this->m = month;
    this->d = day;
    return;
}

int Event::year() const {
    return y;
}

int Event::month() const {
    return m;
}

int Event::day() const {
    return d;
}

std::string Event::fmt() const {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << this->year() << '-' << std::setfill('0') << std::setw(2) << this->month() << '-' << std::setfill('0') << std::setw(2) << this->day() << std::setfill(' ');
    return ss.str();
}

std::string Event::fmt(std::string format) const {
    char outputString[64];
    struct tm* timeinfo;
    time_t rawtime;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    timeinfo->tm_year = this->y - 1900;
    timeinfo->tm_mon = this->m - 1;
    timeinfo->tm_mday = this->d;

    mktime(timeinfo);

    strftime(outputString, sizeof(outputString), format.c_str(), timeinfo);

    return std::string(outputString);
}

void Event::interpreter(std::string str) {
    const unsigned int NUM_SECONDS_IN_DAY = 86400;

    std::string dateString;
    time_t rawtime;
    struct tm* timeinfo;

    for (char &c : str) { // conver string to lowercase
        dateString += tolower(c);
    }

    if (dateString == "today") {
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        set(timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
    } else if (dateString == "tomorrow") {
        time(&rawtime);
        rawtime += NUM_SECONDS_IN_DAY;
        timeinfo = localtime(&rawtime);
        set(timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
    } else if (dateString == "yesterday") {
        time(&rawtime);
        rawtime -= NUM_SECONDS_IN_DAY;
        timeinfo = localtime(&rawtime);
        set(timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
    } else {
        timeinfo = findDayMonthYear(dateString);
        set(timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
    }
}

tm* Event::findDayMonthYear(std::string date) {
    std::string temp = "";
    int year, month, day;     

    if (validateDateChars(date) == 0) { // year.day format
        // first find the year (numbers before the '.')
        int i = 0;
        while (date[i] != '.') {
            temp += date[i];
            i++;
        }

        year = stoi(temp); // FOUND YEAR

        if (year < 1 || year > 9999) {
            throw std::runtime_error("Year: " + std::to_string(year) + " is out of acceptable range.");
        }

        // then find the day (numbers after the '.')
        i++;
        temp = "";
        while (date[i] != '\0') {
            temp += date[i];
            i++;
        }

        day = stoi(temp); // FOUND DAY

        return getAllTimeInfo(year, day);

    } else { // YYYY-MM-DD format
        int i = 0;
        while (date[i] != '-') {
            temp += date[i];
            i++;
        }

        year = stoi(temp);

        if (year < 1 || year > 9999) { // year error checking
            throw std::runtime_error("Year: " + std::to_string(year) + " is out of acceptable range.");
        }


        i++;
        temp = "";
        while (date[i] != '-') {
            temp += date[i];
            i++;
        }

        month = stoi(temp);

        if (month < 1 || month > 12) { // month error checking
            throw std::runtime_error("Month: " + std::to_string(month) + " is out of acceptable range.");
        }


        i++;
        temp = "";
        while (date[i] != '\0') {
            temp += date[i];
            i++;
        }

        day = stoi(temp);

        // check leap year first, then check which month it is, then chekc if day is valid
        if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)) { // NOT A LEAP YEAR
            if ((month == 1 || month  == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && (day > 31 || day < 1)) {
                throw std::runtime_error("Day: " + std::to_string(day) + " is out of acceptable range.");
            } else if ((month == 4 || month  == 6 || month == 9 || month == 11) && (day > 30 || day < 1)) {
                throw std::runtime_error("Day: " + std::to_string(day) + " is out of acceptable range.");
            } else if (month == 2 && (day > 28 || day < 1)) {
                throw std::runtime_error("Day: " + std::to_string(day) + " is out of acceptable range.");
            }
        } else {
            if ((month == 1 || month  == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && (day > 31 || day < 1)) {
                throw std::runtime_error("Day: " + std::to_string(day) + " is out of acceptable range.");
            } else if ((month == 4 || month  == 6 || month == 9 || month == 11) && (day > 30 || day < 1)) {
                throw std::runtime_error("Day: " + std::to_string(day) + " is out of acceptable range.");
            } else if (month == 2 && (day > 29 || day < 1)) {
                throw std::runtime_error("Day: " + std::to_string(day) + " is out of acceptable range.");
            }
        }

        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        timeinfo->tm_year = year - 1900;
        timeinfo->tm_mon = month - 1;
        timeinfo->tm_mday = day;
        mktime(timeinfo);

        return timeinfo;
    }
}

int Event::validateDateChars(std::string date) {
    const std::string acceptableChars1 = "0123456789.";
    const std::string acceptableChars2 = "0123456789-";
    int dotCount = 0;
    int dashCount = 0;

    for (size_t i = 0; i < date.size(); i++) { // iterate through date and count # of . and -
        if (date[i] == '.') {
            dotCount++;
        } else if (date[i] == '-') {
            dashCount++;
        }
    }

    if (dotCount == 1 && dashCount == 0) { // assume year.day format
        for (size_t j = 0; j < date.size(); j++) {
            if (acceptableChars1.find(date[j]) != std::string::npos && date[j] != ' ') {

            } else {
                if (date[j] == ' ') {
                    throw std::runtime_error("Date: '" + date + "' contained whitespace.");
                } else {
                    throw std::runtime_error("Date: '" + date + "' contained invalid character: '" + date[j] + "'");
                }
            }
        }
        return 0;
    } else if (dashCount == 2 && dotCount == 0) { // assume YYYY-MM-DD format
        for (size_t j = 0; j < date.size(); j++) {
            if (acceptableChars2.find(date[j]) != std::string::npos && date[j] != ' ') {

            } else {
                if (date[j] == ' ') {
                    throw std::runtime_error("Date: '" + date + "' contained whitespace.");
                } else {
                    throw std::runtime_error("Date: '" + date + "' contained invalid character: '" + date[j] + "'");
                }
            }
        }
        return 1;
    } else { // assume invalid format
        throw std::runtime_error("Date: '" + date + "' has invalid format.");
    }
}

tm* Event::getAllTimeInfo(int year, int day) {
    time_t rawtime;
    struct tm * timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    timeinfo->tm_year = year - 1900;

    if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)) { // NOT LEAP YEAR
        if (day <= 31) {
            timeinfo->tm_mon = 0;
            timeinfo->tm_mday = day;
            mktime(timeinfo);
            return timeinfo;
        } else if (31 < day && day <= 59) {
            timeinfo->tm_mon = 1;
            timeinfo->tm_mday = day - 31;
            mktime(timeinfo);
            return timeinfo;
        } else if (59 < day && day <= 90) {
            timeinfo->tm_mon = 2;
            timeinfo->tm_mday = day - 59;
            mktime(timeinfo);
            return timeinfo;
        } else if (90 < day && day <= 120) {
            timeinfo->tm_mon = 3;
            timeinfo->tm_mday = day - 90;
            mktime(timeinfo);
            return timeinfo;
        } else if (120 < day && day <= 151) {
            timeinfo->tm_mon = 4;
            timeinfo->tm_mday = day - 120;
            mktime(timeinfo);
            return timeinfo;
        } else if (151 < day && day <= 181) {
            timeinfo->tm_mon = 5;
            timeinfo->tm_mday = day - 151;
            mktime(timeinfo);
            return timeinfo;
        } else if (181 < day && day <= 212) {
            timeinfo->tm_mon = 6;
            timeinfo->tm_mday = day - 181;
            mktime(timeinfo);
            return timeinfo;
        } else if (212 < day && day <= 243) {
            timeinfo->tm_mon = 7;
            timeinfo->tm_mday = day - 212;
            mktime(timeinfo);
            return timeinfo;
        } else if (243 < day && day <= 273) {
            timeinfo->tm_mon = 8;
            timeinfo->tm_mday = day - 243;
            mktime(timeinfo);
            return timeinfo;
        } else if (273 < day && day <= 304) {
            timeinfo->tm_mon = 9;
            timeinfo->tm_mday = day - 273;
            mktime(timeinfo);
            return timeinfo;
        } else if (304 < day && day <= 334) {
            timeinfo->tm_mon = 10;
            timeinfo->tm_mday = day - 304;
            mktime(timeinfo);
            return timeinfo;
        } else if (334 < day && day <= 365) {
            timeinfo->tm_mon = 11;
            timeinfo->tm_mday = day - 334;
            mktime(timeinfo);
            return timeinfo;
        } else {
            throw std::runtime_error("Day: " + std::to_string(day) + " is out of acceptable range.");
        }
    } else { // LEAP YEAR
        if (day <= 31) {
            timeinfo->tm_mon = 0;
            timeinfo->tm_mday = day;
            mktime(timeinfo);
            return timeinfo;
        } else if (31 < day && day <= 60) {
            timeinfo->tm_mon = 1;
            timeinfo->tm_mday = day - 31;
            mktime(timeinfo);
            return timeinfo;
        } else if (60 < day && day <= 91) {
            timeinfo->tm_mon = 2;
            timeinfo->tm_mday = day - 60;
            mktime(timeinfo);
            return timeinfo;
        } else if (91 < day && day <= 121) {
            timeinfo->tm_mon = 3;
            timeinfo->tm_mday = day - 91;
            mktime(timeinfo);
            return timeinfo;
        } else if (121 < day && day <= 152) {
            timeinfo->tm_mon = 4;
            timeinfo->tm_mday = day - 121;
            mktime(timeinfo);
            return timeinfo;
        } else if (152 < day && day <= 182) {
            timeinfo->tm_mon = 5;
            timeinfo->tm_mday = day - 152;
            mktime(timeinfo);
            return timeinfo;
        } else if (182 < day && day <= 213) {
            timeinfo->tm_mon = 6;
            timeinfo->tm_mday = day - 182;
            mktime(timeinfo);
            return timeinfo;
        } else if (213 < day && day <= 244) {
            timeinfo->tm_mon = 7;
            timeinfo->tm_mday = day - 213;
            mktime(timeinfo);
            return timeinfo;
        } else if (244 < day && day <= 274) {
            timeinfo->tm_mon = 8;
            timeinfo->tm_mday = day - 244;
            mktime(timeinfo);
            return timeinfo;
        } else if (274 < day && day <= 305) {
            timeinfo->tm_mon = 9;
            timeinfo->tm_mday = day - 274;
            mktime(timeinfo);
            return timeinfo;
        } else if (305 < day && day <= 335) {
            timeinfo->tm_mon = 10;
            timeinfo->tm_mday = day - 305;
            mktime(timeinfo);
            return timeinfo;
        } else if (335 < day && day <= 366) {
            timeinfo->tm_mon = 11;
            timeinfo->tm_mday = day - 335;
            mktime(timeinfo);
            return timeinfo;
        } else {
            throw std::runtime_error("Day: " + std::to_string(day) + " is out of acceptable range.");
        }
    }
}

Event &operator++(Event &event) {
    int year = event.year();
    int month = event.month();
    int day = event.day();

    ++day;

    if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)) { // NOT A LEAP YEAR
        if ((month == 1 || month  == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day == 32) {
            day = 1;
            ++month;
        } else if ((month == 4 || month  == 6 || month == 9 || month == 11) && day == 31) {
            day = 1;
            ++month;
        } else if (month == 2 && day == 29) {
            day = 1;
            ++month;
        }
    } else {
        if ((month == 1 || month  == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day == 32) {
            day = 1;
            ++month;
        } else if ((month == 4 || month  == 6 || month == 9 || month == 11) && day == 31) {
            day = 1;
            ++month;
        } else if (month == 2 && day == 30) {
            day = 1;
            ++month;
        }
    }

    if (month == 13) {
        month = 1;
        ++year;
    }

    if (year > 9999) throw std::runtime_error("Year: " + std::to_string(year) + " is out of acceptable range.");

    event.set(year, month, day);

    return event;
}

Event operator++(Event &event, int) {
    Event temp = event;
    int year = event.year();
    int month = event.month();
    int day = event.day();

    ++day;

    if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)) { // NOT A LEAP YEAR
        if ((month == 1 || month  == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day == 32) {
            day = 1;
            ++month;
        } else if ((month == 4 || month  == 6 || month == 9 || month == 11) && day == 31) {
            day = 1;
            ++month;
        } else if (month == 2 && day == 29) {
            day = 1;
            ++month;
        }
    } else {
        if ((month == 1 || month  == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day == 32) {
            day = 1;
            ++month;
        } else if ((month == 4 || month  == 6 || month == 9 || month == 11) && day == 31) {
            day = 1;
            ++month;
        } else if (month == 2 && day == 30) {
            day = 1;
            ++month;
        }
    }

    if (month == 13) {
        month = 1;
        ++year;
    }

    if (year > 9999) throw std::runtime_error("Year: " + std::to_string(year) + " is out of acceptable range.");

    event.set(year, month, day);

    return temp;
}

Event &operator--(Event &event) {
    int year = event.year();
    int month = event.month();
    int day = event.day();

    if (day == 1 && month == 1 && year == 1) { // invalid year range
        throw std::runtime_error("Year: " + std::to_string(year) + " is out of acceptable range.");
    } else if (day == 1 && month == 1) { // day underflows month -> month underflows year -> year still valid
        day = 31;
        month = 12;
        --year;
    } else if (day == 1) { // day underflows month
        --month;

        // check what day needs to be set to based on if its leap year or not.
        if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)) { // NOT A LEAP YEAR
            if ((month == 1 || month  == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)) {
                day = 31;
            } else if ((month == 4 || month  == 6 || month == 9 || month == 11)) {
                day = 30;
            } else if (month == 2) {
                day = 28;
            }
        } else {
            if ((month == 1 || month  == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)) {
                day = 31;
            } else if ((month == 4 || month  == 6 || month == 9 || month == 11)) {
                day = 30;
            } else if (month == 2) {
                day = 29;
            }
        }
    } else { // day doesn't underflow, just subtract 1
        --day;
    }

    event.set(year, month, day);

    return event;
}

Event operator--(Event &event, int) {
    Event temp = event;
    int year = event.year();
    int month = event.month();
    int day = event.day();

    if (day == 1 && month == 1 && year == 1) { // invalid year range
        throw std::runtime_error("Year: " + std::to_string(year) + " is out of acceptable range.");
    } else if (day == 1 && month == 1) { // day underflows month -> month underflows year -> year still valid
        day = 31;
        month = 12;
        --year;
    } else if (day == 1) { // day underflows month
        --month;

        // check what day needs to be set to based on if its leap year or not.
        if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)) { // NOT A LEAP YEAR
            if ((month == 1 || month  == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)) {
                day = 31;
            } else if ((month == 4 || month  == 6 || month == 9 || month == 11)) {
                day = 30;
            } else if (month == 2) {
                day = 28;
            }
        } else {
            if ((month == 1 || month  == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)) {
                day = 31;
            } else if ((month == 4 || month  == 6 || month == 9 || month == 11)) {
                day = 30;
            } else if (month == 2) {
                day = 29;
            }
        }
    } else { // day doesn't underflow, just subtract 1
        --day;
    }

    event.set(year, month, day);

    return temp;
}

bool Event::operator==(const Event rhs) const {
    Event e(*this);
    if (e.day() == rhs.day() && e.month() == rhs.month() && e.year() == rhs.year()) return true;
    return false;
}

bool Event::operator!=(const Event rhs) const {
    Event e(*this);
    if (e.day() != rhs.day() || e.month() != rhs.month() || e.year() != rhs.year()) return true;
    return false;
}

bool Event::operator<=(const Event rhs) const {
    Event e(*this);

    if (e.day() == rhs.day() && e.month() == rhs.month() && e.year() == rhs.year()) return true;

    if (e.year() < rhs.year()) return true;
    else if (e.year() == rhs.year() && e.month() < rhs.month()) return true;
    else if (e.year() == rhs.year() && e.month() == rhs.month() && e.day() < rhs.day()) return true;
    else return false; 
}

bool Event::operator>=(const Event rhs) const {
    Event e(*this);

    if (e.day() == rhs.day() && e.month() == rhs.month() && e.year() == rhs.year()) return true;

    if (e.year() > rhs.year()) return true;
    else if (e.year() == rhs.year() && e.month() > rhs.month()) return true;
    else if (e.year() == rhs.year() && e.month() == rhs.month() && e.day() > rhs.day()) return true;
    else return false;
}

bool Event::operator<(const Event rhs) const {
    Event e(*this);
    if (e.year() < rhs.year()) return true;
    else if (e.year() == rhs.year() && e.month() < rhs.month()) return true;
    else if (e.year() == rhs.year() && e.month() == rhs.month() && e.day() < rhs.day()) return true;
    else return false; 
}

bool Event::operator>(const Event rhs) const {
    Event e(*this);
    if (e.year() > rhs.year()) return true;
    else if (e.year() == rhs.year() && e.month() > rhs.month()) return true;
    else if (e.year() == rhs.year() && e.month() == rhs.month() && e.day() > rhs.day()) return true;
    else return false; 
}

std::ostream &operator<<(std::ostream &os, Event &event) {
    return os << std::setfill('0') << std::setw(4) << event.year() << '-' << std::setfill('0') << std::setw(2) << event.month() << '-' << std::setfill('0') << std::setw(2) << event.day() << std::setfill(' ');
}

std::ostream &operator<<(std::ostream &os, const Event &event) {
    Event x(event);
    return os << std::setfill('0') << std::setw(4) << x.year() << '-' << std::setfill('0') << std::setw(2) << x.month() << '-' << std::setfill('0') << std::setw(2) << x.day() << std::setfill(' ');
}