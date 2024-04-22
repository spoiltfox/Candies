#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <random>
#include <ctime>

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



class Lollipop : public Candy {
public:
    Lollipop(SizeEnum size, TasteEnum taste, bool isEdible = true) {
        Size = size; Taste = taste; Type = TypeEnum::Lollipop;
        CandyIsEdible = isEdible; CandyIsCrushed = false;
    }
    void Eat() override { if (IsEdible()) { cout << "I ate the lollipop\n"; } else { cout << "I didn't eat the lollipop\n"; } }
    void Give() override { cout << "I gave the lollipop!\n"; }
    void Crush() override { if (!IsCrushed()) { CandyIsCrushed = true; cout << "I crushed the lollipop\n"; } else { cout << "already crushed\n"; } }
};

class Biscuit : public Candy {
public:
    Biscuit(SizeEnum size, TasteEnum taste, bool isEdible = true) {
        Size = size; Taste = taste; Type = TypeEnum::Biscuit;
        CandyIsEdible = isEdible; CandyIsCrushed = false;
    }
    void Eat() override { if (IsEdible()) { cout << "I ate the biscuit\n"; } else { cout << "I didn't eat the biscuit\n"; } }
    void Give() override { cout << "I gave the biscuit!\n"; }
    void Crush() override { if (!IsCrushed()) { CandyIsCrushed = true; cout << "I crushed the biscuit\n"; } else { cout << "already crushed\n"; } }
};

class LiquidCandy : public Candy {
public:
    LiquidCandy(SizeEnum size, TasteEnum taste, bool isEdible = true) {
        Size = size; Taste = taste; Type = TypeEnum::Liquid;
        CandyIsEdible = isEdible; CandyIsCrushed = false;  // Предположим, что жидкая конфета не может быть "раздавлена"
    }
    void Eat() override { if (IsEdible()) { cout << "I drank the liquid candy\n"; } else { cout << "I didn't drink the liquid candy\n"; } }
    void Give() override { cout << "I gave the liquid candy!\n"; }
    void Crush() override { cout << "You can't really crush a liquid candy!\n"; }
};
class Iterator {
public:
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual shared_ptr<Candy> GetCurrent() const = 0;
    virtual ~Iterator() {}
};
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
    friend class ListIterator; // Разрешает итератору доступ к закрытым элементам
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

// Классы итераторов и другие необходимые части...

//class Iterator {
//public:
//    virtual void First() = 0;
//    virtual void Next() = 0;
//    virtual bool IsDone() const = 0;
//    virtual shared_ptr<Candy> GetCurrent() const = 0;
//    virtual ~Iterator() {}
//};

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

// Реализации функций CreateIterator
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






class CandyFactory {
public:
    // Функция для создания конфеты случайного типа и характеристик
    static shared_ptr<Candy> CreateRandomCandy() {
        static std::mt19937 rng(time(nullptr)); // Генератор случайных чисел
        std::uniform_int_distribution<int> dist(0, 3); 
        SizeEnum sizes[] = { SizeEnum::Small, SizeEnum::Medium, SizeEnum::Big, SizeEnum::Large };
        TasteEnum tastes[] = { TasteEnum::Vanilla, TasteEnum::Strawberry, TasteEnum::Choco, TasteEnum::Pistacio };
        TypeEnum types[] = { TypeEnum::Chocolate, TypeEnum::Lollipop, TypeEnum::Biscuit, TypeEnum::Liquid };

        SizeEnum size = sizes[dist(rng)];
        TasteEnum taste = tastes[dist(rng)];
        TypeEnum type = types[dist(rng)];

        switch (type) {
        case TypeEnum::Chocolate:
            return make_shared<Chocolate>(size, taste);
        case TypeEnum::Lollipop:
            return make_shared<Lollipop>(size, taste);
        case TypeEnum::Biscuit:
            
            return make_shared<Biscuit>(size, taste);
        case TypeEnum::Liquid:
            
            return make_shared<LiquidCandy>(size, taste);
        default:
            return nullptr;
        }
    }
};


int main() {
    setlocale(LC_ALL, "RU");
    VectorContainer vc;
    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<int> countDist(10, 35); // Количество конфет от 10 до 35
    vc.AddCandy(std::make_shared<Chocolate>(SizeEnum::Small, TasteEnum::Choco, true));
    vc.AddCandy(std::make_shared<Lollipop>(SizeEnum::Large, TasteEnum::Strawberry, false));
    vc.AddCandy(std::make_shared<Biscuit>(SizeEnum::Medium, TasteEnum::Vanilla, true));
    vc.AddCandy(std::make_shared<LiquidCandy>(SizeEnum::Large, TasteEnum::Pistacio, true));



    int candyCount = countDist(rng);
    cout << "Добавляем " << candyCount << " конфет в контейнер:" << endl;
    for (int i = 0; i < candyCount; ++i) {
        auto candy = CandyFactory::CreateRandomCandy();
        vc.AddCandy(candy);
        cout << "Добавлена конфета: " << candy->GetType() << ", размер: " << candy->GetSize() << ", вкус: " << candy->GetTaste() << endl;
    }

 

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
    cout << "Контейнер содержит следующие конфеты среднего размера:" << endl;
    for (sizeFilter.First(); !sizeFilter.IsDone(); sizeFilter.Next()) {
        auto candy = sizeFilter.GetCurrent();
        if (candy) {
            cout <<"Конфета типа: " << candy->GetType() << ", вкус: " << candy->GetTaste() << endl;
        }
    }


    // Демонстрация содержимого контейнера
    unique_ptr<Iterator> it4(vc.CreateIterator());
    cout << "Содержимое контейнера:" << endl;
    for (it4->First(); !it4->IsDone(); it4->Next()) {
        auto candy = it4->GetCurrent();
        if (candy) {
            cout << "Конфета типа: " << candy->GetType() << ", размер: " << candy->GetSize() << ", вкус: " << candy->GetTaste() << endl;
        }
    }




    return 0;
}


