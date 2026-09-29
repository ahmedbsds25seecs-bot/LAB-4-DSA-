#include <iostream>
#include <string>
using namespace std;

// Each node stores one binary bit
class Node
{
public:
    int bit;
    Node* next;
    Node* previous;

    // Constructor creates a bit node
    Node(int value)
    {
        bit = value;
        next = nullptr;
        previous = nullptr;
    }
};

// This class manages the binary number using a doubly linked list
class BinaryNumber
{
private:
    Node* head;
    Node* tail;

public:

    // Constructor starts with an empty binary number
    BinaryNumber()
    {
        head = nullptr;
        tail = nullptr;
    }

    // Copy Constructor (performs deep copy to prevent pointer aliasing)
    BinaryNumber(const BinaryNumber& other)
    {
        head = nullptr;
        tail = nullptr;
        Node* temp = other.head;
        while (temp != nullptr)
        {
            addBit(temp->bit);
            temp = temp->next;
        }
    }

    // Assignment Operator (handles safe assignment between objects)
    BinaryNumber& operator=(const BinaryNumber& other)
    {
        if (this != &other)
        {
            clear();
            Node* temp = other.head;
            while (temp != nullptr)
            {
                addBit(temp->bit);
                temp = temp->next;
            }
        }
        return *this;
    }

    // Destructor removes all nodes
    ~BinaryNumber()
    {
        clear();
    }

    // Delete all nodes from the list
    void clear()
    {
        Node* temp = head;

        while (temp != nullptr)
        {
            Node* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }

        head = nullptr;
        tail = nullptr;
    }

    // Add a bit at the end of the list
    void addBit(int bit)
    {
        Node* newNode = new Node(bit);

        // If the list is empty
        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
            return;
        }

        // Connect the new node to the last node
        tail->next = newNode;
        newNode->previous = tail;
        tail = newNode;
    }

    // Add a bit at the beginning of the list
    void addBitToFront(int bit)
    {
        Node* newNode = new Node(bit);

        // If the list is empty
        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
            return;
        }

        // Connect the new node before the first node
        newNode->next = head;
        head->previous = newNode;
        head = newNode;
    }

    // Pads the binary number with leading 0s to make its length a multiple of 8
    void alignTo8Bit()
    {
        int length = 0;
        Node* temp = head;
        while (temp != nullptr)
        {
            length++;
            temp = temp->next;
        }

        int remainder = length % 8;
        if (remainder != 0 || length == 0)
        {
            int padBits = 8 - remainder;
            for (int i = 0; i < padBits; i++)
            {
                addBitToFront(0);
            }
        }
    }

    // Store a binary number from user input
    void input()
    {
        string binary;

        cout << "Enter binary number: ";
        cin >> binary;

        // Check that the input contains only 0 and 1
        while (!validBinary(binary))
        {
            cout << "Invalid binary number.\n";
            cout << "Enter only 0 and 1: ";
            cin >> binary;
        }

        clear();

        // Store each bit in a separate node
        for (size_t i = 0; i < binary.length(); i++)
            addBit(binary[i] - '0');

        // Ensure 8-bit grouping block alignment
        alignTo8Bit();

        cout << "Binary number stored successfully.\n";
    }

    // Check whether the input contains only binary digits
    bool validBinary(string binary)
    {
        if (binary.length() == 0)
            return false;

        for (size_t i = 0; i < binary.length(); i++)
        {
            if (binary[i] != '0' && binary[i] != '1')
                return false;
        }

        return true;
    }

    // Display the binary number in 8-bit groups
    void display()
    {
        if (head == nullptr)
        {
            cout << "Binary number is empty.\n";
            return;
        }

        Node* temp = head;
        int count = 0;

        cout << "Binary: ";

        while (temp != nullptr)
        {
            cout << temp->bit;

            count++;

            // Add a space after every 8 bits
            if (temp->next != nullptr && count % 8 == 0)
                cout << " ";

            temp = temp->next;
        }

        cout << endl;
    }

    // Create a copy of the current binary number
    BinaryNumber copy() const
    {
        BinaryNumber result;

        Node* temp = head;

        while (temp != nullptr)
        {
            result.addBit(temp->bit);
            temp = temp->next;
        }

        return result;
    }

    // Perform 1's complement by flipping every bit
    BinaryNumber onesComplement() const
    {
        BinaryNumber result = copy();

        Node* temp = result.head;

        while (temp != nullptr)
        {
            if (temp->bit == 0)
                temp->bit = 1;
            else
                temp->bit = 0;

            temp = temp->next;
        }

        return result;
    }

    // Perform 2's complement
    BinaryNumber twosComplement() const
    {
        // First calculate 1's complement
        BinaryNumber result = onesComplement();

        // Start adding 1 from the right side
        Node* temp = result.tail;
        int carry = 1;

        while (temp != nullptr && carry == 1)
        {
            if (temp->bit == 0)
            {
                temp->bit = 1;
                carry = 0;
            }
            else
            {
                temp->bit = 0;
                carry = 1;
            }

            temp = temp->previous;
        }

        return result;
    }

    // Add two binary numbers using DLLs
    BinaryNumber add(const BinaryNumber& other) const
    {
        BinaryNumber result;

        Node* first = tail;
        Node* second = other.tail;

        int carry = 0;

        // Start addition from the rightmost bits
        while (first != nullptr || second != nullptr || carry != 0)
        {
            int bit1 = 0;
            int bit2 = 0;

            if (first != nullptr)
            {
                bit1 = first->bit;
                first = first->previous;
            }

            if (second != nullptr)
            {
                bit2 = second->bit;
                second = second->previous;
            }

            // Add the two bits and carry
            int sum = bit1 + bit2 + carry;

            // Add result at the beginning of the result object
            result.addBitToFront(sum % 2);

            carry = sum / 2;
        }

        // Align output to 8-bit blocks
        result.alignTo8Bit();

        return result;
    }

    // Shift the binary number one position to the left
    void shiftLeft()
    {
        if (head == nullptr)
            return;

        // A left shift means multiplying by 2 (appends a 0 bit at the end)
        addBit(0);
    }

    // Multiply two binary numbers using repeated addition
    BinaryNumber multiply(const BinaryNumber& other) const
    {
        BinaryNumber result;
        result.addBit(0);

        // Start from the rightmost bit of the second number
        Node* temp = other.tail;

        // This is the current shifted version of the first number
        BinaryNumber shifted = copy();

        while (temp != nullptr)
        {
            if (temp->bit == 1)
                result = result.add(shifted);

            // Shift left for the next bit
            shifted.shiftLeft();

            temp = temp->previous;
        }

        // Align output to 8-bit blocks
        result.alignTo8Bit();

        return result;
    }

    // Convert binary number to decimal
    unsigned long long toDecimal() const
    {
        unsigned long long decimal = 0;

        Node* temp = head;

        while (temp != nullptr)
        {
            decimal = decimal * 2 + temp->bit;
            temp = temp->next;
        }

        return decimal;
    }
};
// Main function
int main()
{
    BinaryNumber number1;
    BinaryNumber number2;

    int choice;

    do
    {
        cout << "\n----------------------------------\n";
        cout << "    BINARY ARITHMETIC USING DLL\n";
        cout << "1. Enter/Change Binary Number 1\n";
        cout << "2. Enter/Change Binary Number 2\n";
        cout << "3. Display Binary Numbers\n";
        cout << "4. 1's Complement\n";
        cout << "5. 2's Complement\n";
        cout << "6. Binary Addition\n";
        cout << "7. Binary Multiplication\n";
        cout << "8. Convert Binary to Decimal\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                number1.input();
                break;
            case 2:
                number2.input();
                break;
            case 3:
                cout << "\nNumber 1:\n";
                number1.display();

                cout << "Number 2:\n";
                number2.display();
                break;
            case 4:
            {
                cout << "\n--- 1's Complement ---";
                
                // Number 1
                BinaryNumber result1 = number1.onesComplement();
                cout << "\nNumber 1 Original: ";
                number1.display();
                cout << "1's Complement:   ";
                result1.display();

                // Number 2
                BinaryNumber result2 = number2.onesComplement();
                cout << "\nNumber 2 Original: ";
                number2.display();
                cout << "1's Complement:   ";
                result2.display();

                break;
            }
            case 5:
            {
                cout << "\n--- 2's Complement ---";

                // Number 1
                BinaryNumber result1 = number1.twosComplement();
                cout << "\nNumber 1 Original: ";
                number1.display();
                cout << "2's Complement:   ";
                result1.display();

                // Number 2
                BinaryNumber result2 = number2.twosComplement();
                cout << "\nNumber 2 Original: ";
                number2.display();
                cout << "2's Complement:   ";
                result2.display();

                break;
            }
            case 6:
            {
                BinaryNumber result = number1.add(number2);

                cout << "\nBinary Addition:\n";
                cout<<"  ";
                number1.display();
                cout << "+ ";
                number2.display();
                cout << "----------------\n";
                result.display();

                break;
            }
            case 7:
            {
                BinaryNumber result = number1.multiply(number2);

                cout << "\nBinary Multiplication:\n";
                cout<<"  ";
                number1.display();
                cout << "* ";
                number2.display();
                cout << "----------------\n";
                result.display();

                break;
            }
            case 8:
                cout << "\nNumber 1 in decimal: "
                     << number1.toDecimal() << endl;

                cout << "Number 2 in decimal: "
                     << number2.toDecimal() << endl;
                break;
            case 9:
                cout << "\nProgram ended.\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != 9);

    return 0;
}