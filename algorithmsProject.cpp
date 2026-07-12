/*
=========================================================
                IMPORTANT FINAL FIXES
=========================================================

This is the FINAL improved version after fixing:

1) Empty vector handling in:
   - sortMenu()
   - searchMenu()
   - updateSpeed()

2) Resetting competitors' original speed
   before every new race.

Without resetting speed:
------------------------
A second race would start with modified
speeds from the previous race.

This creates inconsistent race behavior.

=========================================================
*/

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

/*
=========================================================
                COMPETITOR STRUCT
=========================================================

originalSpeed:
--------------
Stores the competitor's initial speed.

Why?
----
Because race speed changes dynamically
during simulation.

When a new race starts:
-----------------------
We restore the original speed values.

=========================================================
*/
struct Competitor
{

    int id;
    string name;

    double speed;

    /*
    Original speed before race modifications.
    */
    double originalSpeed;

    double distance;
};

vector<Competitor> competitors;

const double FINISH_LINE = 200;

int sortType;

/*
=========================================================
                CHECK DUPLICATE ID
=========================================================
*/
bool checkDuplicateID(int id)
{

    for (int i = 0; i < competitors.size(); i++)
    {

        if (competitors[i].id == id)
        {

            return true;
        }
    }

    return false;
}

/*
=========================================================
                ADD COMPETITOR
=========================================================
*/
void addCompetitor()
{

    Competitor c;

    cout << "\n========== Add Competitor ==========\n";

    /*
    ------------------------------------
    Prevent Duplicate IDs
    ------------------------------------
    */
    while (true)
    {

        cout << "Enter Competitor ID: ";
        cin >> c.id;

        if (checkDuplicateID(c.id))
        {

            cout << "\nID Already Exists!\n";
        }

        else
        {

            break;
        }
    }

    cin.ignore();

    /*
    ------------------------------------
    Prevent Empty Names
    ------------------------------------
    */
    cout << "Enter Competitor Name: ";
    getline(cin, c.name);

    while (c.name.empty())
    {

        cout << "\nName Cannot Be Empty!\n";

        cout << "Enter Competitor Name: ";
        getline(cin, c.name);
    }

    /*
    ------------------------------------
    Speed Validation
    ------------------------------------
    */
    while (true)
    {

        cout << "Enter Speed: ";
        cin >> c.speed;

        if (c.speed < 0)
        {

            cout << "\nSpeed Cannot Be Negative!\n";
        }

        else if (c.speed > 300)
        {

            cout << "\nMaximum Allowed Speed is 300!\n";
        }

        else
        {

            break;
        }
    }

    /*
    ------------------------------------
    Save Original Speed
    ------------------------------------
    This value will be restored before
    every new race.
    */
    c.originalSpeed = c.speed;

    c.distance = 0;

    competitors.push_back(c);

    cout << "\nCompetitor Added Successfully!\n";
}

/*
=========================================================
                SHOW COMPETITORS
=========================================================
*/
void showCompetitors()
{

    if (competitors.empty())
    {

        cout << "\nNo Competitors Found!\n";
        return;
    }

    cout << "\n=========== Competitors ===========\n";

    cout << "ID\tName\tSpeed\tDistance\n";

    for (int i = 0; i < competitors.size(); i++)
    {

        cout << competitors[i].id << "\t"
             << competitors[i].name << "\t"
             << competitors[i].speed << "\t"
             << competitors[i].distance << endl;
    }
}

/*
=========================================================
                RESET RACE
=========================================================

Purpose:
--------
1) Reset all distances
2) Restore original speeds

Why Restore Speeds?
-------------------
Because race simulation changes speed
dynamically every round.

Without resetting:
------------------
A second race would inherit modified
speeds from the previous race.

=========================================================
*/
void resetRace()
{

    for (int i = 0; i < competitors.size(); i++)
    {

        competitors[i].distance = 0;

        competitors[i].speed =
            competitors[i].originalSpeed;
    }
}

/*
=========================================================
                UPDATE SPEEDS
=========================================================
*/
void updateSpeeds()
{

    for (int i = 0; i < competitors.size(); i++)
    {

        int randomChange = rand() % 21 - 10;

        competitors[i].speed += randomChange;

        /*
        Prevent Negative Speeds
        */
        if (competitors[i].speed < 0)
        {

            competitors[i].speed = 0;
        }

        /*
        Prevent Unrealistic Speeds
        */
        if (competitors[i].speed > 300)
        {

            competitors[i].speed = 300;
        }
    }
}

/*
=========================================================
                UPDATE DISTANCES
=========================================================
*/
void updateDistances()
{

    for (int i = 0; i < competitors.size(); i++)
    {

        competitors[i].distance +=
            competitors[i].speed;
    }
}

/*
=========================================================
                    MERGE FUNCTION
=========================================================
*/
void merge(int left, int mid, int right)
{

    vector<Competitor> temp;

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right)
    {

        if (competitors[i].distance >=
            competitors[j].distance)
        {

            temp.push_back(competitors[i]);

            i++;
        }

        else
        {

            temp.push_back(competitors[j]);

            j++;
        }
    }

    while (i <= mid)
    {

        temp.push_back(competitors[i]);

        i++;
    }

    while (j <= right)
    {

        temp.push_back(competitors[j]);

        j++;
    }

    for (int k = 0; k < temp.size(); k++)
    {

        competitors[left + k] = temp[k];
    }
}

/*
=========================================================
                MERGE SORT
=========================================================

Complexity:
-----------
O(n log n)

Used For:
---------
Real-time ranking by distance.

=========================================================
*/
void mergeSort(int left, int right)
{

    if (left >= right)
    {

        return;
    }

    int mid = (left + right) / 2;

    mergeSort(left, mid);

    mergeSort(mid + 1, right);

    merge(left, mid, right);
}

/*
=========================================================
                SORT BY DISTANCE
=========================================================
*/
void sortByDistance()
{

    mergeSort(0, competitors.size() - 1);
}

/*
=========================================================
                QUICK SORT PARTITION
=========================================================
*/
int partition(int low, int high)
{

    double pivot;

    if (sortType == 1)
        pivot = competitors[high].id;

    else
        pivot = competitors[high].speed;

    int i = low - 1;

    for (int j = low; j < high; j++)
    {

        bool condition;

        if (sortType == 1)
            condition =
                competitors[j].id < pivot;

        else
            condition =
                competitors[j].speed > pivot;

        if (condition)
        {

            i++;

            swap(competitors[i],
                 competitors[j]);
        }
    }

    swap(competitors[i + 1],
         competitors[high]);

    return i + 1;
}

/*
=========================================================
                QUICK SORT
=========================================================

Average Complexity:
-------------------
O(n log n)

=========================================================
*/
void quickSort(int low, int high)
{

    if (low < high)
    {

        int pi = partition(low, high);

        quickSort(low, pi - 1);

        quickSort(pi + 1, high);
    }
}

/*
=========================================================
                SORT BY ID
=========================================================
*/
void sortByID()
{

    sortType = 1;

    quickSort(0, competitors.size() - 1);
}

/*
=========================================================
                SORT BY SPEED
=========================================================
*/
void sortBySpeed()
{

    sortType = 2;

    quickSort(0, competitors.size() - 1);
}

/*
=========================================================
                DISPLAY LEADERBOARD
=========================================================
*/
void displayLeaderboard()
{

    cout << "\n=========== Live Ranking ===========\n";

    for (int i = 0; i < competitors.size(); i++)
    {

        cout << i + 1 << ") "
             << competitors[i].name
             << " -> "
             << competitors[i].distance
             << "m"
             << endl;
    }
}

/*
=========================================================
                RACE FINISHED?
=========================================================
*/
bool raceFinished()
{

    for (int i = 0; i < competitors.size(); i++)
    {

        if (competitors[i].distance >=
            FINISH_LINE)
        {

            return true;
        }
    }

    return false;
}

/*
=========================================================
                START RACE
=========================================================
*/
void startRace()
{

    if (competitors.empty())
    {

        cout << "\nNo Competitors Found!\n";

        return;
    }

    /*
    Reset race before starting.
    */
    resetRace();

    int round = 1;

    cout << "\n========== 200m Race Started ==========\n";

    while (!raceFinished())
    {

        cout << "\n=================================\n";

        cout << "           Round "
             << round << endl;

        cout << "=================================\n";

        updateSpeeds();

        updateDistances();

        sortByDistance();

        displayLeaderboard();

        round++;
    }

    /*
    First competitor becomes winner
    because competitors are already
    sorted by distance.
    */
    cout << "\n=================================\n";

    cout << "🏆 Winner: "
         << competitors[0].name
         << endl;

    cout << "Distance: "
         << competitors[0].distance
         << "m\n";
}

/*
=========================================================
                SEARCH BY NAME
=========================================================

Algorithm Used:
---------------
Linear Search

=========================================================
*/
void searchByName()
{

    string target;

    cin.ignore();

    cout << "\nEnter Competitor Name: ";

    getline(cin, target);

    bool found = false;

    for (int i = 0; i < competitors.size(); i++)
    {

        if (competitors[i].name == target)
        {

            cout << "\nCompetitor Found!\n";

            cout << "ID: "
                 << competitors[i].id << endl;

            cout << "Name: "
                 << competitors[i].name << endl;

            cout << "Speed: "
                 << competitors[i].speed << endl;

            cout << "Distance: "
                 << competitors[i].distance
                 << endl;

            found = true;

            break;
        }
    }

    if (!found)
    {

        cout << "\nCompetitor Not Found!\n";
    }
}

/*
=========================================================
                BINARY SEARCH BY ID
=========================================================

Complexity:
-----------
O(log n)

=========================================================
*/
int binarySearchByID(int target)
{

    int left = 0;

    int right =
        competitors.size() - 1;

    while (left <= right)
    {

        int mid =
            (left + right) / 2;

        if (competitors[mid].id == target)
        {

            return mid;
        }

        else if (competitors[mid].id < target)
        {

            left = mid + 1;
        }

        else
        {

            right = mid - 1;
        }
    }

    return -1;
}

/*
=========================================================
                SEARCH BY ID
=========================================================
*/
void searchByID()
{

    sortByID();

    int target;

    cout << "\nEnter Competitor ID: ";

    cin >> target;

    int index =
        binarySearchByID(target);

    if (index != -1)
    {

        cout << "\nCompetitor Found!\n";

        cout << "ID: "
             << competitors[index].id
             << endl;

        cout << "Name: "
             << competitors[index].name
             << endl;

        cout << "Speed: "
             << competitors[index].speed
             << endl;

        cout << "Distance: "
             << competitors[index].distance
             << endl;
    }

    else
    {

        cout << "\nCompetitor Not Found!\n";
    }
}

/*
=========================================================
                SEARCH MENU
=========================================================

Important Validation:
---------------------
Prevent entering the menu when there
are no competitors inside the system.

=========================================================
*/
void searchMenu()
{

    if (competitors.empty())
    {

        cout << "\nNo Competitors Found!\n";

        return;
    }

    int choice;

    cout << "\n====== Find Competitor Using ======\n";

    cout << "1) Name\n";

    cout << "2) ID\n";

    cout << "\nEnter Choice: ";

    cin >> choice;

    switch (choice)
    {

    case 1:

        searchByName();

        break;

    case 2:

        searchByID();

        break;

    default:

        cout << "\nInvalid Choice!\n";
    }
}

/*
=========================================================
                UPDATE SPEED BY NAME
=========================================================
*/
void updateSpeedByName()
{

    string target;

    cin.ignore();

    cout << "\nEnter Competitor Name: ";

    getline(cin, target);

    bool found = false;

    for (int i = 0; i < competitors.size(); i++)
    {

        if (competitors[i].name == target)
        {

            double newSpeed;

            while (true)
            {

                cout << "Enter New Speed: ";

                cin >> newSpeed;

                if (newSpeed < 0)
                {

                    cout << "\nSpeed Cannot Be Negative!\n";
                }

                else if (newSpeed > 300)
                {

                    cout << "\nMaximum Allowed Speed is 300!\n";
                }

                else
                {

                    break;
                }
            }

            /*
            Update both:
            1) Current speed
            2) Original speed

            This ensures that future races
            start correctly.
            */
            competitors[i].speed = newSpeed;

            competitors[i].originalSpeed =
                newSpeed;

            cout << "\nSpeed Updated Successfully!\n";

            found = true;

            break;
        }
    }

    if (!found)
    {

        cout << "\nCompetitor Not Found!\n";
    }
}

/*
=========================================================
                UPDATE SPEED BY ID
=========================================================
*/
void updateSpeedByID()
{

    sortByID();

    int id;

    cout << "\nEnter Competitor ID: ";

    cin >> id;

    int index =
        binarySearchByID(id);

    if (index != -1)
    {

        double newSpeed;

        while (true)
        {

            cout << "Enter New Speed: ";

            cin >> newSpeed;

            if (newSpeed < 0)
            {

                cout << "\nSpeed Cannot Be Negative!\n";
            }

            else if (newSpeed > 300)
            {

                cout << "\nMaximum Allowed Speed is 300!\n";
            }

            else
            {

                break;
            }
        }

        /*
        Update both current and original speed.
        */
        competitors[index].speed = newSpeed;

        competitors[index].originalSpeed =
            newSpeed;

        cout << "\nSpeed Updated Successfully!\n";
    }

    else
    {

        cout << "\nCompetitor Not Found!\n";
    }
}

/*
=========================================================
                UPDATE SPEED MENU
=========================================================

Validation:
-----------
Prevent opening the menu if no competitors exist.

=========================================================
*/
void updateSpeed()
{

    if (competitors.empty())
    {

        cout << "\nNo Competitors Found!\n";

        return;
    }

    int choice;

    cout << "\n====== Find Competitor Using ======\n";

    cout << "1) Name\n";

    cout << "2) ID\n";

    cout << "\nEnter Choice: ";

    cin >> choice;

    switch (choice)
    {

    case 1:

        updateSpeedByName();

        break;

    case 2:

        updateSpeedByID();

        break;

    default:

        cout << "\nInvalid Choice!\n";
    }
}

/*
=========================================================
                SORT MENU
=========================================================

Validation:
-----------
Prevent sorting when the vector is empty.

=========================================================
*/
void sortMenu()
{

    if (competitors.empty())
    {

        cout << "\nNo Competitors Found!\n";

        return;
    }

    int choice;

    cout << "\n========== Sort Menu ==========\n";

    cout << "1) Sort By Speed\n";

    cout << "2) Sort By Distance\n";

    cout << "3) Sort By ID\n";

    cout << "\nEnter Choice: ";

    cin >> choice;

    switch (choice)
    {

    case 1:

        sortBySpeed();

        cout << "\nSorted By Speed Successfully!\n";

        break;

    case 2:

        sortByDistance();

        cout << "\nSorted By Distance Successfully!\n";

        break;

    case 3:

        sortByID();

        cout << "\nSorted By ID Successfully!\n";

        break;

    default:

        cout << "\nInvalid Choice!\n";
    }
}

/*
=========================================================
                    MAIN FUNCTION
=========================================================

Main Responsibilities:
----------------------
1) Display menus
2) Navigate between operations
3) Keep the program running continuously

=========================================================
*/
int main()
{

    /*
    srand(time(0))
    ----------------
    Generates different random values
    every time the program runs.
    */
    srand(time(0));

    int choice;

    while (true)
    {

        cout << "\n=================================\n";

        cout << "     Race Visualizer System\n";

        cout << "=================================\n";

        cout << "1) Add Competitor\n";

        cout << "2) Show Competitors\n";

        cout << "3) Start 200m Race\n";

        cout << "4) Search Competitor\n";

        cout << "5) Update Speed\n";

        cout << "6) Sort Competitors\n";

        cout << "7) Exit\n";

        cout << "\nEnter Your Choice: ";

        cin >> choice;

        switch (choice)
        {

        case 1:

            addCompetitor();

            break;

        case 2:

            showCompetitors();

            break;

        case 3:

            startRace();

            break;

        case 4:

            searchMenu();

            break;

        case 5:

            updateSpeed();

            break;

        case 6:

            sortMenu();

            break;

        case 7:

            cout << "\nExiting Program...\n";

            return 0;

        default:

            cout << "\nInvalid Choice!\n";
        }
    }
}