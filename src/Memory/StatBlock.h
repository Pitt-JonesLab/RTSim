#pragma once

#include "Logging/Logging.h"

#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace NVM::Memory {

class StatValue {
    private:
    class Printable {
        public:
        virtual std::string print() const = 0;
        virtual std::unique_ptr<Printable> clone() const = 0;
    };

    template<typename T> class PrintableWrapper : public Printable {
        public:
        PrintableWrapper(T* vPtr) : valuePtr(vPtr) {}

        std::string print() const {
            std::stringstream ss;
            ss << *valuePtr;
            return ss.str();
        }

        std::unique_ptr<Printable> clone() const {
            return std::unique_ptr<Printable>(
                new PrintableWrapper<T>(valuePtr));
        }

        private:
        T* valuePtr;
    };

    std::unique_ptr<Printable> value;
    std::string name, unit;

    public:
    template<typename T>
    StatValue(T* v, std::string n, std::string u) :
        value(new PrintableWrapper<T>(v)),
        name(n),
        unit(u) {}

    StatValue(const StatValue& other) :
        value(other.value->clone()),
        name(other.name),
        unit(other.unit) {}

    StatValue& operator=(const StatValue& other) {
        value = other.value->clone();
        name = other.name;
        unit = other.unit;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream&, const StatValue&);
};

class StatBlock {
    private:
    std::string tag;
    std::vector<StatValue> values;
    std::vector<StatBlock> children;

    public:
    StatBlock(std::string t = "");

    template<typename T>
    void addStat(T* statPtr, std::string name, std::string unit = "") {
        values.emplace_back(statPtr, name, unit);
    }

    void addChild(StatBlock childBlock);
    void log() const;
    void log(std::ostream& out) const;
};

std::ostream& operator<<(std::ostream&, const StatValue&);

} // namespace NVM::Memory