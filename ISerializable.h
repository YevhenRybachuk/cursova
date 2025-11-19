#pragma once
#include <string>
using namespace std;

class ISerializable {
public:
    virtual void loadFromString(const string &data) = 0;
    virtual string toString() const = 0;
    virtual ~ISerializable() {}
};
