#pragma once

#include <iostream>
#include <curl/curl.h>
#include <string_view>
#include <boost/json.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <map>
#include "User.h"
#include <thread>
#include "DataLock.h"
#include <nlohmann/json.hpp>

namespace pt = boost::property_tree;

auto curlWriteCallback(void* contents, size_t size, size_t nmemb, void* nodePtr) -> size_t
{
	size_t bytesWritten = nmemb * size;
	std::stringstream* buffer = static_cast<std::stringstream*>(nodePtr);
	if (buffer)
	{
		buffer->write(static_cast<char*>(contents), bytesWritten);
	}
	//auto data = std::string_view((char*)contents, bytesWritten);
	//auto wrapper = CurlNodePtrWrapper(nodePtr);
	//ss << data;
	//std::cout << ss.str() << "\n" << std::endl;
	//Parser<std::stringstream> JsonParser;
	//nodePtr->mCallback(wrapper, { reinterpret_cast<uint8_t*>(contents), bytesWritten });

	return bytesWritten;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) 
{
	size_t totalSize = size * nmemb;
	output->append((char*)contents, totalSize);
	return totalSize;
}

template<typename t>
class API
{
	std::string URL, APIkey;
	struct curl_slist* list;
	std::string response;
	//std::stringstream ss;
	//pt::ptree root;
	std::vector<std::string> quoteVec;
	nlohmann::json Jsondata;

public:
	void GetAPI(t type, double& price);
	void GetAPI(std::vector<Stock<std::string>>& stockVec);
	void GetAPI(std::vector<Stock<std::string>>& stockVec, SharedData& dataLock, EndProgram& endProgramLock);
	API();
	~API();
	//const std::stringstream& getSS();
	//pt::ptree getRoot();
	void readJson(double& price);
	void readJson(std::vector<Stock<std::string>>& stocks);
	void getIndexes();
};

template<typename t>
inline void API<t>::GetAPI(t type, double& price)
{
	CURL* curl = curl_easy_init();
	const char* env_variable = "StockApiKey";
	char* value;

	value = std::getenv(env_variable);

	APIkey = value;
	std::string slistString = "X-RapidAPI-Key: " + APIkey;
	const char* slistChar = slistString.c_str();
	URL = "https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/quotes?ticker=" + type;

	list = NULL;

	list = curl_slist_append(list, slistChar);
	list = curl_slist_append(list, "X-RapidAPI-Host: yahoo-finance15.p.rapidapi.com");

	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


	CURLcode res = curl_easy_perform(curl);

	if (res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}

	readJson(price);

	curl_easy_cleanup(curl);
	response.clear();
}

template<typename t>
inline void API<t>::GetAPI(std::vector<Stock<std::string>>& stockVec, SharedData& dataLock, EndProgram& endProgramLock)
{
	int i{ 0 };
	std::lock_guard<std::mutex> lock(dataLock.mutex);
	std::lock_guard<std::mutex> endLock(endProgramLock.EndProgramMutex);
	do
	{
		if (endProgramLock.hasEnded)
		{
			break;
		}
		stockVec.clear();
		for (const auto& type : quoteVec)
		{
			
			dataLock.newDataAvailable = true;
			CURL* curl = curl_easy_init();

			const char* env_variable = "StockApiKey";
			char* value;

			value = std::getenv(env_variable);

			APIkey = value;
			std::string slistString = "X-RapidAPI-Key: " + APIkey;
			const char* slistChar = slistString.c_str();
			URL = "https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/quotes?ticker=" + type;

			list = NULL;

			list = curl_slist_append(list, slistChar);
			list = curl_slist_append(list, "X-RapidAPI-Host: yahoo-finance15.p.rapidapi.com");

			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
			curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


			CURLcode res = curl_easy_perform(curl);

			if (res != CURLE_OK) 
			{
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			}

			readJson(stockVec);

			curl_easy_cleanup(curl);
			curl_global_cleanup();
			response = "";
		}
		i++;
		dataLock.newDataAvailable = false;
		std::this_thread::sleep_for(std::chrono::seconds(60));
	} while (i < 10);

}

template<typename t>
inline API<t>::API()
{
	URL = "";
	APIkey = "";
	list = nullptr;
	response = "";
}

template<typename t>
inline API<t>::~API()
{
}

//template<typename t>
//inline const std::stringstream& API<t>::getSS()
//{
//	return ss;
//}

//template<typename t>
//inline pt::ptree API<t>::getRoot()
//{
//	return root;
//}

template<typename t>
inline void API<t>::readJson(double & price)
{
	try
	{
		try
		{
			//pt::read_json(ss, root);

			Jsondata = nlohmann::json::parse(response);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error getting data from API: " << e.what() << std::endl;
		}

		//auto body = root.get_child("body");
		//auto result = body.get_child("result");

		auto& body = Jsondata["body"];

		if (body.empty())
		{
			std::cerr << "Nie istnieje spolka o tym skrocie" << std::endl;
		}
		else
		{

			double  marketPriceDouble = Jsondata["body"][0]["regularMarketPrice"];
			price = marketPriceDouble;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error parsing JSON structure: " << e.what() << std::endl;
		system("pause");
	}
	response.clear();
}

template<typename t>
inline void API<t>::readJson(std::vector<Stock<std::string>>& stocks)
{
	
	try
	{
		try
		{
			//pt::read_json(ss, root);

			Jsondata = nlohmann::json::parse(response);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error getting data from API: " << e.what() << std::endl;
		}
		
		//auto body = root.get_child("body");
		//auto result = body.get_child("result");
		
		auto& body = Jsondata["body"];

		if (body.empty())
		{
			std::cerr << "Nie istnieje spolka o tym skrocie" << std::endl;
		}
		else
		{
			/*auto shortName = result.begin()->second.get<std::string>("underlyingSymbol");
			auto quote = result.begin()->second.get_child("quote");
			auto currency = quote.get<std::string>("currency");
			auto marketPrice = quote.get<std::string>("regularMarketPrice");
			auto longName = quote.get<std::string>("longName");
			auto high = quote.get<std::string>("regularMarketDayHigh");
			auto low = quote.get<std::string>("regularMarketDayLow");*/

			/*auto shortNameRoot = root.begin()->second.begin()->second.get<std::string>("symbol");
			auto shortName = body.get<std::string>("symbol");
			auto marketPrice = body.get<std::string>("regularMarketPrice");
			auto currency = body.get<std::string>("currency");*/

			/*auto shortName = root.get<std::string>("body[0].symbol");
			auto marketPrice = root.get<std::string>("body[0].regularMarketPrice");
			auto currency = root.get<std::string>("body[0].currency");*/

			std::string shortName = Jsondata["body"][0]["symbol"];
			std::cout << std::fixed << std::setprecision(3);
			double  marketPrice = Jsondata["body"][0]["regularMarketPrice"];
			std::string currency = Jsondata["body"][0]["currency"];

			Stock<std::string> stock(shortName, marketPrice, currency);
			stocks.push_back(stock);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error parsing JSON structure: " << e.what() << std::endl;
		system("pause");
	}
	response.clear();
}

template<typename t>
inline void API<t>::getIndexes()
{
	std::string response, index;
	/*do
	{
		std::cout << "Podaj indeksy gieldowe na ktorych chcialbys operowac: " << std::endl;
		std::cin >> response;
		boost::to_upper(response);
		quoteVec.push_back(response);
		std::cout << "Czy chcesz kontynuowac wpisywanie?" << std::endl;
		std::cin >> response;
	} while (response == "Tak" or response == "tak");
	*/
	std::cout << "Podaj wszystkie indeksy gieldowe na ktorych chcialbys operowac: " << std::endl;
	std::cin.ignore();
	std::getline(std::cin, response);
	bool isDuplicate = false;
	for (const char& a : response)
	{
		if (a != ',' and a != ' ')
		{
			index.append(1, a);
		}
		else if (a == ' ' or a == ',')
		{
			if (index != "")
			{
				for (const auto& b : quoteVec)
				{
					if (b == index)
					{
						isDuplicate = true;
						break;
					}
				}
				if (!isDuplicate) { boost::to_upper(index);  quoteVec.push_back(index); }
			}
			index.clear();
			isDuplicate = false;
		}
	}
	isDuplicate = false;
	for (const auto& a : quoteVec)
	{
		if (a == index)
		{
			isDuplicate = true;
			break;
		}
	}
	if (!isDuplicate)
	{
		boost::to_upper(index);
		quoteVec.push_back(index);
	}
	system("CLS");
}
