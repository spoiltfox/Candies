#include <iostream>
#include <vector>
#include <list>
#include <memory>

using namespace std;

enum class SizeEnum : int {
    Small, Medium, Big, Large
};

enum class TasteEnum : int {
    Banilla, Strawberry, Choco, Pistacio
};

enum class TypeEnum : int {
    Chocolate, Lollipop, Biscuit, Liquid
};

std::ostream& operator<<(std::ostream& os, const TypeEnum& type) {
    switch (type) {
    case TypeEnum::Chocolate: os << "Chocolate"; break;
    case TypeEnum::Lollipop: os << "Lollipop"; break;
    case TypeEnum::Biscuit: os << "Biscuit"; break;
    case TypeEnum::Liquid: os << "Liquid"; break;
    default: os << "Unknown Type";
    }
    return os;
}

class Candy {
protected:
    bool CandyIsEdible;
    bool CandyIsCrushed;
    SizeEnum Size;
    TasteEnum Taste;
    TypeEnum Type;
public:
    bool IsEdible() const { return CandyIsEdible; }
    bool IsCrushed() const { return CandyIsCrushed; }
    SizeEnum GetSize() const { return Size; }
    TypeEnum GetType() const { return Type; }
    TasteEnum GetTaste() const { return Taste; }
    virtual void Eat() = 0;
    virtual void Give() = 0;
    virtual void Crush() = 0;
};

class Chocolate : public Candy {
public:
    Chocolate(SizeEnum size, TasteEnum taste) {
        Size = size; Taste = taste; Type = TypeEnum::Chocolate;
        CandyIsEdible = true; CandyIsCrushed = false;
    }
    void Eat() override { if (IsEdible()) { cout << "I ate it\n"; } else { cout << "I didn't eat it\n"; } }
    void Give() override { cout << "I gave it!\n"; }
    void Crush() override { if (!IsCrushed()) { CandyIsCrushed = true; cout << "I crush it!\n"; } else { cout << "already crushed\n"; } }
};

class Iterator;

class Container {
public:
    virtual void AddCandy(shared_ptr<Candy> candy) = 0;
    virtual shared_ptr<Candy> GetCandy(int index) const = 0;
    virtual int GetSize() const = 0;
    virtual void RemoveCandy(int index) = 0;
    virtual ~Container() {}
    virtual Iterator* CreateIterator() const = 0;
};

class VectorContainer : public Container {
private:
    vector<shared_ptr<Candy>> candies;
public:
    void AddCandy(shared_ptr<Candy> candy) override {
        candies.push_back(candy);
    }
    shared_ptr<Candy> GetCandy(int index) const override {
        if (index >= 0 && index < candies.size()) {
            return candies[index];
        }
        return nullptr;
    }
    int GetSize() const override {
        return candies.size();
    }
    void RemoveCandy(int index) override {
        if (index >= 0 && index < candies.size()) {
            candies.erase(candies.begin() + index);
        }
    }
    Iterator* CreateIterator() const override;
};

class ListContainer : public Container {
private:
    list<shared_ptr<Candy>> candies;
public:
    friend class ListIterator; // Allow the iterator access to private members
    void AddCandy(shared_ptr<Candy> candy) override {
        candies.push_back(candy);
    }
    shared_ptr<Candy> GetCandy(int index) const override {
        if (index < 0 || index >= candies.size()) {
            return nullptr;
        }
        auto it = candies.begin();
        advance(it, index);
        return *it;
    }
    int GetSize() const override {
        return candies.size();
    }
    void RemoveCandy(int index) override {
        if (index < 0 || index >= candies.size()) {
            return;
        }
        auto it = candies.begin();
        advance(it, index);
        candies.erase(it);
    }
    Iterator* CreateIterator() const override;
};

// Iterator classes and other necessary parts...

class Iterator {
public:
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual shared_ptr<Candy> GetCurrent() const = 0;
    virtual ~Iterator() {}
};

class VectorIterator : public Iterator {
private:
    const VectorContainer* container;
    size_t current;
public:
    VectorIterator(const VectorContainer* cont) : container(cont), current(0) {}
    void First() override { current = 0; }
    void Next() override { if (!IsDone()) current++; }
    bool IsDone() const override { return current >= container->GetSize(); }
    shared_ptr<Candy> GetCurrent() const override { return container->GetCandy(current); }
};

class ListIterator : public Iterator {
private:
    const ListContainer* container;
    list<shared_ptr<Candy>>::const_iterator current;
public:
    ListIterator(const ListContainer* cont) : container(cont), current(cont->candies.begin()) {}
    void First() override { current = container->candies.begin(); }
    void Next() override { if (!IsDone()) ++current; }
    bool IsDone() const override { return current == container->candies.end(); }
    shared_ptr<Candy> GetCurrent() const override { return *current; }
};

// Implementations for CreateIterator functions
Iterator* VectorContainer::CreateIterator() const {
    return new VectorIterator(this);
}

Iterator* ListContainer::CreateIterator() const {
    return new ListIterator(this);
}

int main() {
    VectorContainer vc;
    vc.AddCandy(make_shared<Chocolate>(SizeEnum::Small, TasteEnum::Choco));
    vc.AddCandy(make_shared<Chocolate>(SizeEnum::Large, TasteEnum::Strawberry));

    ListContainer lc;
    lc.AddCandy(make_shared<Chocolate>(SizeEnum::Medium, TasteEnum::Banilla));
    lc.AddCandy(make_shared<Chocolate>(SizeEnum::Large, TasteEnum::Pistacio));

    unique_ptr<Iterator> it(vc.CreateIterator());
    cout << "VectorContainer contents:" << endl;
    for (it->First(); !it->IsDone(); it->Next()) {
        auto candy = it->GetCurrent();
        if (candy) {
            cout << "Found candy of type: " << candy->GetType() << endl;
        }
    }

    it.reset(lc.CreateIterator());
    cout << "ListContainer contents:" << endl;
    for (it->First(); !it->IsDone(); it->Next()) {
        auto candy = it->GetCurrent();
        if (candy) {
            cout << "Found candy of type: " << candy->GetType() << endl;
        }
    }

    return 0;
}
