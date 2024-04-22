#include <iostream>
#ifndef MainH
#define MainH
using namespace std;
enum class SizeEnum :int
{
	Small,
	Medium,
	Big,
	Large
};
enum class TasteEnum :int
{
	Banilla,
	Strawberry,
	Choco,
	Pistacio,
};
enum class TypeEnum :int
{
	Chocolate,
	Lollipop,
	Biscuit,
	Liquid
};
class Candy
{
protected:
	bool CandyIsEdible;
	bool CandyIsCrushed;
	SizeEnum Size;
	TasteEnum Taste;
	TypeEnum Type;
public:
	bool IsEdible() const { return CandyIsEdible; }
	bool IsCrushed() const { return CandyIsCrushed; }
	virtual SizeEnum GetSize() { return Size; }
	virtual TypeEnum GetType() { return Type; }
	virtual TasteEnum GetTaste() { return Taste; }
	virtual void Eat() = 0;
	virtual void Give() = 0;
	virtual void Crush() = 0;
};
class Chocolate :public Candy
{
public:
	Chocolate(SizeEnum size, TasteEnum taste) :Candy() { Size = size; Taste = taste; }
	TypeEnum GetType() { return TypeEnum::Chocolate; }
	void Eat() { if (IsEdible()) { cout << "i ate it\n"; } else { cout << "i didnt ate it\n"; } }
	void Give() { cout << "i gave it!\n"; }
	void Crush() { if (!(IsCrushed())) { cout << "i crush it!\n"; } else { cout << "already crushed\n"; } }
};
class Lollipop :public Candy
{
public:
	Lollipop(SizeEnum size, TasteEnum taste) :Candy() { Size = size; Taste = taste; }
	TypeEnum GetType() { return TypeEnum::Lollipop; }
	void Eat() { if (IsEdible()) { cout << "i ate it\n"; } else { cout << "i didnt ate it\n"; } }
	void Give() { cout << "i gave it!\n"; }
};
class Biscuit :public Candy
{
public:
	Biscuit(SizeEnum size, TasteEnum taste) :Candy() { Size = size; Taste = taste; }
	TypeEnum GetType() { return TypeEnum::Biscuit; }
	void Eat() { if (IsEdible()) { cout << "i ate it\n"; } else { cout << "i didnt ate it\n"; } }
	void Give() { cout << "i gave it!\n"; }
};
#endif

int main()
{
	setlocale(LC_ALL, "Ru");
	Chocolate Alenka(SizeEnum::Small, TasteEnum::Strawberry);
	Alenka.Crush();
	Alenka.Eat();
	return 0;
}
