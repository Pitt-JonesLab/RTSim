#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace NVM::Stats {

// TODO: Stats should take by value
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

    std::string getName() const { return name; }

    friend std::ostream& operator<<(std::ostream&, const StatValue&);
};

class ValueStatValue {
    private:
    class Stat {
        public:
        virtual std::string print() const = 0;
        virtual std::unique_ptr<Stat> clone() const = 0;
        virtual Stat& operator+(Stat& rhs) = 0;
    };

    template<typename T> class PrintableWrapper : public Stat {
        public:
        PrintableWrapper(T v) : value(v) {}

        std::string print() const {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }

        std::unique_ptr<Stat> clone() const {
            return std::unique_ptr<Stat>(new PrintableWrapper<T>(value));
        }

        Stat& operator+(Stat& rhs) {
            value += dynamic_cast<PrintableWrapper<T>&>(rhs).value;
            return *this;
        }

        private:
        T value;
    };

    std::unique_ptr<Stat> value;
    std::string name, unit;

    public:
    template<typename T>
    ValueStatValue(T v, std::string n, std::string u) :
        value(new PrintableWrapper<T>(v)),
        name(n),
        unit(u) {}

    ValueStatValue(const ValueStatValue& other) :
        value(other.value->clone()),
        name(other.name),
        unit(other.unit) {}

    ValueStatValue& operator=(const ValueStatValue& other) {
        value = other.value->clone();
        name = other.name;
        unit = other.unit;
        return *this;
    }

    std::string getName() const { return name; }

    std::string getUnit() const { return unit; }

    ValueStatValue& operator+(ValueStatValue& rhs) {
        *value = *value + *rhs.value;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream&, const ValueStatValue&);
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
    void addChildStat(StatBlock childBlock, std::string name,
                      std::string unit = "");
    void log() const;
    void log(std::ostream& out) const;
};

class ValueStatBlock {
    private:
    std::string tag;
    std::vector<ValueStatValue> values;
    std::vector<ValueStatBlock> children;

    public:
    ValueStatBlock(std::string t = "");

    ValueStatBlock& operator+=(ValueStatBlock& rhs);

    void setTag(std::string t);

    template<typename T>
    void addStat(T stat, std::string name, std::string unit = "") {
        values.emplace_back(stat, name, unit);
    }

    void addChild(ValueStatBlock childBlock);
    void addChildStat(ValueStatBlock childBlock, std::string name,
                      std::string unit = "");
    void log() const;
    void log(std::string parentTag) const;
};

std::ostream& operator<<(std::ostream&, const StatValue&);
std::ostream& operator<<(std::ostream&, const ValueStatValue&);

} // namespace NVM::Stats