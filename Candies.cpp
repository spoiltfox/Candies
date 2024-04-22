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

// Перегрузка оператора вывода для SizeEnum (если вам это также нужно)
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

// Оставшиеся классы и функции ...

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
	SizeEnum GetSize() { return Size; }
	TypeEnum GetType() { return Type; }
	TasteEnum GetTaste() { return Taste; }
	virtual void Eat() = 0;
	virtual void Give() = 0;
	virtual void Crush() = 0;
};

class Chocolate : public Candy {
public:
	Chocolate(SizeEnum size, TasteEnum taste) : Candy() { Size = size; Taste = taste; Type = TypeEnum::Chocolate; }
	void Eat() { if (IsEdible()) { cout << "I ate it\n"; } else { cout << "I didn't eat it\n"; } }
	void Give() { cout << "I gave it!\n"; }
	void Crush() { if (!IsCrushed()) { CandyIsCrushed = true; cout << "I crush it!\n"; } else { cout << "already crushed\n"; } }
};



class Container {
public:
	// Добавление объекта в контейнер
	virtual void AddCandy(shared_ptr<Candy> candy) = 0;

	// Получение объекта по индексу
	virtual shared_ptr<Candy> GetCandy(int index) = 0;

	// Получение размера контейнера (количество элементов)
	virtual int GetSize() const = 0;

	// Удаление объекта по индексу
	virtual void RemoveCandy(int index) = 0;

	// Виртуальный деструктор для корректного удаления объектов производных классов
	virtual ~Container() {}
};

// Контейнер, использующий std::vector для хранения элементов
class VectorContainer : public Container {
private:
	vector<shared_ptr<Candy>> candies;  // Вектор для хранения конфет

public:
	void AddCandy(shared_ptr<Candy> candy) override {
		candies.push_back(candy);  // Добавление конфеты в конец вектора
	}

	shared_ptr<Candy> GetCandy(int index) override {
		if (index >= 0 && index < candies.size())
			return candies[index];
		return nullptr;  // Возвращаем nullptr, если индекс некорректен
	}

	int GetSize() const override {
		return candies.size();  // Возвращаем размер вектора
	}

	void RemoveCandy(int index) override {
		if (index >= 0 && index < candies.size())
			candies.erase(candies.begin() + index);  // Удаление элемента по индексу
	}
};

// Контейнер, использующий std::list для хранения элементов
class ListContainer : public Container {
private:
	list<shared_ptr<Candy>> candies;  // Список для хранения конфет

public:
	void AddCandy(shared_ptr<Candy> candy) override {
		candies.push_back(candy);  // Добавление конфеты в конец списка
	}

	shared_ptr<Candy> GetCandy(int index) override {
		if (index < 0 || index >= candies.size())
			return nullptr;
		auto it = candies.begin();
		advance(it, index);  // Перемещаем итератор на нужный элемент
		return *it;
	}

	int GetSize() const override {
		return candies.size();  // Возвращаем размер списка
	}

	void RemoveCandy(int index) override {
		if (index < 0 || index >= candies.size())
			return;
		auto it = candies.begin();
		advance(it, index);  // Перемещаем итератор на нужный элемент
		candies.erase(it);  // Удаление элемента
	}
};
//#endif



int main()
{
	
	setlocale(LC_ALL, "Ru");
	Chocolate Alenka(SizeEnum::Small, TasteEnum::Strawberry);
	Alenka.Crush();
	Alenka.Eat(); 

	VectorContainer vc;
	ListContainer lc;

	// Создаем экземпляры Chocolate с помощью make_shared для управления памятью
	shared_ptr<Candy> choco1 = make_shared<Chocolate>(SizeEnum::Small, TasteEnum::Choco);
	shared_ptr<Candy> choco2 = make_shared<Chocolate>(SizeEnum::Medium, TasteEnum::Strawberry);

	// Добавление конфет в контейнеры
	vc.AddCandy(choco1);
	lc.AddCandy(choco2);
	cout << "Vector Container holds: " << vc.GetSize() << " items." << endl;
	cout << "List Container holds: " << lc.GetSize() << " items." << endl;
	cout << "Vector Container first item is a Chocolate: " << vc.GetCandy(0)->GetType() << endl;
	cout << "List Container first item is a Chocolate: " << lc.GetCandy(0)->GetType() << endl;

	return 0;
}
