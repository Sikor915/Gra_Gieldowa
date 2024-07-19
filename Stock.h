#pragma once
#include <string>

template<typename T>
class Stock
{
	T index, currency;
	double boughtPrice;

public:

	Stock(): index(""), boughtPrice(0), currency("") {}
	Stock(T& otherIndex): index(otherIndex), boughtPrice(0), currency("") {}
	Stock(T& otherIndex, double& otherPrice, T& otherCurr): index(otherIndex), boughtPrice(otherPrice), currency(otherCurr) {}
	Stock(T&& otherIndex, double&& otherPrice, T&& otherCurr) : index(otherIndex), boughtPrice(otherPrice), currency(otherCurr) 
	{
		otherIndex = "";
		otherPrice = 0;
		otherCurr = "";
	}
	~Stock()
	{

	}
	void setIndex(T& otherIndex)
	{
		index = otherIndex;
	}
	void setPrice(double& otherPrice)
	{
		boughtPrice = otherPrice;
	}
	void setCurr(T& otherCurr)
	{
		currency = otherCurr;
	}
	T getIndex() const
	{
		return index;
	}
	double getPrice() const
	{
		return boughtPrice;
	}
	T getCurr() const
	{
		return currency;
	}

	friend std::ostream& operator<<(std::ostream& out, const Stock<T>& stock)
	{
		out << stock.index << " " << stock.boughtPrice << " " << stock.currency;
		return out;
	}

	Stock& operator=(const Stock& other)
	{
		if (this != &other)
		{
			index = other.index;
			boughtPrice = other.boughtPrice;
			currency = other.currency;
		}
		return *this;
	}

	bool operator==(const Stock& other)
	{
		return index == other.index;//&& boughtPrice == other.boughtPrice && currency == other.currency;
	}

	bool operator<(const Stock& other)
	{
		return index < other.index;
	}

	bool operator>(const Stock& other)
	{
		return index > other.index;
	}
};