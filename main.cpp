#include <iostream>
#include <vector>
#include <list>

using namespace std;

class TrainReservationSystem {
public:
    TrainReservationSystem(int num_seats) {
        // Initialize the available resources.
        available_seats = num_seats;

        // Initialize the allocated and needed resources for each reservation.
        allocated_seats = vector<int>(num_seats, 0);
        needed_seats = vector<int>(num_seats, 0);

        // Initialize the waiting list.
        waiting_list = list<int>();
    }

    // Reserve seats for a passenger.
    bool reserve_seats(int passenger_id, int num_seats) {
        // Check if the request can be granted without leaving the system in an unsafe state.
        if (num_seats > available_seats) {
            // Add the passenger to the waiting list.
            waiting_list.push_back(passenger_id);
            return false;
        }

        // Grant the request.
        for (int i = 0; i < num_seats; i++) {
            int available_seat = find_available_seat();
            allocated_seats[available_seat] = passenger_id;
            available_seats--;

            cout << "Seat " << available_seat << " has been assigned to passenger " << passenger_id << "." << endl;
        }

        return true;
    }

    // Cancel a passenger's reservation.
    void cancel_reservation(int passenger_id) {
        for (int i = 0; i < allocated_seats.size(); i++) {
            if (allocated_seats[i] == passenger_id) {
                allocated_seats[i] = 0;
                available_seats++;
            }
        }

        // Check if there are any waiting passengers.
        if (!waiting_list.empty()) {
            // Assign the seat to the passenger at the top of the waiting list.
            int passenger_id_at_top_of_waiting_list = waiting_list.front();
            waiting_list.pop_front();

            // Reserve the seat for the passenger.
            bool request_success = reserve_seats(passenger_id_at_top_of_waiting_list, 1);
            if (request_success) {
                cout << "Passenger " << passenger_id_at_top_of_waiting_list << " has been assigned the seat cancelled by passenger " << passenger_id << "." << endl;
            } else {
                cout << "Failed to reserve a seat for passenger " << passenger_id_at_top_of_waiting_list << "." << endl;
            }
        }
    }

private:
    int available_seats;
    vector<int> allocated_seats;
    vector<int> needed_seats;

    // Waiting list for passengers who requested seats but did not get any.
    list<int> waiting_list;

    // Function to find an available seat.
    int find_available_seat() {
        for (int i = 0; i < allocated_seats.size(); i++) {
            if (allocated_seats[i] == 0) {
                return i;
            }
        }
        return -1; // No available seats
    }
};

int main() {
    // Get the number of seats from the user.
    int num_seats;
    cout << "Enter the number of seats: ";
    cin >> num_seats;

    // Create a train reservation system with the specified number of seats.
    TrainReservationSystem train_reservation_system(num_seats);

    // Get the number of passengers from the user.
    int num_passengers;
    cout << "Enter the number of passengers: ";
    cin >> num_passengers;

    // For each passenger, get the number of seats required.
    for (int passenger_id = 1; passenger_id <= num_passengers; passenger_id++) {
        int num_seats_required;
        cout << "Enter the number of seats required for passenger " << passenger_id << ": ";
        cin >> num_seats_required;

        // Reserve seats for the passenger.
        bool request_success = train_reservation_system.reserve_seats(passenger_id, num_seats_required);
        if (!request_success) {
            cout << "Request failed for passenger " << passenger_id << "." << endl;
        }
    }

    // Prompt the user to cancel a reservation (if desired).
    while (true) {
        int passenger_id_to_cancel;
        cout << "Enter the passenger ID to cancel reservation (or enter -1 to exit): ";
        cin >> passenger_id_to_cancel;

        if (passenger_id_to_cancel == -1) {
            break;
        }

        // Cancel the reservation for the specified passenger.
        train_reservation_system.cancel_reservation(passenger_id_to_cancel);
    }

    return 0;
}