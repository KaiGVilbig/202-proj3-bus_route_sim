#include "BusCompany.h"

#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

const string FILE_END = "End Route";

BusCompany::BusCompany() {

}

BusCompany::BusCompany(string fileName) {
	BusCompany();
	ReadFile(fileName);
	MainMenu();
}

BusCompany::~BusCompany() {
	for (unsigned int i = 0; i < m_routes.size(); i++) {
		delete m_routes[i];
	}
}

void BusCompany::ReadFile(string fileName) {

	fstream inputStream;
	inputStream.open(fileName);
	string exitLoop;

	do {
		//setting name to "g" (just a random letter) because test1 file ends with end route 
		//but the other two files end with end route with a comma and a new line
		//I imagine that is just an error but I sweated through it and made it work so I will leave
		//it liek this. I know that if all of the files ended the same way, I would just not set name
		//to anything and get rid of the check for name != "g" at the end of the do-while loop.
		Route *m_newRoute = new Route();
		string name = "g", carriageReturn;
		int location, riders;
		double cost;

		//at the end of a route there will be an "end". therefor this will end the route there.
		//at the end of the "end", there is an \n making a new blank line at the end of the file
		//which makes name = ""; this will end the loop. 
		while (name != FILE_END && name != "" && !inputStream.eof()) {

			getline(inputStream, name, ',');
			//used to see if code is working
			//cout << name << "\n";
			
			if (name != FILE_END) {
				inputStream >> location >> riders >> cost;
				m_newRoute->InsertAt(name, location, riders, cost);
				//makes sure the name on the net line doesn't include a \n at the beginning.
				getline(inputStream, carriageReturn);
			}
			else {
				getline(inputStream, carriageReturn);
			}
		}
		//only adds valid routes
		if (name != "" && name != "g") {
			m_routes.push_back(m_newRoute);
		}
		//since the loop will loop one last time after the last route and create a new empty route
		//the program must delete it if it does not add it to the routes vector
		else {
			delete m_newRoute;
		}

		exitLoop = name;
	} while (exitLoop != "" && exitLoop != "g");

	//print success message after successfully loading in all routes
	for (unsigned int i = 0; i < m_routes.size(); i++) {
		cout << "\nRoute " << i + 1 << " loaded with " << m_routes[i]->GetSize() << " stops.\n";
	}
	cout << endl;
	inputStream.close();
}

void BusCompany::MainMenu() {

	int choice = 0;
	cout << "*********************************\n";
	cout << "Welcome to UMBC Transit Simulator\n";

	while (choice != 4) {
		cout << "What would you like to do?: \n";
		cout << "1. Display Routes\n";
		cout << "2. Display Earnings vs Expenses By Route\n";
		cout << "3. Optimize Route\n";
		cout << "4. Exit\n";

		cin >> choice;

		switch (choice) {
		case 1:
			DisplayRoutes();
			break;
		case 2:
			DisplayRouteData();
			break;
		case 3:
			OptimizeRoute();
			break;
		case !4:
			//input validation
			cout << "Please chose a valid choice (1 - 4)\n";
			break;
		}
		cout << endl;
	}
}

void BusCompany::DisplayRoutes() {

	int routeChoice = 1;
	int size = m_routes.size();

	cout << endl;
	if (size > 1) {

		//input validation
		do {
			cout << "Which route would you like to display?\n";
			for (int i = 1; i <= size; i++) {
				cout << "Route " << i << endl;
			}
			cout << "Enter 1 - " << size << " inclusive:\n";
			cin >> routeChoice;
			cout << endl;
		} while (routeChoice < 1 || routeChoice > size);
	}
	//print out selected route
	cout << "********* Route " << routeChoice << " *********\n";

	//because the overloaded operator and the displayroutes() in the route.cpp
	//file does the same thing, and in the buscompany.h file it mentions that
	//this function will call the overloaded operator, this uses the overloaded
	//operator instead of the displayroutes in routes.cpp
	cout << *m_routes[routeChoice - 1];
}

void BusCompany::OptimizeRoute() {

	int routeChoice = 1;
	int size = m_routes.size();

	cout << endl;
	if (size > 1) {

		//input validation
		do {
			cout << "Which route would you like to optimize?\n";
			for (int i = 1; i <= size; i++) {
				cout << "Route " << i << endl;
			}
			cout << "Enter 1 - " << size << " inclusive:\n";
			cin >> routeChoice;
			cout << endl;
		} while (routeChoice < 1 || routeChoice > size);
	}
	//optimize selected route
	m_routes[routeChoice - 1]->OptimizeRoute(RIDER_FARE);
	cout << "Route " << routeChoice << " has been optimized\n";
}

void BusCompany::DisplayRouteData() {
	double earnings, expenses;
	int routeChoice = 1;
	int size = m_routes.size();

	cout << endl;
	if (size > 1) {

		//input validation
		do {
			cout << "Which route would you like to get expenses from?\n";
			for (int i = 1; i <= size; i++) {
				cout << "Route " << i << endl;
			}
			cout << "Enter 1 - " << size << " inclusive:\n";
			cin >> routeChoice;
			cout << endl;
		} while (routeChoice < 1 || routeChoice > size);
	}

	//display the data of earnings vs expenses of the entire route
	earnings = m_routes[routeChoice - 1]->GetRouteEarnings(RIDER_FARE);
	expenses = m_routes[routeChoice - 1]->GetRouteExpenses();
	cout << endl;
	cout << "Earnings: " << earnings << endl;
	cout << "Expenses: " << expenses << endl;

	//display the data of earnings vs expenses of each individual stop
	m_routes[routeChoice - 1]->DisplayStopData(RIDER_FARE);
}