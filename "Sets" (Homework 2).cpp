#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <math.h>

class Set
{
public:
	virtual bool belongToSet(unsigned int userNumber) const = 0; //Проверява дали естественото число принадлежи на множеството
	virtual void description(std::ostream& output) const = 0; //Метод, който извежда подходящо математическо описание на множеството в поток
	virtual ~Set() = default;
};

class NaturalNumbers : public Set
{
public:
	NaturalNumbers() {}
	void description(std::ostream& output) const override
	{
		output << "[0, +inf)";
	}
	bool belongToSet(unsigned int userNumber) const override
	{
		return userNumber >= 0; //return true винаги
	}
};

class EmptySet : public Set
{
public:
	EmptySet() {}
	void description(std::ostream& output) const override //трябва да се пренапише
	{
		output << "∅";
	}
	bool belongToSet(unsigned int userNumber) const override
	{
		return false;
	}
};

class Interval : public Set
{
	unsigned int lowerLimit, upperLimit;
public:
	Interval(unsigned int leftVal, unsigned int rightVal) //Отбелязваме границите на интервала с този конструктор
	{
		lowerLimit = leftVal;
		upperLimit = rightVal;
	}
	void description(std::ostream& output) const override
	{
		output << "[" << lowerLimit << "," << upperLimit << "]";
	}
	bool belongToSet(unsigned int userNumber) const override
	{
		return (userNumber >= lowerLimit && userNumber <= upperLimit);
	}
};

class Union : public Set
{
	const Set *firstSet, *secondSet;
public:
	Union(const Set& pSet_1, const Set& pSet_2)
	{
		firstSet = &pSet_1;
		secondSet = &pSet_2;
	}
	void description(std::ostream& output) const override
	{
		output << "{ a ∈ ";
		firstSet->description(std::cout);
		output << " } U { a ∈ ";
		secondSet->description(std::cout);
		output << " }";
	}
	bool belongToSet(unsigned int userNumber) const override
	{
		return (firstSet->belongToSet(userNumber) || secondSet->belongToSet(userNumber));
	}
	friend Union operator||(const Set& pSet_1, const Set& pSet_2);
};

class Intersect : public Set
{
	const Set *firstSet, *secondSet;
public:
	Intersect(const Set& pSet_1, const Set& pSet_2)
	{
		firstSet = &pSet_1;
		secondSet = &pSet_2;
	}
	void description(std::ostream& output) const override
	{
		output << "{ a ∈ ";
		firstSet->description(std::cout);
		output << " } ∩ { a ∈ ";
		secondSet->description(std::cout);
		output << " }";
	}
	bool belongToSet(unsigned int userNumber) const override
	{
		return (firstSet->belongToSet(userNumber) && secondSet->belongToSet(userNumber));
	}
	friend Intersect operator&&(const Set& pSet_1, const Set& pSet_2);
};

class Filter : public Set
{
	const Set *firstSet;
	std::string setConditionString;
	std::function<bool(const unsigned int)> setConditionFunction;
public:
	Filter(const Set& pSet_1, std::function<bool(const unsigned int)> pFunction, const std::string& pSetCondition)
	{
		firstSet = &pSet_1;
		setConditionFunction = pFunction;
		setConditionString = pSetCondition;
	}
	void description(std::ostream& output) const override
	{
		output << "{ a ∈ ";
		firstSet->description(std::cout);
		output << " | " << setConditionString << " }";
	}
	bool belongToSet(unsigned int userNumber) const override
	{
		return (firstSet->belongToSet(userNumber) && setConditionFunction(userNumber));
	}
};

class Series : public Set
{
	const Set* firstSet;
	std::string setConditionString;
	std::function<bool(const unsigned int)> setConditionFunction;
public:
	Series(const Set& pSet_1, std::function<bool(const unsigned int)> pFunction, const std::string& pSetCondition)
	{
		firstSet = &pSet_1;
		setConditionFunction = pFunction;
		setConditionString = pSetCondition;
	}
	void description(std::ostream& output) const override
	{
		output << "{ " << setConditionString;
		output << " | a ∈ ";
		firstSet->description(std::cout);
		output << " }";

	}
	bool belongToSet(unsigned int userNumber) const override
	{
		return setConditionFunction(userNumber); //ne raboti
	}
};

class Family : public Set
{
	unsigned int element;
	std::vector<const Set*> familyOfSets;
public:
	Family() = default;
	Family(const std::vector<Set*> pVectorSets, const unsigned int pElement)
	{
		element = pElement;
		for(Set* vec : pVectorSets)
		{
			if(vec->belongToSet(element))
			{
				familyOfSets.push_back(vec);
			}
		}
	}
	void description(std::ostream& output) const override
	{
		output << "{";
		for(size_t index = 0; index < familyOfSets.size(); index++)
		{
			familyOfSets[index]->description(std::cout);
			if(index < familyOfSets.size() - 1)
			{
				output << ", ";
			}
		}
		output << "}";
	}
	bool belongToSet(const unsigned int userNumber) const override
	{
		for(const Set* set : familyOfSets)
		{
			if(set->belongToSet(userNumber))
			{
				return true;
			}
		}
		return false;
	}
	void addSetToFamily(const Set* pSet)
	{
		familyOfSets.push_back(pSet);
	}
	Family& operator+=(const Set* pSet)
	{
		addSetToFamily(pSet);
		return *this;
	}
	Family operator+(const Set* pSet) const
	{
		Family result = *this;
		result.addSetToFamily(pSet);
		return result;
	}
};

Union operator||(const Set& pSet_1, const Set& pSet_2)
{
	return Union(pSet_1, pSet_2);
}

Intersect operator&&(const Set& pSet_1, const Set& pSet_2)
{
	return Intersect(pSet_1, pSet_2);
}


bool perfect_square(const unsigned int pNum) //working
{
	for(size_t index = 0; index < pNum; index++)
	{
		if(pNum == pow(index, 2))
		{
			return pNum;
		}
	}
	return 0;
}

bool prime(const unsigned int pNum) //working
{
	if(pNum < 2)
	{
		return false;
	}
	for(size_t index = 2; index < pNum; index++)
	{
		if(pNum % index == 0)
		{
			return false;
		}
	}
	return true;
}


int main()
{
	/*
	 * clone()
	 * move constructor
	 * virtual destructor
	 */
	unsigned int userNumber;
	std::cout << "Enter number: ";
	std::cin >> userNumber;

	NaturalNumbers natNumObj;
	EmptySet emptySetObj;
	Interval intrObj(11, 32);

	natNumObj.belongToSet(userNumber);
	emptySetObj.belongToSet(userNumber);
	intrObj.belongToSet(userNumber);

	natNumObj.description(std::cout);
	std::cout << "\n";
	emptySetObj.description(std::cout);
	std::cout << "\n";
	intrObj.description(std::cout);
	std::cout << "\n";

	Union united = Union(Interval(3, 15), Interval(9, 20) /*"x is in one of the two set"*/);
	Intersect intersection = Intersect(Interval(0, 12), Interval(9, 24) /*"x is an element of both sets"*/);
	Filter evens = Filter(Interval(12, 114), [](unsigned int pNum){ return (pNum % 2 == 0); }, "x is even");
	Filter odd = Filter(Interval(18, 58), [](unsigned int pNum){ return (pNum % 2 == 1); }, "x is odd");
	Filter primes = Filter(Interval(55, 125), prime, "x is prime");
	Filter squares = Filter(Interval(10, 25), perfect_square, "x is perfect square");
	Series cubics = Series(Interval(0, 10), [](unsigned int x){ return (x * x * x) == x; }, "x*x*x");

	Union primes_or_squares = primes || squares;
	Intersect evens_and_cubics = evens && cubics;

	united.description(std::cout);
	std::cout << "\n";
	intersection.description(std::cout);
	std::cout << "\n";
	evens.description(std::cout);
	std::cout << "\n";
	odd.description(std::cout);
	std::cout << "\n";
	primes.description(std::cout);
	std::cout << "\n";
	squares.description(std::cout);
	std::cout << "\n";
	cubics.description(std::cout);
	std::cout << "\n";
	primes_or_squares.description(std::cout);
	std::cout << "\n";
	evens_and_cubics.description(std::cout);
	std::cout << "\n\n";

	std::cout << "Does number " << userNumber << " belong to the union of the two sets?\n" << united.belongToSet(userNumber) << "\n";
	std::cout << "Does number " << userNumber << " belong to the intersection of the two sets?\n" << intersection.belongToSet(userNumber) << "\n";
	std::cout << "Does number " << userNumber << " belong to the set of even numbers?\n" << evens.belongToSet(userNumber) << "\n";
	std::cout << "Does number " << userNumber << " belong to the set of even numbers?\n" << odd.belongToSet(userNumber) << "\n";
	std::cout << "Does number " << userNumber << " belong to the set of prime numbers?\n" << primes.belongToSet(userNumber) << "\n";
	std::cout << "Does number " << userNumber << " belong to the set of cubic numbers?\n" << cubics.belongToSet(userNumber) << "\n";
	std::cout << "Does number " << userNumber << " belong to the sequence of perfect squares?\n" << squares.belongToSet(userNumber) << "\n";

	Family family;
	family += &evens;
	family += &primes;
	family += &united;
	family += &intersection;

	family.description(std::cout);
	return 0;
}

/*Special signs
 * ∈
 * ∉
 * ∅
 * ∪
 * ∩
 */
