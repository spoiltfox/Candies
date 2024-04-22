#include <iostream>
#include <vector>
#include <list>
#include <memory>

using namespace std;

enum class SizeEnum : int {
    Small, Medium, Big, Large
};
std::ostream& operator<<(std::ostream& os, const SizeEnum& size) {
    switch (size) {
    case SizeEnum::Small: os << "Small"; break;
    case SizeEnum::Medium: os << "Medium"; break;
    case SizeEnum::Big: os << "Big"; break;
    case SizeEnum::Large: os << "Large"; break;
    default: os << "Unknown Size";
    }
    return os;
}

enum class TasteEnum : int {
    Vanilla, Strawberry, Choco, Pistacio
};

// Перегрузка оператора вывода для перечисления TasteEnum
std::ostream& operator<<(std::ostream& os, const TasteEnum& taste) {
    switch (taste) {
    case TasteEnum::Vanilla: os << "Vanilla"; break;
    case TasteEnum::Strawberry: os << "Strawberry"; break;
    case TasteEnum::Choco: os << "Choco"; break;
    case TasteEnum::Pistacio: os << "Pistacio"; break;
    default: os << "Unknown Taste";
    }
    return os;
}

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
    // Измененный конструктор, включающий параметр isEdible
    Chocolate(SizeEnum size, TasteEnum taste, bool isEdible = true) {
        Size = size;
        Taste = taste;
        Type = TypeEnum::Chocolate;
        CandyIsEdible = isEdible; // Устанавливаем съедобность из аргумента
        CandyIsCrushed = false;
    }
    void Eat() override {
        if (IsEdible()) {
            cout << "I ate it\n";
        }
        else {
            cout << "I didn't eat it\n";
        }
    }
    void Give() override {
        cout << "I gave it!\n";
    }
    void Crush() override {
        if (!IsCrushed()) {
            CandyIsCrushed = true;
            cout << "I crush it!\n";
        }
        else {
            cout << "already crushed\n";
        }
    }
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




#include <functional> // Для std::function

// Базовый класс декоратора для итератора
class IteratorDecorator : public Iterator {
protected:
    unique_ptr<Iterator> baseIterator; // Уникальный указатель на базовый итератор

public:
    IteratorDecorator(Iterator* iterator) : baseIterator(iterator) {}
    void First() override { baseIterator->First(); }
    void Next() override { baseIterator->Next(); }
    bool IsDone() const override { return baseIterator->IsDone(); }
    shared_ptr<Candy> GetCurrent() const override { return baseIterator->GetCurrent(); }
};

// Декоратор для фильтрации по съедобности
class FilterByEdible : public IteratorDecorator {
public:
    FilterByEdible(Iterator* iterator) : IteratorDecorator(iterator) {}
    void First() override {
        baseIterator->First();
        while (!IsDone() && !GetCurrent()->IsEdible()) {
            baseIterator->Next();
        }
    }
    void Next() override {
        baseIterator->Next();
        while (!IsDone() && !GetCurrent()->IsEdible()) {
            baseIterator->Next();
        }
    }
};

// Декоратор для фильтрации по вкусу
class FilterByTaste : public IteratorDecorator {
private:
    TasteEnum requiredTaste; // Требуемый вкус для фильтрации

public:
    FilterByTaste(Iterator* iterator, TasteEnum taste) : IteratorDecorator(iterator), requiredTaste(taste) {}
    void First() override {
        baseIterator->First();
        while (!IsDone() && GetCurrent()->GetTaste() != requiredTaste) {
            baseIterator->Next();
        }
    }
    void Next() override {
        baseIterator->Next();
        while (!IsDone() && GetCurrent()->GetTaste() != requiredTaste) {
            baseIterator->Next();
        }
    }
};

// Декоратор для фильтрации по размеру
class FilterBySize : public IteratorDecorator {
private:
    SizeEnum requiredSize; // Требуемый размер для фильтрации

public:
    FilterBySize(Iterator* iterator, SizeEnum size) : IteratorDecorator(iterator), requiredSize(size) {}

    void First() override {
        baseIterator->First();
        while (!IsDone() && GetCurrent()->GetSize() != requiredSize) {
            baseIterator->Next();
        }
    }

    void Next() override {
        baseIterator->Next();
        while (!IsDone() && GetCurrent()->GetSize() != requiredSize) {
            baseIterator->Next();
        }
    }
};


int main() {
    setlocale(LC_ALL, "RU");
    VectorContainer vc;
    vc.AddCandy(make_shared<Chocolate>(SizeEnum::Small, TasteEnum::Choco));
    vc.AddCandy(make_shared<Chocolate>(SizeEnum::Large, TasteEnum::Strawberry, false)); // Добавим некоторые несъедобные

    // Создаем итератор для VectorContainer
    unique_ptr<Iterator> it(vc.CreateIterator());

    // Оборачиваем итератор декоратором для фильтрации по съедобности
    FilterByEdible edibleFilter(it.release());
    for (edibleFilter.First(); !edibleFilter.IsDone(); edibleFilter.Next()) {
        auto candy = edibleFilter.GetCurrent();
        if (candy) {
            cout << "Съедобная конфета: " << candy->GetType() << endl;
        }
    }

    // Теперь демонстрация фильтрации по вкусу
    unique_ptr<Iterator> it2(vc.CreateIterator());
    FilterByTaste tasteFilter(it2.release(), TasteEnum::Choco);
    for (tasteFilter.First(); !tasteFilter.IsDone(); tasteFilter.Next()) {
        auto candy = tasteFilter.GetCurrent();
        if (candy) {
            cout << "Конфета вкуса Choco: " << candy->GetType() << endl;
        }
    }
    unique_ptr<Iterator> it3(vc.CreateIterator());

    // Фильтрация по размеру Medium
    FilterBySize sizeFilter(it3.release(), SizeEnum::Medium);
    cout << "VectorContainer содержит следующие конфеты среднего размера:" << endl;
    for (sizeFilter.First(); !sizeFilter.IsDone(); sizeFilter.Next()) {
        auto candy = sizeFilter.GetCurrent();
        if (candy) {
            cout << "Конфета размера Medium: " << candy->GetSize() << ", вкус: " << candy->GetTaste() << endl;
        }
    }


    return 0;
}
