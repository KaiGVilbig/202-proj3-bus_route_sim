#include "Route.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

Route::Route() {
	m_start = nullptr;
	m_size = 0;
}

Route::~Route() {
	Clear();
}

void Route::Clear() {
	Stop *curr = m_start;
	Stop *next;
	for (int i = 0; i < m_size; i++) {

		next = curr->m_next;
		m_start = next;
		curr->m_next = nullptr;
		delete curr;
		curr = next;
	}
	m_start = nullptr;
	m_size = 0;
}

void Route::InsertAt(string name, int location, int riders, double cost) {
	Stop *newStop = new Stop(name, location, riders, cost);
	Stop *curr = m_start;
	Stop *prev = NULL;
	
	while (newStop != NULL) {

		if (curr == NULL || newStop->m_location < curr->m_location) {

			//Stop comes at the begining
			if (prev == NULL) {
				newStop->m_next = curr;
				m_start = newStop;
				newStop = NULL;
			}
			//stop comes at the end
			else if (curr == NULL) {
				prev->m_next = newStop;
				newStop = NULL;
			}
			//stop comes between two stops
			else {
				newStop->m_next = curr;
				prev->m_next = newStop;
				newStop = NULL;
			}
		}
		else {
			prev = curr;
			curr = curr->m_next;
		}
	}
	m_size++;
}

void Route::DisplayRoute() {

	Stop *curr = m_start;
	for (int i = 1; i <= m_size; i++) {
		cout << "Stop " << i << " - " << curr->m_name;
		cout << "(" << curr->m_location << ")\n";
		curr = curr->m_next;
	}
}

bool Route::IsEmpty() {
	return m_size == 0;
}

double Route::GetRouteEarnings(double riderFare) {

	double total = 0;
	Stop *curr = m_start;

	while (curr->m_next != NULL) {
		total += (curr->m_riders * riderFare);
		curr = curr->m_next;
	}
	return total;
}

double Route::GetRouteExpenses() {
	double total = 0;
	Stop *curr = m_start;

	while (curr->m_next != NULL) {
		total += curr->m_cost;
		curr = curr->m_next;
	}
	return total;
}

int Route::GetSize() {
	return m_size;
}

void Route::OptimizeRoute(double riderFare) {
	Stop *curr = m_start;
	Stop *prev = NULL;

	while (curr->m_next != NULL) {
		double earnings = 0;
		double expenses = 0;

		earnings += (curr->m_riders * riderFare);
		expenses += curr->m_cost;

		if (earnings < expenses) {

			//removes stop
			RemoveStop(curr->m_location);
			if (curr == m_start) {
				curr = m_start;
			}
			else {
				curr = prev;
			}
		}
		prev = curr;
		curr = curr->m_next;
	}
}

void Route::RemoveStop(int stopLocation) {
	Stop *curr = m_start;
	Stop *prev = NULL;

	while (curr->m_next != NULL) {

		if (curr->m_location == stopLocation) {

			//case where stopLocation is the first stop
			if (curr->m_location == 1) {
				m_start = curr->m_next;
			}
			else {
				prev->m_next = curr->m_next;
			}
			curr->m_next = nullptr;
			delete curr;
			curr = prev;
			m_size--;
			break;
		}
		else {
			prev = curr;
			curr = curr->m_next;
		}
	}
}

void Route::DisplayStopData(double riderFare) {
	Stop *curr = m_start;

	while (curr->m_next != NULL) {
		double earnings = 0;
		double expenses = 0;
		double total = 0;

		earnings += (curr->m_riders * riderFare);
		expenses += curr->m_cost;
		total = earnings - expenses;

		cout << curr->m_name << endl;
		cout << "\tEarnings: " << curr->m_riders << " @ " << riderFare;
		cout << " = $" << earnings << endl;
		cout << "\tExpenses: $" << expenses << endl;
		cout << "\tTotal: $" << fixed << setprecision(2) << total << endl;

		curr = curr->m_next;
	}
}

ostream &operator<< (ostream &output, Route &myRoute) {

	Stop *tempStop;
	tempStop = myRoute.m_start;

	if (myRoute.IsEmpty()) {
		cout << "Route is empty\n";
		return output;
	}

	for (int i = 1; i <= myRoute.m_size; i++) {
		output << "Stop " << i << " - " << tempStop->m_name;
		output << "(" << tempStop->m_location << ")\n";
		tempStop = tempStop->m_next;
	}
	output << "END\n";
	return output;
}