#include <iostream>
using namespace std;


//This class represents one person in the circle
class Person
{
public:
    int id;
    Person* next;

    //Constructor creates a person
    Person(int personID)
    {
        id = personID;
        next = nullptr;
    }
};
//This class manages the circular linked list
class list
{
private:
    Person* head;
    Person* tail;

public:
    //Constructor starts with an empty circle
    list()
    {
        head = nullptr;
        tail = nullptr;
    }
    //Create a circular linked list of N people
    void createCircle(int n)
    {
        for (int i = 1; i <= n; i++)
        {
            Person* newPerson = new Person(i);

            //If this is the first person
            if (head == nullptr)
            {
                head = newPerson;
                tail = newPerson;
                tail->next = head;
            }
            else
            {
                //Add the new person at the end
                tail->next = newPerson;
                tail = newPerson;

                //Last person points back to the first person
                tail->next = head;
            }
        }
    }
    //Perform the Josephus elimination process
    void eliminate(int k)
    {
        if (head == nullptr)
            return;

        Person* current = head;
        Person* previous = tail;

        cout << "\nElimination Order: ";

        //Continue until only one person remains
        while (current->next != current)
        {
            //Move k-1 times because current is counted as 1
            for (int count = 1; count < k; count++)
            {
                previous = current;
                current = current->next;
            }

            //Display the person being eliminated
            cout << current->id;

            //Remove current person from the circle
            previous->next = current->next;

            Person* deletedPerson = current;
            current = current->next;

            delete deletedPerson;

            //Print comma between eliminated IDs
            if (current->next != current)
                cout << ", ";
        }
        //The remaining person is the survivor
        head = current;
        tail = previous;
    }
    //Display the surviving person
    void displaySurvivor()
    {
        if (head == nullptr)
        {
            cout << "\nNo survivor.\n";
            return;
        }

        cout << "\nSurvivor: Person " << head->id << endl;
    }
    //Destructor deletes the remaining node
    ~list()
    {
        if (head != nullptr)
            delete head;
    }
};
//Main function
int main()
{
    int n;
    int k;

    cout << "Enter number of people: ";
    cin >> n;

    //Make sure number of people is valid
    while (n <= 0)
    {
        cout << "Enter a positive number: ";
        cin >> n;
    }
    cout << "Enter step count: ";
    cin >> k;

    //Make sure step count is valid
    while (k <= 0)
    {
        cout << "Enter a positive step count: ";
        cin >> k;
    }
    //Create object
    list circle;

    //Build the circular linked list
    circle.createCircle(n);

    //Start the elimination process
    circle.eliminate(k);

    //Display the final survivor
    circle.displaySurvivor();

    return 0;
}