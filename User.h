#pragma once
#include <vector>
#include <map>
#include "List.h"

template<typename T>
class User
{
	std::string login, password;
	DoublyLinkedList<std::pair<Stock<std::string>, int>> userStocks;
	double money;
public:
	User() : login(), password(), userStocks(), money() {}
	User(std::string& otherLogin, std::string& otherPassword) : login(otherLogin), password(otherPassword), userStocks(), money() {}
	User(std::string& otherLogin, std::string& otherPassword, DoublyLinkedList<std::pair<Stock<std::string>, int>>& otherStocks, double& otherMoney) : login(otherLogin), password(otherPassword), userStocks(otherStocks), money(otherMoney) {}
	User(std::string&& otherLogin, std::string&& otherPassword, DoublyLinkedList<std::pair<Stock<std::string>, int>>&& otherStocks, double&& otherMoney) : login(otherLogin), password(otherPassword), userStocks(otherStocks), money(otherMoney)
	{
		otherLogin = "";
		otherPassword = "";
		otherStocks.clear();
		otherMoney = 0;
	}
	void BuyStock(std::string& name, int quantity, std::vector<Stock<std::string>>& stocksList);
	void SellStock(std::string& name, int quantity, std::vector<Stock<std::string>>& stocksList);
	void SellStock(std::string& name, int quantity, double& price);
	void PrintStocks();
	void ReadStocks(std::string& fileName);
	double GetMoney() const 
	{ 
		return money; 
	}
	std::string GetLogin() const 
	{ 
		return login; 
	}
	DoublyLinkedList<std::pair<Stock<std::string>, int>>& GetStocks() 
	{
		return userStocks; 
	}
	int countAllStocks()
	{
		int sum{};
		auto temp = userStocks.getHead();
		while (temp != nullptr)
		{
			sum += temp->data.second;
			temp = temp->next;
		}
		return sum;
	}
	void SetMoney(double& otherMoney) 
	{ 
		money = otherMoney; 
	}
	void SetStocks(DoublyLinkedList<std::pair<Stock<std::string>, int>>& otherStocks) 
	{ 
		userStocks = otherStocks; 
	}
	friend std::ostream& operator<<(std::ostream& out, const DoublyLinkedList<std::pair<Stock<std::string>, int>>& list)
	{
		for (auto& stock : list)
		{
			out << stock.data.first << " " << stock.data.second << std::endl;
		}
		return out;
	}
	~User() {}
};


template<typename T>
inline void User<T>::BuyStock(std::string& name, int quantity, std::vector<Stock<std::string>>& stocksList)
{
	for (auto& stock : stocksList)
	{
		if (stock.getIndex() == name)
		{
			if (stock.getCurr() == "PLN" || stock.getCurr() == "USD") 
			{
				double stockPrice = stock.getPrice() * quantity;
				if (stock.getCurr() == "USD") 
				{
					stockPrice *= 3.9;  
				}

				if (money >= stockPrice)
				{
					auto existingStock = userStocks.Find(stock);
					if (existingStock != userStocks.EndList())
					{
						existingStock->data.second += quantity;
					}
					else
					{
						userStocks.Insert(std::make_pair(stock, quantity));
					}

					money -= stockPrice;
				}
				else
				{
					std::cout << "Nie masz wystarczajaco funduszy!" << std::endl;
					system("pause");
				}
			}
			else
			{
				std::cout << "Nie wspieramy waluty akcji ktora chcesz kupic" << std::endl;
				system("pause");
			}
		}
	}
	//userStocks.sort();
	//std::sort(userStocks.begin(), userStocks.end());
	/*for (auto It = userStocks.begin(); It != userStocks.end(); It++)
	{
		std::cout << It->first << " " << It->second << std::endl;
	}*/

}

template<typename T>
inline void User<T>::SellStock(std::string& name, int quantity, std::vector<Stock<std::string>>& stocksList)
{
	for (auto& stock : stocksList)
	{
		if (stock.getIndex() == name)
		{
			auto existingStock = userStocks.Find(stock);
			if (existingStock != userStocks.EndList())
			{
				if (existingStock->data.second >= quantity)
				{
					existingStock->data.second -= quantity;

					if (existingStock->data.second == 0)
					{
						userStocks.erase(existingStock->data.first);
					}

					double stockPrice = stock.getPrice() * quantity;
					if (stock.getCurr() == "USD")
					{
						stockPrice *= 3.9;
					}
					money += stockPrice;
				}
				else
				{
					std::cout << "Nie masz az tyle zakupionych akcji!" << std::endl;
					system("pause");
				}
			}
			else
			{
				std::cout << "Nie posiadasz akcji tej spolki!" << std::endl;
				system("pause");
			}
		}
	}
	//userStocks.sort();
}

template<typename T>
inline void User<T>::SellStock(std::string& name, int quantity, double& price)
{
	auto existingStock = userStocks.Find(name);
	if (existingStock and existingStock->data.second >= quantity)
	{
		existingStock->data.second -= quantity;

		if (existingStock->data.second == 0)
		{
			userStocks.erase(existingStock->data.first);
		}
		if (existingStock->data.first.getCurr() == "USD")
		{
			price *= 3.9;
		}
		double moneyGot = price * quantity;
		money += moneyGot;
		std::cout << "Akcje zostaly sprzedane za " << price << " PLN"  << " kazda" << " pobrane z API. W sumie zyskales "  << moneyGot << " PLN" << std::endl;
		system("pause");
	}
	else
	{
		std::cout << "Nie masz az tyle zakupionych akcji!" << std::endl;
		system("pause");
	}
	//userStocks.sort();
}

template<typename T>
inline void User<T>::PrintStocks()
{
	userStocks.display();
}

template<typename T>
inline void User<T>::ReadStocks(std::string& fileName)
{
	std::ifstream fileRead(fileName);
	if (fileRead.is_open())
	{
		std::string line;
		std::getline(fileRead, line);
		if(line != "")
		{
			std::istringstream ss(line);
			std::string name, curr, moneySTR, allStocksString, priceSTR;
			double price{};
			ss >> moneySTR >> curr >> allStocksString;
			int quantity{}, allStocks{ std::stoi(allStocksString) };
			money = std::stod(moneySTR);
			while (allStocks > 0)
			{
				static std::string stockCount1;
				ss >> stockCount1 >> name >> priceSTR >> curr;
				price = std::stod(priceSTR);
				quantity = std::stoi(stockCount1);
				Stock<std::string> stock(name, price, curr);
				userStocks.Insert(std::make_pair(stock, quantity));
				allStocks -= quantity;
			}

		}
		else
		{
			money = 1000;
		}
	}
	else
	{
		money = 1000;
		std::cout << "Ustawiono poczatkowa ilosc pieniedzy na 1000 PLN" << std::endl;
		system("pause");
	}
	fileRead.close();
}

