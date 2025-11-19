#pragma once
#include "ISerializable.h"

class Entity : public ISerializable {
protected:
    int id;
public:
    virtual void display() const = 0;
    virtual ~Entity() {}
};
