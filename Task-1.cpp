#include <iostream>
#include <string>
using namespace std;

//This class represents one song in the playlist
class Song
{
public:
    int song_id;
    string song_name;
    int minutes;
    int seconds;
    Song* next;
    Song* previous;

    //Constructor initializes a new song
    Song(int id, string name, int min, int sec)
{
    song_id = id;
    song_name = name;
    minutes = min;
    seconds = sec;
    next = nullptr;
    previous = nullptr;
}
};
//This class manages the complete playlist
class Playlist
{
private:
    Song* head;
    Song* tail;
    Song* current;

public:

    //Constructor starts with an empty playlist
    Playlist()
    {
        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }
  //Add a new song at the end of the playlist
void addSong()
{
    int id;
    string name;
    int minutes;
    int seconds;

    cout << "\nEnter Song ID: ";
    cin >> id;

    //Check if the ID already exists
    Song* temp = head;

    while (temp != nullptr)
    {
        if (temp->song_id == id)
        {
            cout << "Song ID already exists.\n";
            return;
        }

        temp = temp->next;
    }
    cin.ignore();

    cout << "Enter Song Name: ";
    getline(cin, name);

    cout << "Enter Duration\n";

    cout << "Enter Minutes: ";
    cin >> minutes;

    //Keep asking until seconds are valid
    do
    {
        cout << "Enter Seconds (0-59): ";
        cin >> seconds;

        if (seconds < 0 || seconds > 59)
            cout << "Invalid seconds. Enter a value from 0 to 59.\n";

    } while (seconds < 0 || seconds > 59);
    //Create a new song node
    Song* newSong = new Song(id, name, minutes, seconds);

    //If playlist is empty, new song becomes first and last
    if (head == nullptr)
    {
        head = newSong;
        tail = newSong;
        current = newSong;

        cout << "Song added successfully.\n";
        return;
    }
    //Connect new song after the current last song
    tail->next = newSong;
    newSong->previous = tail;

    //Update tail to the new last song
    tail = newSong;

    cout << "Song added successfully.\n";
}
    //Delete a song using its ID
    void deleteSong()
    {
        int id;

        cout << "\nEnter Song ID to delete: ";
        cin >> id;

        Song* temp = head;

        //Search for the song
        while (temp != nullptr && temp->song_id != id)
            temp = temp->next;

        //Check if the song was found
        if (temp == nullptr)
        {
            cout << "Song not found.\n";
            return;
        }

        //If the song is the first song
        if (temp == head)
            head = temp->next;

        //If the song is the last song
        if (temp == tail)
            tail = temp->previous;

        //Connect the previous song to the next song
        if (temp->previous != nullptr)
            temp->previous->next = temp->next;

        //Connect the next song back to the previous song
        if (temp->next != nullptr)
            temp->next->previous = temp->previous;

        //Move current if the deleted song was playing
        if (current == temp)
        {
            if (temp->next != nullptr)
                current = temp->next;
            else
                current = temp->previous;
        }
        delete temp;

        cout << "Song deleted successfully.\n";
    }
    //Display all songs from first to last
    void displayForward()
    {
        if (head == nullptr)
        {
            cout << "\nPlaylist is empty.\n";
            return;
        }

        Song* temp = head;

        cout << "\n--- Playlist Forward ---\n";

        while (temp != nullptr)
        {
            displaySong(temp);
            temp = temp->next;
        }
    }
    //Display all songs from last to first
    void displayBackward()
    {
        if (tail == nullptr)
        {
            cout << "\nPlaylist is empty.\n";
            return;
        }
        Song* temp = tail;

        cout << "\n--- Playlist Backward ---\n";

        while (temp != nullptr)
        {
            displaySong(temp);
            temp = temp->previous;
        }
    }
    //Search for a song using its ID
    void searchSong()
    {
        int id;

        cout << "\nEnter Song ID to search: ";
        cin >> id;

        Song* temp = head;
        //Search through the playlist
        while (temp != nullptr)
        {
            if (temp->song_id == id)
            {
                cout << "\nSong Found!\n";
                displaySong(temp);
                return;
            }
            temp = temp->next;
        }
        cout << "Song not found.\n";
    }
    //Play the next song
    void playNext()
    {
        if (current == nullptr)
        {
            cout << "\nPlaylist is empty.\n";
            return;
        }
        //Check if current song is already the last song
        if (current->next == nullptr)
        {
            cout << "\nAlready at the last song.\n";
            displaySong(current);
            return;
        }
        //Move current to the next song
        current = current->next;

        cout << "\nNow Playing:\n";
        displaySong(current);
    }
    //Play the previous song
    void playPrevious()
    {
        if (current == nullptr)
        {
            cout << "\nPlaylist is empty.\n";
            return;
        }
        //Check if current song is already the first song
        if (current->previous == nullptr)
        {
            cout << "\nAlready at the first song.\n";
            displaySong(current);
            return;
        }
        //Move current to the previous song
        current = current->previous;

        cout << "\nNow Playing:\n";
        displaySong(current);
    }
    //Reverse the playlist by changing the pointers
    void reversePlaylist()
    {
        if (head == nullptr || head == tail)
        {
            cout << "\nNot enough songs to reverse.\n";
            return;
        }

        Song* temp = head;
        //Swap next and previous pointers of every node
        while (temp != nullptr)
        {
            Song* swap = temp->next;
            temp->next = temp->previous;
            temp->previous = swap;

            //Move to the next node in the original direction
            temp = temp->previous;
        }
        //Swap the first and last nodes
        Song* swap = head;
        head = tail;
        tail = swap;

        cout << "\nPlaylist reversed successfully.\n";
    }
    //Display the song that is currently playing
    void showCurrent()
    {
        if (current == nullptr)
        {
            cout << "\nNo song is currently playing.\n";
            return;
        }
        cout << "\nCurrently Playing:\n";
        displaySong(current);
    }
    //Display the details of one song
    void displaySong(Song* song)
    {
        cout << "ID: " << song->song_id << endl;
        cout << "Name: " << song->song_name << endl;
        cout << "Duration: " << song->minutes << ":" << song->seconds << endl;
        cout << "----------------------\n";
    }
};
//Main function
int main()
{
    Playlist playlist;
    int choice;
    do
    {
        cout << "\n-----------------------------------\n";
        cout << "     PLAYLIST MANAGEMENT SYSTEM\n";
        cout << "1. Add Song\n";
        cout << "2. Delete Song\n";
        cout << "3. Display Playlist Forward\n";
        cout << "4. Display Playlist Backward\n";
        cout << "5. Search Song\n";
        cout << "6. Play Next Song\n";
        cout << "7. Play Previous Song\n";
        cout << "8. Reverse Playlist\n";
        cout << "9. Show Current Song\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                playlist.addSong();
                break;

            case 2:
                playlist.deleteSong();
                break;

            case 3:
                playlist.displayForward();
                break;

            case 4:
                playlist.displayBackward();
                break;

            case 5:
                playlist.searchSong();
                break;

            case 6:
                playlist.playNext();
                break;

            case 7:
                playlist.playPrevious();
                break;

            case 8:
                playlist.reversePlaylist();
                break;

            case 9:
                playlist.showCurrent();
                break;

            case 10:
                cout << "\nProgram ended.\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }
    } while (choice != 10);

    return 0;
}