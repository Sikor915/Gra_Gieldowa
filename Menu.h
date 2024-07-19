#pragma once
#include "CurlAPI.h"
#include <stdlib.h>
#include <Windows.h>
#include "DataLock.h"
#include <stdio.h>
#include <filesystem>
#include <cstdio>

template<typename T>
class Menu 
{
	std::string answer, login, password;
public:
	Menu();
	~Menu();
	void DrawMenu();
	void Login();
	void RegisterUser();
	void GameMenu(User<std::string>& user);
	bool hasStockWithIndex(const std::vector<Stock<std::string>>& stocksList, const std::string& targetIndex);
	void UpdateUsers(User<std::string>& user);
};

template<typename T>
inline Menu<T>::Menu()
{
	answer = "";
}

template<typename T>
inline Menu<T>::~Menu()
{
}

template<typename T>
inline void Menu<T>::DrawMenu()
{
	std::cout << "       $$$$$     " << std::endl;
	std::cout << "       $:::$     " << std::endl;
	std::cout << "   $$$$$:::$$$$$$" << std::endl;
	std::cout <<"  $$::::::::::::::$" << std::endl;
	std::cout <<" $:::::$$$$$$$::::$" << std::endl;
	std::cout <<" $::::$       $$$$$" << std::endl;
	std::cout <<" $::::$" << std::endl;
	std::cout <<" $::::$" << std::endl;
	std::cout <<" $:::::$$$$$$$$$" << std::endl;
	std::cout <<" $$::::::::::::$$" << std::endl;
	std::cout <<"   $$$$$$$$$:::::$" << std::endl;
	std::cout <<"            $::::$" << std::endl;
	std::cout <<"            $::::$" << std::endl;
	std::cout <<"$$$$$       $::::$" << std::endl;
	std::cout <<"$::::$$$$$$$:::::$" << std::endl;
	std::cout <<"$::::::::::::::$$" << std::endl;
	std::cout <<" $$$$$$:::$$$$$" << std::endl;
	std::cout <<"      $:::$" << std::endl;
	std::cout <<"      $$$$$" << std::endl;

	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "Witam w Symulatorze gry na gieldzie 3000!" << std::endl;
	std::cout << "Prosze wybrac czynnosc: \n";
	std::cout << "1. Logowanie\n2. Rejestracja\n3. Wyjscie" << std::endl;
	std::cin >> answer;
	if (answer == "1" or answer == "Logowanie" or answer == "logowanie")
	{
		Login();
	}
	else if (answer == "2" or answer == "Rejestracja" or answer == "rejestracja")
	{
		RegisterUser();
	}
	else if (answer == "3" or answer == "Wyjscie" or answer == "wyjscie")
	{
		system("CLS");
		std::cout << "Nastepuje wyjscie z programu. See you on the flip side" << std::endl;
	}
	else
	{
		std::cout << "Podano zla czynnosc. Nastepuje autodestrukcja programu" << std::endl;
	}


}

template<typename T>
inline void Menu<T>::Login()
{
	bool hasLoggedIn(false);
	std::cout << "Logowanie" << std::endl;
	std::cout << "Podaj login: ";
	std::cin >> login;
	std::cout << std::endl;
	std::ifstream fileCheck("users.txt");
	if (fileCheck.is_open())
	{
		std::string line{}, stringLogin{}, stringPassword{}, stringMoney{}, currency{}, stockCountString{};
		double money{};
		int stockCount{};

		while (std::getline(fileCheck, line))
		{
			std::istringstream iss(line);
			iss >> stringLogin >> stringPassword >> stringMoney >> currency >> stockCountString;
			if (stringMoney != "")
			{
				money = std::stod(stringMoney);
			}

			if (stockCountString != "")
			{
				stockCount = std::stoi(stockCountString);
			}

			if (login == stringLogin)
			{
				std::cout << "Podaj haslo: ";
				std::cin >> password;
				if (password == stringPassword)
				{

					User<std::string> user(login, password);
					user.SetMoney(money);
					std::string fileLogin = boost::to_lower_copy(login) + "_stocks.txt";

					//DoublyLinkedList<std::pair<Stock<std::string>, int>> otherStocks;
					/*while (stockCount > 0)
					{
						static std::string stockCount1{}, stockIndex{}, stockPrice{}, stockCurrency{};
						iss >> stockCount1 >> stockIndex >> stockPrice >> stockCurrency;

						double stockPricedouble = std::stod(stockPrice);

						Stock<std::string> stock(stockIndex, stockPricedouble, stockCurrency);
						otherStocks.pushBack(std::make_pair(stock, std::stoi(stockCount1)));
						
						int stockCount2 = std::stoi(stockCount1);
						stockCount -= stockCount2;
					}*/
					//user.SetStocks(otherStocks);

					user.ReadStocks(fileLogin);
					system("CLS");
					std::cout << "Udane logowanie" << std::endl;
					hasLoggedIn = true;
					system("pause");
					GameMenu(user);
					break;
				}
				else
				{
					std::cout << "Podano zle haslo" << std::endl;
					break;
				}
				break;
			}
		}
		if (!hasLoggedIn)
		{
			std::cout << "Jezeli tutaj dotarles, to znaczy, ze nie ma twojego konta" << std::endl;
			std::cout << "Nastepuje wyjscie z programu" << std::endl;
			system("pause");
		}
	}
	else
	{
		std::cerr << "ERROR" << std::endl;
	}
	fileCheck.close();
}

template<typename T>
inline void Menu<T>::RegisterUser()
{
	bool isRegistered(false), isTaken(false);
	double startMoney = 1000;
	std::cout << "Rejestracja uzytkownika" << std::endl;
	std::cout << "Login: ";
	std::cin >> login;
	std::cout << std::endl << "Haslo: ";
	std::cin >> password;
	std::cout << std::endl;

	std::ifstream fileCheck("users.txt");
	if (fileCheck.is_open())
	{
		std::string line{}, stringLogin{};
		while (std::getline(fileCheck, line))
		{
			std::istringstream iss(line);
			iss >> stringLogin;
			if (login == stringLogin)
			{
				std::cout << "Podany login jest juz zajety" << std::endl;
				isTaken = true;
				break;
			}
		}
	}
	fileCheck.close();

	std::fstream userOS("users.txt", std::ios::app);
	if (userOS.is_open() and !isTaken)
	{
		
		userOS << login << " " << password << " " << startMoney << " PLN" << std::endl;
		isRegistered = true;
	}
	else
	{
		std::cerr << "Error" << std::endl;
	}
	userOS.close();

	if (isRegistered)
	{
		DoublyLinkedList<std::pair<Stock<std::string>, int>> otherStocks;
		User<std::string> user(login, password, otherStocks, startMoney);
		GameMenu(user);
	}

}

template<typename T>
inline void Menu<T>::GameMenu(User<std::string>& user)
{
	
	system("CLS");
	API<std::string> apiController;
	std::vector<Stock<std::string>> stockVec;
	SharedData dataLockCheck;
	EndProgram endProgramLock;
	apiController.getIndexes();
	dataLockCheck.newDataAvailable = true;

	std::thread apiThread([&apiController, &stockVec, &dataLockCheck, &endProgramLock]() 
		{
		apiController.GetAPI(stockVec, dataLockCheck, endProgramLock);
		}
	);

	while (true)
	{

		Sleep(2000);

		std::cout << "Pobieranie akcji z gieldy..." << std::endl;
		
		while (dataLockCheck.newDataAvailable)
		{
			std::cout << "." << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		system("CLS");

		for (const auto& stock : stockVec)
		{
			std::cout << stock << std::endl << std::endl;
		}

		std::cout << "Co chcesz zrobic?" << std::endl;
		std::cout << "1. Kupic akcje\n2. Sprzedac akcje\n3. Wyswietlic posiadane akcje\n4. Nic nie robie\n5. Wyjscie" << std::endl;
		std::cout << "Stan konta: " << user.GetMoney() << " PLN" << std::endl;
		std::cin >> answer; 
		if (answer == "1" or answer == "Kupic akcje" or answer == "kupic akcje")
		{
			std::cout << "Kupowanie akcji" << std::endl;
			std::cout << "Podaj nazwe akcji: ";
			std::cin >> answer;
			boost::to_upper(answer);
			std::cout << std::endl;
			std::cout << "Podaj ilosc akcji: ";
			int amount;
			std::cin >> amount;
			std::cout << std::endl;
			user.BuyStock(answer, amount, stockVec);
		}
		else if (answer == "2" or answer == "Sprzedac akcje" or answer == "sprzedac akcje")
		{
			std::cout << "Sprzedawanie akcji" << std::endl;
			std::cout << "Podaj nazwe akcji: ";
			std::cin >> answer;
			boost::to_upper(answer);
			std::cout << std::endl;
			std::cout << "Podaj ilosc akcji: ";
			int amount;
			std::cin >> amount;
			std::cout << std::endl;

			bool foundStock = hasStockWithIndex(stockVec, answer);
			if (!foundStock)
			{
				double Price{};
				apiController.GetAPI(answer, Price);
				user.SellStock(answer, amount, Price);

			}
			else
			{
				user.SellStock(answer, amount, stockVec);
			}
		}
		else if (answer == "3" or answer == "Wyswietlic posiadane akcje" or answer == "wyswietlic posiadane akcje")
		{
			std::cout << "Ilosc zakupionych akcji: " << user.countAllStocks() << std::endl;
			std::cout << "Zakupione akcje: " << std::endl;
			user.PrintStocks();
			system("pause");
		}
		else if (answer == "4" or answer == "Nic nie robie" or answer == "nic nie robie")
		{

		}
		else if (answer == "5" or answer == "Wyjscie" or answer == "wyjscie")
		{
			std::cout << "Nastepuje wyjscie z programu. See you on the flip side" << std::endl;
			endProgramLock.hasEnded = true;
			std::lock_guard<std::mutex> Endlock(endProgramLock.EndProgramMutex);
			break;
		}
		else
		{
			std::cout << "Podano zla czynnosc." << std::endl;
			system("pause");
		}
		system("CLS");
	}


	UpdateUsers(user);
	apiThread.join();
}

template<typename T>
inline bool Menu<T>::hasStockWithIndex(const std::vector<Stock<std::string>>& stocksList, const std::string& targetIndex)
{
	bool hasStock = false;
	for (int i = 0; i < stocksList.size(); i++)
	{
		if (stocksList[i].getIndex() == targetIndex)
		{
			hasStock = true;
		}
	}
	return hasStock;
}


template<typename T>
inline void Menu<T>::UpdateUsers(User<std::string>& user)
{
	//std::ifstream fileCheck("users.txt");
	/*std::fstream userUpdate("usersTemp.txt", std::ios::app);*/
	std::string loginString = boost::to_lower_copy(user.GetLogin()) + "_stocks.txt";
	std::ofstream userUpdate(loginString);

	//if (fileCheck.is_open() and userUpdate.is_open())
	//{
	//	std::string line{}, stringLogin{}, stringPassword{}, stringMoney{}, currency{}, stockCountString{};
	//	bool foundUser(false), updatedUser(false);
	//	int stockCount{};

	//	while (!fileCheck.eof())
	//	{
	//		std::getline(fileCheck, line);
	//		if (line == "")
	//		{
	//			std::cout << "Koniec pliku" << std::endl;
	//			break;
	//		}
	//		std::istringstream iss(line);
	//		iss >> stringLogin >> stringPassword >> stringMoney >> currency >> stockCountString;

	//		if (stockCountString != "")
	//		{
	//			stockCount = std::stoi(stockCountString);
	//		}

	//		if (stringLogin == login and stringPassword == password)
	//		{
	//			foundUser = true;
	//		}

	//		if (userUpdate.is_open())
	//		{
	//			if (foundUser and !updatedUser)
	//			{
	//				const auto temphead = user.GetStocks().getHead();
	//				auto current = temphead;
	//				userUpdate << stringLogin << " " << stringPassword << " " << user.GetMoney() << " " + currency + " " << user.countAllStocks() << " ";
	//				while (current != nullptr)
	//				{
	//					userUpdate << current->data << " ";
	//					current = current->next;
	//				}
	//				userUpdate << std::endl;
	//				updatedUser = true;
	//				stringLogin.clear();
	//				stringPassword.clear();
	//				stringMoney.clear();
	//				currency.clear();
	//				stockCountString.clear();
	//				stockCount = 0;

	//			}
	//			else
	//			{
	//				userUpdate << stringLogin << " " << stringPassword << " " << stringMoney << " " + currency + " " << stockCountString << " ";
	//				while (stockCount > 0)
	//				{
	//					static std::string stockCount1{}, stockIndex{}, stockPrice{}, stockCurrency{};
	//					iss >> stockCount1 >> stockIndex >> stockPrice >> stockCurrency;
	//					int stockCount2 = std::stoi(stockCount1);
	//					userUpdate << stockCount << " " << stockIndex << " " << stockPrice << " " << stockCurrency << " ";
	//					stockCount -= stockCount2;
	//				}
	//				userUpdate << std::endl;
	//				stringLogin.clear();
	//				stringPassword.clear();
	//				stringMoney.clear();
	//				currency.clear();
	//				stockCountString.clear();
	//				stockCount = 0;
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	std::cerr << "ERROR" << std::endl;
	//}
	if (userUpdate.is_open())
	{
		int stockCount{ user.countAllStocks() };		
		userUpdate << user.GetMoney() << " PLN " << stockCount << " ";
		const auto temphead = user.GetStocks().getHead();
		auto current = temphead;
		while (current != nullptr)
		{
			userUpdate << current->data << " ";
			current = current->next;
		}
		stockCount = 0;
	}
	else
	{
		std::cerr << "ERROR" << std::endl;
	}
	userUpdate.close();
	//fileCheck.close();
	//TODO: naprawic usuwanie pliku


	//if (!userUpdate.is_open() && !fileCheck.is_open())
	//{
	//	std::string path = "C:/Users/sikor/source/repos/a365342d-gr22-repo/Projekt/Gra_Gieldowa";
	//	std::filesystem::path p = path;

	//	std::filesystem::remove(p / "user.txt");

	//	/*const char* oldName = "users.txt";
	//	const char* newName = "usersTemp.txt";
	//	if (std::remove(oldName))
	//	{
	//		std::cout << "File not removed" << std::endl;
	//		std::perror("std::remove");
	//		system("pause");
	//	}*/

	//	std::filesystem::rename(p / "usersTemp.txt", p / "users.txt");

	//	/*if (std::rename(newName, oldName))
	//	{
	//		std::cout << "File not renamed" << std::endl;
	//		std::perror("std::rename");
	//		system("pause");
	//	}*/
	//}
	
}
